#include "base/log.h"
#include "base/mutexer.h"
#include<cstring>
#include<string>

namespace agora {
namespace base {
uint32_t log_config::facility = USER_LOG_FCLT;
int log_config::enabled_level = INFO_LOG;
uint64_t log_config::dropped_count = 0;
uint32_t log_config::drop_cancel = 10;
Mutexer log_config::logger_mutex;

void _log_(const char* logdir, uint32_t fclt, log_levels level, const char* format, va_list ap)
{
    if (!log_config::log_enabled(level)) {
        return;
    }
    
    #if 0
    if(logdir != NULL && strlen(logdir))
    {
        log_config::lock();
        std::string logpath=std::string(logdir)+"recording_sys.log";

        int file_fd = ::open(logpath.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
        int orig_fd=dup(STDERR_FILENO); //backup error fd
        dup2(file_fd, STDERR_FILENO); //redirect to file
        openlog(NULL, LOG_PID|LOG_NDELAY|LOG_PERROR, fclt);

        ::vsyslog(level, format, ap);

        dup2(orig_fd, STDERR_FILENO);//restore error fd
        close(orig_fd);//close backup fd
        close(file_fd);//close file fd

        log_config::unlock();
    }
    else
    #endif
    {
        openlog(NULL, LOG_PID|LOG_NDELAY, fclt);
        ::vsyslog(level, format, ap);
        return ;
    }
}

void log_dir(const char* logdir, log_levels level, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _log_(logdir, log_config::getFacility(), level, format, args);
    va_end(args);
}

void log(log_levels level, const char* format, ...) {
  if (!log_config::log_enabled(level)) {
    return;
  }
  open_log();
  va_list args;
  va_start(args, format);
  ::vsyslog(level, format, args);
  va_end(args);
}

void open_log() {
    openlog(NULL, LOG_PID|LOG_NDELAY, log_config::getFacility());
}






}
}
