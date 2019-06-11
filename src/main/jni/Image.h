#ifndef PHOTOPROCESS_IMAGE_H
#define PHOTOPROCESS_IMAGE_H

#include <stdio.h>
#include <jni.h>

namespace img_codec {
    enum ColorSpace {
        RGBA8888    = 0,
        RGB565      = 1,
        RGB888      = 2,
        UNKNOWN     = 4     ///only used when compress to file
    };

    class Image {
    public:
        Image() {
            data = NULL;
            channels = 4;
            colorSpace = RGBA8888;
            bytesPerPixel = 4;
            quality = 100;
        };

        virtual ~Image() {
            if (data) {
                delete[] data;
                data = NULL;
            }
        };

        virtual jobject toBitmap(JNIEnv* env) const;
        virtual bool fromBitmap(JNIEnv* env, jobject& bitmap);

        int type;               /// image format type
        int width;              /// image's width
        int height;             /// image's height
        int channels;           /// image's channel count
        unsigned char *data;    /// image's pixels' data
        int bytesPerPixel;      /// bytes count of one pixel
        int quality;            /// image's compress quality
        ColorSpace colorSpace;
    };
}

#endif //PHOTOPROCESS_IMAGE_H
