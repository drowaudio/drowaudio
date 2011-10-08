# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib/" -L"../../../../../../juce_source/juce/modules/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  TARGET := Decks
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib/" -L"../../../../../../juce_source/juce/modules/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  TARGET := Decks
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/ApplicationStartup_3272e644.o \
  $(OBJDIR)/Settings_1a9966da.o \
  $(OBJDIR)/dRowAudio_OverflowingAbstractFifo_27f57822.o \
  $(OBJDIR)/dRowAudio_SampleRateConverter_2c829ea.o \
  $(OBJDIR)/DecksAudioFormatManager_c4d4a1fd.o \
  $(OBJDIR)/AudioEngine_c12c07ab.o \
  $(OBJDIR)/DeckManager_dbb2a2b7.o \
  $(OBJDIR)/DecksColours_f3e45819.o \
  $(OBJDIR)/DecksLookAndFeel_6536b270.o \
  $(OBJDIR)/MainAppWindow_b0560b46.o \
  $(OBJDIR)/ContainerComponent_d4c380f8.o \
  $(OBJDIR)/TitleBarComponent_240eab0.o \
  $(OBJDIR)/QuitScreen_c63f3697.o \
  $(OBJDIR)/ControlComponent_abefc827.o \
  $(OBJDIR)/Transport_fb740666.o \
  $(OBJDIR)/DeckTransport_f4e0103d.o \
  $(OBJDIR)/TrackInfo_bd3403d6.o \
  $(OBJDIR)/ClickableLoopPointComponent_c4cc3d0.o \
  $(OBJDIR)/ClickableCuePointComponent_5e60a74b.o \
  $(OBJDIR)/LoopAndCuePoints_1e85ecd0.o \
  $(OBJDIR)/LoopPointList_7c6db6c7.o \
  $(OBJDIR)/LoopPointListBoxModel_70e05.o \
  $(OBJDIR)/CuePointList_9e758088.o \
  $(OBJDIR)/CuePointListBoxModel_adcd86c6.o \
  $(OBJDIR)/TrackSuggestions_83d5f51.o \
  $(OBJDIR)/MixTypeComponent_5b7bf314.o \
  $(OBJDIR)/TrackSuggestionTable_623f988c.o \
  $(OBJDIR)/DraggableDisplay_69d2841b.o \
  $(OBJDIR)/LoopPointDisplayComponent_4491dcab.o \
  $(OBJDIR)/CuePointDisplayComponent_b39202be.o \
  $(OBJDIR)/dRowAudio_CompleteDraggableWaveDisplay_fd3fb73a.o \
  $(OBJDIR)/dRowAudio_CompleteColouredDraggableWaveDisplay_9d54b8c5.o \
  $(OBJDIR)/Mixer_7f8f6326.o \
  $(OBJDIR)/MixerChannelStrip_64f8d31b.o \
  $(OBJDIR)/CrossFader_56140e3.o \
  $(OBJDIR)/MasterChannelStrip_276542a4.o \
  $(OBJDIR)/Library_dab40b0a.o \
  $(OBJDIR)/PrepareTable_3aa12962.o \
  $(OBJDIR)/SettingsComponent_72d2086a.o \
  $(OBJDIR)/MixerSettingsComponent_8a33bf8b.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/dRowAudio_66a49a7e.o \
  $(OBJDIR)/juce_audio_basics_15b0217e.o \
  $(OBJDIR)/juce_audio_devices_efd90ebc.o \
  $(OBJDIR)/juce_audio_formats_1e5a585c.o \
  $(OBJDIR)/juce_audio_processors_3aa29d36.o \
  $(OBJDIR)/juce_audio_utils_b936647c.o \
  $(OBJDIR)/juce_core_b5776660.o \
  $(OBJDIR)/juce_data_structures_b481521c.o \
  $(OBJDIR)/juce_events_e22fa2d4.o \
  $(OBJDIR)/juce_graphics_f8def838.o \
  $(OBJDIR)/juce_gui_basics_13b83b4.o \
  $(OBJDIR)/juce_gui_extra_6053123c.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking Decks
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Decks
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

