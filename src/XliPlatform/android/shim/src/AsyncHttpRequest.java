import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;

import android.os.AsyncTask;
import android.util.Log;

public class AsyncHttpRequest extends AsyncTask<Object, Integer, HttpWrappedResponse> {
	long requestPointer;
	boolean verifyHost;
	
	@Override
	protected HttpWrappedResponse doInBackground(Object... params) {
		String url = (String)params[0];
		String method = (String)params[1];
		@SuppressWarnings("unchecked")
		HashMap<String,String> headers = (HashMap<String,String>)params[2];
		int timeout = (Integer)params[3];
		byte[] body = (byte[])params[4];
		requestPointer = (Long)params[5];
		verifyHost = (Boolean)params[6];
		String[] responseHeaders;
		boolean hasUploadContent = (body != null);
		HttpURLConnection connection = null;
		if (this.isCancelled()) { XliJ.XliJ_HttpAbortedCallback(requestPointer); return null; }
		try {
			connection = NewHttpConnection(url,method,hasUploadContent,timeout,requestPointer, verifyHost);
			if (connection==null) {
				return new HttpWrappedResponse(null, new String[0], -1, "JavaError (NewHttpConnection): Could not make connection", requestPointer);
			}
		} catch (Exception e) {
			XliJ.XliJ_JavaThrowError(-1, "JavaError (NewHttpConnection): Could not make connection. Check Android permissions");
			return null;
		}
		try {        		
			//set headers
			Iterator<Map.Entry<String, String>> it = headers.entrySet().iterator();
			while (it.hasNext()) {
				Map.Entry<String, String>pair = (Map.Entry<String, String>)it.next();
				connection.addRequestProperty(pair.getKey(), pair.getValue());
			}
			if (this.isCancelled()) { XliJ.XliJ_HttpAbortedCallback(requestPointer); return null; }
			//set content payload
			if (hasUploadContent)
			{
				if (body!=null)
				{
					int progressThreshold = Math.max((body.length / 100), 2048);
					int steps = 1;
					int runningTotal=0;
					int bufferSize = 2048;
					try {
						connection.setFixedLengthStreamingMode(body.length);
						BufferedOutputStream out = new BufferedOutputStream(connection.getOutputStream());

						while (runningTotal<body.length) {
							if (this.isCancelled()) { XliJ.XliJ_HttpAbortedCallback(requestPointer); return null; }
							out.write(body, (int)runningTotal, (int)Math.min(bufferSize, (body.length-runningTotal)));
							if ((runningTotal / progressThreshold) > steps) {
								steps = (runningTotal / progressThreshold);
								publishProgress(runningTotal,body.length);
							}        							
							runningTotal+=bufferSize;
						}
						publishProgress(body.length,body.length);
						out.flush();
					} catch(Exception e) {
						XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "Unable to upload data: "+e.getLocalizedMessage());
					}
				}
			}

			//get result payload
			BufferedInputStream stream_b = new BufferedInputStream(connection.getInputStream());
			responseHeaders = HeadersToStringArray(connection);        		
			return new HttpWrappedResponse(stream_b, responseHeaders, connection.getResponseCode(), connection.getResponseMessage(), requestPointer);
		} catch (SocketTimeoutException e) {
			XliJ.XliJ_HttpTimeoutCallback(requestPointer);
			return null;
		} catch (IOException e) {
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
			return null;
		}
	}
	@Override
	protected void onProgressUpdate(Integer... progress) {
		XliJ.XliJ_HttpProgressCallback(requestPointer, progress[0], progress[1], true, 0);
	}
	@Override
	protected void onPostExecute(HttpWrappedResponse result)
	{
		if (result!=null)
			XliJ.XliJ_HttpCallback(XliJ.HoldObject(result.body), result.headers, result.responseCode, result.responseMessage, result.functionPointer);
	}
	
    //[TODO] Could optimize by changing chunk mode if length known
	public static HttpURLConnection NewHttpConnection(String url, String method, boolean hasPayload, int timeout, long requestPointer, boolean verifyHost)
    {
        URL j_url = null;
        try {
            j_url = new URL(url);
        } catch (MalformedURLException e) {
        	XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "Malformed URL: "+e.getLocalizedMessage());
            return null;
        }
        HttpURLConnection urlConnection = null;

        try {
        	if (j_url.getProtocol().toLowerCase(Locale.ENGLISH).equals("https") && !verifyHost) {		
    			Log.d("XliApp","Non-verified Connection");
    			HttpsURLConnection uc = (HttpsURLConnection)j_url.openConnection();            		
    	       	// Install the all-trusting trust manager
    	    	try {
    	    		SSLContext sc = SSLContext.getInstance("TLS");
    	    		sc.init(null, HttpHelper.trustAllCerts, new java.security.SecureRandom());
    	    		uc.setSSLSocketFactory(sc.getSocketFactory());
    	    		uc.setHostnameVerifier(HttpHelper.DO_NOT_VERIFY);
    	    	} catch (Exception e) {
    	    		e.printStackTrace();
    	    	}
    	    	urlConnection = uc;          
        	} else {
        		Log.d("XliApp","Default Connection");
        		urlConnection = (HttpURLConnection)j_url.openConnection();
        	}
        	  	
            urlConnection.setConnectTimeout(timeout);
            urlConnection.setDoOutput(hasPayload);
            urlConnection.setRequestMethod(method);
        } catch (IOException e) {
        	XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
            return null;
        }
        return urlConnection;
    }
	
    public static String[] HeadersToStringArray(HttpURLConnection connection)
    {
    	ArrayList<String> headers = new ArrayList<String>();
    	try {
	    	String separator = ",";
	        Map<String, List<String>> a = connection.getHeaderFields();
	
	        for (String key : a.keySet()) {
	        	if (key != null) {
	        		headers.add(key);
	        	} else {
	        		headers.add("NULL");
	        	}
	        	StringBuilder sb = new StringBuilder();
	            String sep = "";
	            for(String s: a.get(key)) {
	                sb.append(sep).append(s);
	                sep = separator;
	            }
	            headers.add(sb.toString());
	
	            String[] result = headers.toArray(new String[headers.size()]);	
	            return result;
	        }
    	} catch (Exception e) {
    		XliJ.XliJ_JavaThrowError(-1,"Error in HeadersToStringArray: "+e.getLocalizedMessage());
    	}
        return null;
    }
}