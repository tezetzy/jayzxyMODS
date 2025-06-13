LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := jayzpatch
LOCAL_SRC_FILES := bandul.cpp mod/logger.cpp mod/config.cpp
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++17
LOCAL_C_INCLUDES += ./include
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
