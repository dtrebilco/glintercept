/**
 * Corona Image I/O Library
 * Version 1.0.1
 * (c) 2002 Chad Austin
 *
 * This API uses principles explained at
 * http://aegisknight.org/cppinterface.html
 *
 * This code licensed under the terms of the zlib license.  See
 * license.txt.
 */


#ifndef CORONA_H
#define CORONA_H


#ifndef __cplusplus
#error Corona requires C++
#endif


#include <string>
#include <vector>
  

// DLLs in Windows should use the standard calling convention
#ifndef COR_CALL
#  if defined(WIN32) || defined(_WIN32)
#    define COR_CALL __stdcall
#  else
#    define COR_CALL
#  endif
#endif

// Export functions from the DLL
#ifndef COR_DECL
#  if defined(WIN32) || defined(_WIN32)
#    ifdef CORONA_EXPORTS
#      define COR_DECL __declspec(dllexport)
#    else
#      define COR_DECL
#    endif
#  else
#    define COR_DECL
#  endif
#endif


// evil "identifier is too long in debug information" warning
#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif


#define COR_FUNCTION(ret) extern "C" COR_DECL ret COR_CALL


namespace corona {

  /**
   * File formats supported for reading or writing.
   */
  enum FileFormat {
    FF_AUTODETECT = 0x0100,
    FF_PNG        = 0x0101,
    FF_JPEG       = 0x0102,
    FF_PCX        = 0x0103,
    FF_BMP        = 0x0104,
    FF_TGA        = 0x0105,
    FF_GIF        = 0x0106,
  };

  /**
   * Pixel format specifications.  Pixel data can be packed in one of
   * the following ways.
   */
  enum PixelFormat {
    PF_DONTCARE = 0x0200,  /**< special format used when specifying a
                                desired pixel format */
    PF_R8G8B8A8 = 0x0201,  /**< RGBA, channels have eight bits of precision */
    PF_R8G8B8   = 0x0202,  /**< RGB, channels have eight bits of precision  */
    PF_I8       = 0x0203,  /**< Palettized, 8-bit indices into palette      */
    PF_B8G8R8A8 = 0x0204,  /**< BGRA, channels have eight bits of precision */
    PF_B8G8R8   = 0x0205,  /**< BGR, channels have eight bits of precision  */
  };

  /**
   * Axis specifications.  The image can be flipped along the following
   * axes.
   */
  enum CoordinateAxis {
    CA_X     = 0x0001,
    CA_Y     = 0x0002,
  };

  /**
   * A helper class for DLL-compatible interfaces.  Derive your cross-DLL
   * interfaces from this class.
   *
   * When deriving from this class, do not declare a virtual destructor
   * on your interface.
   */
  class DLLInterface {
  private:
    /**
     * Destroy the object, freeing all associated memory.  This is
     * the same as a destructor.
     */
    virtual void COR_CALL destroy() = 0;

  public:
    /**
     * "delete image" should actually call image->destroy(), thus putting the
     * burden of calling the destructor and freeing the memory on the image
     * object, and thus on Corona's side of the DLL boundary.
     */
    void operator delete(void* p) {
      if (p) {
        DLLInterface* i = static_cast<DLLInterface*>(p);
        i->destroy();
      }
    }
  };


  /**
   * A helper class for DLL-compatible interface implementations.  Derive
   * your implementations from DLLImplementation<YourInterface>.
   */
  template<class Interface>
  class DLLImplementation : public Interface {
  public:
    /**
     * So the implementation can put its destruction logic in the destructor,
     * as natural C++ code does.
     */
    virtual ~DLLImplementation() { }

    /**
     * Call the destructor in a Win32 ABI-compatible way.
     */
    virtual void COR_CALL destroy() {
      delete this;
    }

    /**
     * So destroy()'s "delete this" doesn't go into an infinite loop,
     * calling the interface's operator delete, which calls destroy()...
     */
    void operator delete(void* p) {
      ::operator delete(p);
    }
  };

  
  /**
   * An image object represents a rectangular collections of pixels.
   * They have a width, a height, and a pixel format.  Images cannot
   * be resized.
   */
  class Image : public DLLInterface {
  public:
    /**
     * Get image width.
     * @return  image width
     */
    virtual int COR_CALL getWidth() = 0;

