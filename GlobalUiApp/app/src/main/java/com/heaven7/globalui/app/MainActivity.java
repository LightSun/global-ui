package com.heaven7.globalui.app;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.ImageView;

import com.heaven7.core.util.ImageParser;
import com.heaven7.core.util.MainWorker;
import com.heaven7.core.util.PermissionHelper;
import com.heaven7.core.util.Toaster;
import com.heaven7.globalui.util.AssetsUtils;
import com.heaven7.globalui.util.JNIBridge;
import com.heaven7.java.base.util.IOUtils;
import com.heaven7.java.pc.schedulers.Schedulers;
import com.heaven7.java.visitor.FireVisitor;
import com.heaven7.java.visitor.ResultVisitor;
import com.heaven7.java.visitor.collection.VisitServices;

import org.libsdl.app.R;
import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class MainActivity extends AppCompatActivity {

    ImageView mIv;

    final PermissionHelper mHelper = new PermissionHelper(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mIv = findViewById(R.id.iv);

        requestPermission();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        mHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
    private void requestPermission() {
        mHelper.startRequestPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, 1, new PermissionHelper.ICallback() {
            @Override
            public void onRequestPermissionResult(String s, int i, boolean b) {
                if (!b) {
                    Toaster.show(getApplication(), "require sd permission");
                    finish();
                }
            }

            @Override
            public boolean handlePermissionHadRefused(String s, int i, Runnable runnable) {
                return false;
            }
        });
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

    public void onClickTmpTest(View view){
        final String img1 = "tmp/3.jpg";
        final String img2 = "tmp/4.jpg";
        final String outFile = Environment.getExternalStorageDirectory() + "/test2.png";
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
               //Bitmap result = merge(Arrays.asList(img1, img2), outFile);
                //Bitmap result = merge(Arrays.asList("tmp/5.jpg", "tmp/6.jpg", "tmp/7.jpg"), Environment.getExternalStorageDirectory() + "/test3.png");
                /*Bitmap result = merge(Arrays.asList("tmp/8.jpg", "tmp/9.jpg", "tmp/10.jpg"), Environment.getExternalStorageDirectory() + "/test4.png");
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mIv.setImageBitmap(result);
                    }
                });*/
                Bitmap bitmap = toBitmap("tmp/1024x1024.png");
                bitmap = Bitmap.createScaledBitmap(bitmap, 512, 512, true);
                saveBitmap(bitmap, Environment.getExternalStorageDirectory() + "/test2.png");
            }
        });
    }
    private Bitmap merge(List<String> paths, String outFile){
        AtomicInteger maxWidth = new AtomicInteger(0);
        AtomicInteger height  = new AtomicInteger(0);
        List<Bitmap> list = VisitServices.from(paths).map(new ResultVisitor<String, Bitmap>() {
            @Override
            public Bitmap visit(String s, Object param) {
                return toBitmap(s);
            }
        }).fire(new FireVisitor<Bitmap>() {
            @Override
            public Boolean visit(Bitmap bitmap, Object param) {
                if(bitmap.getWidth() > maxWidth.get()){
                    maxWidth.set(bitmap.getWidth());
                }
                height.addAndGet(bitmap.getHeight());
                return null;
            }
        }).getAsList();

        final Bitmap result = Bitmap.createBitmap(maxWidth.get(), height.get(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(result);
        int top = 0;
        for (int i = 0; i < list.size(); i++) {
            Bitmap bitmap = list.get(i);
            canvas.drawBitmap(bitmap, 0, top, null);
            top += bitmap.getHeight();
        }
        saveBitmap(result, outFile);
        return result;
    }

    private static void saveBitmap(Bitmap result, String outFile) {
        OutputStream out = null;
        try {
            out = new FileOutputStream(outFile);
            result.compress(Bitmap.CompressFormat.PNG,100, out);
        }catch (IOException e){
            e.printStackTrace();
        }finally {
            IOUtils.closeQuietly(out);
        }
    }

    private Bitmap toBitmap(String path){
        byte[] bytes = null;
        InputStream in = null;
        try {
            in = getAssets().open(path);
            bytes = IOUtils.readBytes(in);
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            IOUtils.closeQuietly(in);
        }
        if(bytes == null){
            return null;
        }
        return new ImageParser(0, 0, Bitmap.Config.ARGB_8888, true).parseToBitmap(bytes);
    }
}
