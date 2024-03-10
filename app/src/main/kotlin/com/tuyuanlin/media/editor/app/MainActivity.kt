package com.tuyuanlin.media.editor.app

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.tuyuanlin.media.editor.app.databinding.MainActivityContentBinding


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val viewBinding = MainActivityContentBinding.inflate(layoutInflater)
        setContentView(viewBinding.root)

        setSupportActionBar(viewBinding.toolbar)
        supportActionBar?.setHomeButtonEnabled(true)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        viewBinding.openCameraButton.setOnClickListener {
            startActivity(Intent(this, CameraActivity::class.java))
        }

        viewBinding.openPlayerButton.setOnClickListener {
            startActivity(Intent(this, PlayerActivity::class.java))
        }

        viewBinding.openEditorButton.setOnClickListener {
            startActivity(Intent(this, EditorActivity::class.java))
        }
    }
}