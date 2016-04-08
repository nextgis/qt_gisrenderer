
Building prebuilt libraries with CrystaX NDK in Msys2 on Windows for Android
(this is similar on the other systems)


1. If CrystaX NDK and Standalone Toolchain is installed (if not then see 2nd paragraph)

mount e:/Dev/Tools/Android/crystax-ndk-10.3.1 /local32/crystax-ndk-10.3.1
export ANDROID_NDK_ROOT=/local32/crystax-ndk-10.3.1
export NDK_STANDALONE_TOOLCHAIN_ROOT=$ANDROID_NDK_ROOT/standalone_toolchain/android-9/arm-linux-androideabi-5
export PATH=$PATH:$NDK_STANDALONE_TOOLCHAIN_ROOT/bin:$NDK_STANDALONE_TOOLCHAIN_ROOT/arm-linux-androideabi/bin


2. Download and install CrystaX NDK
https://www.crystax.net/ru/download


3. Installing Android NDK (CrystaX NDK) Standalone Toolchain

# If you use C11 standard library features,
# you MUST specify using the gnustl C library (as opposed to STLport)
# or compilation will fail.

mount e:/Dev/Tools/Android/crystax-ndk-10.3.1 /local32/crystax-ndk-10.3.1
export ANDROID_NDK_ROOT=/local32/crystax-ndk-10.3.1

# if use window 7/8 64bit add additional options: --system=windows-x86_64
$ANDROID_NDK_ROOT/build/tools/make-standalone-toolchain.sh --platform=android-9 --toolchain=arm-linux-androideabi-5 --stl=gnustl --system=windows-x86_64 --install-dir=$ANDROID_NDK_ROOT/standalone_toolchain/android-9/arm-linux-androideabi-5

export NDK_STANDALONE_TOOLCHAIN_ROOT=$ANDROID_NDK_ROOT/standalone_toolchain/android-9/arm-linux-androideabi-5
export PATH=$PATH:$NDK_STANDALONE_TOOLCHAIN_ROOT/bin:$NDK_STANDALONE_TOOLCHAIN_ROOT/arm-linux-androideabi/bin


4. For build prebuild-libs see <lib>/readme.txt
