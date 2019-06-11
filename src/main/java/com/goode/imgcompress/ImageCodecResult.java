package com.goode.imgcompress;

/**
 * Image codec's errors' enum
 *
 * @author fengyu.li
 * @date 2018/3/28
 */

public enum ImageCodecResult {
    /**
     * Image compress success
     */
    SUCCESS(0x0),
    /**
     * Input illegal arguments
     * e.g. source image path is empty, or destination image path is empty
     * when {@link ImageCodecOptions#inJustDecodeBounds} is false
     */
    ILLEGAL_ARGUMENTS(0x1001),
    /**
     * No encoder found according to destination image path suffix
     * e.g. encoder image raw data to *.png isn't supported
     */
    ENCODE_NOT_SUPPORT(0x1002),
    /**
     * No decoder found according to source image format
     * e.g. decoder *.png to image raw data isn't supported
     */
    DECODE_NOT_SUPPORT(0x1003),
    /**
     * {@link ImageCodecOptions#inSampleSize}'s value isn't supported
     * by chose decoder
     */
    INSAMPLESIZE_NOT_SUPPORT(0x1004),
    /**
     * Encoder encode image raw data to file failed
     */
    ENCODE_FAILED(0x1005),
    /**
     * Decoder decode file to image raw data failed
     */
    DECODE_FAILED(0x1006),
    /**
     * {@link ImageCodecOptions#inColorSpace}'s value isn't supported
     * by encoder, decoder or {@link android.graphics.Bitmap}
     */
    COLOR_SPACE_NOT_SUPPORT(0x1007),
    ;

    int value;

    ImageCodecResult(int i) {
        value = i;
    }

    public int getValue() {
        return value;
    }
}
