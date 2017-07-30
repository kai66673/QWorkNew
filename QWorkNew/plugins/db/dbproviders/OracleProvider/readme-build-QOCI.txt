I. Linux

Install oracle client:
  oracle-instantclient
  oracle-instantclient-devel

  FOR ME!
  download
  oracle-instantclient-10.2.0.4-6.1.x86_64.rpm
  oracle-instantclient-devel-10.2.0.4-6.1.i386.rpm

  Extract to tree:
  /home/kai/Dev/Oracle/lib/*.so.*
  # cd /home/kai/Dev/Oracle/lib
  # ln -s libclntsh.so.10.1 libclntsh.so
  /home/kai/Dev/Oracle/include/*.h

Build QOCI driver:
Download qtbase-opensource-src-5.2.1
# cd /home/kai/Dev/qtbase-opensource-src-5.2.1/src/plugins/sqldrivers/oci
# qmake "INCLUDEPATH+=/home/kai/Dev/Oracle/include" "LIBS+=-L/home/kai/Dev/Oracle/lib -lclntsh" oci.pro
# make
# sudo cp /home/kai/Dev/qtbase-opensource-src-5.2.1/plugins/sqldrivers/libqsqloci.so /usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/

II. Windows

Install Oracle client
FOR ME:
  c:\oraclexe\app\oracle\product\11.2.0\server\oci
    \include
    \lib\MSVC

run Qt command prompt
> set INCLUDE=%INCLUDE%;c:\oraclexe\app\oracle\product\11.2.0\server\oci\include
> set LIB=%LIB%;c:\oraclexe\app\oracle\product\11.2.0\server\oci\lib\MSVC
> cd %QTDIR%\src\plugins\sqldrivers\oci
> qmake oci.pro
> nmake
> nmake release
