package com.goode.imgcompress.reduce.data;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.goode.imgcompress.reduce.compress.CompressEngineFactory;
import com.goode.imgcompress.reduce.compress.engine.BatchBitmapCompressEngine;
import com.goode.imgcompress.reduce.compress.engine.BatchFileCompressEngine;
import com.goode.imgcompress.reduce.compress.engine.BitmapCompressEngine;
import com.goode.imgcompress.reduce.compress.engine.FileCompressEngine;

import java.io.File;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.net.URL;

/**
 * 构建一个压缩引擎根据源数据类型
 * <p>
 * 源数据类型支持路径模式和数据模式
 * 包含文件路径，URI网络路径，Resource资源路径以及File，Bitmap，Drawable，Byte Array等
 * </P>
 *
 * @author fengyu.li
 * @date 2018/4/16
 */
public class SourceBuilder implements ModelTypes {

    private WeakReference<Context> context;

    public SourceBuilder() {
    }

    public SourceBuilder setContext(@NonNull Context context) {
        this.context = new WeakReference<>(context);
        return this;
    }

    @NonNull
    @Override
    public BitmapCompressEngine source(@Nullable Bitmap bitmap) {
        return CompressEngineFactory.createBitmapCompressEngine(bitmap);
    }

    @NonNull
    @Override
    public BatchBitmapCompressEngine source(@Nullable Bitmap[] bitmap) {
        return CompressEngineFactory.createBatchBitmapCompressEngine(bitmap);
    }

    @NonNull
    @Override
    public BitmapCompressEngine source(@Nullable Drawable drawable) {
        return CompressEngineFactory.createDrawableCompressEngine(drawable);
    }

    @NonNull
    @Override
    public FileCompressEngine source(@Nullable String string) {
        return CompressEngineFactory.createPathCompressEngine(string);
    }

    @NonNull
    @Override
    public BitmapCompressEngine source(@Nullable Uri uri) throws IOException {
        if (context.get() == null) {
            throw new IllegalArgumentException("Error,The context can not be null.");
        }
        return CompressEngineFactory.createUriCompressEngine(context.get(), uri);
    }

    @NonNull
    @Override
    public FileCompressEngine source(@Nullable File file) {
        return CompressEngineFactory.createFileCompressEngine(file);
    }

    @NonNull
    @Override
    public BatchFileCompressEngine source(@Nullable File[] files) {
        return CompressEngineFactory.createBatchFileCompressEngine(files);
    }

    @NonNull
    @Override
    public BitmapCompressEngine source(@Nullable Integer resourceId) {
        if (context.get() == null) {
            throw new IllegalArgumentException("Error,The context can not be null.");
        }
        return CompressEngineFactory.createResourceCompressEngine(context.get(), resourceId);
    }

    @Override
    public BitmapCompressEngine source(@Nullable URL url) throws IOException {
        return CompressEngineFactory.createUrlCompressEngine(url);
    }

    @NonNull
    @Override
    public BitmapCompressEngine source(@Nullable byte[] model) {
        return CompressEngineFactory.createBytesCompressEngine(model);
    }

}
