package com.goode.imgcompress.reduce.compress;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.text.TextUtils;

import com.goode.imgcompress.reduce.compress.engine.BatchBitmapCompressEngine;
import com.goode.imgcompress.reduce.compress.engine.BatchFileCompressEngine;
import com.goode.imgcompress.reduce.compress.engine.BitmapCompressEngine;
import com.goode.imgcompress.reduce.compress.engine.FileCompressEngine;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * 用于构建压缩引擎的工厂类
 * <p>
 * 工厂模式构建压缩引擎
 * 包含单任务File，Bitmap AbstractCompressEngine，批量File，Bitmap AbstractCompressEngine
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public class CompressEngineFactory {

    public static BitmapCompressEngine createBitmapCompressEngine(@NonNull Bitmap bitmap) {
        BitmapCompressEngine engine = new BitmapCompressEngine(bitmap);
        return engine;
    }

    public static BatchBitmapCompressEngine createBatchBitmapCompressEngine(@NonNull Bitmap[] bitmap) {
        BatchBitmapCompressEngine engine = new BatchBitmapCompressEngine(bitmap);
        return engine;
    }

    public static FileCompressEngine createFileCompressEngine(@NonNull File file) {
        FileCompressEngine engine = new FileCompressEngine(file);
        return engine;
    }

    public static BatchFileCompressEngine createBatchFileCompressEngine(@NonNull File[] files) {
        BatchFileCompressEngine engine = new BatchFileCompressEngine(files);
        return engine;
    }

    public static FileCompressEngine createPathCompressEngine(@NonNull String path) {
        if (TextUtils.isEmpty(path)) {
            throw new IllegalArgumentException("Error,the file path is empty");
        }
        File file = new File(path);
        FileCompressEngine engine = new FileCompressEngine(file);
        return engine;
    }

    public static BitmapCompressEngine createDrawableCompressEngine(@NonNull Drawable drawable) {
        BitmapDrawable bitmap = (BitmapDrawable) drawable;
        BitmapCompressEngine engine = new BitmapCompressEngine(bitmap.getBitmap());
        return engine;
    }

    public static BitmapCompressEngine createUriCompressEngine(@NonNull Context context, @NonNull Uri uri) throws IOException {
        InputStream input = context.getContentResolver().openInputStream(uri);
        Bitmap bitmap = BitmapFactory.decodeStream(input);
        input.close();
        BitmapCompressEngine engine = new BitmapCompressEngine(bitmap);
        return engine;
    }

    public static BitmapCompressEngine createResourceCompressEngine(@NonNull Context context, @NonNull int resourceId) {
        Resources res = context.getResources();
        Bitmap bitmap = BitmapFactory.decodeResource(res, resourceId);
        BitmapCompressEngine engine = new BitmapCompressEngine(bitmap);
        return engine;
    }

    public static BitmapCompressEngine createUrlCompressEngine(@NonNull URL url) throws IOException {
        Bitmap bitmap;
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setDoInput(true);
        conn.connect();
        InputStream is = conn.getInputStream();
        bitmap = BitmapFactory.decodeStream(is);
        is.close();
        BitmapCompressEngine engine = new BitmapCompressEngine(bitmap);
        return engine;
    }

    public static BitmapCompressEngine createBytesCompressEngine(@NonNull byte[] bytes) {
        Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
        BitmapCompressEngine engine = new BitmapCompressEngine(bitmap);
        return engine;
    }
}
