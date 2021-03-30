package com.heaven7.globalui.util;

import android.content.Context;
import android.view.Display;
import android.view.WindowManager;

import org.libsdl.app.SDL;

import java.io.File;

/**
 * this class used to help jni.
 */
public final class JNIBridge {
    private static boolean sInit = false;
    static {
        loadLibraries();
    }

    public static void init(Context context){
        if(!sInit){
            sInit = true;
            final Display display = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
            File f = context.getCacheDir();
            nInit(f.getAbsolutePath(), display.getWidth(), display.getHeight());
        }
    }
    private static void loadLibraries() {
        for (String lib : SDL.getLibraries()) {
            SDL.loadLibrary(lib);
        }
    }
    public static void release(){
        if(sInit){
            sInit = false;
            nRelease();
        }
    }
    //init extra jni call. for sdl2
    private static native void nInit(String str, int w, int h);
    private static native void nRelease();
}