    /**
     * Get image height.
     * @return  image height
     */
    virtual int COR_CALL getHeight() = 0;

    /**
     * Get pixel format.
     * @return  pixel format
     */
    virtual PixelFormat COR_CALL getFormat() = 0;

    /**
     * Get pixel buffer.  The pixels are packed in the format defined
     * by the image's pixel format.
     *
     * @return  pointer to first element in pixel buffer
     */
    virtual void* COR_CALL getPixels() = 0;

    /**
     * Get the palette.  Pixels are packed in the format defined by
     * getPaletteFormat().
     *
     * @return  pointer to first palette entry
     */
    virtual void* COR_CALL getPalette() = 0;

    /**
     * Get the number of entries in the palette.
     *
     * @return  number of palette entries
     */
    virtual int COR_CALL getPaletteSize() = 0;

    /**
     * Get the format of the colors in the palette.
     *
     * @return  pixel format of palette entries
     */
    virtual PixelFormat COR_CALL getPaletteFormat() = 0;
  };


  /**
   * Represents a random-access file, usually stored on a disk.  Files
   * are always binary: that is, they do no end-of-line
   * transformations.  File objects are roughly analogous to ANSI C
   * FILE* objects.
   */
  class File : public DLLInterface {
  public:

    /**
     * The different ways you can seek within a file.
     */
    enum SeekMode {
      BEGIN,    /**< relative to the beginning of the file */
      CURRENT,  /**< relative to the current position in the file */
      END       /**< relative to the end of the file: position should
                     be negative*/
    };

    /**
     * Read size bytes from the file, storing them in buffer.
     *
     * @param buffer  buffer to read into
     * @param size    number of bytes to read
     *
     * @return  number of bytes successfully read
     */
    virtual int COR_CALL read(void* buffer, int size) = 0;

    /**
     * Write size bytes from buffer to the file.
     *
     * @param buffer  buffer that contains the data to write
     * @param size    number of bytes to write
     *
     * @return  number of bytes successfully written
     */
    virtual int COR_CALL write(const void* buffer, int size) = 0;

    /**
     * Jump to a new position in the file, using the specified seek
     * mode.  Remember: if mode is END, the position must be negative,
     * to seek backwards from the end of the file into its contents.
     * If the seek fails, the current position is undefined.
     *
     * @param position  position relative to the mode
     * @param mode      where to seek from in the file
     *
     * @return  true on success, false otherwise
     */
    virtual bool COR_CALL seek(int position, SeekMode mode) = 0;

    /**
     * Get current position within the file.
     *
     * @return  current position
     */
    virtual int COR_CALL tell() = 0;
  };


  /// PRIVATE API - for internal use only
  namespace hidden {

    // these are extern "C" so we don't mangle the names


    // API information

    COR_FUNCTION(const char*) CorGetVersion();

    /// @{
    /**
     * Returns a formatted string that lists the file formats that
     * Corona supports.  This function is DLL-safe.
     *
     * It is formatted in the following way:
     *
     * description1:ext1,ext2,ext3;description2:ext1,ext2,...
     */
    COR_FUNCTION(const char*) CorGetSupportedReadFormats();
    COR_FUNCTION(const char*) CorGetSupportedWriteFormats();
    /// @}

    // creation

    COR_FUNCTION(Image*) CorCreateImage(
      int width,
      int height,
      PixelFormat format);

    COR_FUNCTION(Image*) CorCreatePalettizedImage(
      int width,
      int height,
      PixelFormat format, // must be a palettized format
      int palette_size,
      PixelFormat palette_format);

    COR_FUNCTION(Image*) CorCloneImage(
      Image* source,
      PixelFormat format);

    // loading

    COR_FUNCTION(Image*) CorOpenImage(
      const char* filename,
      FileFormat file_format);

