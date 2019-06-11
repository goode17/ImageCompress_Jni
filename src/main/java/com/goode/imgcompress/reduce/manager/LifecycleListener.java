package com.goode.imgcompress.reduce.manager;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;

/**
 * An interface for listener to {@link android.app.Fragment} and {@link android.app.Activity}
 * lifecycle events.
 */
public interface LifecycleListener {

    /**
     * Callback for when {@link android.app.Fragment#onCreate(Bundle)}}} or {@link
     * android.app.Activity#onCreate(Bundle)} is called.
     */
    void onCreate(Bundle savedInstanceState);


    /**
     * Callback for when {@link android.app.Fragment#onStart()}} or {@link
     * android.app.Activity#onStart()} is called.
     */
    void onStart();

    /**
     * Callback for when {@link Fragment#onResume()} ()}} or {@link
     * Activity#onResume()} is called.
     */
    void onResume();

    /**
     * Callback for when {@link Fragment#onPause()}} or {@link
     * Activity#onPause()} is called.
     */
    void onPause();

    /**
     * Callback for when {@link android.app.Fragment#onStop()}} or {@link
     * android.app.Activity#onStop()}} is called.
     */
    void onStop();

    /**
     * Callback for when {@link android.app.Fragment#onDestroy()}} or {@link
     * android.app.Activity#onDestroy()} is called.
     */
    void onDestroy();

}
