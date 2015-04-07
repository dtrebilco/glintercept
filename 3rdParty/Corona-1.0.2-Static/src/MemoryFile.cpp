#include <string.h>
#include "MemoryFile.h"
#include "Utility.h"

namespace corona {

  COR_EXPORT(File*) CorCreateMemoryFile(const void* buffer, int size) {
    if (size && !buffer) {
      return 0;
    }
    if (size < 0) {
      return 0;
    }

    return new MemoryFile(buffer, size);
  }


  int getNextPowerOfTwo(int value) {
    int i = 1;
    while (i < value) {
      i *= 2;
    }
    return i;
  }

  MemoryFile::MemoryFile(const void* buffer, int size) {
    m_capacity = getNextPowerOfTwo(size);
    m_size = size;
    m_buffer = new byte[m_capacity];
    memcpy(m_buffer, buffer, size);

    m_position = 0;
  }

  MemoryFile::~MemoryFile() {
    delete[] m_buffer;
  }

  int COR_CALL MemoryFile::read(void* buffer, int size) {
    int real_read = std::min((m_size - m_position), size);
    memcpy(buffer, m_buffer + m_position, real_read);
    m_position += real_read;
    return real_read;
  }

  int COR_CALL MemoryFile::write(const void* buffer, int size) {
    ensureSize(m_position + size);
    memcpy(m_buffer + m_position, buffer, size);
    m_position += size;
    return size;
  }

  bool COR_CALL MemoryFile::seek(int position, SeekMode mode) {
    int real_pos;
    switch (mode) {
      case BEGIN:   real_pos = position;              break;
      case CURRENT: real_pos = m_position + position; break;
      case END:     real_pos = m_size + position;     break;
      default:      return false;
    }

    if (real_pos < 0 || real_pos > m_size) {
      m_position = 0;
      return false;
    } else {
      m_position = real_pos;
      return true;
    }
  }

  int COR_CALL MemoryFile::tell() {
    return m_position;
  }

  void MemoryFile::ensureSize(int min_size) {
    bool realloc_needed = false;
    while (m_capacity < min_size) {
      m_capacity *= 2;
      realloc_needed = true;
    }

    if (realloc_needed) {
      byte* new_buffer = new byte[m_capacity];
      memcpy(new_buffer, m_buffer, m_size);
      delete[] m_buffer;
      m_buffer = new_buffer;
    }

    m_size = min_size;
  }

};
