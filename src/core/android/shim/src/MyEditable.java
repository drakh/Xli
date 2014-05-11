import android.text.SpannableStringBuilder;

public class MyEditable extends SpannableStringBuilder {
	public static String DUMMY;
    MyEditable(CharSequence source) {
        super(source);
    }
    @Override
    public SpannableStringBuilder replace(final int start, final int end, CharSequence tb, int tbstart, int tbend) {
        if (tbend > tbstart) {
            super.replace(0, length(), "", 0, 0);
            return super.replace(0, 0, tb, tbstart, tbend);
        }
        else if (end > start) {
            super.replace(0, length(), "", 0, 0);
            return super.replace(0, 0, DUMMY, 0, DUMMY.length());
        }
        return super.replace(start, end, tb, tbstart, tbend);
    }
    public static void PopulateDummyString()
    {    	
    	MyEditable.DUMMY = "";
        for (int i = 0; i < Math.max(0, (500 - MyEditable.DUMMY.length())); i++)
            MyEditable.DUMMY += "\0";
    }
}
