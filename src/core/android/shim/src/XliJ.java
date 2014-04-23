import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.Reader;
import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.net.CookieHandler;
import java.net.CookiePolicy;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;
import java.nio.ByteBuffer;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.app.NativeActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.net.ConnectivityManager;
import android.content.res.AssetManager;
import android.graphics.Rect;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Build;
import android.os.ConditionVariable;
import android.text.InputType;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.Pair;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;
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
	// Callbacks to C++ code
    public static native void XliJ_OnKeyUp(int keyCode);
    public static native void XliJ_OnKeyDown(int keyCode);
    public static native void XliJ_OnTextInput(String keyCode);
    public static native void XliJ_HttpCallback(Object body, String[] headers, int responseCode, String responseMessage, long requestPointer);
    public static native void XliJ_HttpContentStringCallback(String content, long requestPointer);
    public static native void XliJ_HttpContentByteArrayCallback(byte[] result, long requestPointer);
    public static native void XliJ_HttpTimeoutCallback(long requestPointer);
    public static native void XliJ_HttpProgressCallback(long requestPointer, long position, long totalLength, boolean lengthKnown, int direction);
    public static native void XliJ_HttpErrorCallback(long requestPointer, int errorCode, String errorMessage);
    public static native void XliJ_JavaThrowError(int code, String throwMessage);
	
    // The shim's state. Try to not any more than the three UI related fields below
    static Hidden hidden_text;
    protected static ViewGroup hidden_layout;
    static int KeyboardSize;
    
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

                try {
                	KeyboardSize = 0;
                	hidden_layout.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener(){
                            public void onGlobalLayout(){
                                int heightDiff = hidden_layout.getRootView().getHeight()- hidden_layout.getHeight();
                                if (heightDiff < 150) heightDiff = 0;
                                KeyboardSize = heightDiff;
                            }
                        });
                    Log.i("XliApp","Successfully attached View Tree Observer.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to attach keyboard height monitor.");
                    result[0] = 0;
                }
            }});
        }
        return result[0];
    }
    
    public static void hideStatusBar(final NativeActivity activity) {
    	Window win = activity.getWindow();
    	win.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        win.addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
    }

    public static void showStatusBar(final NativeActivity activity) {
    	Window win = activity.getWindow();
    	win.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
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

    public static int GetKeyboardSize()
    {
        return (int)KeyboardSize;
    }

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

	@SuppressLint("NewApi")
	public static DisplayMetrics GetDisplayMetrics(final NativeActivity activity) 
	{
		DisplayMetrics metrics = new DisplayMetrics();
    	if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {    		
    		activity.getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
            return metrics;
    	} else {
    		activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
            return metrics;
    	}
    }
	
	public static float GetStatusBarHeight(NativeActivity activity)
	{
		Rect rectangle= new Rect();
		Window window= activity.getWindow();
		window.getDecorView().getWindowVisibleDisplayFrame(rectangle);
		int statusBarHeight = rectangle.top;
		int contentViewTop = window.findViewById(Window.ID_ANDROID_CONTENT).getTop();
		//int titleBarHeight= contentViewTop - statusBarHeight; //dont need this yet but is useful code
		return statusBarHeight;
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
            XliJ_JavaThrowError(-1,"Error in HeadersToStringArray: "+e.getLocalizedMessage());
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
    		XliJ_HttpErrorCallback(requestPointer, -1, "Unable to build Async Http Request: "+e.getLocalizedMessage());
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
    		XliJ_HttpErrorCallback(requestPointer, -1, "Unable to build Async Http Request: "+e.getLocalizedMessage());
    		return null;
    	}
    }

    //{TODO} the longs should be ints really
    public static class ASyncHttpRequest extends AsyncTask<Object, Integer, HttpWrappedResponse> {
    	long requestPointer;
        @Override
        protected HttpWrappedResponse doInBackground(Object... params) {
            String url = (String)params[0];
            String method = (String)params[1];
            @SuppressWarnings("unchecked")
			HashMap<String,String> headers = (HashMap<String,String>)params[2];
            int timeout = (Integer)params[3];
            byte[] body = (byte[])params[4];
            requestPointer = (Long)params[5];
            String[] responseHeaders;
            boolean hasUploadContent = (body != null);
            HttpURLConnection connection = NewHttpConnection(url,method,hasUploadContent,timeout,requestPointer);
            if (connection==null) {
            	return new HttpWrappedResponse(null, new String[0], -1, "JavaError (NewHttpConnection): Could not make connection", requestPointer);
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
        			if (body!=null)
        			{
        				int progressThreshold = Math.max((body.length / 100), 2048);
        				int steps = 1;
        				int runningTotal=0;
        				int bufferSize = 2048;
        				try {
        					connection.setFixedLengthStreamingMode(body.length);
        					BufferedOutputStream out = new BufferedOutputStream(connection.getOutputStream());
        					
        					byte[] data = new byte[bufferSize];
        					while (runningTotal<body.length) {
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
        					XliJ_HttpErrorCallback(requestPointer, -1, "Unable to upload data: "+e.getLocalizedMessage());
        				}
        			}
        		}
        		
        		//get result payload
        		BufferedInputStream stream_b = new BufferedInputStream(connection.getInputStream());
        		responseHeaders = HeadersToStringArray(connection);        		
        		return new HttpWrappedResponse(stream_b, responseHeaders, connection.getResponseCode(), connection.getResponseMessage(), requestPointer);
			} catch (SocketTimeoutException e) {
				XliJ_HttpTimeoutCallback(requestPointer);
				return null;
			} catch (IOException e) {
				XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
				return null;
			}
        }
        @Override
        protected void onProgressUpdate(Integer... progress) {
        	XliJ_HttpProgressCallback(requestPointer, progress[0], progress[1], true, 0);
        }
        @Override
        protected void onPostExecute(HttpWrappedResponse result)
        {
        	if (result!=null)
        		XliJ_HttpCallback(result.body, result.headers, result.responseCode, result.responseMessage, result.functionPointer);
        }
    }

    @SuppressWarnings("rawtypes")
	public static void AbortAsyncTask(AsyncTask task)
    {
    	task.cancel(true);
    }

    //[TODO] Could optimize by changing chunk mode if length known
    public static HttpURLConnection NewHttpConnection(String url, String method, boolean hasPayload, int timeout, long requestPointer)
    {
        URL j_url = null;
        try {
            j_url = new URL(url);
        } catch (MalformedURLException e) {
        	XliJ_HttpErrorCallback(requestPointer, -1, "Malformed URL: "+e.getLocalizedMessage());
            return null;
        }
        HttpURLConnection urlConnection = null;

        try {
            urlConnection = (HttpURLConnection)j_url.openConnection();
            urlConnection.setConnectTimeout(timeout);
            urlConnection.setDoOutput(hasPayload);
            urlConnection.setRequestMethod(method);
        } catch (IOException e) {
        	XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
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

    @SuppressWarnings("rawtypes")
	public static AsyncTask AsyncInputStreamToString(InputStream stream, long requestPointer) throws IOException, UnsupportedEncodingException
    {
        AsyncTask<Object, Void, String> a = new ASyncInputStreamToStringTask();
        a.execute(stream, requestPointer);
        return a;
    }
    public static class ASyncInputStreamToStringTask extends AsyncTask<Object, Void, String> {
    	public long requestPointer;
        @Override
        protected String doInBackground(Object... params) {
        	requestPointer = (long)((Long)params[1]);
            try {
				return InputStreamToString((InputStream)params[0]);
			} catch (UnsupportedEncodingException e) {
				XliJ_HttpErrorCallback(requestPointer, -1, "UnsupportedEncodingException: "+e.getLocalizedMessage());
				return null;
			} catch (IOException e) {
				XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
				return null;
			}
        }
        @Override
        protected void onPostExecute(String result)
        {    		
        	if (result!=null)
        		XliJ_HttpContentStringCallback(result, requestPointer);
        }
    }

    @SuppressWarnings("rawtypes")
	public static AsyncTask AsyncInputStreamToByteArray(InputStream stream, long requestPointer)
    {
        AsyncTask<Object, Void, byte[]> a = new ASyncInputStreamToBytesTask();
        a.execute(stream, requestPointer);
        return a;
    }
    public static class ASyncInputStreamToBytesTask extends AsyncTask<Object, Void, byte[]> {
    	public long requestPointer;
        @Override
        protected byte[] doInBackground(Object... params) {
        	requestPointer = (long)((Long)params[1]);
            try {
				return ReadAllBytesFromHttpInputStream((InputStream)params[0], requestPointer);
			} catch (IOException e) {				
				XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
				return null;
			}
        }
        @Override
        protected void onPostExecute(byte[] result)
        {    		
        	XliJ_HttpContentByteArrayCallback(result, requestPointer);
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
			  XliJ_HttpProgressCallback(requestPointer, runningTotal, 0, false, 1);
		  }
		}
		buffer.flush();
		return buffer.toByteArray();
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

// Haven't got this working yet but will make development so much easier when errors are forwarded to c++

//public static class JNI_ExceptionHandler implements Thread.UncaughtExceptionHandler {
//	@Override  public void uncaughtException(Thread aThread, Throwable aThrowable) {			
//		Log.e("XliApp","UNCAUGHT JAVA EXCEPTION: STACK TRACE:\n"+getStackTrace(aThrowable));
//		XliJ_JavaThrowError(-1, aThrowable.getLocalizedMessage());
//	}
//	private String getStackTrace(Throwable aThrowable) {
//		final Writer result = new StringWriter();
//		final PrintWriter printWriter = new PrintWriter(result);
//		aThrowable.printStackTrace(printWriter);
//		return result.toString();
//	}
//}    
//public static void Init()
//{
//	//Attach uncaught exception handler
//	Thread current = Thread.currentThread();
//	current.setUncaughtExceptionHandler(new JNI_ExceptionHandler());
//}
