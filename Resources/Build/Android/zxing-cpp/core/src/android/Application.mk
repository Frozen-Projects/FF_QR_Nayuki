APP_ABI := armeabi-v7a arm64-v8a x86 x86_64

APP_STL := c++_static
APP_CPPFLAGS  := -std=c++17

APP_CFLAGS := -pthread -frtti -fexceptions -O3 -mfloat-abi=softfp -mfpu=neon

LOCAL_ARM_NEON := true
APP_OPTIM := release