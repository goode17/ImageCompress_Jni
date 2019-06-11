#ifndef BASEIMAGECOMPRESS_H
#define BASEIMAGECOMPRESS_H

#include <stdio.h>
#include <string>
#include "Image.h"

#define DECODE_INSAMPLESIZE_DEFAULT 1
#define DECODE_INSAMPLESIZE_MAX     1
#define ENCODE_QUALITY_DEFAULT      100

using namespace std;

namespace img_codec {

    struct ResultCode {
        int success = 0;
        int failed = -1;
        int color_space_not_support = -2;
    };

    static ResultCode resultCode;

    class BaseImageDecoder {
    public:
        BaseImageDecoder();

        virtual ~BaseImageDecoder();

        /**
         * @brief signatureLength
         * @return length of specific encode header
         */
        virtual size_t signatureLength() const;

        /**
         * @brief checkSignature
         * check image's signature
         * @param signature
         * @return true if marchs with current decoder
         */
        virtual bool checkSignature(const string &signature);

        virtual bool setSource(const string &filename);

        /**
         * @brief setInSampleSize
         * set decoder's parameter
         * @param inSampleSize decoder's scale number
         * @return true if success
         */
        virtual bool setInSampleSize(int inSampleSize);

        virtual bool setColorSpace(ColorSpace);

        int getInSampleSizeMax() {
            return m_insamplesize_max;
        };

        /**
         * Check if inSampleSize is supported
         * @param inSampleSize   inSampleSize
         * @return true means decoder supports inSampleSize setting
         */
        virtual bool inSampleSizeSupported(int inSampleSize) {
            return inSampleSize > 0 && inSampleSize <= m_insamplesize_max;
        };

        /**
         * @brief readHeader
         * pre-read image's header to get it's size
         * @param image pointer to the pointer of Image instance
         *      pointer may be changed in this function
         * @return result code
         */
        virtual int readHeader(Image **image) = 0;

        /**
         * @brief readData
         * read image's whole pixel data
         * @param data output image's whole pixel data
         *      pointer may be changed in this function
         * @return result code
         */
        virtual int readData(Image **image) = 0;

        virtual BaseImageDecoder* newDecoder() = 0;

    protected:
        string m_filename;
        string m_signature;
        int m_insamplesize;
        int m_insamplesize_max;
        ColorSpace colorSpace;
    };

    class BaseImageEncoder {
    public:
        BaseImageEncoder();

        virtual ~BaseImageEncoder();

        virtual bool setDestination(const string &filename);

        /**
         * @brief setQuality
         * set encoder's parameter
         * @param quality encoder's compress quality
         * @return true if success
         */
        virtual bool setQuality(int quality);

        /**
         * set encoder's parameter
         * @param optimize does encoder do optimize encode or not
         * @return true if success
         */
        virtual bool setOptimize(bool optimize);

        /**
         * @brief writeData
         * write image's pixel data to destination file
         * @param image image structure ready for write
         * @return result code
         */
        virtual int writeData(const Image *image) = 0;

        virtual string getDescription();

        virtual BaseImageEncoder* newEncoder() = 0;

    protected:
        string m_filename;
        int m_quality;
        bool m_optimize;
        /// supported dest file exts
        string m_description;
    };
}

#endif // BASEIMAGECOMPRESS_H
