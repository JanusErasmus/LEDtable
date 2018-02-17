# ------------------------------------------------
# Generic STM32 Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
#   2017-11-09 - KSES version
# ------------------------------------------------

include source.mk
include ../ecos/ecos_install/include/pkgconf/ecos.mak


######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -Os

FLOAT-ABI = -mfloat-abi=soft

# firmware library path
PERIFLIB_PATH = 

######################################
# firmware library
######################################
PERIFLIB_SOURCES = 


#######################################
# binaries
#######################################
PREFIX = $(ECOS_COMMAND_PREFIX)
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi

# mcu
MCU =

# macros for gcc
# AS defines
AS_DEFS = 


# AS includes
AS_INCLUDES = 

# compile gcc flags
ASFLAGS = $(AS_DEFS) $(AS_INCLUDES)

C_INCLUDES += -I../ecos/ecos_install/include

CFLAGS = $(C_DEFS) $(C_INCLUDES)  $(filter-out -Woverloaded-virtual -fno-rtti,$(ECOS_GLOBAL_CFLAGS))
CCFLAGS = $(C_DEFS) $(C_INCLUDES) $(filter-out -Wstrict-prototypes,$(ECOS_GLOBAL_CFLAGS))

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" 
CCFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" 

#######################################
# LDFLAGS
#######################################
LIBDIR += -L../ecos/ecos_install/lib/
LDSCRIPT = target.ld
LDFLAGS =  $(LIBDIR) $(LIBS) $(ECOS_GLOBAL_LDFLAGS) -T$(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of CPP objects
CC_OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CC_SOURCES:.cc=.o)))
vpath %.cc $(sort $(dir $(CC_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CC_OBJECTS:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CC_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

#-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst))

# default action: build all
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.cc
	@echo 'CXX $@'
	@$(CXX) -c $(CCFLAGS)  $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@echo 'CXX $@'
	@$(CXX) -c $(CCFLAGS)  $< -o $@

$(BUILD_DIR)/%.o: %.c
	@echo 'CC  $@'
	@$(CC) -c $(CFLAGS)  $< -o $@

$(BUILD_DIR)/%.o: %.s
	@echo 'ASM $@'
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) 
	@echo 'ELF $@'
	@$(CXX)  -o $@ $(OBJECTS) $(LDFLAGS)
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@echo 'HEX $@'
	@$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo 'BIN $@'
	@$(BIN) $< $@
	@cp $@ .
	
$(BUILD_DIR):
	mkdir $@	


#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)  

# *** EOF ***
