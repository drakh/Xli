
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
    
    public static void VibrateUsingPattern(long[] pattern, int repeat)
    {
    	Vibrator vibrator = (Vibrator)XliJ.nActivity.getSystemService(Context.VIBRATOR_SERVICE);
    	vibrator.vibrate(pattern, repeat);
    }
    
    public static void VibrateForMilliseconds(int milliseconds)
    {
    	Vibrator vibrator = (Vibrator)XliJ.nActivity.getSystemService(Context.VIBRATOR_SERVICE);
    	vibrator.vibrate(milliseconds);
    }
}
