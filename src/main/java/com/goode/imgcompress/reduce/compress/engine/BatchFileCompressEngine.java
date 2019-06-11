package com.goode.imgcompress.reduce.compress.engine;

import android.support.annotation.NonNull;

import com.goode.imgcompress.reduce.compress.CompressOptions;
import com.goode.imgcompress.reduce.core.task.FileCompressCallable;
import com.goode.imgcompress.reduce.data.CallableResult;
import com.goode.imgcompress.reduce.target.Callback;

import java.io.File;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

/**
 * Bitmap[]数据源压缩处理任务
 * <p>
 * 实现{@link AbstractCompressEngine#compress(Callback, CompressOptions)
 * {@link #compressSync(CompressOptions)}抽象方法，完成同步和异步处理压缩任务}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/23
 */
public class BatchFileCompressEngine extends AbstractBatchCompressEngine {
    private File[] files;

    public BatchFileCompressEngine(File[] files) {
        this.files = files;
    }

    @Override
    public void compress(@NonNull final Callback callback, @NonNull CompressOptions options) {
        for (final File file : files) {
            CompressOptions opts = new CompressOptions(options.getBuilder().setFileName(file.getName()));
            futureTask = new FutureTask<CallableResult>(new FileCompressCallable(file, opts)) {
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
                    if (totalCount == files.length) {
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
        for (File file : files) {
            CompressOptions opts = new CompressOptions(options.getBuilder().setFileName(file.getName()));
            CallableResult callableResult = new FileCompressCallable(file, opts).call();
            if (callableResult.ret) {
                increaseCount();
                appendPath(callableResult.path);
            } else {
                decreaseCount();
            }
        }
        boolean ret = totalCount == files.length;
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
