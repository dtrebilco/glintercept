#include <stdio.h>
#include "Utility.h"


namespace corona {

  class CFile : public DLLImplementation<File> {
  public:
    CFile(FILE* file) {
      m_file = file;
    }

    ~CFile() {
      fclose(m_file);
    }

    int COR_CALL read(void* buffer, int size) {
      return fread(buffer, 1, size, m_file);
    }

    int COR_CALL write(const void* buffer, int size) {
      return fwrite(buffer, 1, size, m_file);
    }

    bool COR_CALL seek(int position, SeekMode mode) {
      int m;
      switch (mode) {
        case BEGIN:   m = SEEK_SET; break;
        case CURRENT: m = SEEK_CUR; break;
        case END:     m = SEEK_END; break;
        default: return false;
      }
      return fseek(m_file, position, m) == 0;
    }

    int COR_CALL tell() {
      return ftell(m_file);
    }

#ifdef GLI_CHANGES

    //Hack to get the file pointer
    void * GetFilePtr() {
      return m_file;
    }

#endif //GLI_CHANGES

  private:
    FILE* m_file;
  };


  COR_EXPORT(File*) CorOpenFile(const char* filename, bool writeable) {
    FILE* file = fopen(filename, (writeable ? "wb" : "rb"));
    return (file ? new CFile(file) : 0);
  }

}
