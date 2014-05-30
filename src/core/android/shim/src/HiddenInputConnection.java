import android.os.Build;
import android.text.Editable;
import android.text.Selection;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;

class HiddenInputConnection extends BaseInputConnection {
    static MyEditable mEditable;
    
    public HiddenInputConnection(View targetView, boolean fullEditor) {
        super(targetView, fullEditor);
    }

    @Override
     public String getTextBeforeCursor(int n, int flags) {
         //http://code.google.com/p/android/issues/detail?id=62306
         return MyEditable.DUMMY;
     }

    @Override
    public Editable getEditable() {
        if (Build.VERSION.SDK_INT < 14)
            return super.getEditable();
        if (mEditable == null) {
        	mEditable = new MyEditable(MyEditable.DUMMY);
            Selection.setSelection(mEditable, MyEditable.DUMMY.length());
        }
        else if (mEditable.length() == 0) {
        	mEditable.append(MyEditable.DUMMY);
            Selection.setSelection(mEditable, MyEditable.DUMMY.length());
        }
        return mEditable;
    }

    @Override
    public boolean deleteSurroundingText (int beforeLength, int afterLength) {
    	//if ((Build.VERSION.SDK_INT < 14) && beforeLength == 1 && afterLength == 0) {
    		if (beforeLength == 1 && afterLength == 0)
    		{
    			XliJ.XliJ_OnKeyDown(KeyEvent.KEYCODE_DEL);
    			XliJ.XliJ_OnKeyUp(KeyEvent.KEYCODE_DEL);
    		return true;} 
    		return false;
    	//}
        //return super.deleteSurroundingText(beforeLength, afterLength);
    }

    @Override
    public boolean sendKeyEvent(KeyEvent event) {
        int keyCode = event.getKeyCode();
        if (event.getAction() == KeyEvent.ACTION_DOWN) {
        	XliJ.XliJ_OnKeyDown(keyCode);
            return false;
        } else if (event.getAction() == KeyEvent.ACTION_UP) {
        	XliJ.XliJ_OnKeyUp(keyCode);
            char unicodeChar = (char)event.getUnicodeChar();
            XliJ.XliJ_OnTextInput(""+unicodeChar);
            return false;
        } else if (event.getAction() == KeyEvent.ACTION_MULTIPLE){
            String text = event.getCharacters();
            if (text != null)
            {
            	XliJ.XliJ_OnTextInput(text);
            } else {
                char unicodeChar = (char)event.getUnicodeChar();
                XliJ.XliJ_OnTextInput(""+unicodeChar);
            }
            return false;
        }
        return false;
    }
}
