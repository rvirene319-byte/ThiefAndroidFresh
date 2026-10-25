#include <android/log.h>

#define LOG_TAG "ThiefAndroid"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" __attribute__((visibility("default")))
void thief_android_init() {
    LOGI("Fresh Thief Android native runtime initialized.");
}
