#ifndef GMCORE_GETTESTTEXTMSG_H
#define GMCORE_GETTESTTEXTMSG_H

#include "gismaplib/messages/GmCoreMsg.h"


namespace gmcore
{
  class GetTestTextMsg
    : public GmCoreMsg
  {
  public:
    explicit GetTestTextMsg(boost::shared_ptr <GmMsg::Message::Reader> msgPtrQ)
      : GmCoreMsg(msgPtrQ)
    {}

  protected:
    void dataWorker(
        boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
        boost::shared_ptr <GmMsg::Message::Builder>   msgPtrR);
  };
}  // namespace gmcore

#endif  // GMCORE_GETTESTTEXTMSG_H
