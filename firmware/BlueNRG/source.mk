######################################
# target
######################################
TARGET = BlueNRG


# Build path
BUILD_DIR = release

######################################
# source
######################################
# C sources
CC_SOURCES = $(wildcard src/*.cpp)
CC_SOURCES += $(wildcard src/*.cc)

C_SOURCES = $(wildcard src/*.c)
C_SOURCES += $(wildcard src/controller/*.c)
C_SOURCES += $(wildcard src/hci/*.c)
C_SOURCES += $(wildcard src/utils/*.c)

# C defines
#C_DEFS

# C includes
C_INCLUDES =  -I./include -I../caboodle/include
