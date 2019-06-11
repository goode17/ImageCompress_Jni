package com.goode.imgcompress.reduce.compress.engine;

import android.graphics.Bitmap;
import android.support.annotation.NonNull;

import com.goode.imgcompress.reduce.compress.CompressOptions;
import com.goode.imgcompress.reduce.core.task.BitmapCompressCallable;
import com.goode.imgcompress.reduce.data.CallableResult;
import com.goode.imgcompress.reduce.target.Callback;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

/**
 * File[]数据源压缩处理任务
 * <p>
 * 实现{@link AbstractCompressEngine#compress(Callback, CompressOptions)&
 * {@link #compressSync(CompressOptions)}抽象方法，完成同步和异步处理压缩任务}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/23
 */
public class BatchBitmapCompressEngine extends AbstractBatchCompressEngine {

    private Bitmap[] bitmap;

    public BatchBitmapCompressEngine(Bitmap[] bitmap) {
        this.bitmap = bitmap;
    }

    @Override
    public void compress(@NonNull final Callback callback, @NonNull CompressOptions options) {
        for (Bitmap bm : bitmap) {
            CompressOptions opts = new CompressOptions(options.getBuilder().setFileName(System.nanoTime() + "." + options.getImageFormat()));
            futureTask = new FutureTask<CallableResult>(new BitmapCompressCallable(bm, opts)) {
                @Override
                protected void done() {
                    try {
                        CallableResult callableResult = get();
                        if (callableResult.ret) {
                            increaseCount();
                            appendPath(callableResult.path);
                        } else {
                            decreaseCount();
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch (ExecutionException e) {
                        e.printStackTrace();
                    } finally {
                        totalCount();
                    }
                    if (totalCount == bitmap.length) {
                        callback.callback(new CallableResult(true, successCount, failCount, paths.toString()));
                    }
                }
            };
            poolExecutor.execute(futureTask);
        }

    }

    @Override
    @NonNull
    public CallableResult compressSync(@NonNull CompressOptions options) {
        for (Bitmap bm : bitmap) {
            CompressOptions opts = new CompressOptions(options.getBuilder().setFileName(System.nanoTime() + "." + options.getImageFormat()));
            CallableResult callableResult = new BitmapCompressCallable(bm, opts).call();
            if (callableResult.ret) {
                increaseCount();
                appendPath(callableResult.path);
            } else {
                decreaseCount();
            }
        }
        boolean ret = totalCount == bitmap.length;
        return new CallableResult(ret, successCount, failCount, paths.toString());
    }

    @Override
    public boolean cancel() {
        if (futureTask == null || futureTask.isCancelled() || futureTask.isDone()) {
            return true;
        }
        return futureTask.cancel(true);
    }

}
