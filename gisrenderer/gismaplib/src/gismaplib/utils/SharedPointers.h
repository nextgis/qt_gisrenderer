#ifndef GMCORE_SHAREDPOINTERS_H
#define GMCORE_SHAREDPOINTERS_H

#include <memory>
#include <string>


namespace gmcore
{
  // SharedString
  using SharedString = std::shared_ptr <std::string>;


  // http://stackoverflow.com/a/13196986
  template <typename ... Args>
  SharedString makeSharedString(Args&& ... args)
  {
    return (std::make_shared <std::string>(std::forward <Args>(args) ...));
  }
}  // namespace gmcore


#endif  // GMCORE_SHAREDPOINTERS_H
