package com.tuyuanlin.media.editor.app

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.tuyuanlin.media.editor.app.databinding.EditorActivityContentBinding

class EditorActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val viewBinding = EditorActivityContentBinding.inflate(layoutInflater)
        setContentView(R.layout.editor_activity_content)

        setSupportActionBar(viewBinding.toolbar)
    }
}