#include "jpegimagecompress.h"
#include <cstring>

namespace img_codec {

    /**
     * Set Image's compress quality, by default use source image's quality factor.
     * If we use the same quality factor as in the original JPEG image, the scaled
     * image will have size roughly equal to (scale factor * scale factor) proportion
     * of the original image size. The quality factor can be estimate based on the
     * DQT defined in the every JPEG image. This algorithm copy from ImageMagick's
     * open source, can be found here:
     * https://github.com/ImageMagick/ImageMagick/blob
     * /05d2ff7ebf21f659f5b11e45afb294e152f4330c/coders/jpeg.c#L818
     * */
    static bool setDefaultQuality(struct jpeg_decompress_struct *jpeg_info, Image *image) {
        if (NULL == image || NULL == jpeg_info) {
            return false;
        }

        size_t quality = 100;
        ssize_t j, qvalue, sum;
        register ssize_t i;

        /*
          Determine the JPEG compression quality from the quantization tables.
        */
        sum = 0;
        for (i = 0; i < NUM_QUANT_TBLS; i++) {
            if (jpeg_info->quant_tbl_ptrs[i] != NULL) {
                for (j = 0; j < DCTSIZE2; j++) {
                    sum += jpeg_info->quant_tbl_ptrs[i]->quantval[j];
                }
            }
        }
        if ((jpeg_info->quant_tbl_ptrs[0] != NULL) && (jpeg_info->quant_tbl_ptrs[1] != NULL)) {
            ssize_t hash[101] =
                    {
                            1020, 1015,  932,  848,  780,  735,  702,  679,  660,  645,
                            632,  623,  613,  607,  600,  594,  589,  585,  581,  571,
                            555,  542,  529,  514,  494,  474,  457,  439,  424,  410,
                            397,  386,  373,  364,  351,  341,  334,  324,  317,  309,
                            299,  294,  287,  279,  274,  267,  262,  257,  251,  247,
                            243,  237,  232,  227,  222,  217,  213,  207,  202,  198,
                            192,  188,  183,  177,  173,  168,  163,  157,  153,  148,
                            143,  139,  132,  128,  125,  119,  115,  108,  104,   99,
                            94,   90,   84,   79,   74,   70,   64,   59,   55,   49,
                            45,   40,   34,   30,   25,   20,   15,   11,    6,    4,
                            0
                    },
                    sums[101] =
                    {
                            32640, 32635, 32266, 31495, 30665, 29804, 29146, 28599, 28104,
                            27670, 27225, 26725, 26210, 25716, 25240, 24789, 24373, 23946,
                            23572, 22846, 21801, 20842, 19949, 19121, 18386, 17651, 16998,
                            16349, 15800, 15247, 14783, 14321, 13859, 13535, 13081, 12702,
                            12423, 12056, 11779, 11513, 11135, 10955, 10676, 10392, 10208,
                            9928,  9747,  9564,  9369,  9193,  9017,  8822,  8639,  8458,
                            8270,  8084,  7896,  7710,  7527,  7347,  7156,  6977,  6788,
                            6607,  6422,  6236,  6054,  5867,  5684,  5495,  5305,  5128,
                            4945,  4751,  4638,  4442,  4248,  4065,  3888,  3698,  3509,
                            3326,  3139,  2957,  2775,  2586,  2405,  2216,  2037,  1846,
                            1666,  1483,  1297,  1109,   927,   735,   554,   375,   201,
                            128,     0
                    };

            qvalue = (ssize_t) (jpeg_info->quant_tbl_ptrs[0]->quantval[2]
                                + jpeg_info->quant_tbl_ptrs[0]->quantval[53]
                                + jpeg_info->quant_tbl_ptrs[1]->quantval[0]
                                + jpeg_info->quant_tbl_ptrs[1]->quantval[DCTSIZE2-1]);
            for (i = 0; i < 100; i++) {
                if ((qvalue < hash[i]) && (sum < sums[i]))
                    continue;
                if (((qvalue <= hash[i]) && (sum <= sums[i])) || (i >= 50))
                    quality = (size_t) i+1;
                break;
            }
        } else if (jpeg_info->quant_tbl_ptrs[0] != NULL) {
            ssize_t hash[101] =
                    {
                            510,  505,  422,  380,  355,  338,  326,  318,  311,  305,
                            300,  297,  293,  291,  288,  286,  284,  283,  281,  280,
                            279,  278,  277,  273,  262,  251,  243,  233,  225,  218,
                            211,  205,  198,  193,  186,  181,  177,  172,  168,  164,
                            158,  156,  152,  148,  145,  142,  139,  136,  133,  131,
                            129,  126,  123,  120,  118,  115,  113,  110,  107,  105,
                            102,  100,   97,   94,   92,   89,   87,   83,   81,   79,
                            76,   74,   70,   68,   66,   63,   61,   57,   55,   52,
                            50,   48,   44,   42,   39,   37,   34,   31,   29,   26,
                            24,   21,   18,   16,   13,   11,    8,    6,    3,    2,
                            0
                    },
                    sums[101] =
                    {
                            16320, 16315, 15946, 15277, 14655, 14073, 13623, 13230, 12859,
                            12560, 12240, 11861, 11456, 11081, 10714, 10360, 10027,  9679,
                            9368,  9056,  8680,  8331,  7995,  7668,  7376,  7084,  6823,
                            6562,  6345,  6125,  5939,  5756,  5571,  5421,  5240,  5086,
                            4976,  4829,  4719,  4616,  4463,  4393,  4280,  4166,  4092,
                            3980,  3909,  3835,  3755,  3688,  3621,  3541,  3467,  3396,
                            3323,  3247,  3170,  3096,  3021,  2952,  2874,  2804,  2727,
                            2657,  2583,  2509,  2437,  2362,  2290,  2211,  2136,  2068,
                            1996,  1915,  1858,  1773,  1692,  1620,  1552,  1477,  1398,
                            1326,  1251,  1179,  1109,  1031,   961,   884,   814,   736,
                            667,   592,   518,   441,   369,   292,   221,   151,    86,
                            64,     0
                    };

            qvalue = (ssize_t) (jpeg_info->quant_tbl_ptrs[0]->quantval[2]
                                + jpeg_info->quant_tbl_ptrs[0]->quantval[53]);
            for (i = 0; i < 100; i++) {
                if ((qvalue < hash[i]) && (sum < sums[i]))
                    continue;
                if (((qvalue <= hash[i]) && (sum <= sums[i])) || (i >= 50))
                    quality = (size_t)i+1;
                break;
            }
        }
        image->quality = (int) quality;
        return true;
    }

