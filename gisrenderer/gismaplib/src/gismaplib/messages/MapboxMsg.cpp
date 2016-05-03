#include "gismaplib/messages/MapboxMsg.h"

#include "gismaplib/GmLog.h"
#include "gismaplib/utils/SharedPointers.h"

namespace gmcore
{
  void MapboxMsg::dataWorker(
      boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
      boost::shared_ptr <GmMsg::Message::Builder> msgPtrR)
  {
    // get the query data
    GmMsg::MapboxQ::Reader dataQ = dataPtrQ->getAs <GmMsg::MapboxQ>();
    std::string testPath = dataQ.getTestPath().cStr();
//    BOOST_LOG_SEV(GmLog::log, debug) << "testPath: " << testPath;

    // make the reply data
    // get image data
    kj::byte imageData[5] = {1, 2, 3, 4, 5};

    // set the reply data
    if (imageData) {
      capnp::AnyPointer::Builder dataPtrR = msgPtrR->initDataPointer();
      GmMsg::MapboxR::Builder dataR = dataPtrR.initAs <GmMsg::MapboxR>();
      dataR.setImageData(capnp::Data::Reader(imageData, 5));
    } else {
      throw GmCoreErrEx() << GmCoreErrInfo("getTestText() error, nullptr == testText");
    }
  }
}  // namespace gmcore
