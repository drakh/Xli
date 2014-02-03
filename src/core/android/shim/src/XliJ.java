import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.CookieHandler;
import java.net.CookiePolicy;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.nio.ByteBuffer;

import android.app.AlertDialog;
import android.app.NativeActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.net.ConnectivityManager;
import android.content.res.AssetManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.ConditionVariable;
import android.text.InputType;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import java.net.CookieManager;

import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;

@SuppressWarnings("unused")
public class XliJ extends android.app.NativeActivity {
    static Hidden hidden_text;
    protected static ViewGroup hidden_layout;
    
    public static int AttachHiddenView(final NativeActivity activity) 
    {
        final int[] result = {1};
        Log.d("XliApp","Initialising shim on Java side");
        if (hidden_layout == null) 
        {
            hidden_layout = new FrameLayout(activity);
            activity.runOnUiThread(new Runnable() { public void run() {     
                try {
                    activity.setContentView(hidden_layout);
                    hidden_text = new Hidden(activity);
                    hidden_layout.addView(hidden_text);
                    hidden_text.setVisibility(View.VISIBLE);
                    hidden_text.requestFocus();
                    Log.i("XliApp","Successfully created input capture View.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to create Layout or View for input capture.");
                    result[0] = 0;
                }
            }});
        }
        return result[0];
    }
    
    //===========    
    
    public static void makeNoise() { Log.e("XliApp", "************ Noise! ************"); }
    
    //===========
    
    public static void raiseKeyboard(final NativeActivity activity) {
        if (hidden_text == null) 
        { 
            Log.e("XliApp","Hidden View not available"); 
            return;
        }
        activity.runOnUiThread(new Runnable() { public void run() {     
            try {
                InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(hidden_text, 0);
            } catch (Exception e) {
                Log.e("XliApp","Unable to get get resources to raise keyboard");
            }
        }});
    }
    
    public static native void XliJ_OnKeyUp(int keyCode);
    public static native void XliJ_OnKeyDown(int keyCode);
    public static native void XliJ_OnTextInput(String keyCode);
    public static native void XliJ_HttpCallback(Object body, String[] headers, int responseCode, String responseMessage, long functionPointer);
    
    public static class Hidden extends View {
        InputConnection fic;

        public Hidden(Context context) {
            super(context);
            setFocusableInTouchMode(true);
            setFocusable(true);
        }
     
        @Override
        public boolean onKeyPreIme(int keyCode, KeyEvent event) {
            if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE)
            {
                if (event.getAction()==KeyEvent.ACTION_DOWN)
                {
                    XliJ_OnKeyDown(keyCode);
                } else if (event.getAction()==KeyEvent.ACTION_UP) {
                    XliJ_OnKeyUp(keyCode);
                }
                return true;
            }
            return super.onKeyPreIme(keyCode, event);
        }
        
