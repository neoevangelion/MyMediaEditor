package com.tuyuanlin.media.editor.middleware;

import android.view.Surface;

public class Player {
  static {
    System.loadLibrary("player-lib");
  }

  public native void playVideo(String filePath, Surface surfaceView);
}
