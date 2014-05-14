
import android.app.NativeActivity;
import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;

public class KeyboardHelper {
	static Hidden hiddenText;
    static int keyboardSize;
    static ViewGroup hiddenLayout;
	
    public static void RaiseKeyboard(final NativeActivity activity) {
        if (hiddenText == null)
        {
            Log.e("XliApp","Hidden View not available");
            return;
        }
        MyEditable.PopulateDummyString();
        activity.runOnUiThread(new Runnable() { public void run() {
            try {
            	hiddenText.setFocusable(true);
                InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(hiddenText, 0);
            } catch (Exception e) {
                Log.e("XliApp","Unable to get get resources to raise keyboard");
            }
        }});
    }

    public static int GetKeyboardSize()
    {
        return (int)keyboardSize;
    }

    public static void hideKeyboard(final NativeActivity activity) {
        activity.runOnUiThread(new Runnable() { public void run() {
            InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(activity.getWindow().getDecorView().getWindowToken(), 0);
            hiddenText.setFocusable(false);
        }});
    }
    
    public static int AttachHiddenView(final NativeActivity activity)
    {    	
    	final int[] result = {1};
        Log.d("XliApp","Initialising shim on Java side");
        if (hiddenLayout == null)
        {
            hiddenLayout = new FrameLayout(activity);
            activity.runOnUiThread(new Runnable() { public void run() {
                try {
                    activity.setContentView(hiddenLayout);
                    hiddenText = new Hidden(activity, activity);
                    hiddenLayout.addView(hiddenText);
                    hiddenText.setVisibility(View.VISIBLE);
                    hiddenText.requestFocus();
                    hideKeyboard(activity);
                    Log.i("XliApp","Successfully created input capture View.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to create Layout or View for input capture.");
                    result[0] = 0;
                }

                try {
                	keyboardSize = 0;
                	hiddenLayout.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener(){
                            public void onGlobalLayout(){
                                int heightDiff = hiddenLayout.getRootView().getHeight()- hiddenLayout.getHeight();
                                if (heightDiff < 150) heightDiff = 0;
                                keyboardSize = heightDiff;
                            }
                        });
                    Log.i("XliApp","Successfully attached View Tree Observer.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to attach keyboard height monitor.");
                    result[0] = 0;
                }                
            }});
        }
        //WOW...this is awful, just use error callback from ui thread
        return result[0];
    }
}
