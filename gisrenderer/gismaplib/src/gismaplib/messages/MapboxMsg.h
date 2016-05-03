#ifndef GMCORE_MAPBOXMSG_H
#define GMCORE_MAPBOXMSG_H

#include "gismaplib/messages/GmCoreMsg.h"

//#include <array>

namespace gmcore
{
  class MapboxMsg
      : public GmCoreMsg
  {
  public:
    explicit MapboxMsg(boost::shared_ptr <GmMsg::Message::Reader> msgPtrQ)
        : GmCoreMsg(msgPtrQ)
    { }


  protected:
    void dataWorker(
        boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
        boost::shared_ptr <GmMsg::Message::Builder> msgPtrR);
  };
}  // namespace gmcore

#endif  // GMCORE_MAPBOXMSG_H
