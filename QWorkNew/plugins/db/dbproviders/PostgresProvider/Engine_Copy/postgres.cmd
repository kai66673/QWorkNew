@echo off

SET FILE1=PostgresParser.yy
SET FILE2=PostgresParser.tab.hh

IF NOT EXIST %FILE1% GOTO END
IF NOT EXIST %FILE2% GOTO DOIT

rem FOR %%i IN (%FILE1%) DO SET DATE1=%%~ti
rem FOR %%i IN (%FILE2%) DO SET DATE2=%%~ti
rem IF "%DATE1%" == "%DATE2%" @ECHO "=== Postgres Parser Grammar Outdated ==="  && GOTO END

FOR /F %%i IN ('DIR /B /O:D %FILE1% %FILE2%') DO SET NEWEST=%%i

IF %FILE2% == %NEWEST% @ECHO "=== Postgres Parser Grammar Outdated ==="  && GOTO END

:DOIT
@ECHO "~~~~~ Postgres Grammar Generating Started ~~~~~"
win_bison.exe -l -d PostgresParser.yy

:END
