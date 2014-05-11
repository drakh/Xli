
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.CookieHandler;
import java.net.CookieManager;
import java.net.CookiePolicy;
import java.nio.ByteBuffer;
import java.util.HashMap;

import android.app.NativeActivity;
import android.os.AsyncTask;


public class HttpHelper {
    @SuppressWarnings({ "rawtypes", "unchecked" })
	public static AsyncTask SendHttpAsync(NativeActivity activity, String url, String method,
    								 	HashMap<String,String> headers, ByteBuffer body,
    								 	int timeout, long requestPointer) {
    	try
    	{
    		AsyncTask task = new ASyncHttpRequest();
    		byte[] data = null;
    		if (body!=null) data = body.array();
    		((AsyncTask<Object, Void, HttpWrappedResponse>)(task)).execute(url, method, headers, (Integer)timeout, data, (Long)requestPointer);
    		return task;
    	} catch (Exception e) {
    		XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "Unable to build Async Http Request: "+e.getLocalizedMessage());
    		return null;
    	}
    }

    @SuppressWarnings({ "rawtypes", "unchecked" })
	public static AsyncTask SendHttpStringAsync(NativeActivity activity, String url, String method,
    								 			HashMap<String,String> headers, String body,
    								 			int timeout, long requestPointer) {
    	try
    	{
    		AsyncTask task = new ASyncHttpRequest();
    		byte[] data = null;
    		if (body!=null) data = body.getBytes();
    		((AsyncTask<Object, Void, HttpWrappedResponse>)(task)).execute(url, method, headers, (Integer)timeout, data, (Long)requestPointer);
    		return task;
    	} catch (Exception e) {
    		XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "Unable to build Async Http Request: "+e.getLocalizedMessage());
    		return null;
    	}
    }
	
    public static byte[] ReadAllBytesFromHttpInputStream(InputStream stream, long requestPointer) throws IOException
    {    	
		ByteArrayOutputStream buffer = new ByteArrayOutputStream();
		int nRead;
		int runningTotal = 0;
		int progressThreshold = 8000;
		int steps = 1;
		byte[] data = new byte[16384];
		while ((nRead = stream.read(data, 0, data.length)) != -1) {
		  buffer.write(data, 0, nRead);
		  runningTotal+=nRead;
		  if (runningTotal/progressThreshold > steps)
		  {
			  steps = runningTotal/progressThreshold;
			  XliJ.XliJ_HttpProgressCallback(requestPointer, runningTotal, 0, false, 1);
		  }
		}
		buffer.flush();
		return buffer.toByteArray();
    }
    
    public static void InitDefaultCookieManager()
    {
        CookieManager cookieManager = new CookieManager(null, CookiePolicy.ACCEPT_ALL);
        CookieHandler.setDefault(cookieManager);
    }
}
