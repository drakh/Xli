package com.xli.glwindow;

public class GLWindow extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("Xli");
        System.loadLibrary("XliGL");
        System.loadLibrary("XliMedia");
        System.loadLibrary("XliHttp");
        System.loadLibrary("XliGLHelper");
        System.loadLibrary("GLWindow");
    }
}
