@echo off

if not exist ninja (
    mkdir ninja
)
cd ninja


cmake ..\..\..\ -G "Ninja"                                        ^
    -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%/build/cmake/android.toolchain.cmake"        ^
    -DCMAKE_MAKE_PROGRAM="F:\ninja-win\ninja.exe"                                    ^
    -DANDROID_NATIVE_API_LEVEL=16                                                    
    rem -DANDROID_ABI=armeabi-v7a                                                    ^
    rem -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8

pause
