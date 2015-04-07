SUBDIRS = libungif-4.1.0

INCLUDES = -I $(top_srcdir)/src/libungif-4.1.0

lib_LTLIBRARIES = libcorona.la
include_HEADERS = corona.h

if NO_PNG
PNG_DIST    = OpenPNG.cpp SavePNG.cpp
else
PNG_SOURCES = OpenPNG.cpp SavePNG.cpp
endif

if NO_JPEG
JPEG_DIST    = OpenJPEG.cpp
else
JPEG_SOURCES = OpenJPEG.cpp
endif

EXTRA_DIST = $(PNG_DIST) $(JPEG_DIST)

libcorona_la_SOURCES =				\
	$(PNG_SOURCES)				\
	$(JPEG_SOURCES)				\
	Convert.cpp				\
	Corona.cpp				\
	Debug.cpp				\
	Debug.h					\
	DefaultFileSystem.cpp			\
	MemoryFile.cpp                          \
	MemoryFile.h				\
	Open.h					\
	OpenBMP.cpp				\
	OpenGIF.cpp                             \
	OpenPCX.cpp				\
	OpenTGA.cpp				\
	Save.h					\
	SaveTGA.cpp				\
	SimpleImage.h				\
	Types.h					\
	Utility.h

libcorona_la_LIBADD = \
	libungif-4.1.0/libungif.la

libcorona_la_LDFLAGS = \
	-release $(VERSION)
