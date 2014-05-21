import android.annotation.SuppressLint;
import android.app.NativeActivity;
import android.content.Context;
import android.text.InputType;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;

@SuppressLint("ViewConstructor")
public class Hidden extends View implements View.OnKeyListener { //used to extend view
    InputConnection fic;
    NativeActivity activity;

    public Hidden(Context context, NativeActivity activity) {        	
        super(context);
        this.activity = activity;
        init(context);
        setFocusableInTouchMode(true);
        setFocusable(true);
    }

    private void init(Context context) {
        //this.context = context;
        this.setOnKeyListener(this);
        MyEditable.PopulateDummyString();
    }
    
    @Override
    public boolean onKeyPreIme(int keyCode, KeyEvent keyEvent) {
        if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE)
        {
            if (keyEvent.getAction()==KeyEvent.ACTION_DOWN)
            {
            	if (keyCode == KeyEvent.KEYCODE_BACK) XliJ.hideKeyboard();
            	XliJ.XliJ_OnKeyDown(keyCode);
            } else if (keyEvent.getAction()==KeyEvent.ACTION_UP) {
            	XliJ.XliJ_OnKeyUp(keyCode);
            }
            return true;
        }
        return super.onKeyPreIme(keyCode, keyEvent);
    }
    @Override
    public boolean onKey(View view, int keyCode, KeyEvent keyEvent) {
        if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE)
        {
            if (keyEvent.getAction()==KeyEvent.ACTION_DOWN)
            {
                if (keyCode == KeyEvent.KEYCODE_DEL) { XliJ.XliJ_OnKeyDown(KeyEvent.KEYCODE_DEL); }
                XliJ.XliJ_OnKeyDown(keyCode);
            } else if (keyEvent.getAction()==KeyEvent.ACTION_UP) {
                if (keyCode == KeyEvent.KEYCODE_DEL) { XliJ.XliJ_OnKeyUp(KeyEvent.KEYCODE_DEL); }
                XliJ.XliJ_OnKeyUp(keyCode);
            }
            return true;
        }
        return super.onKeyPreIme(keyCode, keyEvent);
    }

    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        fic = new HiddenInputConnection(this, false);
        outAttrs.actionLabel = null;
        //outAttrs.inputType = InputType.TYPE_NULL;
        outAttrs.imeOptions = EditorInfo.IME_ACTION_NEXT;
        return fic;
    }

    @Override
    public boolean onCheckIsTextEditor() { return true; }
}
