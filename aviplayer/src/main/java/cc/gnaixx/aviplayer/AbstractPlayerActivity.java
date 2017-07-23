package cc.gnaixx.aviplayer;

import android.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;

import java.io.IOException;

/**
 * 名称: AbstractPlayerActivity
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/7/19
 */

public abstract class AbstractPlayerActivity extends AppCompatActivity {
    public static final String EXTRA_FILE_NAME = "cc.gnaixx.aviplayer.EXTRA_FILE_NAME";

    protected long avi = 0;

    @Override
    protected void onStart(){
        super.onStart();

        try{
            avi = open(getFileName());
        }catch (IOException e){
            new AlertDialog.Builder(this)
                    .setTitle(R.string.error_alert_title)
                    .setMessage(e.getMessage())
                    .show();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        if(0 != avi){
            close(avi);
            avi = 0;
        }
    }

    protected String getFileName(){
        return getIntent().getExtras().getString(EXTRA_FILE_NAME);
    }

    protected native static long open(String fileName) throws IOException;
    protected native static int getWidth(long avi);
    protected native static int getHeight(long avi);
    protected native static double getFrameRate(long avi);
    protected native static void close(long avi);

    static {
        System.loadLibrary("AVIPlayer");
    }
}
