git clone https://github.com/dwrensha/capnproto-java.git

cp capnproto-java/runtime/src/main/java <project>/capnproto-java/src/main
cp capnproto-java/compiler/src/main/schema/capnp/java.capnp <project>/gmcore/prebuild-libs/capnproto-cpp/include/capnp
cp capnpc-java.exe.zip/*.exe <project>/gmcore/prebuild-libs/capnproto-cpp/tools
