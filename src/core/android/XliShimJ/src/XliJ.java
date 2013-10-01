import android.app.AlertDialog;
import android.app.NativeActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.os.ConditionVariable;
import android.text.InputType;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;

public class XliJ extends android.app.NativeActivity {
	static Hidden hidden_text;
    protected static ViewGroup hidden_layout;	
	
	//===========	
	
	public static void makeNoise() { Log.e("XLI", "************ Noise! ************"); }
	
	//===========
	
	public static void raiseKeyboard(final NativeActivity activity) { 
		if (hidden_layout == null) 
		{
			hidden_layout = new FrameLayout(activity);
			activity.runOnUiThread(new Runnable() { public void run() { activity.setContentView(hidden_layout); }});
		}
		activity.runOnUiThread(new Runnable() { public void run() { 	
			try {
				hidden_text = new Hidden(activity);
				hidden_layout.addView(hidden_text);
				hidden_text.setVisibility(View.VISIBLE);
				hidden_text.requestFocus();
		        InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		        imm.showSoftInput(hidden_text, 0);	    
			} catch (Exception e) {
				Log.e("XLI","oh sweet jesus");
			}
		}});
	}
	
	public static native void XliJ_OnKey(int keyCode, int event);
	public static native void XliJ_OnKeyUp(int keyCode, int event);
	public static native void XliJ_OnKeyDown(int keyCode, int event);
	public static native void XliJ_OnKeyMultiple(int keyCode, int event, int count);
	public static native void XliJ_OnKeyLongPress(int keyCode, int event);
	
	public static class Hidden extends View {
	    public Hidden(Context context) {
	        super(context);
	        setFocusableInTouchMode(true); 
	        setOnKeyListener(new OnKeyListener() {
	            public boolean onKey(View v, int keyCode, KeyEvent event) {
	            	XliJ_OnKey(keyCode);
	                return false;
	            }
	        });
	    }
	   
	    
	    @Override
	    public boolean onKeyUp(int keyCode, KeyEvent event) {
	    	XliJ_OnKeyUp(keyCode);
	    	return true;
	    };
	    @Override
	    public boolean onKeyDown(int keyCode, KeyEvent event) {
	    	XliJ_OnKeyDown(keyCode);
	    	return true;
	    };
	    @Override
	    public boolean onKeyMultiple(int keyCode, int count, KeyEvent event) {
	    	XliJ_OnKeyUp(keyCode);
	    	return true;
	    };
	    @Override
	    public boolean onKeyLongPress(int keyCode, KeyEvent event) {
	    	XliJ_OnKeyDown(keyCode);
	    	return true;
	    };
	    
	    @Override
	    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
	        BaseInputConnection fic = new BaseInputConnection(this, false);
	        outAttrs.actionLabel = null;
	        outAttrs.inputType = InputType.TYPE_NULL;
	        outAttrs.imeOptions = EditorInfo.IME_ACTION_NEXT;
	        return fic;
	    }
	    @Override
	    public boolean onCheckIsTextEditor() { Log.d("XLI", "onCheckIsTextEditor"); return true; }
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
    		Log.e("XLI", e.getMessage());
    	}
    	return result[0];
	}
	
	//===========
	
}






































