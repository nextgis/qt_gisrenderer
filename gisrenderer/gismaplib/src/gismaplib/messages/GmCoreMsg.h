#ifndef GMCORE_GMCOREMSG_H
#define GMCORE_GMCOREMSG_H

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/exception/all.hpp>

#include <capnp/message.h>

#include "gismaplib/capnproto/GmConst.capnp.h"
#include "gismaplib/capnproto/GmMsg.capnp.h"


namespace gmcore
{
  typedef boost::error_info <struct errInfoMsg_, std::string> GmCoreErrInfo;

  struct GmCoreErrEx
      : virtual boost::exception
  {
  };


  class GmCoreMsg
  {
  public:
    explicit GmCoreMsg(boost::shared_ptr <GmMsg::Message::Reader> msgPtrQ);

    boost::shared_ptr <capnp::MallocMessageBuilder> msgWorker();

  protected:
    virtual void dataWorker(
        boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
        boost::shared_ptr <GmMsg::Message::Builder> msgPtrR) = 0;

    boost::shared_ptr <GmMsg::Message::Reader> mMsgPtrQ;
  };
}  // namespace gmcore

#endif  // GMCORE_GMCOREMSG_H
