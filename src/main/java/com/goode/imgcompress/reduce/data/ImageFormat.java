package com.goode.imgcompress.reduce.data;

/**
 * Specifies the known formats a bitmap can be compressed into
 *
 * @author fengyu.li
 * @date 2018/4/23
 */
public enum ImageFormat {
    GIF("gif"),
    JPG("jpg"),
    JPEG("jpeg"),
    RAW("raw"),
    PNG("png"),
    WEBP("webp"),
    UNKNOWN("unkown");

    private final String type;

    ImageFormat(String type) {
        this.type = type;
    }

    public String getType() {
        return type;
    }
}
