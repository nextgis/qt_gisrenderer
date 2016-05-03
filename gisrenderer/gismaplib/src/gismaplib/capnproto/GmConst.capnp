@0xaf2d9e0e16990632;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("GmConst");

using Java = import "/capnp/java.capnp";
$Java.package("com.nextgis.gismaplib.capnproto");
$Java.outerClassname("GmConst");


const unknownId :UInt8 = 0;

const msgTypeUnknown :Int32 = 0;
const msgTypeMapboxImageData :Int32 = 1;
const msgTypeError :Int32 = 1000;

# for test
const msgTypeGetTestText :Int32 = 999;
