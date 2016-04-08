#ifndef GMCORE_SENDERRORMSG_H
#define GMCORE_SENDERRORMSG_H

#include "gismaplib/messages/GmCoreMsg.h"


namespace gmcore
{
  class SendErrorMsg
    : public GmCoreMsg
  {
  public:
    explicit SendErrorMsg(boost::shared_ptr <GmMsg::Message::Reader> msgPtrQ)
      : GmCoreMsg(msgPtrQ)                                                                         {}

  protected:
    void dataWorker(
        boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
        boost::shared_ptr <GmMsg::Message::Builder>   msgPtrR);
  };
}  // namespace gmcore

#endif  // GMCORE_SENDERRORMSG_H
