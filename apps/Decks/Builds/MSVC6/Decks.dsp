# Microsoft Developer Studio Project File - Name="Decks" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **
# TARGTYPE "Win32 (x86) Application" 0x0101
CFG=Decks - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Decks.mak."
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Decks.mak" CFG="Decks - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Decks - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Decks - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
!IF  "$(CFG)" == "Decks - Win32 Debug"
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
# ADD LINK32 "C:\Program Files\Microsoft Visual Studio\VC98\LIB\shell32.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  /debug /nologo /machine:I386 /out:".\Debug\Decks.exe" /subsystem:windows 
!ELSEIF  "$(CFG)" == "Decks - Win32 Release"
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
# ADD LINK32 "C:\Program Files\Microsoft Visual Studio\VC98\LIB\shell32.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  /nologo /machine:I386 /out:".\Release\Decks.exe" /subsystem:windows 
!ENDIF
# Begin Target
# Name "Decks - Win32 Debug"
# Name "Decks - Win32 Release"
# Begin Group "Decks"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Source"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "main"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\main\ApplicationStartup.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\CommandIDs.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\DefaultSettings.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\Settings.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\Settings.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\UISettings.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\MixerSettings.h"
# End Source File
# Begin Group "audio"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\main\dRowAudio_OverflowingAbstractFifo.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\dRowAudio_OverflowingAbstractFifo.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\dRowAudio_SampleRateConverter.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\dRowAudio_SampleRateConverter.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\DecksAudioFormatManager.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\DecksAudioFormatManager.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\AudioEngine.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\AudioEngine.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\DeckManager.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\main\DeckManager.cpp"
# End Source File
# End Group
# End Group
# Begin Group "ui"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\DecksColours.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\DecksColours.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\DecksLookAndFeel.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\DecksLookAndFeel.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\MainAppWindow.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\MainAppWindow.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\ContainerComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\ContainerComponent.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\TitleBarComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\TitleBarComponent.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\QuitScreen.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\QuitScreen.cpp"
# End Source File
# Begin Group "controls"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\controls\ControlComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\ControlComponent.cpp"
# End Source File
# Begin Group "transport"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\Transport.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\Transport.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\DeckTransport.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\DeckTransport.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\TrackInfo.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\TrackInfo.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\TrackSuggestions.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\TrackSuggestions.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\LoopAndCuePoints.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\transport\LoopAndCuePoints.cpp"
# End Source File
# End Group
# Begin Group "draggable_display"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\controls\draggable_display\DraggableDisplay.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\draggable_display\DraggableDisplay.cpp"
# End Source File
# End Group
# Begin Group "mixer"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\Mixer.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\Mixer.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\MixerChannelStrip.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\MixerChannelStrip.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\CrossFader.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\CrossFader.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\MasterChannelStrip.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\mixer\MasterChannelStrip.cpp"
# End Source File
# End Group
# Begin Group "library"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\controls\library\Library.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\library\Library.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\library\PrepareTable.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\controls\library\PrepareTable.cpp"
# End Source File
# End Group
# End Group
# Begin Group "settings"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\ui\settings\SettingsComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\settings\SettingsComponent.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\settings\MixerSettingsComponent.h"
# End Source File
# Begin Source File
SOURCE="..\..\Source\ui\settings\MixerSettingsComponent.cpp"
# End Source File
# End Group
# End Group
# Begin Group "resources"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\Source\resources\Decks.icns"
# End Source File
# Begin Source File
SOURCE="..\..\Source\resources\Decks_icon.png"
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Group
# Begin Group "Libraries"
# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE="..\..\..\..\dRowAudio\dRowAudio.c"
# End Source File
# Begin Source File
SOURCE="..\..\..\..\dRowAudio\dRowAudio.cpp"
# End Source File
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
SOURCE="..\..\JuceLibraryCode\BinaryData.cpp"
# End Source File
# Begin Source File
SOURCE="..\..\JuceLibraryCode\BinaryData.h"
# End Source File
# End Group
# End Target
# End Project
