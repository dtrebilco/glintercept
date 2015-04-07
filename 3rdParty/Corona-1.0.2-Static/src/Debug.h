#ifndef DEBUG_HPP
#define DEBUG_HPP


#ifdef _DEBUG
# ifndef CORONA_DEBUG
//#  define CORONA_DEBUG
# endif
#endif


#ifdef CORONA_DEBUG

  #include <stdio.h>
  #include <string>

  class Log {
  public:
    static void Write(const char* str);
    static void IncrementIndent() { ++indent_count; }
    static void DecrementIndent() { --indent_count; }

  private:
    static void EnsureOpen();
    static void Close();

  private:
    static FILE* handle;
    static int indent_count;
  };


  class Guard {
  public:
    Guard(const char* label)
    : m_label(label) {
      Write("+");
      Log::IncrementIndent();
    }

    ~Guard() {
      Log::DecrementIndent();
      Write("-");
    }

    void Write(const char* prefix) {
      Log::Write((prefix + m_label).c_str());
    }

  private:
    std::string m_label;
  };


  #define COR_GUARD(label) Guard guard_obj__(label)
  #define COR_LOG(label)   (Log::Write(label))
  #define COR_IF_DEBUG     if (true)
  #define COR_ASSERT(condition, label) if (!(condition)) { __asm int 3 }

#else

  #define COR_GUARD(label) 
  #define COR_LOG(label)
  #define COR_IF_DEBUG     if (false)
  #define COR_ASSERT(condition, label)

#endif


#endif
