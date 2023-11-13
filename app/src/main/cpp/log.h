#ifndef LOG_H
#define LOG_H

#include <android/log.h>

#define LOG_INFO(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_INFO, TAG, FORMAT, ##__VA_ARGS__);
#define LOG_DEBUG(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG, FORMAT, ##__VA_ARGS__);
#define LOG_WARN(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_WARN, TAG, FORMAT, ##__VA_ARGS__);
#define LOG_ERROR(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, FORMAT, ##__VA_ARGS__);

#endif