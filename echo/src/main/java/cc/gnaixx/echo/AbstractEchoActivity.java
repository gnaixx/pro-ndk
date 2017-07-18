package cc.gnaixx.echo;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.PersistableBundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

/**
 * 名称: AbstractEchoActivity
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/6/28
 */

public abstract class AbstractEchoActivity extends AppCompatActivity implements View.OnClickListener {

    protected EditText etPort;
    protected Button btnStartTcp;
    protected Button btnStartUdp;
    protected ScrollView svLog;
    protected TextView tvLog;
    private final int layoutId;

    public AbstractEchoActivity(int layoutId){
        this.layoutId = layoutId;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layoutId);

        etPort = (EditText) findViewById(R.id.et_port);
        btnStartTcp = (Button) findViewById(R.id.btn_start_tcp);
        btnStartUdp = (Button) findViewById(R.id.btn_start_udp);
        svLog = (ScrollView) findViewById(R.id.sv_log);
        tvLog = (TextView) findViewById(R.id.tv_log);

        btnStartTcp.setOnClickListener(this);
        btnStartUdp.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if(v == btnStartTcp){
            onStartBtn(0);
        }else{
            onStartBtn(1);
        }
    }

    protected abstract void onStartBtn(int type);

    protected Integer getProt(){
        try {
            Integer port = Integer.valueOf(etPort.getText().toString());
            return port;
        }catch (Exception e){
            return null;
        }
    }

    protected void logMessage(final String msg){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logMsgDirect(msg);
            }
        });
    }

    protected void logMsgDirect(String msg){
        tvLog.append(msg);
        tvLog.append("\n");
        svLog.fullScroll(View.FOCUS_DOWN);
        Log.d("GNAIXX", msg);
    }

    protected abstract class AbstractEchoTask extends Thread{
        private Handler handler;

        public AbstractEchoTask(){
            handler = new Handler();
        }

        protected void onPreExecute(){
            btnStartTcp.setEnabled(false);
            btnStartUdp.setEnabled(false);
            tvLog.setText("");
        }

        @Override
        public synchronized void start(){
            onPreExecute();
            super.start();
        }

        @Override
        public void run() {
            onBackground();
            handler.post(new Runnable() {
                @Override
                public void run() {
                    onPostExecute();
                }
            });
        }

        protected abstract void onBackground();

        protected void onPostExecute(){
            btnStartTcp.setEnabled(true);
            btnStartUdp.setEnabled(true);
        }

    }

    protected String getIp(){
        WifiManager wifiMgr = (WifiManager) this.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiMgr.getConnectionInfo();
        String ip = num2Ip(wifiInfo.getIpAddress());
        return ip;
    }

    //number transition to ip address
    public String num2Ip(int i) {
        return (i & 0xFF) + "." +
                ((i >> 8) & 0xFF) + "." +
                ((i >> 16) & 0xFF) + "." +
                (i >> 24 & 0xFF);
    }

    static {
        System.loadLibrary("echo-lib");
    }
}
