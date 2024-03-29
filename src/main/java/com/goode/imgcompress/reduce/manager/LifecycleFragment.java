package com.goode.imgcompress.reduce.manager;

import android.app.Fragment;
import android.os.Bundle;

/**
 * com.goode.imgcompress.reduce.manager
 *
 * @author fengyu.li
 * @date 2018/4/18
 */
public class LifecycleFragment extends Fragment {

    private LifecycleListenerManager listenerManager;

    public void setLifeListenerManager(LifecycleListenerManager manager) {
        listenerManager = manager;
    }

    public LifecycleListenerManager getLifeListenerManager() {
        return listenerManager;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        if (listenerManager != null) {
            listenerManager.onActivityCreated(savedInstanceState);
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        if (listenerManager != null) {
            listenerManager.onStart();
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if (listenerManager != null) {
            listenerManager.onResume();
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        if (listenerManager != null) {
            listenerManager.onPause();
        }
    }

    @Override
    public void onStop() {
        super.onStop();
        if (listenerManager != null) {
            listenerManager.onStop();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (listenerManager != null) {
            listenerManager.onDestroy();
            listenerManager = null;
        }
    }
}
