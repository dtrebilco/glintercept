@rem clearboth.bat - script to clear both scintilla and scite 
@rem directory trees of all compiler output files.
@rem Current directory must be parent of scintilla and scite before running.
@cd scintilla
@call delbin
@cd ..
@cd scite
@call delbin
@cd ..
