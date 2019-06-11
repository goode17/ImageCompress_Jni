#ifndef IMAGECODEC_H
#define IMAGECODEC_H

#include <string>
#include <vector>
#include "baseimagecompress.h"
#include "jpegimagecompress.h"

using namespace std;

namespace img_codec {

    class ImageCodec {
    public:
        ImageCodec();

        virtual ~ImageCodec();

        /**
         * @brief findDecoder
         * find relative image decoder
         * @param filename source image file absPath
         * @return pointer of image decoder instance
         */
        BaseImageDecoder *findDecoder(const string &filename);

        /**
        * @brief findEncoder
        * find relative image encoder
        * @param filename destinate image file absPath
        * @return pointer of image encoder instance
        */
        BaseImageEncoder *findEncoder(const string &filename);

    private:
        std::vector<BaseImageDecoder *> decoders;
        std::vector<BaseImageEncoder *> encoders;
    };

}
#endif // IMAGECODEC_H
