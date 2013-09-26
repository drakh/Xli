package com.xli.glwindow;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.os.ConditionVariable;
import android.util.Log;

public class GLWindow extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("GLWindow");
    }
    
    public void doit(){
    	GLWindow.this.runOnUiThread( new Runnable() {
		public void run() {
			Builder a = new AlertDialog.Builder(GLWindow.this).setTitle("Delete entry").setMessage("Are you sure you want to delete this entry?");
			AlertDialog b = a.create();
			b.show();
		}});
    }
    
	public void blah()
	{	
		doit();
	}
	
	public int ShowMessageBoxB(CharSequence caption, CharSequence message){
		final ConditionVariable bufferLock = new ConditionVariable();
    	final AlertDialog.Builder b = new AlertDialog.Builder(GLWindow.this);
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
    		 runOnUiThread(new Runnable() {
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
    		//bufferLock.block();
    	} catch (Exception e) {
    		Log.e("XLI", e.getMessage());
    	}
    	return result[0];
	}
}

