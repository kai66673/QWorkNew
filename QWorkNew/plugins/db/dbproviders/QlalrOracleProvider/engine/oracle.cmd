@echo off

SET FILE1=t.g
SET FILE2=QLalrOracleParser.h

IF NOT EXIST %FILE1% GOTO END
IF NOT EXIST %FILE2% GOTO DOIT

rem FOR %%i IN (%FILE1%) DO SET DATE1=%%~ti
rem FOR %%i IN (%FILE2%) DO SET DATE2=%%~ti
rem IF "%DATE1%" == "%DATE2%" @ECHO "=== Oracle Parser Grammar Outdated ==="  && GOTO END

FOR /F %%i IN ('DIR /B /O:D %FILE1% %FILE2%') DO SET NEWEST=%%i

IF %FILE2% == %NEWEST% @ECHO "=== Oracle Parser Grammar Outdated ==="  && GOTO END

:DOIT
@ECHO "~~~~~ Oracle Grammar Generating Started ~~~~~"
qlalr.exe --qt --no-debug --no-lines t.g

:END
