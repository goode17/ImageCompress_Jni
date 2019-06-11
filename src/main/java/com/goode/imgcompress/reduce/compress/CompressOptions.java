package com.goode.imgcompress.reduce.compress;

import android.content.Context;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.text.TextUtils;

import com.goode.imgcompress.reduce.data.ImageFormat;

import java.io.File;
import java.lang.ref.WeakReference;

/**
 * 压缩选项
 * <p>
 * 压缩配置选项,默认{@link CompressOptions
 * {@link #maxLength =150K}
 * {@link #imageFormat = .JPG}
 * {@link #fileDirectory =getExternalFilesDir(DIRECTORY_PICTURES)}
 * {@link #defaultWidthHeight = 720*1280}
 * {@link #quality =100}}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/13
 */
public class CompressOptions {

    private int maxLength;
    private int[] defaultWidthHeight = new int[2];
    private String fileDirectory;
    private String fileName;
    private String outFilePath;
    private int quality;
    private boolean optimize;
    private ImageFormat imageFormat;
    private Builder builder;

    public CompressOptions(Builder builder) {
        this.maxLength = builder.maxLength;
        this.defaultWidthHeight[0] = builder.width;
        this.defaultWidthHeight[1] = builder.height;
        this.fileDirectory = builder.fileDirectory;
        this.fileName = builder.fileName;
        this.outFilePath = builder.outFilePath;
        this.quality = builder.quality;
        this.optimize = builder.optimize;
        this.imageFormat = builder.imageFormat;
        this.builder = builder;
    }

    public int getMaxLength() {
        return maxLength;
    }

    public int[] getDefaultWidthHeight() {
        return defaultWidthHeight;
    }

    public String getFileDirectory() {
        return fileDirectory;
    }

    public String getFileName() {
        return fileName;
    }

    public String getOutFilePath() {
        return TextUtils.isEmpty(outFilePath) ? (fileDirectory + fileName) : outFilePath;
    }

    public int getQuality() {
        return quality;
    }

    public boolean getOptimize() {
        return optimize;
    }

    public ImageFormat getImageFormat() {
        return imageFormat;
    }

    public Builder getBuilder() {
        return builder;
    }

    public static class Builder {
        private WeakReference<Context> context;
        private int maxLength = 150 * 1024;
        private int width = 720;
        private int height = 1280;
        private String fileDirectory = "";
        private String fileName = "";
        private String outFilePath = "";
        private int quality = 100;
        private boolean optimize = true;
        private ImageFormat imageFormat = ImageFormat.JPG;

        /**
         * Init the CompressOptions Builder with context
         *
         * @param context
         */
        public Builder(@NonNull Context context) {
            this.context = new WeakReference<>(context);
            if (this.context.get() != null) {
                fileDirectory = this.context.get().getExternalFilesDir(Environment.DIRECTORY_PICTURES).
                        getAbsolutePath() + File.separator;
            }
            fileName = System.nanoTime() + "." + imageFormat;
        }

        /**
         * Sets the max length of compressed file to {@link CompressOptions#maxLength}
         *
         * @param maxLength
         * @return This builder
         */
        public Builder setMaxLength(@NonNull int maxLength) {
            if (maxLength < 0) {
                throw new IllegalArgumentException("the maxLength must be greater zero");
            }
            this.maxLength = maxLength * 1024;
            return this;
        }

        /**
         * Sets the default width to {@link CompressOptions#defaultWidthHeight}
         *
         * @param width
         * @return This builder
         */
        public Builder setWidth(@NonNull int width) {
            if (width < 0) {
                throw new IllegalArgumentException("the default width must be greater zero");
            }
            this.width = width;
            return this;
        }

        /**
         * Sets the default height to {@link CompressOptions#defaultWidthHeight}
         *
         * @param height
         * @return This builder
         */
        public Builder setHeight(@NonNull int height) {
            if (height < 0) {
                throw new IllegalArgumentException("the default height must be greater zero");
            }
            this.height = height;
            return this;
        }

        /**
         * Sets the out file path to {@link CompressOptions#fileDirectory}
         *
         * @param fileDirectory
         * @return This builder
         */
        public Builder setFileDirectory(@NonNull String fileDirectory) {
            if (TextUtils.isEmpty(fileDirectory) || !new File(fileDirectory).isDirectory() || !fileDirectory.endsWith(File.separator)) {
                throw new IllegalArgumentException("The path is error,out file path is not a directory");
            }
            this.fileDirectory = fileDirectory;
            return this;
        }

        /**
         * Sets the out file name to {@link CompressOptions#fileName}
         *
         * @param fileName
         * @return This builder
         */
        public Builder setFileName(@NonNull String fileName) {
            if (TextUtils.isEmpty(fileName)) {
                throw new IllegalArgumentException("out file name can not be empty");
            }
            this.fileName = fileName;
            return this;
        }

        /**
         * Sets the out file path to {@link CompressOptions#outFilePath}
         *
         * @param outFilePath
         * @return This builder
         */
        public Builder setOutFilePath(@NonNull String outFilePath) {
            if (TextUtils.isEmpty(outFilePath)) {
                throw new IllegalArgumentException("out file path can not be empty");
            }
            this.outFilePath = outFilePath;
            return this;
        }

        /**
         * Sets the default compression quality to {@link CompressOptions#quality}
         *
         * @param quality must be between 1~100
         * @return This builder
         */
        public Builder setQuality(@NonNull int quality) {
            if (quality < 1 || quality > 100) {
                throw new IllegalArgumentException("quality must be between 1 and 100");
            }
            this.quality = quality;
            return this;
        }

        /**
         * Sets the param whether use hfm algo ,at {@link CompressOptions#optimize}
         *
         * @param optimize
         * @return This builder
         */
        public Builder setOptimize(@NonNull boolean optimize) {
            this.optimize = optimize;
            return this;
        }

        /**
         * Sets the out img format to {@link CompressOptions#imageFormat}
         *
         * @param imageFormat
         * @return This builder
         */
        public Builder setImageFormat(ImageFormat imageFormat) {
            this.imageFormat = imageFormat;
            return this;
        }

        public CompressOptions build() {
            return new CompressOptions(this);
        }
    }
}
