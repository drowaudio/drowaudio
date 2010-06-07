# Automatically generated makefile, created by the Jucer
# Don't edit this file! Your changes will be overwritten when you re-save the Jucer project!

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
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -I "/usr/include" -I "/usr/include/freetype2"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib/" -L"../../../../../juce_source/juce/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound -ljuce 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -I "/usr/include" -I "/usr/include/freetype2"
  TARGET := Decks
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -I "/usr/include" -I "/usr/include/freetype2"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib/" -L"../../../../../juce_source/juce/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound -ljuce 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -I "/usr/include" -I "/usr/include/freetype2"
  TARGET := Decks
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/ApplicationStartup.o \
  $(OBJDIR)/Settings.o \
  $(OBJDIR)/DeckManager.o \
  $(OBJDIR)/AudioEngine.o \
  $(OBJDIR)/DecksLookAndFeel.o \
  $(OBJDIR)/MainAppWindow.o \
  $(OBJDIR)/ContainerComponent.o \
  $(OBJDIR)/ControlComponent.o \
  $(OBJDIR)/Transport.o \
  $(OBJDIR)/DeckTransport.o \
  $(OBJDIR)/TrackInfo.o \
  $(OBJDIR)/DraggableDisplay.o \
  $(OBJDIR)/Mixer.o \
  $(OBJDIR)/MixerChannelStrip.o \
  $(OBJDIR)/CrossFader.o \
  $(OBJDIR)/MasterChannelStrip.o \
  $(OBJDIR)/dRowAudio_SegmentedMeter.o \
  $(OBJDIR)/SettingsComponent.o \
  $(OBJDIR)/MixerSettingsComponent.o \
  $(OBJDIR)/dRowAudio.o \
  $(OBJDIR)/dRowAudio.o \
  $(OBJDIR)/BinaryData.o \

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

$(OBJDIR)/ApplicationStartup.o: ../../Source/main/ApplicationStartup.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Settings.o: ../../Source/main/Settings.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DeckManager.o: ../../Source/main/DeckManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioEngine.o: ../../Source/main/AudioEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DecksLookAndFeel.o: ../../Source/ui/DecksLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainAppWindow.o: ../../Source/ui/MainAppWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ContainerComponent.o: ../../Source/ui/ContainerComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ControlComponent.o: ../../Source/ui/controls/ControlComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Transport.o: ../../Source/ui/controls/transport/Transport.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DeckTransport.o: ../../Source/ui/controls/transport/DeckTransport.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackInfo.o: ../../Source/ui/controls/transport/TrackInfo.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DraggableDisplay.o: ../../Source/ui/controls/draggable_display/DraggableDisplay.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Mixer.o: ../../Source/ui/controls/mixer/Mixer.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MixerChannelStrip.o: ../../Source/ui/controls/mixer/MixerChannelStrip.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CrossFader.o: ../../Source/ui/controls/mixer/CrossFader.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MasterChannelStrip.o: ../../Source/ui/controls/mixer/MasterChannelStrip.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_SegmentedMeter.o: ../../Source/ui/controls/mixer/dRowAudio_SegmentedMeter.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SettingsComponent.o: ../../Source/ui/settings/SettingsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MixerSettingsComponent.o: ../../Source/ui/settings/MixerSettingsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio.o: ../../../../dRowAudio/dRowAudio.c
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio.o: ../../../../dRowAudio/dRowAudio.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
