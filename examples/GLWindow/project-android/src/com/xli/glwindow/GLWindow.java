package com.xli.glwindow;

import android.app.AlertDialog;
import android.content.DialogInterface;

public class GLWindow extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("GLWindow");
    }
    
	public int showMessageBox(String message, String caption, int buttons, int hints)
	{
		// TODO: Not working
		
		AlertDialog b = new AlertDialog.Builder(this)
			.setTitle(caption)
			.setMessage(message)
			.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			    public void onClick(DialogInterface dialog, int whichButton) {
			    }
			})
			.create();
		
		b.show();
		
		return 0;
	}
}
