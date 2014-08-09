package com.xli.glwindow;

public class GLWindow extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("Xli");
        System.loadLibrary("XliPlatform");
        System.loadLibrary("XliGL");
        System.loadLibrary("GLWindow");
    }
}
