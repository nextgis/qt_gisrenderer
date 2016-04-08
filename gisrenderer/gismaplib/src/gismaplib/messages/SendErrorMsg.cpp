#include "gismaplib/messages/SendErrorMsg.h"

#include <boost/lexical_cast.hpp>

#include <iostream>

#include "gismaplib/GmLog.h"


namespace gmcore
{
  void SendErrorMsg::dataWorker(
      boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
      boost::shared_ptr <GmMsg::Message::Builder>   msgPtrR)
  {
    // make the reply data
    std::string errorMsg = "ERROR: unknown message type: " + boost::lexical_cast <std::string>(
        mMsgPtrQ->getMsgType());
    BOOST_LOG_SEV(GmLog::log, debug) << errorMsg;

    // set the reply type and data
    msgPtrR->setMsgType(GmConst::MSG_TYPE_ERROR);
    msgPtrR->setErrorFlag(true);
    msgPtrR->setMsgText(errorMsg); // TODO: avoid make copy of errorMsg
  }  // SendErrorMsg::dataWorker
}  // namespace gmcore
