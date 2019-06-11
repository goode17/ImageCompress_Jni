#include <resampler/Base/vnImage.h>
#include <resampler/vnImagine.h>
#include "loadimage.h"
#include "imagecodec.h"

jclass jstringClass;

string jstring2string(JNIEnv *env, jstring jStr);

using namespace img_codec;

ImageCodec *imageCodec;

jobject resultSuccess;

bool setResult(JNIEnv *jniEnv, jobject options, jobject result) {
    jniEnv->SetObjectField(options, jClassImageCodecOptions.outResult, result);
    return jniEnv->IsSameObject(result, resultSuccess);
}

/**
 * Compress image file to file
 * Attention:
 * 1 Image file compress color space is different from pixels' color space,
 *   e.g. RGB color space will always compress to YCbCr color space.
 * 2 RGB565 isn't supported when encode by libjpeg-turbo
 */
bool
native_compress_file_to_file(JNIEnv *jniEnv, jclass jclazz, jstring srcPath, jstring destPath,
                        jobject options) {
    jobject result = resultSuccess;
    if (NULL == options) {
        LOGW("compress options can't be null.");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        return setResult(jniEnv, options, result);
    }

    string cSrcPath = jstring2string(jniEnv, srcPath);
    if (cSrcPath.empty()) {
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        return setResult(jniEnv, options, result);
    }

    string cDestPath = jstring2string(jniEnv, destPath);
    bool inJustDecodeBounds = jniEnv->GetBooleanField(options, jClassImageCodecOptions.inJustDecodeBounds);
    if (!inJustDecodeBounds && cDestPath.empty()) {
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        return setResult(jniEnv, options, result);
    }

    int inSampleSize = jniEnv->GetIntField(options, jClassImageCodecOptions.inSampleSize);
    int quality = jniEnv->GetIntField(options, jClassImageCodecOptions.inQuality);
    bool optimize = jniEnv->GetBooleanField(options, jClassImageCodecOptions.inOptimize);
    bool useOriginalQuality = jniEnv->GetBooleanField(options
            , jClassImageCodecOptions.inUseOriginalQuality);
    inSampleSize = inSampleSize < 1 ? 1 : inSampleSize;
    quality = quality < 1 ? 1 : quality;
    quality = quality > 100 ? 100 : quality;

    ColorSpace colorSpace;
    jobject inColorSpace = jniEnv->GetObjectField(options, jClassImageCodecOptions.inColorSpace);
    jobject RGB888 = jniEnv->GetStaticObjectField(jClassInColorSpace.clazz
            , jClassInColorSpace.RGB888);
    jobject RGB565 = jniEnv->GetStaticObjectField(jClassInColorSpace.clazz
            , jClassInColorSpace.RGB565);
    jobject ARGB8888 = jniEnv->GetStaticObjectField(jClassInColorSpace.clazz
            , jClassInColorSpace.ARGB8888);
    if (jniEnv->IsSameObject(inColorSpace, RGB888)) {
        colorSpace = ColorSpace::RGB888;
    } else if (jniEnv->IsSameObject(inColorSpace, RGB565)) {
        colorSpace = ColorSpace::RGB565;
    } else if (jniEnv->IsSameObject(inColorSpace, ARGB8888)) {
        colorSpace = ColorSpace::RGBA8888;
    } else {
        LOGW("Unsupported target color space");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
        return setResult(jniEnv, options, result);
    }

    BaseImageDecoder *decoder = imageCodec->findDecoder(cSrcPath);
    BaseImageEncoder *encoder = NULL;
    img_codec::Image *image = NULL;
    int retCode;
    if (!decoder) {
        LOGE("can't find image decoder");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.DECODE_NOT_SUPPORT);
        goto finally;
    }

    if (inJustDecodeBounds) {
        // decode header only
        decoder->setSource(cSrcPath);
        if (resultCode.success != decoder->readHeader(&image)) {
            LOGE("decode image header failed");
            result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                    , jClassImageCodecResult.DECODE_NOT_SUPPORT);
            goto finally;
        }
    } else {
        encoder = imageCodec->findEncoder(cDestPath);
        if (!encoder) {
            LOGE("can't find image encoder");
            result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                    , jClassImageCodecResult.ENCODE_NOT_SUPPORT);
            goto finally;
        }
        if (!decoder->inSampleSizeSupported(inSampleSize)) {
            LOGI("1 decoder decode to nearest dimension, to reduce memory uses");
            double num = inSampleSize;
            double denum = decoder->getInSampleSizeMax();
            int remainScaleDenum = (int) ceil(num / denum);
            LOGI("remain insamplesize %d", remainScaleDenum);
            decoder->setSource(cSrcPath);
            // set decode scale number
            decoder->setInSampleSize(decoder->getInSampleSizeMax());
            decoder->setColorSpace(colorSpace);
            // decode image pixels
            retCode = decoder->readData(&image);
            if (resultCode.success != retCode) {
                if (resultCode.color_space_not_support == retCode) {
                    LOGE("decode image color space not support");
                    result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                            , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                    goto finally;
                } else {
                    LOGE("decode image data failed");
                    result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                            , jClassImageCodecResult.DECODE_FAILED);
                    goto finally;
                }
            }

            LOGI("2 generate template CVImage instance");
            CVImage src;
            VN_IMAGE_FORMAT format;
            int bytesPerPixel;
            if (ColorSpace::RGB888 == colorSpace) {
                format = VN_IMAGE_FORMAT_R8G8B8;
                bytesPerPixel = 3;
            } else if (ColorSpace::RGB565 == colorSpace) {
                format = VN_IMAGE_FORMAT_R5G6B5;
                bytesPerPixel = 2;
            } else if (ColorSpace::RGBA8888 == colorSpace) {
                format = VN_IMAGE_FORMAT_R8G8B8A8;
                bytesPerPixel = 4;
            } else {
                LOGW("Unsupported target color space");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                goto finally;
            }
            if (VN_FAILED(vnCreateImage(format, image->width, image->height, &src))) {
                LOGE("Error! Unable to create the source image.");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.DECODE_FAILED);
                goto finally;
            }

            LOGI("3 copy image RGB data");
            size_t imageDataLen = (size_t) (image->width * image->height * bytesPerPixel);
            memcpy(src.QueryData(), image->data, imageDataLen);

            LOGI("4 calculate destination image dimension and do resample resize");
            CVImage dest;
            int destWidth = image->width / remainScaleDenum;
            int destHeight = image->height / remainScaleDenum;
            if (VN_FAILED(vnResizeImage(src, VN_IMAGE_KERNEL_NEAREST, destWidth
                    , destHeight, 0, &dest))) {
                LOGE("Error! Unable to resample the source image.");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.DECODE_FAILED);
                goto finally;
            }

            LOGI("5 release src data memory");
            vnDestroyImage(&src);
            if (image->data) {
                delete[] image->data;
                image->data = NULL;
            }

            image->width = destWidth;
            image->height = destHeight;
            // Attention: same memory pointers, avoid to delete multiple times
            image->data = dest.QueryData();

            LOGI("6 encode image to file");
            encoder->setDestination(cDestPath);
            // set encode quality
            if (useOriginalQuality) {
                // quality from original image
                encoder->setQuality(image->quality);
            } else {
                encoder->setQuality(quality);
            }
            // set encode optimize
            encoder->setOptimize(optimize);
            // encode image pixels to file
            retCode = encoder->writeData(image);
            if (resultCode.success != retCode) {
                if (resultCode.color_space_not_support == retCode) {
                    LOGE("encode image color space not support");
                    result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                            , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                    goto finally;
                } else {
                    LOGE("encode image data failed");
                    result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                            , jClassImageCodecResult.ENCODE_FAILED);
                    goto finally;
                }
            }

            vnDestroyImage(&dest);
            image->data = NULL; // already delete by called vnDestroyImage
            goto finally;
        }
        decoder->setSource(cSrcPath);
        // set decode scale number
        decoder->setInSampleSize(inSampleSize);
        // set decode color space
        decoder->setColorSpace(colorSpace);
        // decode image pixels
        retCode = decoder->readData(&image);
        if (resultCode.success != retCode) {
            if (resultCode.color_space_not_support == retCode) {
                LOGE("decode image color space not support");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                goto finally;
            } else {
                LOGE("decode image data failed");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.DECODE_FAILED);
                goto finally;
            }
        }

        encoder->setDestination(cDestPath);
        // set encode quality
        if (useOriginalQuality) {
            // quality from original image
            encoder->setQuality(image->quality);
        } else {
            encoder->setQuality(quality);
        }
        // set encode optimize
        encoder->setOptimize(optimize);
        // encode image pixels to file
        retCode = encoder->writeData(image);
        if (resultCode.success != retCode) {
            if (resultCode.color_space_not_support == retCode) {
                LOGE("encode image color space not support");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                goto finally;
            } else {
                LOGE("encode image data failed");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.ENCODE_FAILED);
                goto finally;
            }
        }
    }

    finally:
    if (decoder) {
        delete decoder;
        decoder = NULL;
    }
    if (encoder) {
        delete encoder;
        encoder = NULL;
    }
    if (image) {
        // parse to ImageCodecOptions
        jniEnv->SetIntField(options, jClassImageCodecOptions.outWidth, image->width);
        jniEnv->SetIntField(options, jClassImageCodecOptions.outHeight, image->height);
        jniEnv->SetIntField(options, jClassImageCodecOptions.outChannels, image->channels);
        jniEnv->SetIntField(options, jClassImageCodecOptions.inQuality, image->quality);
        delete image;
        image = NULL;
    }
    return setResult(jniEnv, options, result);
}

