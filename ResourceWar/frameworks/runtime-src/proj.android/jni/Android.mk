LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua

LOCAL_SRC_FILES := hellolua/main.cpp \
 ../../Classes/AppDelegate.cpp \
../../Classes/utils.cpp \
../../Classes/thread.cpp \
../../Classes/ByteBuffer.cpp \
../../Classes/TcpClient.cpp \
../../Classes/TcpManager.cpp \
../../Classes/ClientMessage.cpp \
../../Classes/lua_oneself_auto.cpp \
../../Classes/lua_oneself_manual.cpp \
../../Classes/GlobalSchedule.cpp \
../../Classes/Shake.cpp \
../../Classes/ScrollViewEx.cpp \
../../Classes/IniUtil.cpp \
../../Classes/TableUtil.cpp \
../../Classes/LaunchScene.cpp \
../../Classes/PlatformUtils.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
					
LOCAL_STATIC_LIBRARIES := cocos2d_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,scripting/lua-bindings/proj.android)