        @Override
        public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
            fic = new HiddenInputConnection(this, false);
            outAttrs.actionLabel = null;
            outAttrs.inputType = InputType.TYPE_NULL;
            outAttrs.imeOptions = EditorInfo.IME_ACTION_NEXT;
            return fic;
        }

        @Override
        public boolean onCheckIsTextEditor() { return true; }
    }

    static class HiddenInputConnection extends BaseInputConnection {

        public HiddenInputConnection(View targetView, boolean fullEditor) {
            super(targetView, fullEditor);
        }

        @Override
        public String getTextBeforeCursor(int n, int flags) {
            //http://code.google.com/p/android/issues/detail?id=62306
            return "_";
        }

        @Override
        public boolean deleteSurroundingText (int beforeLength, int afterLength) {
            if (beforeLength == 1 && afterLength == 0) {
                XliJ_OnKeyDown(KeyEvent.KEYCODE_DEL);
                XliJ_OnKeyUp(KeyEvent.KEYCODE_DEL);
                return true;
            }
            return false;
        }

        @Override
        public boolean sendKeyEvent(KeyEvent event) {
            int keyCode = event.getKeyCode();
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                XliJ_OnKeyDown(keyCode);
                return false;
            } else if (event.getAction() == KeyEvent.ACTION_UP) {
                XliJ_OnKeyUp(keyCode);
                String text = event.getCharacters();
                char unicodeChar = (char)event.getUnicodeChar();
                XliJ_OnTextInput(""+unicodeChar);
                return false;
            } else if (event.getAction() == KeyEvent.ACTION_MULTIPLE){
                String text = event.getCharacters();
                if (text != null) 
                {
                    XliJ_OnTextInput(text);
                } else {
                    char unicodeChar = (char)event.getUnicodeChar();
                    XliJ_OnTextInput(""+unicodeChar);
                }
                return false;
            }
            return false;
        }
    }
    
    public static void hideKeyboard(final NativeActivity activity) {   
        activity.runOnUiThread(new Runnable() { public void run() {                 
            InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(activity.getWindow().getDecorView().getWindowToken(), 0);
        }});
    }
    
    //===========
    
    public static int ShowMessageBox(NativeActivity activity, CharSequence caption, CharSequence message, int buttons, int hints)
    {
        final ConditionVariable bufferLock = new ConditionVariable();
        final AlertDialog.Builder b = new AlertDialog.Builder(activity);
        final int result[] = {-1};
        
        b.setTitle(caption);
        b.setMessage(message);
        b.setCancelable(false);
        
        switch (hints) {
        case 0:
            b.setIcon(android.R.drawable.stat_notify_error);
            break;
        case 1:
            b.setIcon(android.R.drawable.btn_star_big_on);
            break;
        case 2:
            b.setIcon(android.R.drawable.stat_sys_warning);
            break;           
        }
        
        
        switch (buttons) {
        case 1:
            b.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 1; bufferLock.open(); }
            });          
        case 0:
            b.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 0; bufferLock.open(); }
            });        
            break;
        case 3:
            b.setNeutralButton("Cancel", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 1; bufferLock.open(); }
            });            
        case 2:
            b.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 2; bufferLock.open(); }
            });
            b.setNegativeButton("No", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 3; bufferLock.open(); }
            });  
            break;
        case 4:
            b.setPositiveButton("Continue", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 5; bufferLock.open(); }
            });
            b.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 1; bufferLock.open(); }
            });
            b.setNeutralButton("Try Again", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) { result[0] = 4; bufferLock.open(); }
            });
            break;            
        default:
            break;
        }

        try {
             activity.runOnUiThread(new Runnable() { public void run() { AlertDialog d = b.create(); d.setCanceledOnTouchOutside(false); d.show(); }});
            bufferLock.block();
        } catch (Exception e) {
            Log.e("XliApp", e.getMessage());
        }
        return result[0];
    }
    
    //===========

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
	        		headers.add("-null-"); //{TODO} I need to work out what to do here.
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
    		Log.e("XliApp","Error in HeadersToStringArray: "+e.getLocalizedMessage());
    	}
        return null;
    }
    
    static class HttpWrappedResponse {
    	public Object body;
        public long functionPointer;
        public String[] headers;
        public int responseCode;
        public String responseMessage;
        
        HttpWrappedResponse(Object body, String[] headers, int responseCode, String responseMessage, long fpointer)
        {
        	this.body = body;
        	this.functionPointer = fpointer;
        	this.headers = headers;
        	this.responseCode = responseCode;
        	this.responseMessage = responseMessage;
        }
    }
    
    public static boolean ConnectedToNetwork(NativeActivity activity)
    {
        ConnectivityManager connMgr = (ConnectivityManager)activity.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
        if(networkInfo == null) { return false; } 
        return networkInfo.isConnected();
    }
    
    public static class ASyncHttpRequest extends AsyncTask<Object, Void, HttpWrappedResponse> {
        @Override
        protected HttpWrappedResponse doInBackground(Object... params) {
            String url = (String)params[0];
            String method = (String)params[1];
            @SuppressWarnings("unchecked")
			HashMap<String,String> headers = (HashMap<String,String>)params[2];
            int timeout = (Integer)params[3];
            ByteBuffer body = (ByteBuffer)params[4];
            long callbackPointer = (Long)params[5];
            String[] responseHeaders;
            boolean hasUploadContent = (body != null);
            HttpURLConnection connection = NewHttpConnection(url,method,hasUploadContent,timeout);
            if (connection==null) {
            	return new HttpWrappedResponse(null, new String[0], -1, "JavaError (NewHttpConnection): Could not make connection", callbackPointer);
            }
        	try {        		
        		//set headers
        		Iterator<Map.Entry<String, String>> it = headers.entrySet().iterator();
        		while (it.hasNext()) {
        			Map.Entry<String, String>pair = (Map.Entry<String, String>)it.next();
        			connection.addRequestProperty(pair.getKey(), pair.getValue());
        		}
        		//set content payload
        		if (hasUploadContent)
        		{
        			Log.d("XliApp","uploading content");
        			byte[] data = null;
        			boolean converted = false;
        			try {        				
        				data = new byte[body.capacity()];
        				body.get(data);
        				converted = true;
        			} catch (Exception e) {
        				Log.e("XliApp","Bugger me: "+e.getLocalizedMessage());
        			}
        			if (converted && (data!=null))
        			{
        				try {        				
        					connection.setFixedLengthStreamingMode(data.length);
        					BufferedOutputStream out = new BufferedOutputStream(connection.getOutputStream());        				
        					out.write(data);
        					out.flush();
        				} catch(Exception e) {
        					Log.e("XliApp","Couldnt make output: "+e.getLocalizedMessage());
        				}
        			}
        		}
        		
        		//get result payload
        		BufferedInputStream stream_b = new BufferedInputStream(connection.getInputStream());
        		responseHeaders = HeadersToStringArray(connection);        		
        		return new HttpWrappedResponse(stream_b, responseHeaders, connection.getResponseCode(), connection.getResponseMessage(), callbackPointer);
			} catch (IOException e) {
				Log.e("XliApp","IOException: "+e.getLocalizedMessage());
				return new HttpWrappedResponse(null, new String[0], -1, "JavaError:"+e.getLocalizedMessage(), callbackPointer);	
			}
        }
        @Override
        protected void onPostExecute(HttpWrappedResponse result) 
        {
    		XliJ_HttpCallback(result.body, result.headers, result.responseCode, result.responseMessage, result.functionPointer);
        }
    }
    
    //{TODO} Fix all these crap messages
    @SuppressWarnings({ "rawtypes", "unchecked" })
	public static AsyncTask SendHttpAsync(NativeActivity activity, String url, String method, 
    								 HashMap<String,String> headers, ByteBuffer body, 
    								 int timeout, long callbackPointer) {
    	try
    	{
    		Log.e("XliApp", url+", "+method+", "+headers+", "+body+", "+callbackPointer);
    		AsyncTask task = new ASyncHttpRequest();
    		((AsyncTask<Object, Void, HttpWrappedResponse>)(task)).execute(url, method, headers, (Integer)timeout, body, (Long)callbackPointer);
    		return task;
    	} catch (Exception e) {
    		Log.e("XliApp","Unable to build Async Http Request: "+e.getLocalizedMessage());
    		return null;
    	}
    }
    
    @SuppressWarnings("rawtypes")
	public static void AbortAsyncTask(AsyncTask task)
    {
    	task.cancel(true);
    }
    
    //[TODO] Could optimize by changing chunk mode if length known
    public static HttpURLConnection NewHttpConnection(String url, String method, boolean hasPayload, int timeout) 
    {
        URL j_url = null;
        try {
            j_url = new URL(url);
        } catch (MalformedURLException e) {
            Log.e("XliApp","MalformedUrl: "+e.getLocalizedMessage());
            return null;
        }
        HttpURLConnection urlConnection = null;
        
        try {
            urlConnection = (HttpURLConnection)j_url.openConnection(); 
            urlConnection.setConnectTimeout(timeout);
            urlConnection.setDoOutput(hasPayload);            
            urlConnection.setRequestMethod(method);
        } catch (IOException e) {
            Log.e("XliApp","IOException (NewHttpConnection): "+e.getLocalizedMessage());
            return null;
        }    
        return urlConnection;
    }
    
    public static String InputStreamToString(InputStream stream) throws IOException, UnsupportedEncodingException 
    {
        @SuppressWarnings("resource")
		java.util.Scanner s = new java.util.Scanner(stream).useDelimiter("\\A");
        return s.hasNext() ? s.next() : "";
    }

    
    public static byte[] ReadBytesFromInputStream(BufferedInputStream stream, int bytesToRead)
    {    	
    	byte[] buffer = new byte[bytesToRead];
    	try {
			int bytesRead = stream.read(buffer);
			if (bytesRead>-1)
			{
				return Arrays.copyOf(buffer, bytesRead);
			} else { 
				return null;
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.e("XliApp", "read from stream crashed: "+e.getLocalizedMessage());
			return null;
		}
    }
    		
    public static InputStream HttpGetInputStream(HttpURLConnection connection)
    {
        try {
            return new BufferedInputStream(connection.getInputStream());
        } catch (IOException e) {
            Log.e("XliApp","HttpGetInputStream IOException: "+e.getLocalizedMessage());
            return null;
        }
    }
    
    public static OutputStream HttpGetOutputStream(HttpURLConnection connection)
    {
        try {
            return new BufferedOutputStream(connection.getOutputStream());
        } catch (IOException e) {
            Log.e("XliApp","HttpGetOutputStream IOException: "+e.getLocalizedMessage());
            return null;
        }
    }
    
    public static void HttpShowHeaders(HttpURLConnection connection)
    {
        Map<String, List<String>> a = connection.getHeaderFields();
        for (String key : a.keySet()) {
            for (String header : a.get(key)) {
                Log.e("XliApp", key + " : " + header);
            }
        }
    }
    
    public static void InitDefaultCookieManager()
    {
        CookieManager cookieManager = new CookieManager(null, CookiePolicy.ACCEPT_ALL);
        CookieHandler.setDefault(cookieManager);
    }

    public static AssetManager GetAssetManager(NativeActivity activity)
    {
        return activity.getAssets();
    }
}