/**
 * Compress image file to Android bitmap instance.
 * Only support RGBA8888 and RGB565.
 */
jobject
native_compress_file_to_bitmap(JNIEnv *jniEnv, jclass jclazz, jstring srcPath, jobject options) {
    jobject result = resultSuccess;
    jobject bitmap = NULL;
    if (NULL == options) {
        LOGW("compress options can't be null.");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        setResult(jniEnv, options, result);
        return bitmap;
    }

    string cSrcPath = jstring2string(jniEnv, srcPath);
    if (cSrcPath.empty()) {
        LOGW("invalid source file path.");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        setResult(jniEnv, options, result);
        return bitmap;
    }

    bool inJustDecodeBounds = jniEnv->GetBooleanField(options
            , jClassImageCodecOptions.inJustDecodeBounds);
    int inSampleSize = jniEnv->GetIntField(options, jClassImageCodecOptions.inSampleSize);

    ColorSpace colorSpace;
    jobject inColorSpace = jniEnv->GetObjectField(options, jClassImageCodecOptions.inColorSpace);
    jobject RGB888 = jniEnv->GetStaticObjectField(jClassInColorSpace.clazz
            , jClassInColorSpace.RGB888);
    jobject RGB565 = jniEnv->GetStaticObjectField(jClassInColorSpace.clazz
            , jClassInColorSpace.RGB565);
    jobject ARGB8888 = jniEnv->GetStaticObjectField(jClassInColorSpace.clazz
            , jClassInColorSpace.ARGB8888);
    if (jniEnv->IsSameObject(inColorSpace, RGB888)) {
        colorSpace = ColorSpace::RGB888;
    } else if (jniEnv->IsSameObject(inColorSpace, RGB565)) {
        colorSpace = ColorSpace::RGB565;
    } else if (jniEnv->IsSameObject(inColorSpace, ARGB8888)) {
        colorSpace = ColorSpace::RGBA8888;
    } else {
        LOGW("Unsupported target color space");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
        setResult(jniEnv, options, result);
        return bitmap;
    }

    BaseImageDecoder *decoder = imageCodec->findDecoder(cSrcPath);
    img_codec::Image *image = NULL;
    int retCode;
    if (!decoder) {
        LOGE("can't find image decoder");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.DECODE_NOT_SUPPORT);
        goto finally;
    }

    if (inJustDecodeBounds) {
        // decode header only
        decoder->setSource(cSrcPath);
        if (resultCode.success != decoder->readHeader(&image)) {
            LOGE("decode image header failed");
            result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                    , jClassImageCodecResult.DECODE_NOT_SUPPORT);
            goto finally;
        }
    } else {
        if (!decoder->inSampleSizeSupported(inSampleSize)) {
            LOGI("1 decoder decode to nearest dimension, to reduce memory uses");
            double num = inSampleSize;
            double denum = decoder->getInSampleSizeMax();
            int remainScaleDenum = (int) ceil(num / denum);
            LOGI("remain insamplesize %d", remainScaleDenum);
            decoder->setSource(cSrcPath);
            // set decode scale number
            decoder->setInSampleSize(decoder->getInSampleSizeMax());
            decoder->setColorSpace(colorSpace);
            // decode image pixels
            retCode = decoder->readData(&image);
            if (resultCode.success != retCode) {
                if (resultCode.color_space_not_support == retCode) {
                    LOGE("decode image color space not support");
                    result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                            , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                    goto finally;
                } else {
                    LOGE("decode image data failed");
                    result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                            , jClassImageCodecResult.DECODE_FAILED);
                    goto finally;
                }
            }

            LOGI("2 generate template CVImage instance");
            CVImage src;
            VN_IMAGE_FORMAT format;
            int bytesPerPixel;
            if (ColorSpace::RGB888 == colorSpace) {
                format = VN_IMAGE_FORMAT_R8G8B8;
                bytesPerPixel = 3;
            } else if (ColorSpace::RGB565 == colorSpace) {
                format = VN_IMAGE_FORMAT_R5G6B5;
                bytesPerPixel = 2;
            } else if (ColorSpace::RGBA8888 == colorSpace) {
                format = VN_IMAGE_FORMAT_R8G8B8A8;
                bytesPerPixel = 4;
            } else {
                LOGW("Unsupported target color space");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                goto finally;
            }
            if (VN_FAILED(vnCreateImage(format, image->width, image->height, &src))) {
                LOGE("Error! Unable to create the source image.");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.DECODE_FAILED);
                goto finally;
            }

            LOGI("3 copy image RGB data");
            size_t imageDataLen = (size_t) (image->width * image->height * bytesPerPixel);
            memcpy(src.QueryData(), image->data, imageDataLen);

            LOGI("4 calculate destination image dimension and do resample resize");
            CVImage dest;
            int destWidth = image->width / remainScaleDenum;
            int destHeight = image->height / remainScaleDenum;
            if (VN_FAILED(vnResizeImage(src, VN_IMAGE_KERNEL_NEAREST, destWidth
                    , destHeight, 0, &dest))) {
                LOGE("Error! Unable to resample the source image.");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.DECODE_FAILED);
                goto finally;
            }

            LOGI("5 release src data memory");
            vnDestroyImage(&src);
            if (image->data) {
                delete[] image->data;
                image->data = NULL;
            }

            image->width = destWidth;
            image->height = destHeight;
            // Attention: same memory pointers, avoid to delete multiple times
            image->data = dest.QueryData();

            LOGI("6 convert image to bitmap");
            bitmap = image->toBitmap(jniEnv);

            vnDestroyImage(&dest);
            image->data = NULL; // already delete by called vnDestroyImage

            if (NULL == bitmap) {
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
            }
            goto finally;
        }
        decoder->setSource(cSrcPath);
        // set decode inSampleSize
        decoder->setInSampleSize(inSampleSize);
        // set decode color space
        decoder->setColorSpace(colorSpace);
        // decode image pixels
        retCode = decoder->readData(&image);
        if (resultCode.success != retCode) {
            if (resultCode.color_space_not_support == retCode) {
                LOGE("decode image color space not support");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
                goto finally;
            } else {
                LOGE("decode image data failed");
                result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                        , jClassImageCodecResult.DECODE_FAILED);
                goto finally;
            }
        }

        bitmap = image->toBitmap(jniEnv);
        if (NULL == bitmap) {
            result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                    , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
            goto finally;
        }
    }

    finally:

    if (decoder) {
        delete decoder;
        decoder = NULL;
    }

    if (image) {
        // parse to ImageCodecOptions
        jniEnv->SetIntField(options, jClassImageCodecOptions.outWidth, image->width);
        jniEnv->SetIntField(options, jClassImageCodecOptions.outHeight, image->height);
        jniEnv->SetIntField(options, jClassImageCodecOptions.outChannels, image->channels);
        jniEnv->SetIntField(options, jClassImageCodecOptions.inQuality, image->quality);
        delete image;
        image = NULL;
    }
    setResult(jniEnv, options, result);
    return bitmap;
}

