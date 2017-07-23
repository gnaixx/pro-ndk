package cc.gnaixx.aviplayer;

import android.content.Intent;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;

import java.io.File;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private EditText etFileName;
    private RadioGroup rgPlayer;
    private Button btnPlay;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etFileName = (EditText) findViewById(R.id.et_file_name);
        rgPlayer = (RadioGroup) findViewById(R.id.rg_player);
        btnPlay = (Button) findViewById(R.id.btn_play);

        btnPlay.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_play:
                onPlayBtnClick();
                break;
        }
    }

    private void onPlayBtnClick() {
        Intent intent;
        int radioId = rgPlayer.getCheckedRadioButtonId();
        switch (radioId) {
            case R.id.rbtn_bitmap:
                intent  = new Intent(this, BitmapPlayerActivity.class);
                break;
            case R.id.rbtn_opengl:
                intent  = new Intent(this, OpenGLPlayerActivity.class);
                break;
            case R.id.rbtn_window:
                intent  = new Intent(this, WindowPlayerActivity.class);
                break;
            default:
                throw new UnsupportedOperationException("radioId=" + radioId);
        }
        File file = new File(Environment.getExternalStorageDirectory(), etFileName.getText().toString());
        intent.putExtra(AbstractPlayerActivity.EXTRA_FILE_NAME, file.getAbsolutePath());
        startActivity(intent);
    }
}
