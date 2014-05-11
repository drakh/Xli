import java.io.IOException;
import java.io.InputStream;

import android.os.AsyncTask;

public class ASyncInputStreamToBytesTask extends AsyncTask<Object, Void, byte[]> {
	public long requestPointer;
    @Override
    protected byte[] doInBackground(Object... params) {
    	requestPointer = (long)((Long)params[1]);
        try {
			return XliJ.ReadAllBytesFromHttpInputStream((InputStream)params[0], requestPointer);
		} catch (IOException e) {				
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
			return null;
		}
    }
    @Override
    protected void onPostExecute(byte[] result)
    {    		
    	XliJ.XliJ_HttpContentByteArrayCallback(result, requestPointer);
    }
}
