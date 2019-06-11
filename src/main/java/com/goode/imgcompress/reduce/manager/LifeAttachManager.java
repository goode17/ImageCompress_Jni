package com.goode.imgcompress.reduce.manager;

import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentManager;
import android.support.v4.app.FragmentActivity;

import com.goode.imgcompress.reduce.util.Util;

/**
 * com.goode.imgcompress.reduce.manager
 *
 * @author fengyu.li
 * @date 2018/4/16
 */

public class LifeAttachManager {

    private static final String TAG = "LifeListenFragment";
    private static volatile LifeAttachManager instance = null;

    private LifeAttachManager() {
    }

    public static LifeAttachManager get() {
        if (instance == null) {
            synchronized (LifeAttachManager.class) {
                if (instance == null) {
                    instance = new LifeAttachManager();
                }
            }
        }
        return instance;
    }

    public void attach(Activity activity, LifecycleListener listener) {
        if (Util.isOnBackgroundThread()) {
            return;
        }
        try {
            LifecycleFragment fragment = getLifecycleFragment(activity);
            LifecycleListenerManager manager = findLifeListenerManager(fragment);
            manager.addLifeListener(listener);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void attach(FragmentActivity activity, LifecycleListener listener) {
        if (Util.isOnBackgroundThread()) {
            return;
        }
        try {
            SupportLifecycleFragment fragment = getLifecycleSupportFragment(activity);
            LifecycleListenerManager manager = findLifeListenerManager(fragment);
            manager.addLifeListener(listener);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void attach(Fragment fragment, LifecycleListener listener) {
        if (Util.isOnBackgroundThread()) {
            return;
        }
        try {
            LifecycleFragment lFragment = getLifecycleFragment(fragment);
            LifecycleListenerManager manager = findLifeListenerManager(lFragment);
            manager.addLifeListener(listener);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void attach(android.support.v4.app.Fragment fragment, LifecycleListener listener) {
        if (Util.isOnBackgroundThread()) {
            return;
        }
        try {
            SupportLifecycleFragment lFragment = getLifecycleSupportFragment(fragment);
            LifecycleListenerManager manager = findLifeListenerManager(lFragment);
            manager.addLifeListener(listener);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 找到指定的Activity绑定的空白Fragment,如果没有则会自动绑定一个
     *
     * @param activity
     * @return
     */
    public LifecycleFragment getLifecycleFragment(Activity activity) {
        FragmentManager fm = activity.getFragmentManager();
        return findFragment(fm);
    }

    public SupportLifecycleFragment getLifecycleSupportFragment(FragmentActivity activity) {
        android.support.v4.app.FragmentManager fm = activity.getSupportFragmentManager();
        return findSupportFragment(fm);
    }

    public LifecycleFragment getLifecycleFragment(Fragment fragment) {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN_MR1) {
            FragmentManager fm = fragment.getChildFragmentManager();
            return findFragment(fm);
        }
        return null;
    }

    public SupportLifecycleFragment getLifecycleSupportFragment(android.support.v4.app.Fragment fragment) {
        android.support.v4.app.FragmentManager fm = fragment.getChildFragmentManager();
        return findSupportFragment(fm);
    }

    private LifecycleFragment findFragment(FragmentManager fm) {
        LifecycleFragment current = (LifecycleFragment) fm.findFragmentByTag(TAG);
        if (current == null) {
            current = new LifecycleFragment();
            fm.beginTransaction().add(current, TAG).commitAllowingStateLoss();
        }
        return current;
    }

    private SupportLifecycleFragment findSupportFragment(android.support.v4.app.FragmentManager fm) {
        SupportLifecycleFragment current = (SupportLifecycleFragment) fm.findFragmentByTag(TAG);
        if (current == null) {
            current = new SupportLifecycleFragment();
            fm.beginTransaction().add(current, TAG).commitAllowingStateLoss();
        }
        return current;
    }

    /**
     * 用于获取管理对应Fragment的ActLifeListenerManager
     *
     * @param fragment
     * @return
     */
    private LifecycleListenerManager findLifeListenerManager(LifecycleFragment fragment) {
        LifecycleListenerManager manager = fragment.getLifeListenerManager();
        if (null == manager) {
            manager = new LifecycleListenerManager();
        }
        fragment.setLifeListenerManager(manager);
        return manager;
    }

    private LifecycleListenerManager findLifeListenerManager(SupportLifecycleFragment fragment) {
        LifecycleListenerManager manager = fragment.getLifeListenerManager();
        if (null == manager) {
            manager = new LifecycleListenerManager();
        }
        fragment.setLifeListenerManager(manager);
        return manager;
    }

}
