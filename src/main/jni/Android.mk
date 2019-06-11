LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := jpeg-turbo
LOCAL_SRC_FILES = jpeg/$(TARGET_ARCH_ABI)/libturbojpeg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := imagecompress
LOCAL_EXPORT_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/jpeg/include \
                $(LOCAL_PATH)/resampler/Base \
                $(LOCAL_PATH)/resampler/Operators

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/resampler/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/resampler/Base/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/resampler/Kernels/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/resampler/Operators/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/resampler/Utilities/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS := -L$(LOCAL_PATH) \
        -llog  \
        -lstdc++ \
        -ljnigraphics
LOCAL_STATIC_LIBRARIES := jpeg-turbo
include $(BUILD_SHARED_LIBRARY)
