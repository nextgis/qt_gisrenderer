
Building capnproto library with CrystaX NDK in Msys2 on Windows for Android
(this is similar on the other systems)


1. Installing Android NDK Standalone Toolchain and variables, see
<project>/gismaplib/imported/libs_prebuilt/readme.txt


2. Build capnproto library
Download from: https://capnproto.org/install.html

# unzip capnproto-c++-win32-0.5.3.zip
cp capnproto-tools-win32-0.5.3/capnp.exe /usr/bin

# untar capnproto-c++-0.5.3.tar.gz
cd /local32/capnproto-c++-0.5.3

./configure --host=arm-linux-androideabi --enable-static --disable-shared --prefix=/local32/capnproto-c++-0.5.3_inst --with-external-capnp

make -j8 && make install


3. Copying to project

cp /local32/capnproto-c++-0.5.3_inst/include <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp
cp /local32/capnproto-c++-0.5.3_inst/lib <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp

# must be like so:
# <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp
#     /include
#     /lib
#     readme.txt


4. Copying compiled tools for Windows

cp capnproto-c++-win32-0.5.3.zip/capnproto-tools-win32-0.5.3/*.exe <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp/tools
