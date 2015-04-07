#include "Debug.h"

#ifdef CORONA_DEBUG


FILE* Log::handle;
int Log::indent_count;


////////////////////////////////////////////////////////////////////////////////

void
Log::Write(const char* str)
{
  EnsureOpen();
  if (handle) {
    std::string s(std::string(indent_count * 2, ' ') + str + "\n");
    fputs(s.c_str(), handle);
    fflush(handle);
  }
}

////////////////////////////////////////////////////////////////////////////////

void
Log::EnsureOpen()
{
  if (!handle) {
#ifdef WIN32
    handle = fopen("C:/corona_debug.log", "w");
#else
    std::string home(getenv("HOME"));
    handle = fopen((home + "/corona_debug.log").c_str(), "w");
#endif
    atexit(Close);
  }
}

////////////////////////////////////////////////////////////////////////////////

void
Log::Close()
{
  if(handle)
  {
    fclose(handle);
  }
}

////////////////////////////////////////////////////////////////////////////////


#endif
