package com.heaven7.globalui.app;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.heaven7.core.util.MainWorker;
import com.heaven7.globalui.util.AssetsUtils;
import com.heaven7.globalui.util.JNIBridge;
import com.heaven7.java.pc.schedulers.Schedulers;

import org.libsdl.app.R;
import org.libsdl.app.SDLActivity;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClickStartApp(View view) {
        JNIBridge.init(this);
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
                System.out.println("onClickStartApp");
                String dir = "sdl2";
                File f = getCacheDir();
                f.mkdirs();
                AssetsUtils.copyAll(getApplicationContext(), dir, f.getAbsolutePath());
                next();
            }
        });
    }

    private void next() {
        MainWorker.post(new Runnable() {
            @Override
            public void run() {
                startActivity(new Intent(MainActivity.this, SDLActivity.class));
            }
        });
    }
}
