import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.Build;
import android.util.Log;


public class StreamHelper {
   public static String InputStreamToString(InputStream stream) throws IOException, UnsupportedEncodingException
    {
		@SuppressWarnings("resource")
		java.util.Scanner s = new java.util.Scanner(stream).useDelimiter("\\A");
        return s.hasNext() ? s.next() : "";
    }

	public static int AsyncInputStreamToString(final InputStream stream, final long requestPointer) throws IOException, UnsupportedEncodingException
    {
    	final int taskKey = XliJ.ReserveObject();
    	XliJ.nActivity.runOnUiThread(new Runnable() { @SuppressLint("NewApi")
		public void run() {
    		AsyncTask<Object, Void, String> task = new AsyncInputStreamToStringTask();
    		XliJ.PopulateReservedObject(taskKey, task);
    	    if (Build.VERSION.SDK_INT > 10) {    	    	
    	    	task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, stream, requestPointer);
    	    } else {
    	    	task.execute(stream, requestPointer);
    	    }
    	}});
        return taskKey;
    }
	
	public static int AsyncProgressiveInputStreamToByteArray(final InputStream stream, final long requestPointer)
    {
    	final int taskKey = XliJ.ReserveObject();
    	XliJ.nActivity.runOnUiThread(new Runnable() { @SuppressLint("NewApi")
		public void run() {
    		AsyncTask<Object, Void, Void> task = new AsyncInputStreamToBytesTask();
    		XliJ.PopulateReservedObject(taskKey, task);
    	    if (Build.VERSION.SDK_INT > 10) {    	    	
    	    	task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, stream, requestPointer);
    	    } else {
    	    	task.execute(stream, requestPointer);
    	    }
    	}});
        return taskKey;
    }

    public static byte[] ReadAllBytesFromInputStream(InputStream stream) throws IOException
    {    	
		ByteArrayOutputStream buffer = new ByteArrayOutputStream();
		int nRead;
		byte[] data = new byte[16384];
		while ((nRead = stream.read(data, 0, data.length)) != -1) {
		  buffer.write(data, 0, nRead);
		}
		buffer.flush();
		return buffer.toByteArray();
    }


    public static byte[] ReadBytesFromInputStream(BufferedInputStream stream, int bytesToRead)
    {    	
    	byte[] buffer = new byte[bytesToRead];
    	try {
			int bytesRead = stream.read(buffer);
			if (bytesRead>-1)
			{
				return Arrays.copyOf(buffer, bytesRead); //{TODO} only copy in c++?
			} else {
				return null;
			}
		} catch (IOException e) {
			Log.e("XliApp", "read from stream crashed: "+e.getLocalizedMessage());
				return null;	
		}	
    }
}
