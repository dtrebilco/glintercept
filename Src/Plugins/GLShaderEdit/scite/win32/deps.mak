DirectorExtension.o: DirectorExtension.cxx \
 ../../scintilla/include/Platform.h ../../scintilla/include/PropSet.h \
 ../../scintilla/include/SString.h ../../scintilla/include/Scintilla.h \
 ../../scintilla/include/Accessor.h ../src/Extender.h \
 DirectorExtension.h ../src/SciTE.h ../src/FilePath.h ../src/SciTEBase.h
SciTEWin.o: SciTEWin.cxx SciTEWin.h ../../scintilla/include/Platform.h \
 ../src/SciTE.h ../../scintilla/include/PropSet.h \
 ../../scintilla/include/SString.h ../../scintilla/include/Accessor.h \
  ../../scintilla/include/KeyWords.h ../../scintilla/include/Scintilla.h \
  ../src/Extender.h ../src/FilePath.h ../src/SciTEBase.h ../src/SciTEKeys.h \
  UniqueInstance.h ../src/MultiplexExtension.h DirectorExtension.h \
  SingleThreadExtension.h ../src/LuaExtension.h
SciTEWinBar.o: SciTEWinBar.cxx SciTEWin.h \
 ../../scintilla/include/Platform.h ../src/SciTE.h \
 ../../scintilla/include/PropSet.h ../../scintilla/include/SString.h \
 ../../scintilla/include/Accessor.h ../../scintilla/include/KeyWords.h \
 ../../scintilla/include/Scintilla.h ../src/Extender.h \
 ../src/FilePath.h ../src/SciTEBase.h ../src/SciTEKeys.h UniqueInstance.h
SciTEWinDlg.o: SciTEWinDlg.cxx SciTEWin.h \
 ../../scintilla/include/Platform.h ../src/SciTE.h \
 ../../scintilla/include/PropSet.h ../../scintilla/include/SString.h \
 ../../scintilla/include/Accessor.h ../../scintilla/include/KeyWords.h \
 ../../scintilla/include/Scintilla.h ../src/Extender.h \
 ../src/FilePath.h ../src/SciTEBase.h ../src/SciTEKeys.h UniqueInstance.h
SingleThreadExtension.o: SingleThreadExtension.cxx \
  SingleThreadExtension.h ../src/Extender.h \
  ../../scintilla/include/Scintilla.h
UniqueInstance.o: UniqueInstance.cxx ../../scintilla/include/Platform.h \
  SciTEWin.h ../src/SciTE.h ../../scintilla/include/PropSet.h \
  ../../scintilla/include/SString.h ../../scintilla/include/Accessor.h \
  ../../scintilla/include/KeyWords.h ../../scintilla/include/Scintilla.h \
  ../src/Extender.h ../src/FilePath.h ../src/SciTEBase.h ../src/SciTEKeys.h \
  UniqueInstance.h
Exporters.o: ../src/Exporters.cxx ../../scintilla/include/Platform.h \
 ../src/SciTE.h ../../scintilla/include/PropSet.h \
 ../../scintilla/include/SString.h ../../scintilla/include/Accessor.h \
 ../../scintilla/include/WindowAccessor.h \
 ../../scintilla/include/Scintilla.h ../src/Extender.h \
 ../src/FilePath.h ../src/SciTEBase.h
IFaceTable.o: ../src/IFaceTable.cxx ../src/IFaceTable.h
LuaExtension.o: ../src/LuaExtension.cxx \
  ../../scintilla/include/Scintilla.h ../../scintilla/include/Accessor.h \
  ../src/Extender.h ../src/LuaExtension.h \
  ../../scintilla/include/SString.h ../src/SciTEKeys.h \
  ../src/IFaceTable.h ../lua/include/lua.h ../lua/include/lualib.h \
  ../lua/include/lauxlib.h ../../scintilla/include/Platform.h
MultiplexExtension.o: ../src/MultiplexExtension.cxx \
  ../src/MultiplexExtension.h ../src/Extender.h \
  ../../scintilla/include/Scintilla.h
