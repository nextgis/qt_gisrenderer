#include "gismaplib/messages/MapboxMsg.h"

#include "gismaplib/GmLog.h"
//#include "gismaplib/platforms/android/GlOfflineRenderer.h"
#include "gismaplib/platforms/android/mapbox/MapboxRenderer.h"

namespace gmcore
{
  void MapboxMsg::dataWorker(
      boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
      boost::shared_ptr <GmMsg::Message::Builder> msgPtrR)
  {
    // get the query data
    GmMsg::MapboxQ::Reader dataQ = dataPtrQ->getAs <GmMsg::MapboxQ>();

    uintptr_t imagePointer = static_cast<uintptr_t> (dataQ.getImagePointer());

    capnp::AnyPointer::Builder dataPtrR = msgPtrR->initDataPointer();
    GmMsg::MapboxR::Builder dataR = dataPtrR.initAs <GmMsg::MapboxR>();
    if (!imagePointer) {
      dataR.setIsImageStored(false);
      return;
    }

    void* imagePtr = reinterpret_cast<void*> (imagePointer);
    uint16_t imageWidth = (uint16_t) (dataQ.getImageWidth());
    uint16_t imageHeight = (uint16_t) (dataQ.getImageHeight());

    // get and put image data
//    GlOfflineRenderer renderer{imagePtr, imageWidth, imageHeight};
//    renderer.render();
    MapboxRenderer renderer{imagePtr, imageWidth, imageHeight};
    renderer.render();

    // set the reply data
    dataR.setIsImageStored(true);
  }
}  // namespace gmcore
