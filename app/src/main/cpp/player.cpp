#include <jni.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "log.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

#define DEFAULT_LOG_TAG "PLAYER"

void render_frame(AVCodecContext *video_codec_context, AVFormatContext *formatContext,
                  int video_stream_index,
                  ANativeWindow *native_window) {
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    AVFrame *rgba_frame = av_frame_alloc();
    ANativeWindow_Buffer window_buffer;

    int width = video_codec_context->width;
    int height = video_codec_context->height;

    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width, height, 1);
    auto *out_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(int8_t));
    av_image_fill_arrays(rgba_frame->data, rgba_frame->linesize, out_buffer, AV_PIX_FMT_RGBA, width,
                         height, 1);

    SwsContext *convert_context = sws_getContext(width, height, video_codec_context->pix_fmt, width,
                                                 height,
                                                 AV_PIX_FMT_RGBA, SWS_BICUBIC,
                                                 nullptr, nullptr, nullptr);
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            int result = avcodec_send_packet(video_codec_context, packet);
            if (result < 0 && result != AVERROR(EAGAIN) && result != AVERROR_EOF) {
                LOG_ERROR(DEFAULT_LOG_TAG, "Can not send encoded frame")
                continue;
            }

            result = avcodec_receive_frame(video_codec_context, frame);
            if (result < 0 && result != AVERROR_EOF) {
                LOG_ERROR(DEFAULT_LOG_TAG, "Can not get decoded frame")
                continue;
            }

            result = sws_scale(convert_context, frame->data, frame->linesize, 0, height,
                               rgba_frame->data,
                               rgba_frame->linesize);
            if (result < 0) {
                LOG_ERROR(DEFAULT_LOG_TAG, "Can not scale frame")
                continue;
            }

            result = ANativeWindow_lock(native_window, &window_buffer, nullptr);
            if (result < 0) {
                LOG_ERROR(DEFAULT_LOG_TAG, "Can not lock native window")
                continue;
            }

            auto bits = (uint8_t *) window_buffer.bits;
            for (int h = 0; h < height; ++h) {
                memcpy(bits + h * window_buffer.stride * 4,
                       out_buffer + h * rgba_frame->linesize[0],
                       rgba_frame->linesize[0]);
            }
            ANativeWindow_unlockAndPost(native_window);
        }
        av_packet_unref(packet);
    }
    sws_freeContext(convert_context);
    av_free(out_buffer);
    av_frame_free(&rgba_frame);
    av_frame_free(&frame);
    av_packet_free(&packet);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tuyuanlin_media_editor_middleware_Player_playVideo(JNIEnv *env, jobject thiz,
                                                            jstring file_path,
                                                            jobject surface) {
    const char *input_path = nullptr;
    AVFormatContext *format_context = nullptr;
    AVCodecContext *video_codec_context = nullptr;
    AVStream *video_stream = nullptr;
    ANativeWindow *native_window = nullptr;
    AVCodec *video_codec = nullptr;
    ANativeWindow_Buffer window_buffer;

    int video_stream_index = -1;
    int height = 0;
    int width = 0;

    av_register_all();

    input_path = env->GetStringUTFChars(file_path, JNI_FALSE);

    format_context = avformat_alloc_context();

    int result = avformat_open_input(&format_context, input_path, nullptr, nullptr);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not open video file")
        goto error;
    }

    result = avformat_find_stream_info(format_context, nullptr);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not find video stream info")
        goto error;
    }

    for (int i = 0; i < format_context->nb_streams; ++i) {
        AVStream *stream = format_context->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = stream;
            video_stream_index = i;
            break;
        }
    }

    if (video_stream == nullptr) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not find video stream")
        goto error;
    }

    video_codec_context = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(video_codec_context, video_stream->codecpar);

    video_codec = avcodec_find_decoder(video_codec_context->codec_id);
    if (video_codec == nullptr) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not find video codec")
        goto error;
    }

    result = avcodec_open2(video_codec_context, video_codec, nullptr);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not open codec")
        goto error;
    }

    width = video_codec_context->width;
    height = video_codec_context->height;

    native_window = ANativeWindow_fromSurface(env, surface);
    if (native_window == nullptr) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not create native window")
        goto error;
    }

    result = ANativeWindow_setBuffersGeometry(native_window, width, height,
                                              WINDOW_FORMAT_RGBA_8888);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not set native window geometry")
        goto error;
    }

    render_frame(video_codec_context, format_context, video_stream_index, native_window);

    error:
    if (native_window != nullptr) {
        ANativeWindow_release(native_window);
    }

    if (video_codec_context != nullptr) {
        avcodec_close(video_codec_context);
    }

    if (format_context != nullptr) {
        avformat_close_input(&format_context);
        avformat_free_context(format_context);
    }

    if (input_path != nullptr) {
        env->ReleaseStringUTFChars(file_path, input_path);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tuyuanlin_media_editor_middleware_Player_playAudio(JNIEnv *env, jobject thiz,
                                                            jstring file_path) {
    const char *input_path = nullptr;
    AVFormatContext *format_context = nullptr;
    int audio_stream_index = -1;
    AVCodecContext *audio_codec_context = nullptr;
    AVCodec *audio_codec = nullptr;

    input_path = env->GetStringUTFChars(file_path, JNI_FALSE);

    av_register_all();

    format_context = avformat_alloc_context();

    int result = avformat_open_input(&format_context, input_path, nullptr, nullptr);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not open audio file")
        goto error;
    }

    result = avformat_find_stream_info(format_context, nullptr);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not find audio stream info")
        goto error;
    }

    for (int i = 0; i < format_context->nb_streams; ++i) {
        if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }

    if (audio_stream_index == -1) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not find audio stream")
        goto error;
    }

    audio_codec_context = avcodec_alloc_context3(nullptr);
    result = avcodec_parameters_to_context(audio_codec_context,
                                           format_context->streams[audio_stream_index]->codecpar);
    if (result < 0) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not init audio codec context")
        goto error;
    }

    audio_codec = avcodec_find_decoder(audio_codec_context->codec_id);
    if (audio_codec == nullptr) {
        LOG_ERROR(DEFAULT_LOG_TAG, "Can not find audio codec")
        goto error;
    }

    // TODO play audio track

    error:
    if (audio_codec_context != nullptr) {
        avcodec_free_context(&audio_codec_context);
    }

    if (format_context != nullptr) {
        avformat_close_input(&format_context);
        avformat_free_context(format_context);
    }

    if (input_path != nullptr) {
        env->ReleaseStringUTFChars(file_path, input_path);
    }
}