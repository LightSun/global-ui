package org.libsdl.app;

import android.content.Context;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

import java.lang.Class;
import java.lang.reflect.Method;

import static org.libsdl.app.SDLActivity.SDL_ORIENTATION_LANDSCAPE;
import static org.libsdl.app.SDLActivity.SDL_ORIENTATION_LANDSCAPE_FLIPPED;
import static org.libsdl.app.SDLActivity.SDL_ORIENTATION_PORTRAIT;
import static org.libsdl.app.SDLActivity.SDL_ORIENTATION_PORTRAIT_FLIPPED;
import static org.libsdl.app.SDLActivity.SDL_ORIENTATION_UNKNOWN;

/**
    SDL library initialization
*/
public class SDL {

    // This function should be called first and sets up the native code
    // so it can call into the Java classes
    public static void setupJNI() {
        SDLActivity.nativeSetupJNI();
        SDLAudioManager.nativeSetupJNI();
        SDLControllerManager.nativeSetupJNI();
    }

    // This function should be called each time the activity is started
    public static void initialize() {
        setContext(null);

        SDLActivity.initialize();
        SDLAudioManager.initialize();
        SDLControllerManager.initialize();
    }

    // This function stores the current activity (SDL or not)
    public static void setContext(Context context) {
        mContext = context;
    }

    public static Context getContext() {
        return mContext;
    }
    public static String[] getLibraries(){
        return new String[]{
                "hidapi",
                "SDL2",
                // "SDL2_image",
                // "SDL2_mixer",
                // "SDL2_net",
                // "SDL2_ttf",
                //"main"
                "agg"
        };
    }
    public static int getCurrentOrientation(Context context) {
        final Display display = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();

        int result = SDL_ORIENTATION_UNKNOWN;

        switch (display.getRotation()) {
            case Surface.ROTATION_0:
                result = SDL_ORIENTATION_PORTRAIT;
                break;

            case Surface.ROTATION_90:
                result = SDL_ORIENTATION_LANDSCAPE;
                break;

            case Surface.ROTATION_180:
                result = SDL_ORIENTATION_PORTRAIT_FLIPPED;
                break;

            case Surface.ROTATION_270:
                result = SDL_ORIENTATION_LANDSCAPE_FLIPPED;
                break;
        }

        return result;
    }

    public static void loadLibrary(String libraryName) throws UnsatisfiedLinkError, SecurityException, NullPointerException {

        if (libraryName == null) {
            throw new NullPointerException("No library name provided.");
        }

        try {
            // Let's see if we have ReLinker available in the project.  This is necessary for 
            // some projects that have huge numbers of local libraries bundled, and thus may 
            // trip a bug in Android's native library loader which ReLinker works around.  (If
            // loadLibrary works properly, ReLinker will simply use the normal Android method
            // internally.)
            //
            // To use ReLinker, just add it as a dependency.  For more information, see 
            // https://github.com/KeepSafe/ReLinker for ReLinker's repository.
            //
            Class<?> relinkClass = mContext.getClassLoader().loadClass("com.getkeepsafe.relinker.ReLinker");
            Class<?> relinkListenerClass = mContext.getClassLoader().loadClass("com.getkeepsafe.relinker.ReLinker$LoadListener");
            Class<?> contextClass = mContext.getClassLoader().loadClass("android.content.Context");
            Class<?> stringClass = mContext.getClassLoader().loadClass("java.lang.String");

            // Get a 'force' instance of the ReLinker, so we can ensure libraries are reinstalled if 
            // they've changed during updates.
            Method forceMethod = relinkClass.getDeclaredMethod("force");
            Object relinkInstance = forceMethod.invoke(null);
            Class<?> relinkInstanceClass = relinkInstance.getClass();

            // Actually load the library!
            Method loadMethod = relinkInstanceClass.getDeclaredMethod("loadLibrary", contextClass, stringClass, stringClass, relinkListenerClass);
            loadMethod.invoke(relinkInstance, mContext, libraryName, null, null);
        }
        catch (final Throwable e) {
            // Fall back
            try {
                System.loadLibrary(libraryName);
            }
            catch (final UnsatisfiedLinkError ule) {
                throw ule;
            }
            catch (final SecurityException se) {
                throw se;
            }
        }
    }

    protected static Context mContext;
}
