# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "DROWAUDIO_UNIT_TESTS=1" -D "JUCER_LINUX_MAKE_1C83C68C=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../../../Work -I ../../../../../JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -Wall
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lcurl

  TARGET := dRowAudio\ Demo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "DROWAUDIO_UNIT_TESTS=1" -D "JUCER_LINUX_MAKE_1C83C68C=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../../../Work -I ../../../../../JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os -Wall
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lcurl

  TARGET := dRowAudio\ Demo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/UnityBuild0_44603638.o \
  $(OBJDIR)/UnityBuild1_446e4db9.o \
  $(OBJDIR)/UnityBuild2_447c653a.o \
  $(OBJDIR)/UnityBuild3_448a7cbb.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/dRowAudio_77fd40e6.o \
  $(OBJDIR)/juce_audio_basics_89a6632b.o \
  $(OBJDIR)/juce_audio_devices_3f459929.o \
  $(OBJDIR)/juce_audio_formats_6dc6e2c9.o \
  $(OBJDIR)/juce_audio_processors_eb06e1e3.o \
  $(OBJDIR)/juce_audio_utils_9b3b2d69.o \
  $(OBJDIR)/juce_box2d_68aa8089.o \
  $(OBJDIR)/juce_core_a278a20d.o \
  $(OBJDIR)/juce_cryptography_8b2b6667.o \
  $(OBJDIR)/juce_data_structures_c98de09.o \
  $(OBJDIR)/juce_events_afa14001.o \
  $(OBJDIR)/juce_graphics_4cb436e5.o \
  $(OBJDIR)/juce_gui_basics_d5c7a3e1.o \
  $(OBJDIR)/juce_gui_extra_13d359a9.o \
  $(OBJDIR)/juce_opengl_6fe9682d.o \
  $(OBJDIR)/juce_tracktion_marketplace_305e0fc9.o \
  $(OBJDIR)/juce_video_72be35c9.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking dRowAudio Demo
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning dRowAudio Demo
	@$(CLEANCMD)

strip:
	@echo Stripping dRowAudio Demo
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/UnityBuild0_44603638.o: ../../Source/UnityBuild0.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UnityBuild0.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UnityBuild1_446e4db9.o: ../../Source/UnityBuild1.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UnityBuild1.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UnityBuild2_447c653a.o: ../../Source/UnityBuild2.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UnityBuild2.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UnityBuild3_448a7cbb.o: ../../Source/UnityBuild3.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UnityBuild3.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_77fd40e6.o: ../../../../dRowAudio/dRowAudio.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_89a6632b.o: ../../../../../JUCE/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_3f459929.o: ../../../../../JUCE/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_6dc6e2c9.o: ../../../../../JUCE/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_eb06e1e3.o: ../../../../../JUCE/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_9b3b2d69.o: ../../../../../JUCE/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_box2d_68aa8089.o: ../../../../../JUCE/modules/juce_box2d/juce_box2d.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_box2d.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_a278a20d.o: ../../../../../JUCE/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_8b2b6667.o: ../../../../../JUCE/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_c98de09.o: ../../../../../JUCE/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_afa14001.o: ../../../../../JUCE/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_4cb436e5.o: ../../../../../JUCE/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_d5c7a3e1.o: ../../../../../JUCE/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_13d359a9.o: ../../../../../JUCE/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_6fe9682d.o: ../../../../../JUCE/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_tracktion_marketplace_305e0fc9.o: ../../../../../JUCE/modules/juce_tracktion_marketplace/juce_tracktion_marketplace.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_tracktion_marketplace.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_72be35c9.o: ../../../../../JUCE/modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
