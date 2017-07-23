package cc.gnaixx.aviplayer;

import android.opengl.GLSurfaceView;
import android.os.Bundle;

import java.util.concurrent.atomic.AtomicBoolean;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class OpenGLPlayerActivity extends AbstractPlayerActivity {

    private final AtomicBoolean isPlaying = new AtomicBoolean();
    private long instance;
    private GLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_gl);

        glSurfaceView = (GLSurfaceView) findViewById(R.id.gl_view);
        glSurfaceView.setRenderer(renderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);//请求是渲染帧
    }

    @Override
    protected void onStart() {
        super.onStart();
        instance = init(avi);
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        free(instance);
        instance = 0;
    }

    private final Runnable player = new Runnable() {
        @Override
        public void run() {
            long frameDelay = (long) (1000 / getFrameRate(avi));
            while(isPlaying.get()){
                glSurfaceView.requestRender();
                try {
                    Thread.sleep(frameDelay);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                    break;
                }
            }
        }
    };

    private final GLSurfaceView.Renderer renderer = new GLSurfaceView.Renderer(){
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            initSurface(instance, avi);  //初始化 OpenGL surface
            isPlaying.set(true);
            new Thread(player).start();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

        }

        @Override
        public void onDrawFrame(GL10 gl) {
            if(!render(instance, avi)){
                isPlaying.set(false);
            }
        }
    };

    private native static long init(long avi);
    private native static void initSurface(long instance, long avi);
    private native static boolean render(long instance, long avi);
    private native static void free(long instance);
}
