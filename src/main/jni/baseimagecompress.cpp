#include "baseimagecompress.h"
#include <cstring>

namespace img_codec {
    BaseImageDecoder::BaseImageDecoder() {
        m_insamplesize = DECODE_INSAMPLESIZE_DEFAULT;
        m_insamplesize_max = DECODE_INSAMPLESIZE_MAX;
        colorSpace = ColorSpace::RGBA8888;
    }

    BaseImageDecoder::~BaseImageDecoder() {

    }

    bool BaseImageDecoder::setSource(const string &filename) {
        m_filename = filename;
        return true;
    }

    size_t BaseImageDecoder::signatureLength() const {
        return m_signature.size();
    }

    bool BaseImageDecoder::checkSignature(const string &signature) {
        size_t len = signatureLength();
        return signature.size() >= len
               && memcmp(signature.c_str(), m_signature.c_str(), len) == 0;
    }

    bool BaseImageDecoder::setInSampleSize(int inSampleSize) {
        m_insamplesize = inSampleSize;
        return true;
    }

    bool BaseImageDecoder::setColorSpace(ColorSpace colorSpace) {
        this->colorSpace = colorSpace;
        return true;
    }

    BaseImageEncoder::BaseImageEncoder() {
        m_quality = ENCODE_QUALITY_DEFAULT;
        m_optimize = false;
    }

    BaseImageEncoder::~BaseImageEncoder() {

    }

    bool BaseImageEncoder::setDestination(const string &filename) {
        m_filename = filename;
        return true;
    }

    bool BaseImageEncoder::setQuality(int quality) {
        m_quality = quality;
        return true;
    }

    bool BaseImageEncoder::setOptimize(bool optimize) {
        m_optimize = optimize;
        return true;
    }

    string BaseImageEncoder::getDescription() {
        return m_description;
    }
}
