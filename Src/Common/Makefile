include ../Makefile.config
TARGET=libCommon.a
SOURCES=\
ConfigParser.cpp \
FileUtils.cpp \
InputUtils.cpp \
MiscUtils.cpp \
prefix.cpp

OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

HEADERS=\
AutoReferenceCount.h \
ConfigParser.h \
FileUtils.h \
InputUtils.h \
MiscUtils.h \
OSDefines.h \
prefix.h \
ReferenceCount.h

CXXFLAGS=$(CORONA_CFLAGS) -DGLI_BUILD_LINUX -I./../ -I./ -O0 -g3 -Wall -fmessage-length=0

all:	$(TARGET)

$(TARGET):	$(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(OBJECTS):%.o:	%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)
