#ifndef JPEGIMAGECOMPRESS_H
#define JPEGIMAGECOMPRESS_H

#include "baseimagecompress.h"
#include "jpeg/include/jpeglib.h"
#include "setjmp.h"
#include "alog.h"

#define READ_LINES_ONE_SCAN     1
#define WRITE_LINES_ONE_SCAN    16

namespace img_codec {

    struct jerror_mgr {
        jpeg_error_mgr base;
        jmp_buf jmp;
    };

    class JpegImage : public Image {
    public:
        JpegImage() {
        };

        ~JpegImage() {
        };

        bool setColorSpace(J_COLOR_SPACE jColorSpace) {
            switch (jColorSpace) {
                case JCS_RGB:
                    colorSpace = RGB888;
                    break;
                case JCS_RGB565:
                    colorSpace = RGB565;
                    break;
                case JCS_EXT_RGBA:
                    colorSpace = RGBA8888;
                    break;
                default:
                    LOGW("Unsupported color space %d", jColorSpace);
                    return false;
            }
            return true;
        };

        J_COLOR_SPACE getJColorSpace() {
            switch (colorSpace) {
                case RGB565:
                    return JCS_RGB565;
                case RGBA8888:
                    return JCS_EXT_RGBA;
                case RGB888:
                    return JCS_RGB;
                default:
                    break;
            }
            /// won't come here
            return JCS_UNKNOWN;
        };

    };

    class JpegImageCodec {
    public:
        JpegImageCodec();

        virtual ~JpegImageCodec();

    protected:
        struct jerror_mgr jerror;

        METHODDEF(void) jerror_exit(j_common_ptr jinfo);
    };

    class JpegImageDecoder : public BaseImageDecoder, public JpegImageCodec {
    public:
        JpegImageDecoder();

        ~JpegImageDecoder();

        size_t signatureLength() const;

        bool checkSignature(const string &signature);

        bool setInSampleSize(int inSampleSize);

        int readHeader(Image **image);

        int readData(Image **image);

        bool isJpeg2k() { return m_is_2k; }

        BaseImageDecoder* newDecoder();

    private:
        /* signature of jpeg2000 */
        string m_signature_2k;
        /* this is a jpeg2000 image */
        bool m_is_2k;
        struct jpeg_decompress_struct cinfo_decompress;
        unsigned int m_scale_num;
    };

    class JpegImageEncoder : public BaseImageEncoder, public JpegImageCodec {
    public:
        JpegImageEncoder();

        ~JpegImageEncoder();

        int writeData(const Image *image);

        BaseImageEncoder* newEncoder();
    private:
        struct jpeg_compress_struct cinfo_compress;
    };
}

#endif // JPEGIMAGECOMPRESS_H
