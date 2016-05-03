@0xe73356180f4d0d6c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("GmMsg");

using Java = import "/capnp/java.capnp";
$Java.package("com.nextgis.gismaplib.capnproto");
$Java.outerClassname("GmMsg");

using GmConst = import "/GmConst.capnp";


struct Message
{
  errorFlag @0 :Bool = false;
  msgType @1 :Int32;
  msgText @2 :Text;
  dataPointer @3 :AnyPointer;
}


struct MapboxQ
{
  imagePointer @0 :UInt64;
  imageWidth @1 :UInt32;
  imageHeight @2 :UInt32;
}
struct MapboxR
{
  isImageStored @0 :Bool = false;
}



# for test
struct GetTestTextQ
{
  testPath @0 :Text;
}
struct GetTestTextR
{
  testText @0 :Text;
}
