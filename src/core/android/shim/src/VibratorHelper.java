
import android.annotation.SuppressLint;
import android.app.NativeActivity;
import android.content.Context;
import android.os.Vibrator;


public class VibratorHelper {
    @SuppressLint("NewApi")
	public static boolean HasVibrator(final NativeActivity activity)
    {
    	Vibrator vibrator = (Vibrator)activity.getSystemService(Context.VIBRATOR_SERVICE);
    	if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
    	{
    		return vibrator.hasVibrator();
    	} else {
    		return true;
    	}
    }
    
    public static void VibrateForMilliseconds(final NativeActivity activity, int milliseconds)
    {
    	Vibrator vibrator = (Vibrator)activity.getSystemService(Context.VIBRATOR_SERVICE);
    	vibrator.vibrate(milliseconds);
    }
}