/**
 * Compress bitmap to file, use bitmap's pixels' color space
 * , option color space setting is ignored, only quality is used
 */
bool
native_compress_bitmap_to_file(JNIEnv *jniEnv, jclass jclazz, jobject bitmap, jstring destPath,
                        jobject options) {
    jobject result = resultSuccess;
    if (NULL == options) {
        LOGW("compress options can't be null.");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        return setResult(jniEnv, options, result);
    }

    if (NULL == bitmap) {
        LOGW("invalid source bitmap.");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ILLEGAL_ARGUMENTS);
        return setResult(jniEnv, options, result);
    }

    string cDestPath = jstring2string(jniEnv, destPath);
    int quality = jniEnv->GetIntField(options, jClassImageCodecOptions.inQuality);
    bool optimize = jniEnv->GetBooleanField(options, jClassImageCodecOptions.inOptimize);
    bool useOriginalQuality = jniEnv->GetBooleanField(options
            , jClassImageCodecOptions.inUseOriginalQuality);
    quality = quality < 1 ? 1 : quality;
    quality = quality > 100 ? 100 : quality;

    BaseImageEncoder *encoder = imageCodec->findEncoder(cDestPath);
    Image *image = new Image();
    int retCode;
    if (!encoder) {
        LOGE("can't find image encoder");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.ENCODE_NOT_SUPPORT);
        goto finally;
    }

    if(!image->fromBitmap(jniEnv, bitmap)) {
        LOGE("bitmap convert to Image failed");
        result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
        goto finally;
    }

    encoder->setDestination(cDestPath);
    // set encode quality
    if (useOriginalQuality) {
        encoder->setQuality(image->quality);
    } else {
        encoder->setQuality(quality);
    }
    // set encode optimize
    encoder->setOptimize(optimize);
    // encode image pixels to file
    retCode = encoder->writeData(image);
    if (resultCode.success != retCode) {
        if (resultCode.color_space_not_support == retCode) {
            LOGE("encode image color space not support");
            result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                    , jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT);
            goto finally;
        } else {
            LOGE("encode image data failed");
            result = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
                    , jClassImageCodecResult.ENCODE_FAILED);
            goto finally;
        }
    }

    finally:

    if (encoder) {
        delete encoder;
        encoder = NULL;
    }

    if (image) {
        // parse to ImageCodecOptions
        jniEnv->SetIntField(options, jClassImageCodecOptions.outWidth, image->width);
        jniEnv->SetIntField(options, jClassImageCodecOptions.outHeight, image->height);
        jniEnv->SetIntField(options, jClassImageCodecOptions.outChannels, image->channels);
        jniEnv->SetIntField(options, jClassImageCodecOptions.inQuality, image->quality);
        delete image;
        image = NULL;
    }
    return setResult(jniEnv, options, result);
}

