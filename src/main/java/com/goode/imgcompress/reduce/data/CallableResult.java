package com.goode.imgcompress.reduce.data;

import android.graphics.Bitmap;
import android.support.annotation.NonNull;

import java.io.Serializable;

/**
 * 压缩结果
 * <p>
 * 用于回调数据{@link com.goode.imgcompress.reduce.target.Callback#callback(CallableResult)}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/16
 */
public class CallableResult implements Serializable {

    public boolean ret;
    public int code;
    public String path;
    public Bitmap bitmap;
    public int successCount;
    public int failCount;

    public CallableResult(boolean ret,@NonNull Bitmap bitmap) {
        this.ret = ret;
        this.bitmap = bitmap;
    }

    public CallableResult(boolean ret, @NonNull Bitmap bitmap, @NonNull String path) {
        this.ret = ret;
        this.bitmap = bitmap;
        this.path = path;
    }

    public CallableResult(boolean ret, int successCount, int failCount) {
        this.ret = ret;
        this.successCount = successCount;
        this.failCount = failCount;
    }

    public CallableResult(boolean ret, int successCount, int failCount, @NonNull String path) {
        this.ret = ret;
        this.successCount = successCount;
        this.failCount = failCount;
        this.path = path;
    }

    public CallableResult(boolean ret, int code) {
        this.ret = ret;
        this.code = code;
    }

}
