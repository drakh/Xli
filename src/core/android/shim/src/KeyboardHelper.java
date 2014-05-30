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
	
    public static void ShowKeyboard(final NativeActivity activity) {
        if (hiddenText == null)
        {
            Log.e("XliApp","ShowKeyboard:Hidden View not available");
            return;
        }
        MyEditable.PopulateDummyString();
        activity.runOnUiThread(new Runnable() { public void run() {
            try {
            	hiddenText.setFocusable(true);
                InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(hiddenText, 0);
            } catch (Exception e) {
                Log.e("XliApp","Unable show keyboard");
            }
        }});
    }

    public static int GetKeyboardSize()
    {
        return (int)keyboardSize;
    }

    public static void HideKeyboard(final NativeActivity activity) {
        if (hiddenText == null)
        {
            Log.e("XliApp","HideKeyboard: Hidden View not available");
            return;
        }
        activity.runOnUiThread(new Runnable() { public void run() {
            InputMethodManager imm = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(activity.getWindow().getDecorView().getWindowToken(), 0);
            hiddenText.setFocusable(false);
        }});
    }
        
    public static void AttachHiddenView(final NativeActivity activity)
    {    	
        Log.d("XliApp","Attempting to attach hidden view");
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
                    Log.i("XliApp","Successfully created input capture View.");
                    HideKeyboard(activity);
                } catch (Exception e) {
                    Log.e("XliApp","Unable to create Layout or View for input capture.");
                    XliJ.XliJ_JavaThrowError(-1, "Unable to create Layout or View for input capture.");
                }

                try {
                	keyboardSize = 0;
                	hiddenLayout.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener(){
	                        public void onGlobalLayout() {
	                            int rootViewHeight = hiddenLayout.getRootView().getHeight();
	                            int location[] = new int[2];
	                            hiddenLayout.getLocationOnScreen(location);
	                            int hiddenHeight = (int)(location[1] + hiddenLayout.getMeasuredHeight());
	                            keyboardSize = rootViewHeight - hiddenHeight;
	                        }
	                    });
                    Log.i("XliApp","Successfully attached View Tree Observer.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to attach keyboard height monitor.");
                    XliJ.XliJ_JavaThrowError(-1, "Unable to attach keyboard height monitor.");                    
                }                
            }});
        }
    }
    
//    @SuppressLint("NewApi")
//	@SuppressWarnings("deprecation")
//	public static void DetachHiddenView()
//    {
//    	if (layoutListener!=null && hiddenLayout!=null)
//    	{
//    	    if (Build.VERSION.SDK_INT < 16) {
//    	    	hiddenLayout.getViewTreeObserver().removeGlobalOnLayoutListener(layoutListener);
//    	    } else {
//    	    	hiddenLayout.getViewTreeObserver().removeOnGlobalLayoutListener(layoutListener);
//    	    }
//    	}
//    	if (hiddenText!=null)
//    	{
//	    	hiddenText.Disable(XliJ.nActivity, XliJ.nActivity);
//	    	hiddenText = null;
//    	}
//    	if (hiddenLayout!=null)
//    	{
//	    	hiddenLayout.removeAllViews();
//	    	hiddenLayout = null;
//    	}
//    	keyboardSize = 0;
//    }
    
    public static void KHOnPause()
    {
    	//DetachHiddenView();
    }
    public static void KHOnResume()
    {
    	//AttachHiddenView(XliJ.nActivity);
    }
}
