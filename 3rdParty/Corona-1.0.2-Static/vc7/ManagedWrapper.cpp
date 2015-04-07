// maintained by matthew gambrell - zeromus@zeromus.org
// bug me if something is screwy or just fix it yerself

#using <mscorlib.dll>

using namespace System;
using namespace System::Text;

[assembly: System::Reflection::AssemblyVersion("1.0.2.0")];
[assembly: System::Reflection::AssemblyKeyFileAttribute("Corona.snk")];



#include "../src/corona.h"
#include "../src/Utility.h"
#include <string.h>


namespace Corona {
  public __value enum FileFormat {
    Auto  = 0x0100,
    PNG   = 0x0101,
    JPEG  = 0x0102,
    PCX   = 0x0103,
    BMP   = 0x0104,
    TGA   = 0x0105,
    GIF   = 0x0106,
  };

  public __value enum PixelFormat {
    DontCare  = 0x0200,
    R8G8B8A8  = 0x0201,
    R8G8B8    = 0x0202,
    I8        = 0x0203,
    B8G8R8A8  = 0x0204,
    B8G8R8    = 0x0205,
  };


  __nogc class _StreamFile_Unmanaged
    : public corona::DLLImplementation<corona::File>
  {
  private:
    void** m_Stream;

  public:
    _StreamFile_Unmanaged(void** stream) {
      m_Stream = stream;
    }

    bool COR_CALL seek(int position, corona::File::SeekMode mode) {
      System::IO::Stream __gc* stream =
        *((System::IO::Stream __gc* __nogc*)m_Stream);

      if (!stream->CanSeek)
        return false;

      switch(mode) {
        case corona::File::SeekMode::BEGIN:
          stream->Seek(position, System::IO::SeekOrigin::Begin);
          break;
        case corona::File::SeekMode::CURRENT:
          stream->Seek(position, System::IO::SeekOrigin::Current);
          break;
        case corona::File::SeekMode::END:
          stream->Seek(position, System::IO::SeekOrigin::End); break;
      }

      return true;
    }

    int COR_CALL write(const void* buffer, int size) {
      System::IO::Stream __gc* stream =
        *((System::IO::Stream __gc* __nogc*)m_Stream);

      if (!stream->CanWrite) {
        return 0;
      }

      System::Byte writebuf __gc[] = __gc new System::Byte[size];
      unsigned char __pin* writebuf_pin = &writebuf[0];
      memcpy(writebuf_pin, buffer, size);
      __int64 original = stream->Position;
      stream->Write(writebuf, 0, size);
      return (int)(stream->Position-original);
    }

    int COR_CALL read(void* buffer, int size) {
      System::IO::Stream __gc* stream =
        *((System::IO::Stream __gc* __nogc*)m_Stream);

      if (!stream->CanRead) {
        return 0;
      }
      System::Byte readbuf __gc[] = __gc new System::Byte[size];
      int read = stream->Read(readbuf,0,size);
      unsigned char __pin* readbuf_pin = &readbuf[0];
      memcpy(buffer,readbuf_pin,read);
      return read;
    }

    int COR_CALL tell() {
      System::IO::Stream __gc* stream =
        *((System::IO::Stream __gc* __nogc*)m_Stream);
      return (int)stream->Position;
    }
  };


