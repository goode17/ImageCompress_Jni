package com.goode.imgcompress.reduce.core.task;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.annotation.NonNull;
import android.text.TextUtils;

import com.goode.imgcompress.ImageCodecOptions;
import com.goode.imgcompress.ImageCompressUtils;
import com.goode.imgcompress.reduce.compress.CompressOptions;
import com.goode.imgcompress.reduce.data.CallableResult;

/**
 * Bitmap压缩任务
 * <p>
 * 调用本地方法实现压缩{@link ImageCompressUtils#compress(Bitmap, String, ImageCodecOptions)}
 * <p>
 * {@link CallableResult}the result type of method {@code call}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public class BitmapCompressCallable extends AbstractCompressCallable {

    private Bitmap source;
    private CompressOptions options;

    public BitmapCompressCallable(@NonNull Bitmap bitmap, @NonNull CompressOptions options) {
        this.source = bitmap;
        this.options = options;
    }

    @Override
    public CallableResult call() {
        if (TextUtils.isEmpty(options.getOutFilePath())) {
            throw new IllegalArgumentException("The file path can not be empty");
        }
        ImageCodecOptions codecOptions = new ImageCodecOptions().setDefault();
        codecOptions.setOptimize(options.getOptimize());
        boolean ret = ImageCompressUtils.compress(source, options.getOutFilePath(), codecOptions);
        Bitmap bitmap = BitmapFactory.decodeFile(options.getOutFilePath());
        String path = options.getOutFilePath();
        return ret ? new CallableResult(true, bitmap, path) :
                new CallableResult(false, codecOptions.getResult().getValue());
    }
}
