#ifndef GMCORE_FILEUTILS_H
#define GMCORE_FILEUTILS_H


#include "gismaplib/utils/SharedPointers.h"

namespace gmcore
{
  class FileUtils
  {
  public:
    static const char* FILE_SEPARATOR;

    static SharedString getFileContents(const SharedString pFilePath);
  };
}

#endif  // GMCORE_FILEUTILS_H
