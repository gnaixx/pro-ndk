package cc.gnaixx.aviplayer;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.concurrent.atomic.AtomicBoolean;

import static android.R.attr.bitmap;

public class WindowPlayerActivity extends AbstractPlayerActivity {

    private final AtomicBoolean isPlaying = new AtomicBoolean();
    private SurfaceHolder sfHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_window_player);

        SurfaceView sfView = (SurfaceView) findViewById(R.id.sf_view);
        sfHolder = sfView.getHolder();
        sfHolder.addCallback(sfHolderCallback);
    }

    private final SurfaceHolder.Callback sfHolderCallback = new SurfaceHolder.Callback(){

        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            isPlaying.set(true);
            new Thread(renderer).start(); //独立线程中渲染
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            isPlaying.set(false);
        }
    };

    private final Runnable renderer = new Runnable() {
        @Override
        public void run() {
            Surface surface = sfHolder.getSurface();
            init(avi, surface);

            long frameDelay = (long) (1000 / getFrameRate(avi));

            //渲染
            while(isPlaying.get()){
                render(avi, surface); //将帧渲染到 bitmap

                try{
                    Thread.sleep(frameDelay);
                }catch (InterruptedException e){
                    break;
                }
            }
        }
    };

    private native static void init(long avi, Surface surface);
    private native static boolean render(long avi, Surface surface);
}
