#ifndef CORONA_UTILITY_H
#define CORONA_UTILITY_H


#include <algorithm>
#include "corona.h"
#include "Types.h"


#define COR_EXPORT(ret)  COR_FUNCTION(ret)


#if defined(_MSC_VER) && _MSC_VER <= 1200

// define our own std::min and std::max in VC6
namespace std {
  template<typename T>
  T min(T a, T b) {
    return a < b ? a : b;
  }
  template<typename T>
  T max(T a, T b) {
    return a > b ? a : b;
  }
}

#endif


namespace corona {


  template<typename T>
  class auto_array {
  public:
    explicit auto_array(T* initial = 0) {
      array = initial;
    }
    
    ~auto_array() {
      delete[] array;
    }

    operator T*() const {
      return array;
    }

    T* get() const {
      return array;
    }

    T* release() {
      T* old = array;
      array = 0;
      return old;
    }

    auto_array<T>& operator=(T* a) {
      if (array != a) {
	delete array;
	array = a;
      }
      return *this;
    }

  private:
    T* array;
  };


  inline u16 read16_le(const byte* b) {
    return b[0] + (b[1] << 8);
  }

  inline void write16_le(byte* b, u16 value) {
    b[0] = value & 0xFF;
    b[1] = value >> 8;
  }

  inline u16 read16_be(const byte* b) {
    return (b[0] << 8) + b[1];
  }

  inline void write16_be(byte* b, u16 value) {
    b[0] = value >> 8;
    b[1] = value & 0xFF;
  }

  inline u32 read32_le(const byte* b) {
    return read16_le(b) + (read16_le(b + 2) << 16);
  }

  inline u32 read32_be(const byte* b) {
    return (read16_be(b) << 16) + read16_be(b + 2);
  }


  struct RGB {
    byte red;
    byte green;
    byte blue;
  };

  struct RGBA {
    byte red;
    byte green;
    byte blue;
    byte alpha;
  };

  struct BGR {
    byte blue;
    byte green;
    byte red;
  };

  struct BGRA {
    byte blue;
    byte green;
    byte red;
    byte alpha;
  };

}


#endif
