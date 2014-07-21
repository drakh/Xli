import java.io.IOException;
import java.io.InputStream;

import android.os.AsyncTask;

public class AsyncInputStreamToBytesTask extends AsyncTask<Object, Void, Void> {
	public long requestPointer;
	@Override
	protected Void doInBackground(Object... params) {
		requestPointer = (long)((Long)params[1]);
		try {
			InputStream stream = (InputStream)params[0];

			int nRead;
			int runningTotal = 0;
			int progressThreshold = 30000;
			int steps = 1;
			byte[] data = new byte[16384];
			while ((nRead = stream.read(data, 0, data.length)) != -1) {
				XliJ.XliJ_HttpBytesDownloadedCallback(data, nRead, requestPointer);
				runningTotal+=nRead;
				if (runningTotal/progressThreshold > steps)
				{
					steps = runningTotal/progressThreshold;
					XliJ.XliJ_HttpProgressCallback(requestPointer, runningTotal, 0, false, 1);
				}
			}			
		} catch (IOException e) {				
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException: "+e.getLocalizedMessage());
		}
		return null;
	}
	@Override
	protected void onPostExecute(Void ignore)
	{    	
		XliJ.XliJ_HttpBytesDownloadedCallback(null, -1, requestPointer);
	}
}