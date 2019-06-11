package com.goode.imgcompress;

/**
 * Compress input/output parameters's container
 *
 * @author fengyu.li
 * @date 2018/3/28
 */

public class ImageCodecOptions {

    /**
     * Destination {@link android.graphics.Bitmap}'s color space, describes
     * how pixels are stored.
     */
    public enum ColorSpace {
        /**
         * Each pixel is stored on 2 bytes and only the RGB channels are
         * encoded: red is stored with 5 bits of precision (32 possible
         * values), green is stored with 6 bits of precision (64 possible
         * values) and blue is stored with 5 bits of precision.
         */
        RGB_565,

        /**
         * Each pixel is stored on 4 bytes. Each channel (RGB and alpha
         * for translucency) is stored with 8 bits of precision (256
         * possible values.)
         * Default color space
         */
        ARGB_8888,

        /**
         * Each pixel is stored on 3 bytes and only the RGB channels are
         * encoded.
         * Not supported when compress to bitmap
         */
        RGB_888
    }

    /**
     * If set to true, the decoder won't decode image pixels, but
     * the <code>out...</code> fields will still be set, allowing the caller to
     * query the image size info without having to allocate the memory for its pixels.
     */
    private boolean inJustDecodeBounds;

    /**
     * The resulting width of the image. If {@link #inJustDecodeBounds} is
     * set to false, this will be width of the output image after any
     * scaling is applied. If true, it will be the width of the input image
     * without any accounting for scaling.
     */
    private int outWidth;

    /**
     * The resulting height of the image. If {@link #inJustDecodeBounds} is
     * set to false, this will be height of the output image after any
     * scaling is applied. If true, it will be the height of the input image
     * without any accounting for scaling.
     */
    private int outHeight;

    /**
     * The resulting color channels' count of the image.
     * this will be used to calculate image file's size.
     */
    private int outChannels;

    /**
     * Hint to the compressor, 0-100. 0 meaning compress for small size
     * , 100 meaning compress for max quality.
     */
    private int inQuality;

    /**
     * If set to a value > 1, requests the decoder to subsample the original
     * image, returning a smaller image to save memory. The sample size is
     * the number of pixels in either dimension that correspond to a single
     * pixel in the decoded bitmap. For example, inSampleSize == 4 returns
     * an image that is 1/4 the width/height of the original, Any value <= 1
     * is treated the same as 1.
     * Note:
     * If value > 8, will do image data resample
     */
    private int inSampleSize;

    /**
     * If set to true, Optimize Huffman table is enabled (smaller file,
     * but slow compression)
     */
    private boolean inOptimize;

    /**
     * Destination {@link android.graphics.Bitmap}'s color space
     */
    private ColorSpace inColorSpace;

    /**
     * Image codec's result code, see {@link ImageCodecResult}
     */
    private ImageCodecResult outResult;

    /**
     * If set to true, Image encoder will use original image's compress quality
     */
    private boolean inUseOriginalQuality;

    public ImageCodecOptions setDefault() {
        inJustDecodeBounds = false;
        inSampleSize = 1;
        inQuality = 100;
        inOptimize = false;
        inColorSpace = ColorSpace.ARGB_8888;
        outResult = ImageCodecResult.SUCCESS;
        inUseOriginalQuality = true;
        return this;
    }

    public boolean isInJustDecodeBounds() {
        return inJustDecodeBounds;
    }

    public ImageCodecOptions setInJustDecodeBounds(boolean inJustDecodeBounds) {
        this.inJustDecodeBounds = inJustDecodeBounds;
        return this;
    }

    public int getWidth() {
        return outWidth;
    }

    public int getHeight() {
        return outHeight;
    }

    public int getChannels() {
        return outChannels;
    }

    public ImageCodecOptions setQuality(int quality) {
        quality = quality < 1 ? 1 : quality;
        quality = quality > 100 ? 100 : quality;
        this.inQuality = quality;
        inUseOriginalQuality = false;
        return this;
    }

    public ImageCodecOptions setSampleSize(int sampleSize) {
        this.inSampleSize = sampleSize < 1 ? 1 : sampleSize;
        return this;
    }

    public ImageCodecOptions setColorSpace(ColorSpace colorSpace) {
        this.inColorSpace = colorSpace;
        return this;
    }

    public ImageCodecResult getResult() {
        return outResult;
    }

    public ImageCodecOptions setOptimize(boolean optimize) {
        this.inOptimize = optimize;
        return this;
    }

    /**
     * Get compress quality, it's values can be:
     * 1 User custom quality, if {@link #setQuality(int)} is called
     * 2 Original image's compress quality
     *
     * @return destination image's compress quality
     */
    public int getQuality() {
        return inQuality;
    }
}
