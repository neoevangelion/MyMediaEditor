package com.tuyuanlin.media.editor.app

import android.Manifest
import android.content.Context
import android.os.Build
import android.os.Bundle
import android.util.DisplayMetrics
import android.util.Size
import android.view.Surface
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.content.ContextCompat
import com.google.common.util.concurrent.ListenableFuture
import com.tuyuanlin.media.editor.middleware.PermissionChecker
import kotlinx.android.synthetic.main.camera_activity_content.*


class CameraActivity : AppCompatActivity() {
    private var mPreview: Preview? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.camera_activity_content)

        setSupportActionBar(toolbar)
        supportActionBar?.setHomeButtonEnabled(true)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        start_button.setOnClickListener {
            startCamera()
        }
    }

    private fun startCamera() {
        PermissionChecker.checkAndRequestPermission(this, Manifest.permission.CAMERA)

        val cameraProviderFuture = ProcessCameraProvider.getInstance(this)
        cameraProviderFuture.addListener(
            { setupCameraPreview(cameraProviderFuture) },
            ContextCompat.getMainExecutor(this)
        )
    }

    private fun setupCameraPreview(cameraProviderFuture: ListenableFuture<ProcessCameraProvider>) {
        val rotation: Int
        val width: Int
        val height: Int

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            val metrics = (getSystemService(Context.WINDOW_SERVICE) as WindowManager)
                .currentWindowMetrics
            rotation = display?.rotation ?: Surface.ROTATION_0
            width = metrics.bounds.width()
            height = metrics.bounds.height()
        } else {
            val metrics = DisplayMetrics().also { preview_view.display.getRealMetrics(it) }
            rotation =
                (getSystemService(Context.WINDOW_SERVICE) as WindowManager).defaultDisplay.rotation
            width = metrics.widthPixels
            height = metrics.heightPixels
        }
        val resolution = Size(width, height)
        val cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA

        mPreview = Preview.Builder()
            .setTargetResolution(resolution)
            .setTargetRotation(rotation)
            .build()

        mPreview?.setSurfaceProvider(preview_view.surfaceProvider)

        val cameraProvider = cameraProviderFuture.get()
        cameraProvider.bindToLifecycle(this, cameraSelector, mPreview)
    }
}