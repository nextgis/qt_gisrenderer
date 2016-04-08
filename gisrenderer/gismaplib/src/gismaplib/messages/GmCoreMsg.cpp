#include "gismaplib/messages/GmCoreMsg.h"

#include <string>
#include <iostream>
#include <fstream>

#include <capnp/message.h>



namespace gmcore
{
  GmCoreMsg::GmCoreMsg(boost::shared_ptr <GmMsg::Message::Reader> msgPtrQ)
  {
    mMsgPtrQ = msgPtrQ;
  }


  boost::shared_ptr <capnp::MallocMessageBuilder> GmCoreMsg::msgWorker()
  {
    // the new capnproto reply
    auto cpnPtrR = boost::make_shared <capnp::MallocMessageBuilder>();
    auto msgPtrR =
        boost::make_shared <GmMsg::Message::Builder>(cpnPtrR->initRoot <GmMsg::Message>());

    // set the reply type
    msgPtrR->setMsgType(mMsgPtrQ->getMsgType());

    try {
      // get the query data
      auto dataPtrQ = boost::make_shared <capnp::AnyPointer::Reader>(mMsgPtrQ->getDataPointer());
      // make and set the reply data
      dataWorker(dataPtrQ, msgPtrR);
    }
    catch (boost::exception& ex) {
      // set the error reply data
      std::string errInfo;
      if (const std::string* errInfoPtr = boost::get_error_info <GmCoreErrInfo>(ex)) {
        errInfo += *errInfoPtr;
      }

      msgPtrR->setErrorFlag(true);
      msgPtrR->setMsgText(errInfo);
    }


    return (cpnPtrR);
  }  // GmCoreMsg::msgWorker
}  // namespace gmcore