$(OBJDIR)/ApplicationStartup_3272e644.o: ../../Source/main/ApplicationStartup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ApplicationStartup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Settings_1a9966da.o: ../../Source/main/Settings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Settings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_OverflowingAbstractFifo_27f57822.o: ../../Source/main/dRowAudio_OverflowingAbstractFifo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio_OverflowingAbstractFifo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_SampleRateConverter_2c829ea.o: ../../Source/main/dRowAudio_SampleRateConverter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio_SampleRateConverter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DecksAudioFormatManager_c4d4a1fd.o: ../../Source/main/DecksAudioFormatManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DecksAudioFormatManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioEngine_c12c07ab.o: ../../Source/main/AudioEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DeckManager_dbb2a2b7.o: ../../Source/main/DeckManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DeckManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DecksColours_f3e45819.o: ../../Source/ui/DecksColours.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DecksColours.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DecksLookAndFeel_6536b270.o: ../../Source/ui/DecksLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DecksLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainAppWindow_b0560b46.o: ../../Source/ui/MainAppWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainAppWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ContainerComponent_d4c380f8.o: ../../Source/ui/ContainerComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ContainerComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TitleBarComponent_240eab0.o: ../../Source/ui/TitleBarComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TitleBarComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/QuitScreen_c63f3697.o: ../../Source/ui/QuitScreen.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling QuitScreen.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ControlComponent_abefc827.o: ../../Source/ui/controls/ControlComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ControlComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Transport_fb740666.o: ../../Source/ui/controls/transport/Transport.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Transport.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DeckTransport_f4e0103d.o: ../../Source/ui/controls/transport/DeckTransport.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DeckTransport.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackInfo_bd3403d6.o: ../../Source/ui/controls/transport/TrackInfo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackInfo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ClickableLoopPointComponent_c4cc3d0.o: ../../Source/ui/controls/transport/ClickableLoopPointComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ClickableLoopPointComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ClickableCuePointComponent_5e60a74b.o: ../../Source/ui/controls/transport/ClickableCuePointComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ClickableCuePointComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoopAndCuePoints_1e85ecd0.o: ../../Source/ui/controls/transport/LoopAndCuePoints.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoopAndCuePoints.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoopPointList_7c6db6c7.o: ../../Source/ui/controls/transport/LoopPointList.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoopPointList.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoopPointListBoxModel_70e05.o: ../../Source/ui/controls/transport/LoopPointListBoxModel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoopPointListBoxModel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CuePointList_9e758088.o: ../../Source/ui/controls/transport/CuePointList.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CuePointList.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CuePointListBoxModel_adcd86c6.o: ../../Source/ui/controls/transport/CuePointListBoxModel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CuePointListBoxModel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackSuggestions_83d5f51.o: ../../Source/ui/controls/transport/TrackSuggestions.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackSuggestions.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MixTypeComponent_5b7bf314.o: ../../Source/ui/controls/transport/MixTypeComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MixTypeComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackSuggestionTable_623f988c.o: ../../Source/ui/controls/transport/TrackSuggestionTable.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackSuggestionTable.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DraggableDisplay_69d2841b.o: ../../Source/ui/controls/draggable_display/DraggableDisplay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DraggableDisplay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoopPointDisplayComponent_4491dcab.o: ../../Source/ui/controls/draggable_display/LoopPointDisplayComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoopPointDisplayComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CuePointDisplayComponent_b39202be.o: ../../Source/ui/controls/draggable_display/CuePointDisplayComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CuePointDisplayComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_CompleteDraggableWaveDisplay_fd3fb73a.o: ../../Source/ui/controls/draggable_display/dRowAudio_CompleteDraggableWaveDisplay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio_CompleteDraggableWaveDisplay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_CompleteColouredDraggableWaveDisplay_9d54b8c5.o: ../../Source/ui/controls/draggable_display/dRowAudio_CompleteColouredDraggableWaveDisplay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio_CompleteColouredDraggableWaveDisplay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Mixer_7f8f6326.o: ../../Source/ui/controls/mixer/Mixer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Mixer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MixerChannelStrip_64f8d31b.o: ../../Source/ui/controls/mixer/MixerChannelStrip.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MixerChannelStrip.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CrossFader_56140e3.o: ../../Source/ui/controls/mixer/CrossFader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CrossFader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MasterChannelStrip_276542a4.o: ../../Source/ui/controls/mixer/MasterChannelStrip.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MasterChannelStrip.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Library_dab40b0a.o: ../../Source/ui/controls/library/Library.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Library.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PrepareTable_3aa12962.o: ../../Source/ui/controls/library/PrepareTable.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PrepareTable.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SettingsComponent_72d2086a.o: ../../Source/ui/settings/SettingsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SettingsComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MixerSettingsComponent_8a33bf8b.o: ../../Source/ui/settings/MixerSettingsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MixerSettingsComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_66a49a7e.o: ../../../../../../juce_source/juce/modules/dRowAudio/dRowAudio.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_15b0217e.o: ../../../../../../juce_source/juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_efd90ebc.o: ../../../../../../juce_source/juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_1e5a585c.o: ../../../../../../juce_source/juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_3aa29d36.o: ../../../../../../juce_source/juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_b936647c.o: ../../../../../../juce_source/juce/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_b5776660.o: ../../../../../../juce_source/juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_b481521c.o: ../../../../../../juce_source/juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_e22fa2d4.o: ../../../../../../juce_source/juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_f8def838.o: ../../../../../../juce_source/juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_13b83b4.o: ../../../../../../juce_source/juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_6053123c.o: ../../../../../../juce_source/juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
