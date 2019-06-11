#include "Image.h"
#include <android/bitmap.h>
#include <cstring>
#include "alog.h"

using namespace std;

namespace img_codec {

    jobject Image::toBitmap(JNIEnv* env) const {
        jclass Bitmap = (jclass) env->FindClass("android/graphics/Bitmap");
        jclass BitmapConfig = (jclass) env->FindClass("android/graphics/Bitmap$Config");

        jobject bitmap = NULL;
        uint32_t bytesOfPixels = 0;

        switch (colorSpace) {
            case RGBA8888: {
                // One pixels per loop
                // Convert color space RGBA8888 in libjpeg-turbo
                // 0    1   2   3
                // a    b   g   r
                // to color space ARGB_8888 in Bitmap
                // 0    1   2   3
                // a    r   g   b
                // convert point type to read one pixel per loop
                jmethodID CreateBitmap = env->GetStaticMethodID(Bitmap, "createBitmap"
                        , "([IIILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
                jfieldID ARGB_8888 = env->GetStaticFieldID(BitmapConfig, "ARGB_8888"
                        , "Landroid/graphics/Bitmap$Config;");
                jobject jobjectARGB_8888 = env->GetStaticObjectField(BitmapConfig, ARGB_8888);

                bytesOfPixels = (uint32_t) (width * height * 4);
                uint32_t intsOfPixels = bytesOfPixels >> 2;
                jintArray jColors = env->NewIntArray(intsOfPixels);
                // jint is signed 32 bits, use int(int32_t) is ok
                int * pTmpData = new int[intsOfPixels];
                int argbPixel, abgrPixel;
                int* pintPixel = (int *) data;

                for (uint32_t cursor = 0
                        ; cursor < intsOfPixels
                        ; cursor += 1) {
                    // 1 read src RGBA pixel: stored as ARGB
                    argbPixel = pintPixel[cursor];
                    // 2 convert to dest ARGB pixel: stored as ABGR
                    abgrPixel = ((argbPixel) & 0xFF000000)
                                |((argbPixel << 16) & 0x00FF0000)
                                |((argbPixel) & 0x0000FF00)
                                |((argbPixel >> 16) & 0x000000FF);
                    pTmpData[cursor] = abgrPixel;
                }

                // copy pixels' int array
                env->SetIntArrayRegion(jColors, 0, intsOfPixels, (const jint *) pTmpData);
                // create bitmap instance
                bitmap = ((jobject)env->CallStaticObjectMethod(Bitmap, CreateBitmap
                        , jColors
                        , width
                        , height
                        , jobjectARGB_8888));

                return bitmap;
            }
            case RGB565: {
                jmethodID CreateEmptyBitmap = env->GetStaticMethodID(Bitmap, "createBitmap"
                        , "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
                jclass ByteBuffer = env->FindClass("java/nio/ByteBuffer");
                jmethodID CopyPixelsFromBuffer = env->GetMethodID(Bitmap, "copyPixelsFromBuffer"
                        , "(Ljava/nio/Buffer;)V");
                jmethodID Wrap = env->GetStaticMethodID(ByteBuffer, "wrap"
                        , "([B)Ljava/nio/ByteBuffer;");
                jfieldID RGB_565 = env->GetStaticFieldID(BitmapConfig, "RGB_565"
                        , "Landroid/graphics/Bitmap$Config;");
                jobject jobjectRGB_565 = env->GetStaticObjectField(BitmapConfig, RGB_565);

                bytesOfPixels = (uint32_t) (width * height * 2);
                jbyteArray jBytesColors = env->NewByteArray(bytesOfPixels);
                env->SetByteArrayRegion(jBytesColors, 0, bytesOfPixels, (const jbyte *) data);
                jobject buffer = env->CallStaticObjectMethod(ByteBuffer, Wrap, jBytesColors);
                bitmap = env->CallStaticObjectMethod(Bitmap, CreateEmptyBitmap, width, height
                        , jobjectRGB_565);
                env->CallVoidMethod(bitmap, CopyPixelsFromBuffer, buffer);

                return bitmap;
            }
            default:
                // RGB888 is not supported in Android Bitmap
                LOGW("Not supported color space %d", colorSpace);
                return NULL;
        }

        return NULL;
    }

    bool Image::fromBitmap(JNIEnv* env, jobject& bitmap) {
        AndroidBitmapInfo androidBitmapInfo;
        unsigned char *pixelsColor;
        int ret;

        if ((ret = AndroidBitmap_getInfo(env, bitmap, &androidBitmapInfo)) < 0) {
            LOGD("AndroidBitmap_getInfo() failed error=%d", ret);
            return false;
        }

        if ((ret = AndroidBitmap_lockPixels(env, bitmap, (void **) &pixelsColor)) < 0) {
            LOGD("AndroidBitmap_lockPixels() failed error=%d", ret);
            return false;
        }

        LOGD("Bitmap width=%d, height=%d, size=%d, format=%d",
             androidBitmapInfo.width, androidBitmapInfo.height,
             androidBitmapInfo.height * androidBitmapInfo.width,
             androidBitmapInfo.format);

        bool success = true;
        width = androidBitmapInfo.width;
        height = androidBitmapInfo.height;
        if (data) {
            delete [] data;
        }
        uint32_t numOfPixels = androidBitmapInfo.width * androidBitmapInfo.height;
        uint32_t bytesOfPixels;
        switch (androidBitmapInfo.format) {
            case ANDROID_BITMAP_FORMAT_RGBA_8888: {
                colorSpace = RGBA8888;
                channels = 4;
                bytesOfPixels = numOfPixels << 2;
                // jint is signed 32 bits, use int(int32_t) is ok
                data = new unsigned char[bytesOfPixels];
                memcpy(data, pixelsColor, bytesOfPixels);
                goto finally;
            }
            case ANDROID_BITMAP_FORMAT_RGB_565: {
                bytesOfPixels = numOfPixels << 1;
                channels = 3;
                colorSpace = RGB565;
                data = new unsigned char[bytesOfPixels];
                memcpy(data, pixelsColor, bytesOfPixels);
                goto finally;
            }
            default:
                // not supported
                LOGD("Unsupported bitmap format: %d", androidBitmapInfo.format);
                success = false;
                goto finally;
        }

        finally:
        AndroidBitmap_unlockPixels(env, bitmap);
        return success;
    }

}
