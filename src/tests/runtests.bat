@echo off

del *.run
del *.dif

for %i in (*.err) do (
  call c99 %~ni.c 2> %~ni.run
  fc %~ni.err %~ni.run > %~ni.dif
  if not errorlevel 1 del %~ni.dif
)
