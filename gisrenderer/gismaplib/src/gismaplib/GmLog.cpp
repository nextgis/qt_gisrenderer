#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/block_on_overflow.hpp>
#include <boost/log/sinks/bounded_fifo_queue.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "gismaplib/GmLog.h"


namespace gmcore
{
namespace boostlog = boost::log;
namespace logsrc = boost::log::sources;
namespace logexpr = boost::log::expressions;
namespace logsinks = boost::log::sinks;
namespace logkeys = boost::log::keywords;
namespace logattrs = boost::log::attributes;

  BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level) BOOST_LOG_ATTRIBUTE_KEYWORD(
      timestamp,
      "TimeStamp",
      boost::posix_time::ptime) GmLog GmLog::log;


// The operator puts a human-friendly representation of the severity level to the stream
  std::ostream& operator<< (
      std::ostream&  strm,
      severity_level level)
  {
    static const char* strings[] = {
      "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
    };

    if (static_cast <std::size_t>(level) < sizeof(strings) / sizeof(*strings)) {
      strm << strings[level];
    }
    else {
      strm << static_cast <int>(level);
    }

    return (strm);
  }  // <<


  void GmLog::initGmLog()
  {
    boostlog::add_common_attributes();

    // Construct the sink
    // async, unbound     : THR 43, VSS 657.164K, RSS 121.344K
    // async, bound, 50000: THR 43, VSS 603.872K, RSS 67.788K
    // async, bound, 48000: THR 32, VSS 597.084K, RSS 67.416K
    // async, bound, 10000: THR 32, VSS 575.040K, RSS 45.264K
    // async, bound,  2000: THR 32, VSS 570.300K, RSS 40.384K
    // async, bound,  1000: THR 32, VSS 569.424K, RSS 39.928K
    // async, bound,   200: THR 32, VSS 569.208K, RSS 39.504K
    //  sync              : THR 31, VSS 568.064K, RSS 39.400K
    typedef logsinks::asynchronous_sink <logsinks::text_ostream_backend,
        logsinks::bounded_fifo_queue <1000, logsinks::block_on_overflow> > text_sink;

    boost::shared_ptr <text_sink> sink = boost::make_shared <text_sink>();

    // We have to provide an empty deleter to avoid destroying the global stream object
    boost::shared_ptr <std::ostream> stream(&std::cout, boost::null_deleter());
    sink->locked_backend()->add_stream(stream);

    sink->set_formatter(logexpr::format(
        "GM: %1% [%2%] %3%") % timestamp % severity % logexpr::smessage);

    // Register the sink in the logging core
    boostlog::core::get()->add_sink(sink);
  }  // GmLog::initGmLog
}  // namespace gmcore
