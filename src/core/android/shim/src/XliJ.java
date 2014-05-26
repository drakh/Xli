import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.util.HashMap;

import android.annotation.SuppressLint;
import android.app.NativeActivity;
import android.content.Context;
import android.content.res.AssetManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.util.DisplayMetrics;
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
    public static native void XliJ_HttpContentByteArrayCallback(byte[] result, long requestPointer);
    public static native void XliJ_HttpTimeoutCallback(long requestPointer);
    public static native void XliJ_HttpProgressCallback(long requestPointer, long position, long totalLength, boolean lengthKnown, int direction);
    public static native void XliJ_HttpErrorCallback(long requestPointer, int errorCode, String errorMessage);
    public static native void XliJ_JavaThrowError(int code, String throwMessage);
	
    //--------------------------------------------
    // System
    public static void hideStatusBar() {
    	SystemHelper.hideStatusBar(nActivity);
    }
    public static void showStatusBar() {
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
    	VibratorHelper.VibrateForMilliseconds(nActivity, milliseconds);
    }
    
    
    //--------------------------------------------
    // Keyboard   
    public static void raiseKeyboard() {
        KeyboardHelper.RaiseKeyboard(nActivity);
    }
    public static int GetKeyboardSize()
    {
        return KeyboardHelper.GetKeyboardSize();
    }
    public static void hideKeyboard() {
        KeyboardHelper.hideKeyboard(nActivity);
    }
    public static int AttachHiddenView()
    {    	
    	return KeyboardHelper.AttachHiddenView(nActivity);     
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
	public static int SendHttpAsync(String url, String method,
                                          HashMap<String,String> headers, ByteBuffer body,
                                          int timeout, long requestPointer) {
		if (headers==null) headers = new HashMap<String,String>();
    	return HoldObject(HttpHelper.SendHttpAsync(url, method, headers, body, timeout, requestPointer));
    }
	public static int SendHttpStringAsync(String url, String method,
    								 			HashMap<String,String> headers, String body,
    								 			int timeout, long requestPointer) {
		if (headers==null) headers = new HashMap<String,String>();
    	return HoldObject(HttpHelper.SendHttpStringAsync(url, method, headers, body, timeout, requestPointer));    	
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
        return HoldObject(StreamHelper.AsyncInputStreamToString((InputStream)GetObject(stream), requestPointer));
    }
	public static int AsyncInputStreamToByteArray(int stream, long requestPointer)
    {
        return HoldObject(StreamHelper.AsyncInputStreamToByteArray((InputStream)GetObject(stream), requestPointer));
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
    public static int _objKey=0; //This must not be -1 as otherwise the first held object has the key 0 and 0 is null in cpp.
    public static int HoldObject(Object obj)
    {
    	if (obj==null) return -1;
    	_objKey+=1;
    	_objStore.put(_objKey, obj);
    	return _objKey;
    }
    public static Object GetObject(int key)
    {
    	if (key==-1) return null;
    	return _objStore.get(key);    			
    }
    public static boolean TryReleaseObject(int key)
    {
    	if (_objStore.get(key)==null) return false;
    	_objStore.remove(key);
    	return true;
    }
}






