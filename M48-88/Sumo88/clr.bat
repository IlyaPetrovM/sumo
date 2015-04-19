del *.*~
del *.obj
del *.map
del *.lst
del *.asm
del *.eep
del *.err
del *.rom
del *__.*
del *.hex
del *.vec
del *.inc

del *.sym
del *.cof

del *.a*
del *.o*
del *.c!
del *.c@
del *.c_*
del *.cci
del *.hdr
del *.str*
del *.i

del *.h!
del *.fct
del *.h_cbf

if not exist ..\..\lib\m48\*.* goto exitp
cd ..\..\lib\m48
call clr.bat

:exitp