LIBRARY_NAME = $(notdir $(shell pwd))

ifndef ECOS_INSTALL
ECOS_INSTALL = ../ecos/ecos_install
endif

include $(ECOS_INSTALL)/include/pkgconf/ecos.mak

CROSS_COMPILE = $(ECOS_COMMAND_PREFIX)

INC = include
INC += $(ECOS_INSTALL)/include

CFLAGS = $(filter-out -Woverloaded-virtual -fno-rtti,$(ECOS_GLOBAL_CFLAGS))
CCFLAGS = $(filter-out -Wstrict-prototypes,$(ECOS_GLOBAL_CFLAGS))
LDFLAGS = $(ECOS_GLOBAL_LDFLAGS)

CC_FILES = $(wildcard src/*.cc)
CC_FILES += $(wildcard src/*.c)
CC_FILES += $(wildcard src/*.cpp)

C_OBJ_FILES = $(addprefix release$(LIBRARY_VARIANT)/,$(notdir $(CC_FILES:.c=.o)))
CC_OBJ_FILES = $(addprefix release$(LIBRARY_VARIANT)/,$(notdir $(C_OBJ_FILES:.cc=.o)))
OBJ_FILES = $(addprefix release$(LIBRARY_VARIANT)/,$(notdir $(CC_OBJ_FILES:.cpp=.o)))

INC_PARAMS=$(foreach d, $(INC), -I$d)
LIB_PARAMS=$(foreach d, $(LIB), -L$d)
LIBS_PARAMS=$(foreach d, $(LIBS), -l$d)

DEP+=$(OBJ_FILES:%.o=%.d)

all: lib$(LIBRARY_VARIANT)/lib$(LIBRARY_NAME).a

-include $(DEP) 

force: clean check release$(LIBRARY_VARIANT)/out.hex post-build

check:
	@if [ ! -d "release$(LIBRARY_VARIANT)" ]; then mkdir release$(LIBRARY_VARIANT); fi
	@if [ ! -d "lib$(LIBRARY_VARIANT)" ]; then mkdir lib$(LIBRARY_VARIANT); fi

lib$(LIBRARY_VARIANT)/lib$(LIBRARY_NAME).a: $(OBJ_FILES)
	@echo 'LL     $@'
	@mkdir -p lib$(LIBRARY_VARIANT)
	@$(CROSS_COMPILE)ar -r $@ $^ $(LIBS_PARAMS)	
	@echo 'Finished building library: $@ for $(LIBRARY_NAME)'

release$(LIBRARY_VARIANT)/%.o: src/%.c
	@mkdir -p release$(LIBRARY_VARIANT)
	@echo 'CC     $<'
	@$(CROSS_COMPILE)gcc -DECOS_SYS $(CFLAGS) $(INC_PARAMS) -MMD -MF $(patsubst %.o,%.d,$@) -c $< -o $@
	
release$(LIBRARY_VARIANT)/%.o: src/%.cpp
	@mkdir -p release$(LIBRARY_VARIANT)
	@echo 'C++    $<'
	@$(CROSS_COMPILE)g++ -DECOS_SYS $(CCFLAGS) $(INC_PARAMS) -MMD -MF $(patsubst %.o,%.d,$@) -c $< -o $@

release$(LIBRARY_VARIANT)/%.o: src/%.cc
	@mkdir -p release$(LIBRARY_VARIANT)
	@echo 'C++    $<'
	@$(CROSS_COMPILE)g++ -DECOS_SYS $(CCFLAGS) $(INC_PARAMS) -MMD -MF $(patsubst %.o,%.d,$@) -c $< -o $@

clean:
	@rm -rf release$(LIBRARY_VARIANT)
	@rm -rf lib$(LIBRARY_VARIANT)
	@echo 'Finished cleaning $(LIBRARY_NAME).bin'

ecos:
	make -C../ecos/ecos_build -fmakefile
	
   