string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr) {
        return "";
    }
    const jmethodID getBytes = env->GetMethodID(jstringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes
            , env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    string ret = string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    return ret;
}

JNINativeMethod nativeMethods[] = {
        {"compress", "(Ljava/lang/String;Ljava/lang/String;Lcom/goode/imgcompress/ImageCodecOptions;)Z"
                , (void *) native_compress_file_to_file},
        {"compress", "(Ljava/lang/String;Lcom/goode/imgcompress/ImageCodecOptions;)Landroid/graphics/Bitmap;"
                , (void *) native_compress_file_to_bitmap},
        {"compress", "(Landroid/graphics/Bitmap;Ljava/lang/String;Lcom/goode/imgcompress/ImageCodecOptions;)Z"
                , (void *) native_compress_bitmap_to_file}
};

#ifdef __cplusplus
extern "C" {
#endif

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad");
    JNIEnv *jniEnv;
    if (JNI_OK != vm->GetEnv((void **) &jniEnv, JNI_VERSION_1_6)) {
        LOGE("Failed to GetEnv");
        return -1;
    }
    assert(NULL != jniEnv);
    jclass localClazz = jniEnv->FindClass(JCLAZZ_JNIINTERFACE);
    jniEnv->RegisterNatives(localClazz, nativeMethods,
                            sizeof(nativeMethods) / sizeof(nativeMethods[0]));
    jclass localJstringClass = jniEnv->FindClass("java/lang/String");
    jstringClass = (jclass) jniEnv->NewGlobalRef(localJstringClass);

    jclass imageCodecOptions = jniEnv->FindClass("com/goode/imgcompress/ImageCodecOptions");
    jclass colorSpaceOptions = jniEnv->FindClass("com/goode/imgcompress/ImageCodecOptions$ColorSpace");
    jclass imageCodecResult = jniEnv->FindClass("com/goode/imgcompress/ImageCodecResult");
    jClassImageCodecOptions.clazz = (jclass) jniEnv->NewGlobalRef(imageCodecOptions);
    jClassInColorSpace.clazz = (jclass) jniEnv->NewGlobalRef(colorSpaceOptions);
    jClassImageCodecResult.clazz = (jclass) jniEnv->NewGlobalRef(imageCodecResult);

    jClassImageCodecOptions.inJustDecodeBounds = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "inJustDecodeBounds", "Z");
    jClassImageCodecOptions.inSampleSize = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "inSampleSize", "I");
    jClassImageCodecOptions.outWidth = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "outWidth", "I");
    jClassImageCodecOptions.outHeight = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "outHeight", "I");
    jClassImageCodecOptions.outChannels = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "outChannels", "I");
    jClassImageCodecOptions.inQuality = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "inQuality", "I");
    jClassImageCodecOptions.inOptimize = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "inOptimize", "Z");
    jClassImageCodecOptions.inColorSpace = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "inColorSpace", "Lcom/goode/imgcompress/ImageCodecOptions$ColorSpace;");
    jClassImageCodecOptions.outResult = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "outResult", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecOptions.inUseOriginalQuality = jniEnv->GetFieldID(jClassImageCodecOptions.clazz
            , "inUseOriginalQuality", "Z");

    jClassInColorSpace.ARGB8888 = jniEnv->GetStaticFieldID(jClassInColorSpace.clazz
            , "ARGB_8888", "Lcom/goode/imgcompress/ImageCodecOptions$ColorSpace;");
    jClassInColorSpace.RGB888 = jniEnv->GetStaticFieldID(jClassInColorSpace.clazz
            , "RGB_888", "Lcom/goode/imgcompress/ImageCodecOptions$ColorSpace;");
    jClassInColorSpace.RGB565 = jniEnv->GetStaticFieldID(jClassInColorSpace.clazz
            , "RGB_565", "Lcom/goode/imgcompress/ImageCodecOptions$ColorSpace;");

    jClassImageCodecResult.SUCCESS = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "SUCCESS", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.ILLEGAL_ARGUMENTS = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "ILLEGAL_ARGUMENTS", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.DECODE_NOT_SUPPORT = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "DECODE_NOT_SUPPORT", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.ENCODE_NOT_SUPPORT = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "ENCODE_NOT_SUPPORT", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.INSAMPLESIZE_NOT_SUPPORT = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "INSAMPLESIZE_NOT_SUPPORT", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.ENCODE_FAILED = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "ENCODE_FAILED", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.DECODE_FAILED = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "DECODE_FAILED", "Lcom/goode/imgcompress/ImageCodecResult;");
    jClassImageCodecResult.COLOR_SPACE_NOT_SUPPORT = jniEnv->GetStaticFieldID(jClassImageCodecResult.clazz
            , "COLOR_SPACE_NOT_SUPPORT", "Lcom/goode/imgcompress/ImageCodecResult;");

    imageCodec = new ImageCodec();
    jobject retSuccess = jniEnv->GetStaticObjectField(jClassImageCodecResult.clazz
            , jClassImageCodecResult.SUCCESS);
    resultSuccess = jniEnv->NewGlobalRef(retSuccess);

    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnUnload");

    delete imageCodec;
    JNIEnv *jniEnv;
    if (JNI_OK != vm->GetEnv((void **) &jniEnv, JNI_VERSION_1_6)) {
        LOGE("Failed to GetEnv");
        return;
    }
    assert(NULL != jniEnv);
    jniEnv->DeleteGlobalRef(resultSuccess);
    jniEnv->DeleteGlobalRef(jstringClass);
    jniEnv->DeleteGlobalRef(jClassImageCodecResult.clazz);
    jniEnv->DeleteGlobalRef(jClassInColorSpace.clazz);
    jniEnv->DeleteGlobalRef(jClassImageCodecOptions.clazz);
}

#ifdef __cplusplus
}
#endif
