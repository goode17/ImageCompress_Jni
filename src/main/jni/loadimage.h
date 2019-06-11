#ifndef IMAGE_H
#define IMAGE_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include "alog.h"

#define JCLAZZ_JNIINTERFACE     "com/goode/imgcompress/ImgCompressJNIInterface"

using namespace std;

/**
 * Java class ImageCodecOptions
 */
struct ImageCodecOptions {
    jclass clazz;
    jfieldID inJustDecodeBounds;
    jfieldID outWidth;
    jfieldID outHeight;
    jfieldID outChannels;
    jfieldID inQuality;
    jfieldID inSampleSize;
    jfieldID inColorSpace;
    jfieldID inOptimize;
    jfieldID outResult;
    jfieldID inUseOriginalQuality;
};

/**
 * Java enum class ImageCodecOptions$ColorSpace
 */
struct InColorSpace {
    jclass clazz;
    jfieldID RGB565;
    jfieldID ARGB8888;
    jfieldID RGB888;
};

/**
 * Java enum class ImageCodecResult
 */
struct ImageCodecResult {
    jclass clazz;
    jfieldID SUCCESS;
    jfieldID ILLEGAL_ARGUMENTS;
    jfieldID ENCODE_NOT_SUPPORT;
    jfieldID DECODE_NOT_SUPPORT;
    jfieldID INSAMPLESIZE_NOT_SUPPORT;
    jfieldID ENCODE_FAILED;
    jfieldID DECODE_FAILED;
    jfieldID COLOR_SPACE_NOT_SUPPORT;
};

static ImageCodecOptions jClassImageCodecOptions;
static InColorSpace jClassInColorSpace;
static ImageCodecResult jClassImageCodecResult;

#endif
