package com.tuyuanlin.media.editor.app

import android.os.Bundle
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.tuyuanlin.media.editor.middleware.Player
import kotlinx.android.synthetic.main.player_activity_content.*
import java.io.File

class PlayerActivity : AppCompatActivity() {
    private val player: Player = Player()
    private lateinit var surfaceHolder: SurfaceHolder

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.player_activity_content)
        setSupportActionBar(toolbar)

        surfaceHolder = player_surface_view.holder

        play_button.setOnClickListener {
            val file = File(getExternalFilesDir(null), "video.mp4")
            player.playVideo(file.absolutePath, surfaceHolder.surface)
        }
    }
}