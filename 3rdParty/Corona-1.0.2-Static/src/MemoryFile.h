#ifndef MEMORY_FILE_H
#define MEMORY_FILE_H


#include "corona.h"
#include "Types.h"
#include "Utility.h"


namespace corona {

  class MemoryFile : public DLLImplementation<File> {
  public:
    MemoryFile(const void* buffer, int size);
    ~MemoryFile();

    int  COR_CALL read(void* buffer, int size);
    int  COR_CALL write(const void* buffer, int size);
    bool COR_CALL seek(int position, SeekMode mode);
    int  COR_CALL tell();

  private:
    void ensureSize(int min_size);

    byte* m_buffer;
    int m_position;
    int m_size;

    /// The actual size of m_buffer.  Always a power of two.
    int m_capacity;
  };

}


#endif
