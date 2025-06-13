LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_MODULE := AbtFixes
else
	LOCAL_MODULE := AbtFixes64
endif
LOCAL_SRC_FILES := libjaypatch.cpp mod/config.cpp
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++17
LOCAL_C_INCLUDES += $(LOCAL_PATH)/patches
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_CFLAGS += -mfpu=neon
endif
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)
