#ifndef GMCORE_GMCORE_H
#define GMCORE_GMCORE_H

#include <capnp/common.h>
#include <kj/array.h>

#include "gismaplib/GmLog.h"


namespace gmcore
{
  class GmCore
  {
  public:
    explicit GmCore();

    static void gmcoreInit();

    static kj::Array <capnp::word> messageWorker(
        void*         segmentsPtrsQ,
        long long int segmentsSizesQ);
  };
}  // namespace gmcore


#endif  // GMCORE_GMCORE_H
