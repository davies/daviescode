# Microsoft Developer Studio Project File - Name="MpegMux" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MpegMux - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MpegMux.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MpegMux.mak" CFG="MpegMux - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MpegMux - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MpegMux - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MpegMux - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"bin/MpegMux.exe"

!ELSEIF  "$(CFG)" == "MpegMux - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /ML /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"bin/MpegMuxD.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MpegMux - Win32 Release"
# Name "MpegMux - Win32 Debug"
# Begin Group "MPA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MPA\l2tables.h
# End Source File
# Begin Source File

SOURCE=.\MPA\Mpa.cpp
# End Source File
# Begin Source File

SOURCE=.\MPA\MPA.H
# End Source File
# Begin Source File

SOURCE=.\MPA\MpaHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\MPA\MpaHeader.h
# End Source File
# Begin Source File

SOURCE=.\MPA\MpaStream.cpp
# End Source File
# Begin Source File

SOURCE=.\MPA\MpaStream.h
# End Source File
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\system\pack.cpp
# End Source File
# Begin Source File

SOURCE=.\system\pack.h
# End Source File
# Begin Source File

SOURCE=.\system\PES_packet.cpp
# End Source File
# Begin Source File

SOURCE=.\system\PES_packet.h
# End Source File
# Begin Source File

SOURCE=.\system\SystemHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\system\SystemHeader.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\BitStream.cpp
# End Source File
# Begin Source File

SOURCE=.\common\BitStream.h
# End Source File
# Begin Source File

SOURCE=.\common\def.h
# End Source File
# End Group
# Begin Group "main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Converter.cpp
# End Source File
# Begin Source File

SOURCE=.\Converter.h
# End Source File
# Begin Source File

SOURCE=.\demuxMPEG.cpp
# End Source File
# Begin Source File

SOURCE=.\doVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\muxer.cpp
# End Source File
# Begin Source File

SOURCE=.\muxer.h
# End Source File
# Begin Source File

SOURCE=.\muxMpa.cpp
# End Source File
# Begin Source File

SOURCE=.\muxMPEG.cpp
# End Source File
# Begin Source File

SOURCE=.\normalize.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoStream.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoStream.h
# End Source File
# End Group
# Begin Group "AC3 Decoder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vac3dec\Ac3_bitstream.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\ac3file.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\ac3file.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\bitstream.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\decoder.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\defs.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\delay.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\delay.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\demux.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\demux.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\dither.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\dither.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\eq.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\eq.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\imdct.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\imdct.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\mixer.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\mixer.h
# End Source File
# Begin Source File

SOURCE=.\vac3dec\parser.cpp
# End Source File
# Begin Source File

SOURCE=.\vac3dec\parser.h
# End Source File
# End Group
# Begin Group "MPAEncoder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MPAEncoder\absthr_tables.h
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\alloc_tables.h
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\common.cpp
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\common.h
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\encode.cpp
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\encoder.h
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\enwindow.h
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\MPAFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\mpafile.h
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\psy.cpp
# End Source File
# Begin Source File

SOURCE=.\MPAEncoder\subs.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# Begin Source File

SOURCE=.\VideoConvertLib\VideoConvert.lib
# End Source File
# End Target
# End Project
