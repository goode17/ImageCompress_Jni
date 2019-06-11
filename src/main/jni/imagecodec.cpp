#include "imagecodec.h"
#include <cstring>
#include <cctype>

namespace img_codec {

    ImageCodec::ImageCodec() {
        /* decoders */
        decoders.push_back((BaseImageDecoder *) new JpegImageDecoder());
        /* encoders */
        encoders.push_back((BaseImageEncoder *) new JpegImageEncoder());
    }

    ImageCodec::~ImageCodec() {
        vector<BaseImageDecoder *>::iterator decodersIterator;
        for (decodersIterator = decoders.begin(); decodersIterator != decoders.end();) {
            // release decoder
            delete *decodersIterator;
            // erase element
            decodersIterator = decoders.erase(decodersIterator);
        }

        vector<BaseImageEncoder *>::iterator encodersIterator;
        for (encodersIterator = encoders.begin(); encodersIterator != encoders.end();) {
            // release encoder
            delete *encodersIterator;
            // erase element
            encodersIterator = encoders.erase(encodersIterator);
        }
    }

    BaseImageDecoder *ImageCodec::findDecoder(const string &filename) {
        size_t i, maxlen = 0;

        // iterate through list of registered codecs
        for (i = 0; i < decoders.size(); i++) {
            size_t len = decoders[i]->signatureLength();
            maxlen = std::max(maxlen, len);
        }

        // Open the file
        FILE *f = fopen(filename.c_str(), "rb");
        if (!f) {
            return NULL;
        }

        // read the file signature
        string signature(maxlen, ' ');
        maxlen = fread((void *) signature.c_str(), 1, maxlen, f);
        fclose(f);
        signature = signature.substr(0, maxlen);

        // compare signature against all decoders
        for (i = 0; i < decoders.size(); i++) {
            if (decoders[i]->checkSignature(signature)) {
                return decoders[i]->newDecoder();
            }
        }

        return NULL;
    }

    BaseImageEncoder *ImageCodec::findEncoder(const string &filename) {
        if (filename.size() <= 0) {
            fprintf(stderr, "invalid filename %s\n", filename.c_str());
            return NULL;
        }

        const char *ext = strrchr(filename.c_str(), '.');
        if (!ext) {
            fprintf(stderr, "invalid filename %s\n", filename.c_str());
            return NULL;
        }

        // get ext length for compare
        int ext_len = 0;
        for (ext++; ext_len < 128 && isalnum(ext[ext_len]); ext_len++);

        for (size_t i = 0; i < encoders.size(); i++) {
            const char *exts = encoders[i]->getDescription().c_str();
            while (exts) {
                exts = strchr(exts, '.');
                if (!exts) {
                    fprintf(stderr, "invalid encoder desc %s\n",
                            encoders[i]->getDescription().c_str());
                    break;
                }
                size_t j = 0;
                for (exts++; j < ext_len && isalnum(exts[j]); j++) {
                    int c1 = tolower(exts[j]);
                    int c2 = tolower(ext[j]);
                    if (c1 != c2) {
                        break;
                    }
                }
                if (j == ext_len && !isalnum(exts[j])) {
                    return encoders[i]->newEncoder();
                }
                exts += j;
            }
        }

        fprintf(stderr, "unsupport encoder format %s\n", filename.c_str());
        return NULL;
    }

}
