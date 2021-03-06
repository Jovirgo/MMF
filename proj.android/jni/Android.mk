LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AnimationReaderForCocos2d.cpp \
                   ../../Classes/AppDelegate.cpp \
				   ../../Classes/BagScene.cpp \
                   ../../Classes/CsvFileForCocos2d.cpp \
                   ../../Classes/GameBlock.cpp \
                   ../../Classes/GameBlocksCountManager.cpp \
                   ../../Classes/GameBlockManager.cpp \
                   ../../Classes/GameBlockManagerDerivation.cpp \
                   ../../Classes/GameData.cpp \
                   ../../Classes/GameLevelMapScene.cpp \
                   ../../Classes/GamePlayScene.cpp \
                   ../../Classes/GameScoreMap.cpp \
                   ../../Classes/GameSettings.cpp \
                   ../../Classes/GameZoomMenuItem.cpp \
                   ../../Classes/LoadAndSaveDataForCocos2d.cpp \
                   ../../Classes/NumberCountingLabel.cpp \
                   ../../Classes/PlayerStats.cpp \
                   ../../Classes/ReadyScene.cpp \
                   ../../Classes/ResultScene.cpp \
                   ../../Classes/ShopScene.cpp \
                   ../../Classes/TitleScene.cpp \
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
