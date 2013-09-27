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
    	b.setPositiveButton("positive", new DialogInterface.OnClickListener() {
    		public void onClick(DialogInterface dialog, int which) { 
    			result[0] = 1;
    			bufferLock.open();
    		}
    	});
    	b.setNegativeButton("negative", new DialogInterface.OnClickListener() {
    		public void onClick(DialogInterface dialog, int which) { 
    			result[0] = 2;
    			bufferLock.open();
    		}
    	});  
    	try {
    		 activity.runOnUiThread(new Runnable() {
    			  public void run() {
    				  AlertDialog d = b.create();
    				  d.setOnDismissListener(new DialogInterface.OnDismissListener() {
    					  @Override
    					  public void onDismiss(final DialogInterface dialog) {
    						  result[0]=3;
    						  bufferLock.open();
    					  }});
    				  d.show();
    				  }
    				});
    		bufferLock.block();
    	} catch (Exception e) {
    		Log.e("XLI", e.getMessage());
    	}
    	return result[0];
	}
	
	//===========
	
}






