    JpegImageCodec::JpegImageCodec() {

    }

    JpegImageCodec::~JpegImageCodec() {

    }

    void JpegImageCodec::jerror_exit(j_common_ptr jinfo) {
        jerror_mgr *err = (jerror_mgr *) jinfo->err;
        LOGE("fatal error when decompress: %d", err->base.msg_code);
        longjmp(err->jmp, 1);
    }

    JpegImageDecoder::JpegImageDecoder() {
        m_signature = "\xFF\xD8\xFF";
        static const unsigned char signature_[12] = {0, 0, 0, 0x0c, 'j', 'P', ' ', ' ', 13, 10,
                                                     0x87, 10};
        m_signature_2k = string((const char *) signature_,
                                (const char *) signature_ + sizeof(signature_));
        m_is_2k = false;
        m_insamplesize_max = 8; // official data
        m_scale_num = 1;
    }

    JpegImageDecoder::~JpegImageDecoder() {

    }

    size_t JpegImageDecoder::signatureLength() const {
        return m_signature_2k.size();// the longer one
    }

    bool JpegImageDecoder::checkSignature(const string &signature) {
        size_t len = signatureLength();
        if (signature.size() >= len) {
            if (memcmp(signature.c_str(), m_signature.c_str(), m_signature.size()) == 0) {
                m_is_2k = false;
                return true;
            } else if (memcmp(signature.c_str(), m_signature_2k.c_str(), m_signature_2k.size()) ==
                       0) {
                m_is_2k = true;
                return true;
            }
        }
        return false;
    }

    bool JpegImageDecoder::setInSampleSize(int inSampleSize) {
        /**
         * libjpeg-turbo supports scaling by 1/8, 1/4, 3/8, 1/2, 5/8, 3/4, 7/8, and 1/1
         * round to this range.
         */
        m_insamplesize = inSampleSize;
        double desiredScale = 1.0 / inSampleSize;
        if (desiredScale >= 0.9375) {
            m_scale_num = 8;
        } else if (desiredScale >= 0.8125) {
            m_scale_num = 7;
        } else if (desiredScale >= 0.6875f) {
            m_scale_num = 6;
        } else if (desiredScale >= 0.5625f) {
            m_scale_num = 5;
        } else if (desiredScale >= 0.4375f) {
            m_scale_num = 4;
        } else if (desiredScale >= 0.3125f) {
            m_scale_num = 3;
        } else if (desiredScale >= 0.1875f) {
            m_scale_num = 2;
        } else {
            m_scale_num = 1;
        }
        LOGI("inSampleSize=%d desiredScale=%f scaleNum=%d"
            , inSampleSize, desiredScale, m_scale_num);
        return true;
    }

