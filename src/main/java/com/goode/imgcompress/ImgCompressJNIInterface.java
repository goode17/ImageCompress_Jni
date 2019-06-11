package com.goode.imgcompress;

import android.graphics.Bitmap;

/**
 * JNI library's interfaces
 *
 * @author fengyu.li
 * @date 2018/3/28
 */

public class ImgCompressJNIInterface {

    /**
     * 1 Decode image file's size info without allocating memory for it's pixels
     * 2 Decode source image file with specified scale number;
     * Encode it to destination file with specified quality
     *
     * @param srcPath  source image file absPath, can't be null or empty
     * @param destPath destination image file absPath, may be null when query image's size info
     * @param options  container of decode/encode options
     * @return true if success, otherwise fail
     */
    public static native boolean compress(String srcPath, String destPath, ImageCodecOptions options);

    /**
     * 1 Decode image file's size info without allocating memory for it's pixels
     * 2 Decode source image file with specified scale number;
     * Encode it to destination file with specified quality
     *
     * @param bitmap   source image bitmap
     * @param destPath destination image file absPath, may be null when query image's size info
     * @param options  container of decode/encode options
     * @return true if success, otherwise fail
     */
    public static native boolean compress(Bitmap bitmap, String destPath, ImageCodecOptions options);

    /**
     * 1 Decode image file's size info without allocating memory for it's pixels
     * 2 Decode source image file with specified scale number;
     * Encode it to destination {@link Bitmap} (only do image resize)
     *
     * @param srcPath source image file absPath, can't be null or empty
     * @param options container of decode/encode options
     * @return resized image's Bitmap object, or null if fails
     */
    public static native Bitmap compress(String srcPath, ImageCodecOptions options);

    static {
        System.loadLibrary("imagecompress");
    }

}
