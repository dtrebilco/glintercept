rem ba.bat - download and build all of scintilla and scite
rd /s/q scintilla scite
cvs co scintilla scite
cd scintilla
call delbin
del/q bin\*.a
call delcvs
call zipsrc
cd win32
nmake -f scintilla.mak
cd ..
del/q bin\*.pdb
cd ..
cd scite
del/q bin\*.properties
del/q bin\SciTE
call delbin
call delcvs
call zipsrc
cd win32
nmake -f scite.mak
cd ..
call upxsc1
call zipwscite
call delbin
cd ..
cd scintilla
call delbin
cd ..