    int JpegImageDecoder::readHeader(Image **image) {
        FILE *infile;
        if ((infile = fopen(m_filename.c_str(), "rb")) == NULL) {
            LOGE("can't open %s", m_filename.c_str());
            return resultCode.failed;
        }

        jerror.base.error_exit = jerror_exit;
        cinfo_decompress.err = jpeg_std_error(&jerror.base);
        if (setjmp(jerror.jmp)) {
            LOGE("fatal error when decompress");
            return resultCode.failed;
        }

        jpeg_create_decompress(&cinfo_decompress);
        jpeg_stdio_src(&cinfo_decompress, infile);
        int ret = jpeg_read_header(&cinfo_decompress, TRUE);
        if (ret != JPEG_HEADER_OK) {
            LOGE("can't read image header %s", m_filename.c_str());
            return resultCode.failed;
        }

        if (*image) {
            delete *image;
            *image = NULL;
        }
        *image = new JpegImage();
        (*image)->width = cinfo_decompress.image_width;
        (*image)->height = cinfo_decompress.image_height;
        ((JpegImage *) *image)->setColorSpace(cinfo_decompress.out_color_space);
        // will be update when set color space
        (*image)->channels = cinfo_decompress.num_components;
        // fetch original image's compress quality
        setDefaultQuality(&cinfo_decompress, *image);

        jpeg_destroy_decompress(&cinfo_decompress);
        fclose(infile);

        return resultCode.success;
    }

    int JpegImageDecoder::readData(Image **image) {
        FILE *infile;
        if ((infile = fopen(m_filename.c_str(), "rb")) == NULL) {
            LOGE("can't open %s", m_filename.c_str());
            return resultCode.failed;
        }

        jerror.base.error_exit = jerror_exit;
        cinfo_decompress.err = jpeg_std_error(&jerror.base);
        if (setjmp(jerror.jmp)) {
            LOGE("fatal error when decompress");
            return resultCode.failed;
        }

        jpeg_create_decompress(&cinfo_decompress);
        jpeg_stdio_src(&cinfo_decompress, infile);
        int ret = jpeg_read_header(&cinfo_decompress, TRUE);
        if (ret != JPEG_HEADER_OK) {
            LOGE("can't read image header %s", m_filename.c_str());
            return resultCode.failed;
        }
        // set decode parameters
        cinfo_decompress.scale_num = m_scale_num;
        cinfo_decompress.scale_denom = 8;

        // Check if we will decode to CMYK because a conversion to RGBA is not supported
        bool isCMYK = JCS_CMYK == cinfo_decompress.jpeg_color_space
                      || JCS_YCCK == cinfo_decompress.jpeg_color_space;
        int bytesPerPixel = 0;
        if (isCMYK) {
            LOGW("Unsupported color space %d", colorSpace);
            return resultCode.color_space_not_support;
        } else if (ColorSpace::RGB565 == colorSpace) {
            cinfo_decompress.dither_mode = JDITHER_NONE;
            cinfo_decompress.out_color_space = JCS_RGB565;
            bytesPerPixel = 2;
        } else if (ColorSpace::RGB888 == colorSpace) {
            cinfo_decompress.out_color_space = JCS_RGB;
            bytesPerPixel = 3;
        } else if (ColorSpace::RGBA8888 == colorSpace) {
            cinfo_decompress.out_color_space = JCS_EXT_RGBA;
            bytesPerPixel = 4;
        } else {
            LOGW("Unsupported color space %d", colorSpace);
            return resultCode.color_space_not_support;
        }

        jpeg_start_decompress(&cinfo_decompress);
        int row_stride = cinfo_decompress.output_width * bytesPerPixel;
        int buffer_height = READ_LINES_ONE_SCAN;
        JSAMPARRAY buffer = (JSAMPARRAY) malloc(sizeof(JSAMPROW) * buffer_height);
        for (int i = 0; i < buffer_height; i++) {
            buffer[i] = (JSAMPROW) malloc(sizeof(JSAMPLE) * row_stride);
        }

        if (*image) {
            delete *image;
            image = NULL;
        }
        *image = new JpegImage();
        (*image)->width = cinfo_decompress.output_width;
        (*image)->height = cinfo_decompress.output_height;
        (*image)->channels = cinfo_decompress.output_components;
        (*image)->colorSpace = colorSpace; // output color space
        (*image)->data = new unsigned char[cinfo_decompress.output_width
                                           * cinfo_decompress.output_height
                                           * cinfo_decompress.output_components];
        (*image)->bytesPerPixel = bytesPerPixel;
        // fetch original image's compress quality
        setDefaultQuality(&cinfo_decompress, *image);

        int cursor = 0;
        while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
            uint lines = jpeg_read_scanlines(&cinfo_decompress, buffer, (JDIMENSION) buffer_height);
            for (int i = 0; i < lines; i++) {
                cursor = ((cinfo_decompress.output_scanline - lines) + i) * row_stride;
                memcpy((*image)->data + cursor, buffer[i], (size_t) row_stride);
            }
        }

