#include "gismaplib/utils/FileUtils.h"

#include <fstream>
#include <cerrno>


namespace gmcore
{
  const char* FileUtils::FILE_SEPARATOR = "/";


  SharedString FileUtils::getFileContents(SharedString pFilePath)
  {
    std::ifstream in(*pFilePath, std::ios::in | std::ios::binary);
    if (in) {
      SharedString contents = makeSharedString();
      in.seekg(0, std::ios::end);
      contents->resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&(*contents)[0], contents->size());
      in.close();
      return (contents);
    }
    throw (errno);
  }
}
