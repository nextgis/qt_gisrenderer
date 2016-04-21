
Building capnproto library with CrystaX NDK in Debian Linux for Android


1. [TODO] Installing Android NDK Standalone Toolchain and variables, see
<project>/gismaplib/imported/libs_prebuilt/readme.txt


2. Build capnproto library

git clone https://github.com/thomaslee/capnproto-debian.git

./configure
make -j4 check
sudo make install


3. [TODO] Copying to project

cp /local32/capnproto-c++-0.5.3_inst/include <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp
cp /local32/capnproto-c++-0.5.3_inst/lib <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp

# must be like so:
# <project>/gismaplib/imported/libs_prebuilt/capnproto-cpp
#     /include
#     /lib
#     readme.txt