    COR_FUNCTION(Image*) CorOpenImageFromFile(
      File* file,
      FileFormat file_format);

    // saving

    COR_FUNCTION(bool) CorSaveImage(
      const char* filename,
      FileFormat file_format,
      Image* image);

    COR_FUNCTION(bool) CorSaveImageToFile(
      File* file,
      FileFormat file_format,
      Image* image);

    // conversion

    COR_FUNCTION(Image*) CorConvertImage(
      Image* image,
      PixelFormat format);

    COR_FUNCTION(Image*) CorConvertPalette(
      Image* image,
      PixelFormat palette_format);

    COR_FUNCTION(Image*) CorFlipImage(
      Image* image,
      int coordinate_axis);

    // files

    COR_FUNCTION(File*) CorOpenFile(const char* name, bool writeable);
    COR_FUNCTION(File*) CorCreateMemoryFile(const void* buffer, int size);

    // utility

    COR_FUNCTION(int) CorGetPixelSize(PixelFormat format);
  }


  /* PUBLIC API */


  /**
   * Return the Corona version string.
   *
   * @return  Corona version information
   */
  inline const char* GetVersion() {
    return hidden::CorGetVersion();
  }


  /// Describes a file format that Corona supports.
  struct FileFormatDesc {
    /// Short description of format, such as "PNG Files" or "JPEG Files"
    std::string description;

    /// List of supported extensions, such as {"bmp", "rle", "dib"}
    std::vector<std::string> extensions;
  };

  inline void SplitString(
    std::vector<std::string>& out,
    const char* in,
    char delim)
  {
    out.clear();
    while (*in) {
      const char* next = strchr(in, delim);
      if (next) {
        out.push_back(std::string(in, next));
      } else {
        out.push_back(in);
      }

      in = (next ? next + 1 : "");
    }
  }

  /// Populates a vector of FileFormatDesc structs.
  inline void GetSupportedReadFormats(std::vector<FileFormatDesc>& formats) {
    std::vector<std::string> descriptions;
    SplitString(descriptions, hidden::CorGetSupportedReadFormats(), ';');

    formats.resize(descriptions.size());
    for (unsigned i = 0; i < descriptions.size(); ++i) {
      const char* d = descriptions[i].c_str();
      const char* colon = strchr(d, ':');
      formats[i].description.assign(d, colon);
      SplitString(formats[i].extensions, colon + 1, ',');
    }
  }

  /// Populates a vector of FileFormatDesc structs.
  inline void GetSupportedWriteFormats(std::vector<FileFormatDesc>& formats) {
    std::vector<std::string> descriptions;
    SplitString(descriptions, hidden::CorGetSupportedWriteFormats(), ';');

    formats.resize(descriptions.size());
    for (unsigned i = 0; i < descriptions.size(); ++i) {
      const char* d = descriptions[i].c_str();
      const char* colon = strchr(d, ':');
      formats[i].description.assign(d, colon);
      SplitString(formats[i].extensions, colon + 1, ',');
    }
  }


  /**
   * Create a new, blank image with a specified width, height, and
   * format.
   *
   * @param width   width of the new image
   * @param height  height of the new image
   * @param format  format the pixels are stored in, cannot be PF_DONTCARE
   *
   * @return  newly created blank image
   */
  inline Image* CreateImage(
    int width,
    int height,
    PixelFormat format)
  {
    return hidden::CorCreateImage(width, height, format);
  }

  /**
   * Create a new, blank image with a specified width, height, format,
   * and palette.
   *
   * @param width           width of image
   * @param height          height of image
   * @param format          format of palette indices, should be PF_I8
   * @param palette_size    number of colors in palette
   * @param palette_format  pixel format of palette entries
   */
  inline Image* CreateImage(
    int width,
    int height,
    PixelFormat format,
    int palette_size,
    PixelFormat palette_format)
  {
    return hidden::CorCreatePalettizedImage(
      width, height, format,
      palette_size, palette_format);
  }

