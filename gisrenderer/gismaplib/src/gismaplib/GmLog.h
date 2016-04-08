#ifndef GMCORE_GMLOG_H
#define GMCORE_GMLOG_H

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


namespace gmcore
{
namespace logsrc = boost::log::sources;

  enum severity_level
  {
    trace, debug, info, warning, error, fatal
  };


  class GmLog
    : public logsrc::severity_logger_mt <severity_level>
  {
  public:
    static void initGmLog();

    static GmLog log;
  };
}  // namespace gmcore


#endif  // GMCORE_GMLOG_H
