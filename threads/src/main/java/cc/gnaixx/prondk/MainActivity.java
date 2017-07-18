package cc.gnaixx.prondk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    EditText etThreads;
    EditText etIterations;
    Button btnStart;
    TextView tvLog;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etThreads = (EditText) findViewById(R.id.et_threads);
        etIterations = (EditText) findViewById(R.id.et_iterations);
        btnStart = (Button) findViewById(R.id.btn_start);
        tvLog = (TextView) findViewById(R.id.tv_log);

        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int threads = getNumber(etThreads, 0);
                int iterations = getNumber(etIterations, 0);
                if(threads >0 && iterations>0){
                    startThreads(threads, iterations);
                }
            }
        });

        nativeInit();
    }

    @Override
    protected void onDestroy() {
        nativeFree();
        super.onDestroy();
    }

    private void onNativeMessage(final String message){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvLog.append(message + "\n");
            }
        });
    }

    private static int getNumber(EditText editText, int defaultValue){
        int value;
        try {
            value = Integer.parseInt(editText.getText().toString());
        }catch (Exception e){
            value = defaultValue;
            e.printStackTrace();
        }
        return value;
    }

    private void startThreads(int threads, int iterations){
        //javaThreads(threads, iterations);
        posixThreads(threads, iterations);
    }


    private void javaThreads(int threads, final int iterations){
        for(int i=0; i<threads; i++){
            final int id = i;

            new Thread(new Runnable() {
                @Override
                public void run() {
                    nativeWorker(id, iterations);
                }
            }).start();
        }
    }

    private native void nativeInit();

    private native void nativeFree();

    private native void nativeWorker(int id, int iterations);

    private native void posixThreads(int threads, int iterations);
}
