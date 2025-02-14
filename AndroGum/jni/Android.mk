LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE 				:= libgumjs
LOCAL_SRC_FILES 			:= libfrida-gumjs.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE 		:= androgum

LOCAL_SRC_FILES		:= main.cpp

LOCAL_STATIC_LIBRARIES 		:= libgumjs
LOCAL_CFLAGS 				:= -w

LOCAL_LDLIBS		:= -landroid -llog -lz
include $(BUILD_SHARED_LIBRARY)