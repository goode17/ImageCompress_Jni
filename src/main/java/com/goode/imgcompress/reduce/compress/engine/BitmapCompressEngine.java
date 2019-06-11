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
 * Bitmap数据源压缩处理任务
 * <p>
 * 实现{@link AbstractCompressEngine#compress(Callback, CompressOptions)
 * {@link #compressSync(CompressOptions)}抽象方法，完成同步和异步处理压缩任务}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public class BitmapCompressEngine extends AbstractCompressEngine {
    private Bitmap bitmap;

    public BitmapCompressEngine(Bitmap bitmap) {
        this.bitmap = bitmap;
    }

    @Override
    public void compress(@NonNull final Callback callback, @NonNull CompressOptions options) {
        futureTask = new FutureTask<CallableResult>(new BitmapCompressCallable(bitmap, options)) {
            @Override
            protected void done() {
                try {
                    callback.callback(get());
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (ExecutionException e) {
                    e.printStackTrace();
                }
            }
        };
        poolExecutor.execute(futureTask);
    }

    @Override
    @NonNull
    public CallableResult compressSync(@NonNull CompressOptions options) {
        return new BitmapCompressCallable(bitmap, options).call();
    }

    @Override
    public boolean cancel() {
        if (futureTask == null || futureTask.isCancelled() || futureTask.isDone()) {
            return true;
        }
        return futureTask.cancel(true);
    }

}
