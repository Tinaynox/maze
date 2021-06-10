@echo off

if not exist gradle-android-ninja\app\.externalNativeBuild\cmake\arm7Debug\armeabi-v7a (
    mkdir gradle-android-ninja\app\.externalNativeBuild\cmake\arm7Debug\armeabi-v7a
)
cd gradle-android-ninja\app\.externalNativeBuild\cmake\arm7Debug\armeabi-v7a


%ANDROID_SDK%/cmake/3.6.4111459/bin/cmake.exe ..\..\..\..\..\..\..\..\ -G "Android Gradle - Ninja"                ^
    -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%/build/cmake/android.toolchain.cmake"                                    ^
    -DANDROID_TOOLCHAIN=clang                                                                                    ^
    -DANDROID_ABI=armeabi-v7a                                                                                    ^
    -DCMAKE_MAKE_PROGRAM="%ANDROID_SDK%/cmake/3.6.4111459/bin/ninja.exe"                                        ^
    -DCMAKE_BUILD_TYPE=Debug                                                                                    ^
    -DANDROID_NATIVE_API_LEVEL=16                                                    
    
    
    rem                                                     ^
    rem -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8

pause
