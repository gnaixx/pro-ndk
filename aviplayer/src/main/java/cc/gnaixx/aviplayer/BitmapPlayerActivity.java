package cc.gnaixx.aviplayer;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.concurrent.atomic.AtomicBoolean;

public class BitmapPlayerActivity extends AbstractPlayerActivity {

    private final AtomicBoolean isPlaying = new AtomicBoolean();
    private SurfaceHolder sfHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bitmap_player);

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
            //保存所有帧
            Bitmap bitmap = Bitmap.createBitmap(getWidth(avi), getHeight(avi), Bitmap.Config.RGB_565);
            long frameDelay = (long) (1000 / getFrameRate(avi));  //计算延迟

            //渲染
            while(isPlaying.get()){
                render(avi, bitmap); //将帧渲染到 bitmap
                Canvas canvas = sfHolder.lockCanvas(); //锁住 canvas
                canvas.drawBitmap(bitmap, 0, 0, null); //绘制 canvas
                sfHolder.unlockCanvasAndPost(canvas); //canvas 显示
                try{
                    Thread.sleep(frameDelay);
                }catch (InterruptedException e){
                    break;
                }
            }
        }
    };

    private native static boolean render(long avi, Bitmap bitmap);
}
