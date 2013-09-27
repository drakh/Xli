import android.app.AlertDialog;
import android.app.NativeActivity;
import android.content.DialogInterface;
import android.os.ConditionVariable;
import android.util.Log;

public class XliJ extends android.app.NativeActivity {

	//===========	
	
	public static void makeNoise() { Log.e("XLI", "************ Noise! ************"); }
	
	//===========
	
	public static int ShowMessageBox(NativeActivity activity, CharSequence caption, CharSequence message, int buttons, int hints)
	{
		final ConditionVariable bufferLock = new ConditionVariable();
    	final AlertDialog.Builder b = new AlertDialog.Builder(activity);
    	final int result[] = {-1};
    	
    	b.setTitle(caption);
    	b.setMessage(message);
    	
    	switch (buttons) {
		case 0:
	    	b.setPositiveButton("OK", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 0; bufferLock.open(); }
	    	});		
			break;
		case 1:
	    	b.setPositiveButton("OK", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 0; bufferLock.open(); }
	    	});
	    	b.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 1; bufferLock.open(); }
	    	});  
			break;	
		case 2:
	    	b.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 2; bufferLock.open(); }
	    	});
	    	b.setNegativeButton("No", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 3; bufferLock.open(); }
	    	});  
			break;
		case 3:
	    	b.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 2; bufferLock.open(); }
	    	});
	    	b.setNegativeButton("No", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 3; bufferLock.open(); }
	    	});
	    	b.setNeutralButton("Cancel", new DialogInterface.OnClickListener() {
	    		public void onClick(DialogInterface dialog, int which) { result[0] = 1; bufferLock.open(); }
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
    		 activity.runOnUiThread(new Runnable() {
    			  public void run() { b.show(); }
    				});
    		bufferLock.block();
    	} catch (Exception e) {
    		Log.e("XLI", e.getMessage());
    	}
    	return result[0];
	}
	
	//===========
	
}






































