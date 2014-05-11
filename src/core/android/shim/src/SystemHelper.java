
import android.annotation.SuppressLint;
import android.app.NativeActivity;
import android.content.res.AssetManager;
import android.graphics.Rect;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Window;
import android.view.WindowManager;


public class SystemHelper {
    public static void hideStatusBar(final NativeActivity activity) {
    	Window win = activity.getWindow();
    	win.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        win.addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
    }
    public static void showStatusBar(final NativeActivity activity) {
    	Window win = activity.getWindow();
    	win.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
    }
	@SuppressLint("NewApi")
	public static DisplayMetrics GetDisplayMetrics(final NativeActivity activity) 
	{
		DisplayMetrics metrics = new DisplayMetrics();
    	if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {    		
    		activity.getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
            return metrics;
    	} else {
    		activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
            return metrics;
    	}
    }	
	public static float GetStatusBarHeight(NativeActivity activity)
	{
		Rect rectangle= new Rect();
		Window window= activity.getWindow();
		window.getDecorView().getWindowVisibleDisplayFrame(rectangle);
		int statusBarHeight = rectangle.top;
		//int contentViewTop = window.findViewById(Window.ID_ANDROID_CONTENT).getTop();
		//int titleBarHeight= contentViewTop - statusBarHeight; //dont need this yet but is useful code
		return statusBarHeight;
	}
}
