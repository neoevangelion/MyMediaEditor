package com.tuyuanlin.media.editor.app

import android.os.Bundle
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.tuyuanlin.media.editor.app.databinding.PlayerActivityContentBinding
import com.tuyuanlin.media.editor.middleware.Player
import java.io.File

class PlayerActivity : AppCompatActivity() {
    private val player: Player = Player()
    private lateinit var surfaceHolder: SurfaceHolder

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val viewBinding = PlayerActivityContentBinding.inflate(layoutInflater);
        setContentView(viewBinding.root)
        setSupportActionBar(viewBinding.toolbar)

        surfaceHolder = viewBinding.playerSurfaceView.holder

        viewBinding.playButton.setOnClickListener {
            val file = File(getExternalFilesDir(null), "video.mp4")
            player.playVideo(file.absolutePath, surfaceHolder.surface)
        }
    }
}