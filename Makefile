include ../Build/Common/CommonDefs.mak

BIN_DIR = ../Bin

INC_DIRS = ../../Include =/usr/include/ni
INC_DIRS += /usr/include/c++/4.7
INC_DIRS += /usr/include/x86_64-linux-gnu/c++/4.7

SRC_FILES = ./*.cpp

EXE_NAME = API
USED_LIBS = OpenNI

LIB_DIRS += ../../Lib
include ../Build/Common/CommonCppMakefile

