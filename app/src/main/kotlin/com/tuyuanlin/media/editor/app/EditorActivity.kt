package com.tuyuanlin.media.editor.app

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.tuyuanlin.media.editor.app.databinding.EditorActivityContentBinding

class EditorActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val viewBinding = EditorActivityContentBinding.inflate(layoutInflater)
        setContentView(viewBinding.root)

        setSupportActionBar(viewBinding.toolbar)
    }
}