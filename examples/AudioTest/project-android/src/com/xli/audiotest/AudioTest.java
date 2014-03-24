package com.xli.audiotest;

public class AudioTest extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("Xli");
        System.loadLibrary("XliGL");
        System.loadLibrary("XliMedia");
        System.loadLibrary("XliGLHelper");
        System.loadLibrary("AudioTest");
    }
}
