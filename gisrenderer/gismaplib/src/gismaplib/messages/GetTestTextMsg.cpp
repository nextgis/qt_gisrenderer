#include "gismaplib/messages/GetTestTextMsg.h"

#include "gismaplib/GmLog.h"
#include "gismaplib/utils/SharedPointers.h"


namespace gmcore
{
  std::string* bigStr = nullptr;

  std::string* getBigStr()
  {
    std::string* src = new std::string("abcdefghi ");  // 10 chars
    std::string* dst = new std::string();

    for (int i = 0; i < 100000; ++i) {
      // 10 * 100 000 = 1 000 000 chars
      *dst += *src;
    }

    return (dst);
  }  // getBigStr


  void Deleter(std::string* ptr) {}


  SharedString getBigTestText(const std::string& testPath)
  {
    if (nullptr == bigStr) {
      bigStr = getBigStr();
      BOOST_LOG_SEV(GmLog::log, debug) << "big str created.";
    }
    std::shared_ptr <std::string> str(bigStr, Deleter);

    return (str);
  }  // getBigTestText


  SharedString getTestText(const std::string& testPath)
  {
    auto str = makeSharedString("test text");
    return (str);
  }


  void GetTestTextMsg::dataWorker(
      boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
      boost::shared_ptr <GmMsg::Message::Builder>   msgPtrR)
  {
    // get the query data
    GmMsg::GetTestTextQ::Reader dataQ = dataPtrQ->getAs <GmMsg::GetTestTextQ>();
    // TODO: avoid make copy of cStr()
    std::string                 testPath = dataQ.getTestPath().cStr();
//    BOOST_LOG_SEV(GmLog::log, debug) << "testPath: " << testPath;

    // make the reply data
    // get test text
//    BOOST_LOG_SEV(GmLog::log, debug) << "get test text";

    auto testText = getTestText(testPath);
//    auto testText = getBigTestText(testPath);

//    BOOST_LOG_SEV(GmLog::log, debug) << "test text: " << testText->c_str();
//    BOOST_LOG_SEV(GmLog::log, debug) << "c++ byte size: " << testText->size();

    // set the reply data
    if (nullptr != testText) {
      capnp::AnyPointer::Builder   dataPtrR = msgPtrR->initDataPointer();
      GmMsg::GetTestTextR::Builder dataR = dataPtrR.initAs <GmMsg::GetTestTextR>();
      dataR.setTestText(testText->c_str());
    }
    else {
      throw GmCoreErrEx() << GmCoreErrInfo("getTestText() error, nullptr == testText");
    }
  }  // GetTestTextMsg::dataWorker
}  // namespace gmcore
