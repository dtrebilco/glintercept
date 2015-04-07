#
# SciTE code editor using the Scintilla editing component
# SciTE had syntax highlighting and support for building and running programs
#
# This package contains the gtk enabled varsion, distributed as a binary
# tarball from http://www.scintilla.org/SciTE.html
#
# The tarball is named gscite???.tgz, however the actual name of the app
# is still SciTE.
#
# This package name purposely uses all lower case letters in the its package
# name so that one can issue the command "rpm -q scite" without having to
# remember the exact case of all the letters. However, none of the files 
# contained in this package have been renamed.
#
#
####

Name:           scite
Version:        1.39
Release:        1
Summary:        SciTE code editor with syntax highlighting for building and running programs.
Group:          Applications/Editors
Copyright:      Python-Type License
Vendor:         Scintilla by Neil Hodgson
URL:            http://www.scintilla.org/SciTE.html
Packager:       AAP <devel@aminvestments.com> Victor Soroka <vs240@yahoo.com>
Source0:	http://www.scintilla.org/scite139.tgz
BuildRoot:      /var/tmp/scite-%{version}-rootdir
Requires:       gtk+ >= 1.2.0

%description
SciTE is a SCIntilla based Text Editor. Originally built to demonstrate Scintilla, it has grown to
be a generally useful editor with facilities for building and running programs. It is best used for
jobs with simple configurations - I use it for building test and demonstration programs as well
as SciTE and Scintilla, themselves. 

SciTE is currently available for Intel Win32 and Linux compatible operating systems with
GTK+.

########## 
#
# The following environment variables are automatically defined for use
# in any of the following shell scripts:
#
#   RPM_SOURCE_DIR      { where sources originally reside }
#   RPM_BUILD_DIR       { where sources get unpacked into }
#   RPM_DOC_DIR
#   RPM_OPT_FLAGS
#   RPM_ARCH
#   RPM_OS
#   RPM_ROOT_DIR
#   RPM_BUILD_ROOT      { where final images get placed before packaging }
#   RPM_PACKAGE_NAME
#   RPM_PACKAGE_VERSION
#   RPM_PACKAGE_RELEASE
#
##########

%prep
%setup -q -c {name}-%{version}

%build
cd $RPM_BUILD_DIR/%{name}-%{version}/scintilla/gtk
make
cd $RPM_BUILD_DIR/%{name}-%{version}/scite/gtk
make

####
# Section: Install-After-Build Script (Often Just 'make install')
####
%install
rm -rf $RPM_BUILD_ROOT
install -m0755 --directory      $RPM_BUILD_ROOT/usr/bin
install -m0755 --directory      $RPM_BUILD_ROOT/usr/share/scite
install -m0755 --directory      $RPM_BUILD_ROOT/usr/share/man/man1
install -m0755 --directory      $RPM_BUILD_ROOT/usr/share/pixmaps
install -m0755 --directory      $RPM_BUILD_ROOT/usr/share/gnome/apps/Applications
cd $RPM_BUILD_DIR/%{name}-%{version}/scite/gtk
make gnomeprefix=$RPM_BUILD_ROOT/usr install
install -m 0644 $RPM_BUILD_DIR/%{name}-%{version}/scite/doc/scite.1 $RPM_BUILD_ROOT/usr/share/man/man1
mv $RPM_BUILD_ROOT/usr/share/gnome/apps/Applications $RPM_BUILD_ROOT/usr/share/gnome/apps/Development

%clean
rm -rf $RPM_BUILD_ROOT
rm -rf ${RPM_BUILD_DIR}/%{name}-%{version}



%files
%defattr(-, root, root)
%attr(0644,root,root)	%doc 	scite/doc/*.html scite/doc/*.png scite/doc/*.jpg
%attr(0755,root,root)	%dir	/usr/share/scite
%attr(0755,root,root)	/usr/bin/SciTE
%attr(0644,root,root)	/usr/share/scite/*
%attr(0644,root,root)	/usr/share/pixmaps/Sci48M.png
%attr(0644,root,root)	/usr/share/gnome/apps/Development/SciTE.desktop
%attr(0644,root,root)	/usr/share/man/man1/scite.1*
