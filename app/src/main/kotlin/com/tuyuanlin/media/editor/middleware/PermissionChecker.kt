package com.tuyuanlin.media.editor.middleware

import android.app.Activity
import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat


object PermissionChecker {
    fun checkAndRequestPermission(activity: Activity, permission: String) {
        if (ContextCompat.checkSelfPermission(activity, permission) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(activity, arrayOf(permission), 0)
        }
    }
}