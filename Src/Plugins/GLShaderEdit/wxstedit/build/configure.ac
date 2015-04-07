# ======================================================================================
# Author: Francesco Montorsi
# RCS-ID: $Id: configure.ac,v 1.6 2006/03/06 23:04:33 frm Exp $
#
# A basic "configure.ac" for a wxCode component.
# See
#    http://www.gnu.org/software/autoconf/manual/autoconf-2.57/html_mono/autoconf.html
# for more info about autoconf and the autoconf macros.
# ======================================================================================



# Init this configure script with basic info about the component
# (DON'T PUT ANYTHING BEFORE AC_INIT, JUST COMMENTS)
AC_INIT([wxStEdit], [1.0], [noone@sourceforge.net])

# ENABLES/DISABLES THE DEBUG MODE FOR THIS CONFIGURE SCRIPT
#DBG_CONFIGURE=1

# you need the wxCode/build/bakefiles in your local repository to generate a configure
# script from this configure.ac file since wxcode.m4 is required....
m4_include(../../../build/autoconf/wxcode.m4)
AM_WXCODE_INIT




## CONFIGURE OPTIONS
##
## Before starting checks, declare the options of this configure script
## Here you should use the AC_ARG_ENABLE and AC_ARG_WITH macros
## to add to this configure script the --enable-XXX and/or --with-XXX
## options required. If you did not use any <option> tag in your
## component's bakefile, then you can leave this section as is
#####################################################################

AM_OPTIONS_WXCONFIG
AM_OPTIONS_WXPRESETS





## CONFIGURE CHECKS
##
## Here you should use the AC_CHECK_LIB, AC_COMPILE_IFELSE,
## AC_LINK_IFELSE, etc macros to check that the libraries required
## by your component exist on the host machine and match your
## required options (version, build settings, etc)
#####################################################################

# I can't really imagine a Unix without CP... but let's check anyway...
AC_CHECK_TOOL(CP, cp, :)

AM_WXCODE_CHECKS([2.4.0], [stc])       # 2.4.0 is the minimum required version of wx

# copy setup0.h into setup.h
$CP include/wx/stedit/setup0.h include/wx/stedit/setup.h


# end in a nice way the configure script
AM_WXCODE_END
