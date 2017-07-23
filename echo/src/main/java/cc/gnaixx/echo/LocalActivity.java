package cc.gnaixx.echo;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import android.os.Bundle;
import android.widget.EditText;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;

public class LocalActivity extends AbstractEchoActivity {

    private EditText etMsg;

    public LocalActivity() {
        super(R.layout.activity_local);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        etMsg = (EditText) findViewById(R.id.et_msg);
    }

    @Override
    protected void onStartBtn(int type) {
        String port = etPort.getText().toString();
        String msg = etMsg.getText().toString();

        if (port.length() > 0 && msg.length() > 0) {
            String socketName;

            if(isFileSystemSocket(port)){
                File file = new File(getFilesDir(), port);
                socketName = file.getAbsolutePath();
            }else{
                socketName = port;
            }
            ServerTask serverTask = new ServerTask(socketName);
            serverTask.start();

            ClientTask clientTask = new ClientTask(socketName, msg);
            clientTask.start();
        }
    }

    private boolean isFileSystemSocket(String name){
        return name.startsWith("/");
    }

    private void startLocalClient(String name, String msg) throws Exception{
        LocalSocket clientSocket = new LocalSocket();
        try{
            //设置 socket 名称空间
            LocalSocketAddress.Namespace namespace;
            if(isFileSystemSocket(name)){
                namespace = LocalSocketAddress.Namespace.FILESYSTEM;
            }else{
                namespace = LocalSocketAddress.Namespace.ABSTRACT;
            }
            //构造本地 socket 地址
            LocalSocketAddress address = new LocalSocketAddress(name, namespace);
            //连接本地 socket
            logMessage("Connecting to " + name);
            clientSocket.connect(address);
            logMessage("Connected.");

            byte[] msgBytes = msg.getBytes();
            logMessage("Sending to the socket...");
            OutputStream outputStream = clientSocket.getOutputStream();
            outputStream.write(msgBytes);
            logMessage(String.format("Send %d bytes: %s", msgBytes.length, msg));

            logMessage("Receiving from the socket...");
            InputStream inputStream = clientSocket.getInputStream();
            int readSize = inputStream.read(msgBytes);
            String receiverMsg = new String(msgBytes, 0, readSize);
            logMessage(String.format("Received %d bytes: %s", readSize, receiverMsg));

            outputStream.close();
            inputStream.close();
        }finally {
            clientSocket.close();
        }
    }

    private native void nativeStartLoacalServer(String name) throws Exception;

    private class ServerTask extends AbstractEchoTask{

        private final String name;

        public ServerTask(String name){
            this.name = name;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting server.");
            try{
                nativeStartLoacalServer(name);
            }catch (Exception e){
                logMessage(e.getMessage());
            }
            logMessage("Server terminated.");
        }
    }

    /**
     * 客户端任务
     */
    private class ClientTask extends Thread {
        private final String name;
        private final String msg;

        public ClientTask(String name, String msg){
            this.name = name;
            this.msg = msg;
        }

        @Override
        public void run() {
            logMessage("Starting client.");
            try{
                startLocalClient(name, msg);
            }catch (Exception e){
                logMessage(e.getMessage());
            }
            logMessage("Client terminated.");
        }
    }
}
