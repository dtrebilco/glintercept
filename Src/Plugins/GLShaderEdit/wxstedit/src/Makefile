# Makefile for wxstedit lib (UNIX).

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

WXSTC_LIB = $(WXBASENAME)_stc-$(WXRELEASE)

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
APPEXTRADEFS = -I$(WXDIR)/contrib/include -I../include $(PROFILE)
APPEXTRALIBS = -l$(WXSTC_LIB) $(PROFILE)

# ----------------------------------------------------------------------------

RM        = rm -f
AR        = ar
AROPTIONS = rcu
RANLIB    = ranlib
LN_S      = ln -s

# ----------------------------------------------------------------------------

OBJECTS = \
    stedit.o \
    stedlgs.o \
    stedlgs_wdr.o \
    steexprt.o \
    stefindr.o \
    steframe.o \
    stelangs.o \
    stemenum.o \
    stenoteb.o \
    steopts.o \
    steprefs.o \
    steprint.o \
    stesplit.o \
    steshell.o \
    stestyls.o

DEPFILES=$(OBJECTS:.o=.d)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(TARGET_LIB_STATIC) $(TARGET_LIB_SHARED)

$(TARGET_LIB_STATIC) : $(OBJECTS)
		@$(RM) $@
		$(AR) $(AROPTIONS) $@ $(OBJECTS)
		$(RANLIB) $@

$(TARGET_LIB_SHARED): $(OBJECTS)
		$(CXX) -shared -o $@ $(OBJECTS)
		cd $(WXLIBDIR) \
			&& $(RM) $(TARGET_LIB_LINK1) $(TARGET_LIB_LINK2) \
				&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK1) \
				&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK2)

# wxstedit program using shared stedit library
#$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_SHARED)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(APPEXTRALIBS) -l$(WXSTEDIT_LIB)

# wxstedit program using static stedit library
#$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_STATIC)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(APPEXTRALIBS) $(TARGET_LIB_STATIC)

# wxstedit program using just object files and not the stedit lib
#$(PROGRAM): $(PROGRAM).o $(OBJECTS)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(OBJECTS) $(LDLIBS) $(APPEXTRALIBS)

clean:
	rm -f $(OBJECTS) $(DEPFILES) core \
		$(TARGET_LIB_STATIC) \
		$(TARGET_LIB_SHARED) \
		$(WXSTEDIT_LIBDIR)/$(TARGET_LIB_LINK1) \
		$(WXSTEDIT_LIBDIR)/$(TARGET_LIB_LINK2)

-include $(DEPFILES)

