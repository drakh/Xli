import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;

import android.os.AsyncTask;

public class AsyncInputStreamToStringTask extends AsyncTask<Object, Void, String> {
	public long requestPointer;
    @Override
    protected String doInBackground(Object... params) {
    	requestPointer = (long)((Long)params[1]);
        try {
			return XliJ.InputStreamToString((InputStream)params[0]);
		} catch (UnsupportedEncodingException e) {
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "UnsupportedEncodingException: "+e.getLocalizedMessage());
			return null;
		} catch (IOException e) {
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
			return null;
		}
    }
    @Override
    protected void onPostExecute(String result)
    {    		
    	if (result!=null)
    		XliJ.XliJ_HttpContentStringCallback(result, requestPointer);
    }
}
