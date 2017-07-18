package cc.gnaixx.echo;

import android.os.Bundle;
import android.widget.EditText;

public class ClientActivity extends AbstractEchoActivity {

    private EditText etIp;
    private EditText etMsg;

    public ClientActivity() {
        super(R.layout.activity_client);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        etIp = (EditText) findViewById(R.id.et_ip);
        etMsg = (EditText) findViewById(R.id.et_msg);
    }

    @Override
    protected void onStartBtn(int type) {
        String ip = etIp.getText().toString();
        Integer port = getProt();
        String msg = etMsg.getText().toString();

        if(ip.length() != 0 && port != null && msg.length() != 0){
            ClientTask clientTask = new ClientTask(type, ip, port, msg);
            clientTask.start();
        }
    }


    private native void nativeStartTcpClient(String ip, int port, String msg) throws Exception;
    private native void nativeStartUdpClient(String ip, int port, String msg) throws Exception;


    private class ClientTask extends AbstractEchoTask{
        private final String ip;
        private final int port;
        private final String msg;
        private  final int type;

        public ClientTask(int type, String ip, int port, String msg){
            this.type = type;
            this.ip = ip;
            this.port = port;
            this.msg = msg;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting client.");
            try {
                if(type == 0) {
                    nativeStartTcpClient(ip, port, msg);
                }else{
                    nativeStartUdpClient(ip, port, msg);
                }
            } catch (Exception e) {
                e.printStackTrace();
                logMessage(e.getMessage());
            }
            logMessage("Client terminated.");

        }
    }
}