  /**
   * Create a new image from an old one.  If format is specified, the
   * new image is converted to that pixel format.  If format is not
   * specified, the new image simply uses the same format as the
   * source.  If the image could not be cloned or the pixel format is
   * invalid, CloneImage returns 0.
   *
   * @param source  image to clone
   * @param format  format the new image is stored in, defaults to PF_DONTCARE
   *
   * @return  new image cloned from the source, 0 if failure
   */
  inline Image* CloneImage(
    Image* source,
    PixelFormat format = PF_DONTCARE)
  {
    return hidden::CorCloneImage(source, format);
  }

  /**
   * Opens an image from the default filesystem.  This function simply
   * forwards the call to OpenImage(file, file_format, pixel_format)
   * with a standard C library file.
   *
   * See OpenImage(fs, filename, file_format, pixel_format) for more
   * information.
   *
   * @param filename      image filename to open
   * @param file_format   file format the image is stored in, or FF_AUTODETECT
   *                      to try all loaders
   * @param pixel_format  desired pixel format, or PF_DONTCARE to use image's
   *                      native format
   *
   * @return  the image loaded from the disk, or 0 if it cannot be opened
   */
  inline Image* OpenImage(
    const char* filename,
    PixelFormat pixel_format = PF_DONTCARE,
    FileFormat file_format = FF_AUTODETECT)
  {
    return hidden::CorConvertImage(
      hidden::CorOpenImage(filename, file_format),
      pixel_format);
  }

  /**
   * Opens an image from the specified file.
   *
   * If file_format is FF_AUTODETECT, the loader tries
   * to load each format until it finds one that succeeds.  Otherwise,
   * it tries the specific loader specified.
   *
   * If pixel_format is PF_DONTCARE, the new image object has the
   * pixel format closest to the image's format on disk.  Otherwise,
   * the pixels are converted to the specified format before the image
   * is returned.
   *
   * @param file          name of the file that contains the image
   * @param file_format   file format the image is stored in, or FF_AUTODETECT
   *                      to try all loaders
   * @param pixel_format  desired pixel format, or PF_DONTCARE to use image's
   *                      native format
   *
   * @return  the image loaded from the file, or 0 if it cannot be opened
   */
  inline Image* OpenImage(
    File* file,
    PixelFormat pixel_format = PF_DONTCARE,
    FileFormat file_format = FF_AUTODETECT)
  {
    return hidden::CorConvertImage(
      hidden::CorOpenImageFromFile(file, file_format),
      pixel_format);
  }

  /// For compatibility.  This function may be deprecated.
  inline Image* OpenImage(
    const char* filename,
    FileFormat file_format,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return OpenImage(filename, pixel_format, file_format);
  }

  /// For compatibility.  This function may be deprecated.
  inline Image* OpenImage(
    File* file,
    FileFormat file_format,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return OpenImage(file, pixel_format, file_format);
  }

  /**
   * Saves an image to a file in the default filesystem.  This
   * function simply calls SaveImage(file, file_format, image)
   * with a standard C library file.
   *
   * See SaveImage(fs, filename, file_format, image) for more information.
   *
   * @param filename     name of the file to save the image to
   * @param file_format  file format in which to save image.  if FF_AUTODETECT,
   *                     SaveImage guesses the type from the file extension
   * @param image        image to save
   *
   * @return  true if save succeeds, false otherwise
   */
  inline bool SaveImage(
    const char* filename,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImage(filename, file_format, image);
  }

  /**
   * Saves an image to the specified file.  This function saves image
   * to a file of type file_format.  If file_format is not a supported
   * output type, the function fails.  As of now, Corona only supports
   * saving images of type FF_PNG and FF_TGA.
   *
   * @note This function may create the file even if the save does not
   *       succeed, so users of this library should remove the file after
   *       the call to SaveImage().
   *
   * @param file         file in which to save the image
   * @param file_format  file format in which to save image -- must not be
   *                     FF_AUTODETECT
   * @param image        image to save
   *
   * @return  true if the save succeeds, false otherwise
   */
  inline bool SaveImage(
    File* file,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImageToFile(file, file_format, image);
  }

