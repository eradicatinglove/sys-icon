#---------------------------------------------------------------------------------
# pull in common atmosphere configuration
#---------------------------------------------------------------------------------
LIBAMS := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))lib/ams/libraries
include $(LIBAMS)/config/templates/stratosphere.mk

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
# EXEFS_SRC is the optional input directory containing data copied into exefs, if anything this normally should only contain "main.npdm".
#---------------------------------------------------------------------------------
VERSION     :=  4.0.0
TARGET		:=	sys-icon
BUILD		:=	build
OUTDIR		:=	out
RESOURCES	:=	res
SOURCES		+=	src
INCLUDES	+=	src lib/ams/libraries/libstratosphere/include lib/ams/libraries/libvapours/include
DEFINES		+=	-DTARGET="\"$(TARGET)\"" -DVERSION="\"$(VERSION)\""

# All features always enabled
DEFINES		+=	-DHAVE_NSAM_CONTROL -DHAVE_NSRO_CONTROL -DENABLE_LOGGING

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(OUTDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)
export APP_JSON	:=	$(TOPDIR)/$(RESOURCES)/sysmodule.json

CFILES		:=	$(foreach dir,$(SOURCES),$(filter-out $(notdir $(wildcard $(dir)/*.arch.*.c)) $(notdir $(wildcard $(dir)/*.board.*.c)) $(notdir $(wildcard $(dir)/*.os.*.c)), \
					$(notdir $(wildcard $(dir)/*.c))))
CFILES		+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.arch.$(ATMOSPHERE_ARCH_NAME).c)))
CFILES		+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.board.$(ATMOSPHERE_BOARD_NAME).c)))
CFILES		+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.os.$(ATMOSPHERE_OS_NAME).c)))

CPPFILES	:=	$(foreach dir,$(SOURCES),$(filter-out $(notdir $(wildcard $(dir)/*.arch.*.cpp)) $(notdir $(wildcard $(dir)/*.board.*.cpp)) $(notdir $(wildcard $(dir)/*.os.*.cpp)), \
					$(notdir $(wildcard $(dir)/*.cpp))))
CPPFILES	+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.arch.$(ATMOSPHERE_ARCH_NAME).cpp)))
CPPFILES	+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.board.$(ATMOSPHERE_BOARD_NAME).cpp)))
CPPFILES	+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.os.$(ATMOSPHERE_OS_NAME).cpp)))

SFILES		:=	$(foreach dir,$(SOURCES),$(filter-out $(notdir $(wildcard $(dir)/*.arch.*.s)) $(notdir $(wildcard $(dir)/*.board.*.s)) $(notdir $(wildcard $(dir)/*.os.*.s)), \
				$(notdir $(wildcard $(dir)/*.s))))
SFILES		+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.arch.$(ATMOSPHERE_ARCH_NAME).s)))
SFILES		+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.board.$(ATMOSPHERE_BOARD_NAME).s)))
SFILES		+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.os.$(ATMOSPHERE_OS_NAME).s)))

BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
export LIBPATHS += $(foreach dir,$(AMS_LIBDIRS),-L$(dir)/lib/nintendo_nx_arm64_armv8a/release)

export BUILD_EXEFS_SRC := $(TOPDIR)/$(EXEFS_SRC)

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@[ -d $(OUTDIR) ] || mkdir -p $(OUTDIR)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@mkdir -p out/atmosphere/contents/00FF69636F6EFF00/flags/
	@touch out/atmosphere/contents/00FF69636F6EFF00/flags/boot2.flag
	@cp out/$(TARGET).nsp out/atmosphere/contents/00FF69636F6EFF00/exefs.nsp
	@echo "{" > toolbox.json
	@echo "    \"name\": \"$(TARGET)\"," >> toolbox.json
	@echo "    \"tid\": \"00FF69636F6EFF00\"," >> toolbox.json
	@echo "    \"requires_reboot\": true," >> toolbox.json
	@echo "    \"version\": \"$(VERSION)\"" >> toolbox.json
	@echo "}" >> toolbox.json
	@mv toolbox.json out/atmosphere/contents/00FF69636F6EFF00/toolbox.json

libstrato:
	@$(MAKE) -C $(LIBAMS)/libstratosphere

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).kip $(TARGET).nsp $(TARGET).npdm $(TARGET).nso $(TARGET).elf $(OUTDIR)

mrproper: clean
	@$(MAKE) -C $(LIBAMS)/libstratosphere clean


#---------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all:	$(OUTPUT).nsp

$(OUTPUT).elf:	$(OFILES)

$(OUTPUT).nsp:	$(OUTPUT).nso $(OUTPUT).npdm

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
