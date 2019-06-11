package com.goode.imgcompress;

import android.graphics.Bitmap;
import android.support.annotation.NonNull;

/**
 * 图片压缩帮助类
 * <p>
 * 该类提供了图片压缩的基本功能API
 * </p>
 *
 * @author fengyu.li
 * @date 2018/3/28
 */
public class ImageCompressUtils {

    /**
     * Compress image file to destination file, with default compress options
     * e.g. don't scale, don't reduce quality
     *
     * @param srcPath  input image file absolute path
     * @param destPath destination image file absolute path
     * @return true compress success
     */
    public static boolean compress(@NonNull String srcPath, @NonNull String destPath) {
        return compress(srcPath, destPath, new ImageCodecOptions().setDefault());
    }

    /**
     * Compress image file to destination file, with custom compress options
     * e.g. scale 1/8, reduce quality to 85%.
     * You can use this function to get source image file's dimension, with
     * {@link ImageCodecOptions#inJustDecodeBounds} set to true
     *
     * @param srcPath  input image file absolute path
     * @param destPath destination image file absolute path
     * @param options  custom options' setting
     * @return true compress success
     */
    public static boolean compress(@NonNull String srcPath, @NonNull String destPath, @NonNull ImageCodecOptions options) {
        return ImgCompressJNIInterface.compress(srcPath, destPath, options);
    }

    /**
     * Encode {@link Bitmap} to destination file, with custom quality
     * Attention:
     * 1 Bitmap's color space must be ARGB8888 when destination file is *.jpg.
     * cause RGB565 doesn't support till now by the encoder of libjpeg-turbo
     * 2 Only quality option is used
     *
     * @param bitmap   input image bitmap
     * @param destPath destination image file absolute path
     * @return true compress success
     */
    public static boolean compress(@NonNull Bitmap bitmap, @NonNull String destPath) {
        return compress(bitmap, destPath, new ImageCodecOptions().setDefault());
    }

    /**
     * Encode {@link Bitmap} to destination file, with custom quality
     * Attention:
     * 1 Bitmap's color space must be ARGB8888 when destination file is *.jpg.
     * cause RGB565 doesn't support till now by the encoder of libjpeg-turbo
     * 2 Only quality option is used
     *
     * @param bitmap   input image bitmap
     * @param destPath destination image file absolute path
     * @param options  custom options' setting
     * @return true compress success
     */
    public static boolean compress(@NonNull Bitmap bitmap, @NonNull String destPath, @NonNull ImageCodecOptions options) {
        return ImgCompressJNIInterface.compress(bitmap, destPath, options);
    }

    /**
     * Compress image file to destination {@link Bitmap}, with custom compress options
     * e.g. scale 1/8, reduce quality to 85%
     * Attention:
     * RGB565 doesn't support till now by the encoder of libjpeg-turbo
     * You can use this function to get source image file's dimension, with
     * {@link ImageCodecOptions#inJustDecodeBounds} set to true
     *
     * @param srcPath input image file absolute path
     * @param options custom options' setting
     * @return true compress success
     */
    public static Bitmap compress(@NonNull String srcPath, @NonNull ImageCodecOptions options) {
        return ImgCompressJNIInterface.compress(srcPath, options);
    }

}
