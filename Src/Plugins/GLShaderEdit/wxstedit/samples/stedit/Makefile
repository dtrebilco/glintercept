# Makefile for wxstedit example for the wxstedit lib (UNIX).

# ----------------------------------------------------------------------------
# wxWidgets build settings

WXCONFIG := wx-config

WXDIR      = $(shell $(WXCONFIG) --prefix)
WXEXECDIR  = $(shell $(WXCONFIG) --exec-prefix)
WXLIBDIR   = $(WXEXECDIR)/lib

WXBASENAME = $(shell $(WXCONFIG) --basename)
WXVERSION  = $(shell $(WXCONFIG) --version)
WXRELEASE  = $(shell $(WXCONFIG) --release)

WXCXX      = $(shell $(WXCONFIG) --cxx)
WXCXXFLAGS = $(shell $(WXCONFIG) --cxxflags)
WXLIBS     = $(shell $(WXCONFIG) --libs)

WXSTC_LIB  = $(WXBASENAME)_stc-$(WXRELEASE)

# ----------------------------------------------------------------------------

WXSTEDIT_LIBNAME = $(WXBASENAME)_stedit-$(WXRELEASE)
WXSTEDIT_LIBDIR = $(WXLIBDIR)

LIBVERSION_CURRENT=1
LIBVERSION_REVISION=0
LIBVERSION_AGE=0

TARGET_LIBNAME = lib$(WXSTEDIT_LIBNAME)

TARGET_LIB_STATIC = $(WXSTEDIT_LIBDIR)/$(TARGET_LIBNAME).a
TARGET_LIB_SHARED = $(WXSTEDIT_LIBDIR)/$(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT).$(LIBVERSION_REVISION).$(LIBVERSION_AGE)
TARGET_LIB_LINK1  = $(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT)
TARGET_LIB_LINK2  = $(TARGET_LIBNAME).so

# ----------------------------------------------------------------------------

CXXFLAGS = $(WXCXXFLAGS) -MMD -g  -Wall -Wunused -Wunused-parameter
LDLIBS   = $(WXLIBS)
CXX      = $(WXCXX)

# -pg in libs and defs for profiling
PROFILE = #-pg
APPEXTRADEFS = -I$(WXDIR)/contrib/include -I../../include $(PROFILE)
APPEXTRALIBS =  -l$(WXSTC_LIB) $(PROFILE)

# ----------------------------------------------------------------------------

PROGRAM = wxstedit

OBJECTS = \
    ../../src/stedit.o \
    ../../src/stedlgs.o \
    ../../src/stedlgs_wdr.o \
    ../../src/steexprt.o \
    ../../src/stefindr.o \
    ../../src/steframe.o \
    ../../src/stelangs.o \
    ../../src/stemenum.o \
    ../../src/stenoteb.o \
    ../../src/steopts.o \
    ../../src/steprefs.o \
    ../../src/steprint.o \
    ../../src/stesplit.o \
    ../../src/steshell.o \
    ../../src/stestyls.o

DEPFILES=$(OBJECTS:.o=.d)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(WXSTEDIT_LIBNAME) $(PROGRAM)

$(WXSTEDIT_LIBNAME) :
	@(cd ../../src && make)


# wxstedit program using shared stedit library
$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_SHARED)
	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) -l$(WXSTEDIT_LIBNAME) $(APPEXTRALIBS)

# wxstedit program using static stedit library
#$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_STATIC)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(TARGET_LIB_STATIC) $(APPEXTRALIBS)

# wxstedit program using just object files and not the stedit lib
#$(PROGRAM): $(PROGRAM).o $(OBJECTS)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(OBJECTS) $(LDLIBS) $(APPEXTRALIBS)

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(PROGRAM) $(PROGRAM).o $(PROGRAM).d core

cleanall: clean
	@(cd ../../src && make clean)


-include $(DEPFILES)

