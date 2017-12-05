#include "base/mutexer.h"

namespace agora {
namespace base {

Mutexer::Mutexer()
{
#ifdef _WIN32
  InitializeCriticalSection(&crit);
#else
  pthread_mutexattr_t attr;
  (void) pthread_mutexattr_init(&attr);
  (void) pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  (void) pthread_mutex_init(&mutex_, &attr);
  (void) pthread_mutexattr_destroy(&attr);
#endif
}

Mutexer::~Mutexer() {
#ifdef _WIN32
  DeleteCriticalSection(&crit);
#else
  (void) pthread_mutex_destroy(&mutex_);
#endif
}

void Mutexer::lock()
{
#ifdef _WIN32
  EnterCriticalSection(&crit);
#else
  (void) pthread_mutex_lock(&mutex_);
#endif
}

void Mutexer::unlock()
{
#ifdef _WIN32
  LeaveCriticalSection(&crit);
#else
  (void) pthread_mutex_unlock(&mutex_);
#endif
}

bool Mutexer::trylock() {
#if defined(WEBRTC_WIN)
  return TryEnterCriticalSection(&crit) != FALSE;
#else
  if (pthread_mutex_trylock(&mutex_) != 0)
    return false;

  return true;
#endif
}

}
}