SciTEBase.o: ../src/SciTEBase.cxx ../../scintilla/include/Platform.h \
 ../src/SciTE.h ../../scintilla/include/PropSet.h \
 ../../scintilla/include/SString.h ../../scintilla/include/Accessor.h \
 ../../scintilla/include/WindowAccessor.h \
  ../../scintilla/include/KeyWords.h ../../scintilla/include/Scintilla.h \
 ../../scintilla/include/ScintillaWidget.h \
  ../../scintilla/include/SciLexer.h ../src/Extender.h ../src/FilePath.h ../src/SciTEBase.h
SciTEBuffers.o: ../src/SciTEBuffers.cxx \
 ../../scintilla/include/Platform.h ../src/SciTE.h \
 ../../scintilla/include/PropSet.h ../../scintilla/include/SString.h \
 ../../scintilla/include/Accessor.h \
 ../../scintilla/include/WindowAccessor.h \
  ../../scintilla/include/Scintilla.h ../../scintilla/include/SciLexer.h \
  ../src/Extender.h ../src/FilePath.h ../src/SciTEBase.h
SciTEIO.o: ../src/SciTEIO.cxx ../../scintilla/include/Platform.h \
 ../src/SciTE.h ../../scintilla/include/PropSet.h \
 ../../scintilla/include/SString.h ../../scintilla/include/Accessor.h \
 ../../scintilla/include/WindowAccessor.h \
 ../../scintilla/include/Scintilla.h ../src/Extender.h ../src/Utf8_16.h \
 ../src/FilePath.h ../src/SciTEBase.h
SciTEProps.o: ../src/SciTEProps.cxx ../../scintilla/include/Platform.h \
 ../src/SciTE.h ../../scintilla/include/PropSet.h \
 ../../scintilla/include/SString.h ../../scintilla/include/Accessor.h \
  ../../scintilla/include/Scintilla.h ../../scintilla/include/SciLexer.h \
  ../src/Extender.h ../src/FilePath.h ../src/SciTEBase.h
Utf8_16.o: ../src/Utf8_16.cxx ../src/Utf8_16.h
lapi.o: ../lua/src/lapi.c ../lua/include/lua.h ../lua/src/lapi.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h \
  ../lua/src/lfunc.h ../lua/src/lgc.h ../lua/src/lmem.h \
  ../lua/src/lstring.h ../lua/src/ltable.h ../lua/src/lundump.h \
  ../lua/src/lvm.h
lcode.o: ../lua/src/lcode.c ../lua/include/lua.h ../lua/src/lcode.h \
  ../lua/src/llex.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/lzio.h ../lua/src/lopcodes.h ../lua/src/lparser.h \
  ../lua/src/ltable.h ../lua/src/ldebug.h ../lua/src/lstate.h \
  ../lua/src/ltm.h ../lua/src/ldo.h ../lua/src/lmem.h
ldebug.o: ../lua/src/ldebug.c ../lua/include/lua.h ../lua/src/lapi.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/src/lcode.h \
  ../lua/src/llex.h ../lua/src/lzio.h ../lua/src/lopcodes.h \
  ../lua/src/lparser.h ../lua/src/ltable.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/ltm.h ../lua/src/ldo.h \
  ../lua/src/lfunc.h ../lua/src/lstring.h ../lua/src/lvm.h
ldo.o: ../lua/src/ldo.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h ../lua/src/lfunc.h \
  ../lua/src/lgc.h ../lua/src/lmem.h ../lua/src/lopcodes.h \
  ../lua/src/lparser.h ../lua/src/ltable.h ../lua/src/lstring.h \
  ../lua/src/lundump.h ../lua/src/lvm.h
ldump.o: ../lua/src/ldump.c ../lua/include/lua.h ../lua/src/lobject.h \
  ../lua/src/llimits.h ../lua/src/lopcodes.h ../lua/src/lstate.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/lundump.h
lfunc.o: ../lua/src/lfunc.c ../lua/include/lua.h ../lua/src/lfunc.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/src/lgc.h \
  ../lua/src/lmem.h ../lua/src/lstate.h ../lua/src/ltm.h \
  ../lua/src/lzio.h
lgc.o: ../lua/src/lgc.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h ../lua/src/lfunc.h \
  ../lua/src/lgc.h ../lua/src/lmem.h ../lua/src/lstring.h \
  ../lua/src/ltable.h
