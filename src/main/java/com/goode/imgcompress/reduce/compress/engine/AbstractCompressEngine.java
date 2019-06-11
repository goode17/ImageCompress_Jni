package com.goode.imgcompress.reduce.compress.engine;

import android.support.annotation.NonNull;

import com.goode.imgcompress.reduce.compress.CompressOptions;
import com.goode.imgcompress.reduce.core.CompressThreadPoolExecutor;
import com.goode.imgcompress.reduce.data.CallableResult;
import com.goode.imgcompress.reduce.target.Callback;

import java.util.concurrent.Executor;
import java.util.concurrent.FutureTask;

/**
 * 压缩引擎的抽象类
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public abstract class AbstractCompressEngine {

    Executor poolExecutor = CompressThreadPoolExecutor.getExecutor();

    FutureTask futureTask = null;

    /**
     * 异步压缩处理
     *
     * @param callback 数据回调
     * @param options  压缩配置
     */
    public abstract void compress(@NonNull Callback callback, @NonNull CompressOptions options);

    /**
     * 同步压缩处理
     *
     * @param options 压缩配置
     * @return CallableResult
     */
    @NonNull
    public abstract CallableResult compressSync(@NonNull CompressOptions options);

    /**
     * 取消任务
     */
    public abstract boolean cancel();
}
