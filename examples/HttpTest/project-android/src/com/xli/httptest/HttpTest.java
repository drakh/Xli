package com.xli.httptest;

public class HttpTest extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("Xli");
        System.loadLibrary("XliGL");
        System.loadLibrary("XliMedia");
        System.loadLibrary("XliHttpClient");
        System.loadLibrary("HttpTest");
    }
}