        jpeg_finish_decompress(&cinfo_decompress);
        jpeg_destroy_decompress(&cinfo_decompress);
        fclose(infile);
        if (NULL != buffer) {
            for (int i = 0; i < buffer_height; i++) {
                free(buffer[i]);
            }
            free(buffer);
            buffer = NULL;
        }

        return resultCode.success;
    }

    BaseImageDecoder* JpegImageDecoder::newDecoder() {
        return new JpegImageDecoder();
    }

    JpegImageEncoder::JpegImageEncoder() {
        m_description = "*.jpeg;*.jpg;*.jpe;*.jp2";
    }

    JpegImageEncoder::~JpegImageEncoder() {

    }

    int JpegImageEncoder::writeData(const Image *image) {
        if (!image || !image->data) {
            LOGE("input image is null");
            return resultCode.failed;
        }
        FILE *file;
        if ((file = fopen(m_filename.c_str(), "wb")) == NULL) {
            LOGE("can't open %s", m_filename.c_str());
            return resultCode.failed;
        }

        cinfo_compress.err = jpeg_std_error(&jerror.base);
        jerror.base.error_exit = jerror_exit;
        if (setjmp(jerror.jmp)) {
            LOGE("fatal error when compress");
            return resultCode.failed;
        }

        jpeg_create_compress(&cinfo_compress);

        jpeg_stdio_dest(&cinfo_compress, file);
        cinfo_compress.image_width = (JDIMENSION) image->width;
        cinfo_compress.image_height = (JDIMENSION) image->height;
        cinfo_compress.input_components = image->channels;
        cinfo_compress.in_color_space = ((JpegImage *) image)->getJColorSpace();
        cinfo_compress.optimize_coding = m_optimize;
        int bytesPerPixel;
        if (JCS_RGB565 == cinfo_compress.in_color_space) {
            LOGW("Unsupported color space %d", cinfo_compress.in_color_space);
            return resultCode.color_space_not_support;
        } else if (JCS_RGB == cinfo_compress.in_color_space) {
            bytesPerPixel = 3;
        } else if (JCS_EXT_RGBA == cinfo_compress.in_color_space) {
            bytesPerPixel = 4;
        } else {
            LOGW("Unsupported color space %d", cinfo_compress.in_color_space);
            return resultCode.color_space_not_support;
        }
        jpeg_set_defaults(&cinfo_compress);
        jpeg_set_quality(&cinfo_compress, m_quality, TRUE);

        jpeg_start_compress(&cinfo_compress, TRUE);

        int row_stride_dst = cinfo_compress.image_width * bytesPerPixel;
        int buffer_height = WRITE_LINES_ONE_SCAN;
        JSAMPARRAY buffer = (JSAMPARRAY) malloc(sizeof(JSAMPROW) * buffer_height);

        int remain_lines = cinfo_compress.image_height;
        while (cinfo_compress.next_scanline < cinfo_compress.image_height) {
            for (int i = 0; i < buffer_height && remain_lines > 0; i++, remain_lines--) {
                buffer[i] = &image->data[(cinfo_compress.next_scanline + i) * row_stride_dst];
            }
            jpeg_write_scanlines(&cinfo_compress, buffer, (JDIMENSION) buffer_height);
        }

        jpeg_finish_compress(&cinfo_compress);
        jpeg_destroy_compress(&cinfo_compress);
        fclose(file);

        return resultCode.success;
    }

    BaseImageEncoder* JpegImageEncoder::newEncoder() {
        return new JpegImageEncoder();
    }

}