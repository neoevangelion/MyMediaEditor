package com.tuyuanlin.media.editor.middleware

import android.view.Surface

class Player {
    companion object {
        init {
            System.loadLibrary("player-lib")
        }
    }

    external fun playVideo(filePath: String?, surfaceView: Surface?)

    external fun playAudio(filePath: String?)
}