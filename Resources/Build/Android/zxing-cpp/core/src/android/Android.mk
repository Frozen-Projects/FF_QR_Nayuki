LOCAL_PATH := $(call my-dir)
ZXING_PATH := $(LOCAL_PATH)/../core/src

include $(CLEAR_VARS)
LOCAL_MODULE := zxing

LOCAL_CPP_FEATURES += exceptions

LOCAL_C_INCLUDES += $(ZXING_PATH)

LOCAL_CPPFLAGS := -DNO_ICONV=1

#traverse all the directory and subdirectory
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

#find all the file recursively under core/src/
ALLFILES := $(call walk, $(ZXING_PATH))
FILE_LIST := $(filter %.cpp, $(ALLFILES))
FILE_LIST += $(filter %.c, $(ALLFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

#$(warning $(LOCAL_SRC_FILES))

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)