  /**
   * Converts an image from one format to another, destroying the old
   * image.  If source is 0, the function returns 0.  If format is
   * PF_DONTCARE or the source and target formats match, returns the
   * unmodified source image.  If a valid conversion is not found,
   * ConvertImage destroys the old image and returns 0.  For example,
   * ConvertImage does not support creating a palettized image from a
   * direct color image yet.
   *
   * @param source  image to convert
   * @param format  desired format -- can be PF_DONTCARE
   *
   * @return  valid image object if conversion succeeds, 0 otherwise
   */
  inline Image* ConvertImage(Image* source, PixelFormat format) {
    return hidden::CorConvertImage(source, format);
  }

  /**
   * Converts the palette of a palettized image from one format to
   * another, destroying the old image.  If the source is 0, the
   * palette_format is PF_DONTCARE, or the source and target formats
   * match, the function returns the unmodified source image.  If a
   * valid conversion is not found or invalid inputs are given (such
   * as a direct-color source image), this function destroys the old
   * image and returns 0.
   *
   * @param source          palettized image to convert
   * @param palette_format  desired pixel format of palette
   *
   * @return  valid image object if conversion succeeds, 0 otherwise
   */
  inline Image* ConvertPalette(Image* source, PixelFormat palette_format) {
    return hidden::CorConvertPalette(source, palette_format);
  }

  /**
   * Flips the pixels in the image around the given axis.
   *
   * @param source           image to flip
   * @param coordinate_axis  Axis around which to flip.  Both CA_X and CA_Y
   *                         can be specified by ORing them together.
   *
   * @return  the image passed in
   */
  inline Image* FlipImage(Image* source, int coordinate_axis) {
    return hidden::CorFlipImage(source, coordinate_axis);
  }

  /**
   * Returns a default File implementation.
   *
   * @param  filename   name of the file on local filesystem
   * @param  writeable  whether the file can be written to
   */
  inline File* OpenFile(const char* filename, bool writeable) {
    return hidden::CorOpenFile(filename, writeable);
  }

  /**
   * Creates a File implementation that reads from a buffer in memory.
   * It stores a copy of the buffer that is passed in.
   *
   * The File object does <i>not</i> take ownership of the memory buffer.
   * When the file is destroyed, it will not free the memory.
   *
   * @param buffer  Pointer to the beginning of the data.
   * @param size    Size of the buffer in bytes.
   *
   * @return  0 if size is non-zero and buffer is null. Otherwise,
   *          returns a valid File object.
   */
  inline File* CreateMemoryFile(const void* buffer, int size) {
    return hidden::CorCreateMemoryFile(buffer, size);
  }

  /**
   * Returns the number of bytes needed to store a pixel of a gixen format.
   *
   * @param format  The format to query.
   *
   * @return  Number of bytes each pixel takes, or 0 if the format is invalid.
   */
  inline int GetPixelSize(PixelFormat format) {
    return hidden::CorGetPixelSize(format);
  }

  /**
   * Returns true if the pixel format does not require a palette; that
   * is, if each pixel itself contains color data.
   *
   * @param format  The format to query.
   *
   * @return  True if format is direct color, false otherwise.
   */
  inline bool IsDirect(PixelFormat format) {
    return (format == PF_R8G8B8A8 || format == PF_R8G8B8 ||
            format == PF_B8G8R8A8 || format == PF_B8G8R8);
  }

  /**
   * Returns true if the pixel format requires a palette; that
   * is, if each pixel is an index into a separate palette.
   *
   * @param format  The format to query.
   *
   * @return  True if format is palettized, false otherwise.
   */
  inline bool IsPalettized(PixelFormat format) {
    return format == PF_I8;
  }

  /**
   * Returns the number of color entries in a palette for an image
   * of the given format.
   *
   * @param format  The format to query.
   *
   * @return  Number of color entries, or 0 if the format is not palettized.
   */
  inline int GetPaletteSize(PixelFormat format) {
    return (format == PF_I8 ? 256 : 0);
  }

}


#endif
