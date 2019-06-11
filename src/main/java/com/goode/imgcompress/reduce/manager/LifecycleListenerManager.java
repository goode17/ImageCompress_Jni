package com.goode.imgcompress.reduce.manager;

import android.os.Bundle;

import java.util.ArrayList;
import java.util.List;

/**
 * com.goode.imgcompress.reduce.manager
 *
 * @author fengyu.li
 * @date 2018/4/16
 */
public class LifecycleListenerManager {

    private List<LifecycleListener> lifecycleListeners = new ArrayList<>();

    public void addLifeListener(LifecycleListener listener) {
        if (listener != null && !lifecycleListeners.contains(listener)) {
            lifecycleListeners.add(listener);
        }
    }

    public void removeLifeListener(LifecycleListener listener) {
        if (listener != null && lifecycleListeners.contains(listener)) {
            lifecycleListeners.remove(listener);
        }
    }


    public void onActivityCreated(Bundle savedInstanceState) {
        for (LifecycleListener listener : lifecycleListeners) {
            if (listener != null) {
                listener.onCreate(savedInstanceState);
            }
        }
    }


    public void onStart() {
        for (LifecycleListener listener : lifecycleListeners) {
            if (listener != null) {
                listener.onStart();
            }
        }
    }


    public void onResume() {
        for (LifecycleListener listener : lifecycleListeners) {
            if (listener != null) {
                listener.onResume();
            }
        }
    }


    public void onPause() {
        for (LifecycleListener listener : lifecycleListeners) {
            if (listener != null) {
                listener.onPause();
            }
        }
    }


    public void onStop() {
        for (LifecycleListener listener : lifecycleListeners) {
            if (listener != null) {
                listener.onStop();
            }
        }
    }


    public void onDestroy() {
        for (int i = lifecycleListeners.size() - 1; i >= 0; i--) {
            LifecycleListener listener = lifecycleListeners.get(i);
            if (listener != null) {
                listener.onDestroy();
                removeLifeListener(listener);
                listener = null;
            }
        }
    }
}
