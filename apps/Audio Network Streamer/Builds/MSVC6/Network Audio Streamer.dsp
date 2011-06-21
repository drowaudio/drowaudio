# Microsoft Developer Studio Project File - Name="Network Audio Streamer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **
# TARGTYPE "Win32 (x86) Application" 0x0101
CFG=Network Audio Streamer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Network Audio Streamer.mak."
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Network Audio Streamer.mak" CFG="Network Audio Streamer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Network Audio Streamer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Network Audio Streamer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
!IF  "$(CFG)" == "Network Audio Streamer - Win32 Debug"
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Od /D WIN32 /D _WINDOWS /D DEBUG /D _DEBUG /D JUCER_MSVC6_734A9119=1 /YX /FD /c /Gm /ZI /GZ /Zm1024
# ADD CPP /nologo /MTd /W3 /GR /GX /Od /I  /D WIN32 /D _WINDOWS /D DEBUG /D _DEBUG /D JUCER_MSVC6_734A9119=1 /D "_UNICODE" /D "UNICODE" /FD /c /Zm1024 /Gm /ZI /GZ 
# ADD BASE MTL /nologo /D WIN32 /D _WINDOWS /D DEBUG /D _DEBUG /D JUCER_MSVC6_734A9119=1 /mktyplib203 /win32
# ADD MTL /nologo /D WIN32 /D _WINDOWS /D DEBUG /D _DEBUG /D JUCER_MSVC6_734A9119=1 /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d WIN32 /D _WINDOWS /D DEBUG /D _DEBUG /D JUCER_MSVC6_734A9119=1
# ADD RSC /l 0x40c /d WIN32 /D _WINDOWS /D DEBUG /D _DEBUG /D JUCER_MSVC6_734A9119=1
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 "C:\Program Files\Microsoft Visual Studio\VC98\LIB\shell32.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  /debug /nologo /machine:I386 /out:".\Debug\Network Audio Streamer.exe" /subsystem:windows 
!ELSEIF  "$(CFG)" == "Network Audio Streamer - Win32 Release"
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D WIN32 /D _WINDOWS /D NDEBUG /D JUCER_MSVC6_734A9119=1 /YX /FD /c  /Zm1024
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I  /D WIN32 /D _WINDOWS /D NDEBUG /D JUCER_MSVC6_734A9119=1 /D "_UNICODE" /D "UNICODE" /FD /c /Zm1024  
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D WIN32 /D _WINDOWS /D NDEBUG /D JUCER_MSVC6_734A9119=1 /mktyplib203 /win32
# ADD MTL /nologo /D WIN32 /D _WINDOWS /D NDEBUG /D JUCER_MSVC6_734A9119=1 /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d WIN32 /D _WINDOWS /D NDEBUG /D JUCER_MSVC6_734A9119=1
# ADD RSC /l 0x40c /d WIN32 /D _WINDOWS /D NDEBUG /D JUCER_MSVC6_734A9119=1
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 "C:\Program Files\Microsoft Visual Studio\VC98\LIB\shell32.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  /nologo /machine:I386 /out:".\Release\Network Audio Streamer.exe" /subsystem:windows 
!ENDIF
# Begin Target
# Name "Network Audio Streamer - Win32 Debug"
# Name "Network Audio Streamer - Win32 Release"
# Begin Group "Network Audio Streamer"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Source"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\Main.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\Settings.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\Settings.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\AudioNetworkStreamer.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\AudioNetworkStreamer.cpp"
# End Source File
# Begin Group "audio"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\CompressionFunctions.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\AudioEngine.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\AudioEngine.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\dRowAudio_FIFOBuffer.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\dRowAudio_FIFOBuffer.cpp"
# End Source File
# End Group
# Begin Group "ui"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\MainWindow.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\MainWindow.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\MainComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\MainComponent.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\MainView.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\MainView.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\StatusComp.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\StatusComp.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\SettingsComp.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\SettingsComp.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\AudioSettingsComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\AudioSettingsComponent.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\dRowAudio_AudioOscilloscope.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\dRowAudio_AudioOscilloscope.cpp"
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Juce Library Code"
# Begin Source File
SOURCE="..\..\JuceLibraryCode\AppConfig.h"
# End Source File
# Begin Source File
SOURCE="..\..\JuceLibraryCode\JuceHeader.h"
# End Source File
# Begin Source File
SOURCE="..\..\JuceLibraryCode\JuceLibraryCode1.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\JuceLibraryCode\JuceLibraryCode2.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\JuceLibraryCode\JuceLibraryCode3.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\JuceLibraryCode\JuceLibraryCode4.cpp"
# End Source File
# End Group
# End Target
# End Project
