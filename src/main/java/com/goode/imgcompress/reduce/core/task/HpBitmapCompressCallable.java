package com.goode.imgcompress.reduce.core.task;

import android.graphics.Bitmap;
import android.support.annotation.NonNull;
import android.text.TextUtils;

import com.goode.imgcompress.ImageCodecOptions;
import com.goode.imgcompress.ImageCompressUtils;
import com.goode.imgcompress.reduce.compress.CompressOptions;
import com.goode.imgcompress.reduce.data.CallableResult;
import com.goode.imgcompress.reduce.util.Util;

import java.io.File;

/**
 * File压缩任务
 * <p>
 * 调用本地方法实现压缩{@link ImageCompressUtils#compress(String, String, ImageCodecOptions)}
 * {@link CallableResult}the result type of method {@code call}
 * <p>
 * 与{@link BitmapCompressCallable}&{@link FileCompressCallable}不同的是
 * 可以直接返回Bitmap的压缩方式完成，更高效
 * </P>
 *
 * @author fengyu.li
 * @date 2018/5/8
 */
public class HpBitmapCompressCallable extends AbstractCompressCallable {

    private File source;
    private CompressOptions options;

    public HpBitmapCompressCallable(@NonNull File file, @NonNull CompressOptions options) {
        this.source = file;
        this.options = options;
    }

    @Override
    public CallableResult call() {
        if (TextUtils.isEmpty(source.getAbsolutePath())) {
            throw new IllegalArgumentException("The source absolutePath is empty");
        }
        ImageCodecOptions codecOptions = new ImageCodecOptions().setDefault();
        codecOptions.setSampleSize(Util.getInSampleSizeByWHorLength(source.getAbsolutePath(), options));
        codecOptions.setOptimize(options.getOptimize());
        Bitmap result = ImageCompressUtils.compress(source.getAbsolutePath(), codecOptions);
        return result != null ? new CallableResult(true, result) :
                new CallableResult(false, codecOptions.getResult().getValue());
    }

}