  public __gc class Image : public System::IDisposable {
  private:
    bool m_Disposed;
    corona::Image __nogc* m_CorImage;

  private:
    //Constructs an Image from a corimage
    Image(corona::Image __nogc* corimage) {
      Init(corimage);
    }

    //internal initialization from a corimage
    void Init(corona::Image __nogc* corimage) {
      m_Disposed = false;
      m_CorImage = corimage;
    }

  public:
    ~Image() {
      Dispose();
    }

    void Dispose() {
      if (!m_Disposed) {
        m_Disposed = true;
        GC::SuppressFinalize(this);
        delete m_CorImage;
        m_CorImage = 0;
      }
    }

    // copy constructor
    Image(Image __gc* img) {
      if (!img) {
        throw new ArgumentNullException(
          "Null Corona.Image passed to a Corona.Image copy constructor");
      }

      if (img->m_Disposed) {
        throw new ObjectDisposedException(
          "Disposed Corona.Image passed to Corona.Image copy constructor");
      }

      Init(corona::CloneImage(img->m_CorImage, img->m_CorImage->getFormat()));
    }

    // copy constructor forcing to a specified PixelFormat
    Image(Image __gc* img, Corona::PixelFormat pixel_format) {
      if (!img) {
        throw new ArgumentNullException(
          "Null Corona.Image passed to a Corona.Image copy constructor");
      }

      if (img->m_Disposed) {
        throw new ObjectDisposedException(
          "Disposed Corona.Image passed to Corona.Image copy constructor");
      }

      Init(corona::CloneImage(
        img->m_CorImage,
        static_cast<corona::PixelFormat>(pixel_format)));
    }

  public:
    __property int get_Width() { return m_CorImage->getWidth(); }
    __property int get_Height() { return m_CorImage->getHeight(); }
    __property Corona::PixelFormat get_PixelFormat() { return static_cast<Corona::PixelFormat>(m_CorImage->getFormat()); }
    __property int get_PaletteSize() { return m_CorImage->getPaletteSize(); }
    __property Corona::PixelFormat get_PaletteFormat() { return static_cast<Corona::PixelFormat>(m_CorImage->getPaletteFormat()); }
    __property void __nogc* get_Pixels() { return m_CorImage->getPixels(); }
    __property void __nogc* get_Palette() { return m_CorImage->getPalette(); }
    __property int get_PixelSize() { return GetFormatSize(PixelFormat); }
    __property int get_PaletteElementSize() { return GetFormatSize(PaletteFormat); }

  public:
    static int GetFormatSize(Corona::PixelFormat pixel_format) {
      return corona::GetPixelSize(
        static_cast<corona::PixelFormat>(pixel_format));
    }

    static Image __gc* Create(int width, int height, Corona::PixelFormat pixel_format) {
      corona::Image __nogc* newimage = corona::CreateImage(
        width,
        height,
        static_cast<corona::PixelFormat>(pixel_format));
      return (newimage ? __gc new Image(newimage) : 0);
    }

    // Open a file from a System.IO.Stream.  Beware that some loaders
    // may seek to the end of the stream so your stream needs to be no
    // larger than the actual desired input file.  I sure hope your
    // stream is readable.
    static Image __gc* Open(
      System::IO::Stream __gc* input,
      Corona::PixelFormat pixel_format,
      FileFormat file_format)
    {
      if (!input->CanRead) {
        return 0;
      }

      System::IO::Stream __pin* input_pinned = input;
      _StreamFile_Unmanaged __nogc* streamfile =
        new _StreamFile_Unmanaged((void **)&input_pinned);

      corona::Image __nogc* newimage = corona::OpenImage(
        streamfile,
        static_cast<corona::PixelFormat>(pixel_format),
        static_cast<corona::FileFormat>(file_format));

      delete streamfile;
      return (newimage ? __gc new Image(newimage) : 0);
    }


    static Image __gc* Open(
      String __gc* filename,
      Corona::PixelFormat pixel_format,
      FileFormat file_format)
    {
      char strbuf __gc[] = Encoding::ASCII->GetBytes(filename);
      char __pin* pstr = &strbuf[0];
      char* str = static_cast<char*>(pstr);

      corona::Image __nogc* newimage = corona::OpenImage(
        str,
        static_cast<corona::PixelFormat>(pixel_format),
        static_cast<corona::FileFormat>(file_format));
      return (newimage ? __gc new Image(newimage) : 0);
    }

  public:
    bool Save(String __gc* filename, Corona::FileFormat file_format) {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "Save() executed on disposed Corona.Image");
      }

      char strbuf __gc[] = Encoding::ASCII->GetBytes(filename);
      char __pin* pstr = &strbuf[0];
      char* str = static_cast<char*>(pstr);
      
      return corona::SaveImage(
        str,
        static_cast<corona::FileFormat>(file_format),
        m_CorImage);
    }

    // Save an image to a System.IO.Stream.  I sure hope your stream
    // is writable.
    bool Save(System::IO::Stream __gc* output, FileFormat file_format) {
      if (!output->CanWrite) {
        return false;
      }

      if (m_Disposed) {
        throw new ObjectDisposedException(
          "Save() executed on disposed Corona.Image");
      }

      System::IO::Stream __pin* output_pinned = output;
      _StreamFile_Unmanaged __nogc* streamfile = new _StreamFile_Unmanaged((void **)&output_pinned);

      bool result = corona::SaveImage(
        streamfile,
        static_cast<corona::FileFormat>(file_format),
        m_CorImage);

      delete streamfile;
      return result;
    }

    Image __gc* Convert(Corona::PixelFormat pixel_format) {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "Convert() executed on disposed Corona.Image");
      }

      corona::Image __nogc* newimage = corona::ConvertImage(
        m_CorImage,
        static_cast<corona::PixelFormat>(pixel_format));
      return (newimage ? __gc new Image(newimage) : 0);
    }

    Image __gc* Clone(Corona::PixelFormat pixel_format) {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "Clone() executed on disposed Corona.Image");
      }

      return new Image(this, pixel_format);
    }

    Image __gc* Clone() {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "Clone() executed on disposed Corona.Image");
      }
      return new Image(this);
    }

    Byte GrabPixels2d() __gc[,] {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "GrabPixels2d() executed on disposed Corona.Image");
      }

      Byte arr __gc[,] = __gc new Byte[Width*PixelSize, Height];
      Byte __pin* arrptr = &arr[0,0];
      memcpy(arrptr, Pixels, Width * Height * PixelSize);
      return arr;
    }

    Byte GrabPixels() __gc[] {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "GrabPixels executed on disposed Corona.Image");
      }

      Byte arr __gc[] = __gc new Byte[Width * Height * PixelSize];
      Byte __pin* arrptr = &arr[0];
      memcpy(arrptr, Pixels, Width * Height * PixelSize);
      return arr;
    }

    Byte GrabPalette() __gc[] {
      if (m_Disposed) {
        throw new ObjectDisposedException(
          "GrabPalette() executed on disposed Corona.Image");
      }

      Byte arr __gc[] = __gc new Byte[PaletteSize * PaletteElementSize];
      Byte __pin* arrptr = &arr[0];
      memcpy(arrptr, Palette, PaletteSize * PaletteElementSize);
      return arr;
    }
  };
}
