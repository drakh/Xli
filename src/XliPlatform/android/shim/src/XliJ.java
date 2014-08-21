/*
 * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;

import android.annotation.SuppressLint;
import android.app.NativeActivity;
import android.content.Context;
import android.content.res.AssetManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.SparseArray;

public class XliJ extends android.app.NativeActivity {
	
	//--------------------------------------------
	// Cached Activity
	public static NativeActivity nActivity;
    public static void CacheActivity(final NativeActivity activity) { nActivity = activity; }
	
	//--------------------------------------------
	// Callbacks to C++ code
    public static native void XliJ_OnKeyUp(int keyCode);
    public static native void XliJ_OnKeyDown(int keyCode);
    public static native void XliJ_OnTextInput(String keyCode);
    public static native void XliJ_HttpCallback(int body, String[] headers, int responseCode, String responseMessage, long requestPointer);
    public static native void XliJ_HttpContentStringCallback(String content, long requestPointer);
    public static native void XliJ_HttpBytesDownloadedCallback(byte[] chunk, int chunkLength, long requestPointer);
    public static native void XliJ_HttpTimeoutCallback(long requestPointer);
    public static native void XliJ_HttpProgressCallback(long requestPointer, long position, long totalLength, boolean lengthKnown, int direction);
    public static native void XliJ_HttpErrorCallback(long requestPointer, int errorCode, String errorMessage);
    public static native void XliJ_HttpAbortedCallback(long requestPointer);
    public static native void XliJ_JavaThrowError(int code, String throwMessage);
    
    //--------------------------------------------
    // System
    public static void HideStatusBar() {
    	SystemHelper.hideStatusBar(nActivity);
    }
    public static void ShowStatusBar() {
    	SystemHelper.showStatusBar(nActivity);
    }
	public static DisplayMetrics GetDisplayMetrics() 
	{
		return SystemHelper.GetDisplayMetrics(nActivity);
    }
	public static float GetStatusBarHeight()
	{
		return SystemHelper.GetStatusBarHeight(nActivity);
	}
    public static AssetManager GetAssetManager()
    {
        return nActivity.getAssets();
    }

    //--------------------------------------------
    // Vibration
    @SuppressLint("NewApi")
	public static boolean HasVibrator()
    {
    	return VibratorHelper.HasVibrator(nActivity);
    }
    public static void VibrateForMilliseconds(int milliseconds)
    {
    	VibratorHelper.VibrateForMilliseconds(milliseconds);    	
    }
    
    
    //--------------------------------------------
    // Keyboard   
    public static void RaiseKeyboard() {
        KeyboardHelper.ShowKeyboard();
    }
    public static int GetKeyboardSize()
    {
        return KeyboardHelper.GetKeyboardSize();
    }
    public static void HideKeyboard() {
        KeyboardHelper.HideKeyboard();
    }
    public static void AttachHiddenView()
    {
    	KeyboardHelper.AttachHiddenView();
    }

    //--------------------------------------------
    // MessageBox
    public static int ShowMessageBox(CharSequence caption, CharSequence message, int buttons, int hints)
    {
        return MessageBoxHelper.ShowMessageBox(nActivity, caption, message, buttons, hints);
    }

    //--------------------------------------------
    // Network    
    public static boolean ConnectedToNetwork()
    {
        ConnectivityManager connMgr = (ConnectivityManager)nActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
        if(networkInfo == null) { return false; }
        return networkInfo.isConnected();
    }

    //--------------------------------------------
    // Http
    public static Object GetHeaderMap()
    {
    	return (Object)new HashMap<String,String>();
    }
    
    public static HashMap<String,String> StringToHeadersMap(String headers)
    {
    	//{TODO} Add validation/sanity-checks for headers
    	HashMap<String,String> h = new HashMap<String,String>();    	    	
    	if (headers.trim().length()==0) return h;
    	
    	for (String pair : headers.split("\n"))
    	{
    		String[] kv = pair.split(":",2);
    		if (kv.length>1)
    		{
    			h.put(kv[0], kv[1]);
    			Log.e("XliApp", kv[0]+":"+kv[1]);
    		} else if (kv.length == 1) {
    			h.put(kv[0], "");
    			Log.e("XliApp", kv[0]+":*null*");        			
    		}    		
    	}
    	return h;
    }
    
    public static int SendHttpAsync(String url, String method, String headers, ByteBuffer body,
                                          int timeout, long requestPointer, boolean verifyHost) {
    	return HoldObject(HttpHelper.SendHttpAsync(url, method, StringToHeadersMap(headers), body, timeout, requestPointer, verifyHost));
    }
	
	public static int SendHttpStringAsync(String url, String method, String headers, String body,
    								 			int timeout, long requestPointer, boolean verifyHost) {				
    	return HoldObject(HttpHelper.SendHttpStringAsync(url, method, StringToHeadersMap(headers), body, timeout, requestPointer, verifyHost));    	
    }
    
	public static byte[] ReadAllBytesFromHttpInputStream(InputStream stream, long requestPointer) throws IOException
    {    	
		return HttpHelper.ReadAllBytesFromHttpInputStream(stream, requestPointer);
    }
    
	public static void InitDefaultCookieManager()
    {
        HttpHelper.InitDefaultCookieManager();
    }

    //--------------------------------------------
    // Stream
    public static String InputStreamToString(InputStream stream) throws IOException, UnsupportedEncodingException
    {
		return StreamHelper.InputStreamToString(stream);
    }
	public static int AsyncInputStreamToString(int stream, long requestPointer) throws IOException, UnsupportedEncodingException
    {
        return StreamHelper.AsyncInputStreamToString((InputStream)GetObject(stream), requestPointer);
    }
	public static int AsyncInputStreamToByteArray(int stream, long requestPointer)
    {
		return StreamHelper.AsyncProgressiveInputStreamToByteArray((InputStream)GetObject(stream), requestPointer);
    }
    public static byte[] ReadAllBytesFromInputStream(InputStream stream) throws IOException
    {    	
		return StreamHelper.ReadAllBytesFromInputStream(stream);
    }
    public static byte[] ReadBytesFromInputStream(BufferedInputStream stream, int bytesToRead)
    {    	
    	return StreamHelper.ReadBytesFromInputStream(stream, bytesToRead);
    }

    //--------------------------------------------
    // Async
    @SuppressWarnings("rawtypes")
	public static void AbortAsyncTask(int taskId)
    {
    	AsyncTask atask = (AsyncTask)GetObject(taskId);
    	if (atask==null) return;
    	atask.cancel(true);
    }
    
    //--------------------------------------------
    // Object Store
    public static SparseArray<Object> _objStore = new SparseArray<Object>();
    public static ArrayList<Integer> _objReservedList = new ArrayList<Integer>();
    public static int _objKey=0; //This must not be -1 as otherwise the first held object has the key 0 and 0 is null in cpp.
    public static int HoldObject(Object obj)
    {
    	if (obj==null) return -1;
    	_objKey+=1;
    	_objStore.put(_objKey, obj);
    	return _objKey;
    }
    public static int ReserveObject()
    {
    	_objKey+=1;
    	_objReservedList.add(_objKey);
    	return _objKey;
    }
    public static void PopulateReservedObject(int key, Object obj)
    {
    	if (key <= _objKey && _objStore.get(key)==null && _objReservedList.contains(key))
    	{
    		_objReservedList.remove(_objReservedList.indexOf(key));
    		_objStore.put(key, obj);
    	} else {
    		XliJ_JavaThrowError(-1, "Tried to populate invalid reserved object");
    	}
    }
    public static Object GetObject(int key)
    {
    	if (key==-1) return null;
    	Object result = _objStore.get(key);
    	if (result!=null)
    	{
    		return result;
    	} else {
    		XliJ_JavaThrowError(-1, "Tried to access invalid object from java object store");
    		return null;
    	}    	
    }
    public static boolean TryReleaseObject(int key)
    {
    	if (_objStore.get(key)==null) return false;
    	_objStore.remove(key);
    	return true;
    }
}






