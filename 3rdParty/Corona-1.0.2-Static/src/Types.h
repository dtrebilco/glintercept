#ifndef CORONA_TYPES_H
#define CORONA_TYPES_H


namespace corona {

  // VC++-specific types
  #ifdef _MSC_VER

    typedef unsigned char    byte;
    typedef unsigned __int16 u16;
    typedef unsigned __int32 u32;

  // reasonable defaults
  // should work on any 32-bit platform
  #else

    typedef unsigned char  byte;
    typedef unsigned short u16;
    typedef unsigned long  u32;

  #endif

}


#endif
