cd ..
del/q wscite.zip
mkdir wscite
copy scite\license.txt wscite
copy scite\bin\SciTE.exe wscite
copy scite\bin\SciTE.pdb wscite
copy scite\bin\SciLexer.dll wscite
copy scite\bin\SciLexer.pdb wscite
copy scite\src\*.properties wscite
copy scite\lua\COPYRIGHT wscite\luaCOPYRIGHT
del/q wscite\Embedded.properties
copy scite\doc\*.html wscite
copy scite\doc\*.png wscite
copy scite\doc\*.jpg wscite
copy scite\doc\*.1 wscite
zip wscite.zip wscite\*.*
del/q wscite\*.*
rmdir wscite
cd scite
