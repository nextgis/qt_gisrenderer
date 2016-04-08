#include "gismaplib/GmCore.h"

#include <boost/thread.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <capnp/serialize.h>

#include "gismaplib/capnproto/GmMsg.capnp.h"
#include "gismaplib/capnproto/GmConst.capnp.h"

#include "gismaplib/messages/SendErrorMsg.h"
#include "gismaplib/messages/GetTestTextMsg.h"


namespace gmcore
{
  GmCore::GmCore() {}


  void GmCore::gmcoreInit()
  {
    std::cout << "logger initialization" << std::endl;
    GmLog::initGmLog();
    BOOST_LOG_SEV(GmLog::log, info) << "logger is initialized";
  }


  kj::Array <capnp::word> GmCore::messageWorker(
      void*         segmentsPtrsQ,
      long long int segmentsSizesQ)
  {
    // to the capnp query
    capnp::word* queryWords = static_cast <capnp::word*>(segmentsPtrsQ);
    size_t       querySize = segmentsSizesQ / sizeof(capnp::word);

    kj::DestructorOnlyArrayDisposer adp;
    kj::Array <capnp::word>         msgArray(queryWords, querySize, adp);
    kj::ArrayPtr <capnp::word>      arrayPtrQ = msgArray.asPtr();
    capnp::FlatArrayMessageReader   cpnQuery(arrayPtrQ);


    // read the capnproto struct
    GmMsg::Message::Reader msgQ = cpnQuery.getRoot <GmMsg::Message>();
    auto                   msgPtrQ = boost::make_shared <GmMsg::Message::Reader>(msgQ);

    // the new capnproto reply
    boost::shared_ptr <capnp::MallocMessageBuilder> cpnReply;

    // work the query by the query type
    switch (msgPtrQ->getMsgType()) {
        case GmConst::MSG_TYPE_GET_TEST_TEXT: {
          GetTestTextMsg msg(msgPtrQ);
          cpnReply = msg.msgWorker();
          break;
        }

        default: {
          SendErrorMsg msg(msgPtrQ);
          cpnReply = msg.msgWorker();
          break;
        }
    }  // switch


    // send the reply
    if (nullptr != cpnReply) {
      // capnproto message to the serialization
      kj::Array <capnp::word> replyWords = messageToFlatArray(*cpnReply);
      return (kj::mv(replyWords));
    }

    kj::Array <capnp::word> empty = kj::heapArray <capnp::word>(0);
    return (kj::mv(empty));
  }  // GmCore::messageWorker
}  // namespace gmcore
