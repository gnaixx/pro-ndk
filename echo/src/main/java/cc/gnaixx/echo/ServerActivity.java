package cc.gnaixx.echo;


import android.os.Bundle;
import android.widget.TextView;

public class ServerActivity extends AbstractEchoActivity{

    TextView tvIp;

    public ServerActivity() {
        super(R.layout.activity_server);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        tvIp = (TextView) findViewById(R.id.tv_ip);
        tvIp.setText(getIp());
    }

    @Override
    protected void onStartBtn(int type) {
        Integer port = getProt();
        if(port != null){
            ServerTask serverTask = new ServerTask(type, port);
            serverTask.start();
        }
    }

    private native void nativeStartTcpServer(int port) throws Exception;
    private native void nativeStartUdpServer(int port) throws Exception;

    private class ServerTask extends AbstractEchoTask{
        private final int port;
        private final int type;

        public ServerTask(int type, int port){
            this.port = port;
            this.type = type;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting server.");
            try {
                if(type == 0) {
                    nativeStartTcpServer(port);
                }else {
                    nativeStartUdpServer(port);
                }
            } catch (Exception e) {
                e.printStackTrace();
                logMessage(e.getMessage());
            }
            logMessage("Server terminated.");
        }
    }
}
