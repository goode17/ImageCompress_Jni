package com.goode.imgcompress.reduce.core.task;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public class FileCompressCallable extends AbstractCompressCallable {

    private File source;
    private CompressOptions options;

    public FileCompressCallable(@NonNull File file, @NonNull CompressOptions options) {
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
        boolean ret = ImageCompressUtils.compress(source.getAbsolutePath(), options.getOutFilePath(), codecOptions);
        Bitmap bitmap = BitmapFactory.decodeFile(options.getOutFilePath());
        String path = options.getOutFilePath();
        return ret ? new CallableResult(true, bitmap, path) :
                new CallableResult(false, codecOptions.getResult().getValue());
    }
}
