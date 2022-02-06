package com.tuyuanlin.media.editor.app

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.main_activity_content.*


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity_content)

        setSupportActionBar(toolbar)
        supportActionBar?.setHomeButtonEnabled(true)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        open_camera_button.setOnClickListener {
            startActivity(Intent(this, CameraActivity::class.java))
        }

        open_player_button.setOnClickListener {
            startActivity(Intent(this, PlayerActivity::class.java))
        }

        open_editor_button.setOnClickListener {
            startActivity(Intent(this, EditorActivity::class.java))
        }
    }
}