llex.o: ../lua/src/llex.c ../lua/include/lua.h ../lua/src/ldo.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/src/lstate.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/llex.h \
  ../lua/src/lparser.h ../lua/src/ltable.h ../lua/src/lstring.h
lmem.o: ../lua/src/lmem.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h ../lua/src/lmem.h
lobject.o: ../lua/src/lobject.c ../lua/include/lua.h ../lua/src/ldo.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/src/lstate.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/lmem.h \
  ../lua/src/lstring.h ../lua/src/lvm.h
lopcodes.o: ../lua/src/lopcodes.c ../lua/include/lua.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/src/lopcodes.h
lparser.o: ../lua/src/lparser.c ../lua/include/lua.h ../lua/src/lcode.h \
  ../lua/src/llex.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/lzio.h ../lua/src/lopcodes.h ../lua/src/lparser.h \
  ../lua/src/ltable.h ../lua/src/ldebug.h ../lua/src/lstate.h \
  ../lua/src/ltm.h ../lua/src/lfunc.h ../lua/src/lmem.h \
  ../lua/src/lstring.h
lstate.o: ../lua/src/lstate.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h ../lua/src/lfunc.h \
  ../lua/src/lgc.h ../lua/src/llex.h ../lua/src/lmem.h \
  ../lua/src/lstring.h ../lua/src/ltable.h
lstring.o: ../lua/src/lstring.c ../lua/include/lua.h ../lua/src/lmem.h \
  ../lua/src/llimits.h ../lua/src/lobject.h ../lua/src/lstate.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/lstring.h
ltable.o: ../lua/src/ltable.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h ../lua/src/lgc.h \
  ../lua/src/lmem.h ../lua/src/ltable.h
ltests.o: ../lua/src/ltests.c ../lua/include/lua.h ../lua/src/lapi.h \
  ../lua/src/lobject.h ../lua/src/llimits.h ../lua/include/lauxlib.h \
  ../lua/src/lcode.h ../lua/src/llex.h ../lua/src/lzio.h \
  ../lua/src/lopcodes.h ../lua/src/lparser.h ../lua/src/ltable.h \
  ../lua/src/ldebug.h ../lua/src/lstate.h ../lua/src/ltm.h \
  ../lua/src/ldo.h ../lua/src/lfunc.h ../lua/src/lmem.h \
  ../lua/src/lstring.h ../lua/include/lualib.h
ltm.o: ../lua/src/ltm.c ../lua/include/lua.h ../lua/src/lobject.h \
  ../lua/src/llimits.h ../lua/src/lstate.h ../lua/src/ltm.h \
  ../lua/src/lzio.h ../lua/src/lstring.h ../lua/src/ltable.h
lundump.o: ../lua/src/lundump.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/lfunc.h ../lua/src/lmem.h \
  ../lua/src/lopcodes.h ../lua/src/lstring.h ../lua/src/lundump.h
lvm.o: ../lua/src/lvm.c ../lua/include/lua.h ../lua/src/ldebug.h \
  ../lua/src/lstate.h ../lua/src/lobject.h ../lua/src/llimits.h \
  ../lua/src/ltm.h ../lua/src/lzio.h ../lua/src/ldo.h ../lua/src/lfunc.h \
  ../lua/src/lgc.h ../lua/src/lopcodes.h ../lua/src/lstring.h \
  ../lua/src/ltable.h ../lua/src/lvm.h
lzio.o: ../lua/src/lzio.c ../lua/include/lua.h ../lua/src/llimits.h \
  ../lua/src/lmem.h ../lua/src/lzio.h
lauxlib.o: ../lua/src/lib/lauxlib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h
lbaselib.o: ../lua/src/lib/lbaselib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
ldblib.o: ../lua/src/lib/ldblib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
liolib.o: ../lua/src/lib/liolib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
lmathlib.o: ../lua/src/lib/lmathlib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
loadlib.o: ../lua/src/lib/loadlib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
lstrlib.o: ../lua/src/lib/lstrlib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
ltablib.o: ../lua/src/lib/ltablib.c ../lua/include/lua.h \
  ../lua/include/lauxlib.h ../lua/include/lualib.h
