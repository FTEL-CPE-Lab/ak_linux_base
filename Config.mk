# edit these lines for cross flatform
CROSS_PREFIX = 
PREFIX = /mnt/local
CC = $(CROSS_PREFIX)gcc
AR = $(CROSS_PREFIX)ar

# input version compiler
VERSION = 1
STD = gnu11

# input DIR
ROOT_DIR = $(shell pwd)
OBJ_DIR = build
HEADER_DIR = ak
EXAMPLE_DIR = example
SRC_DIR = src

# input FLAGS
LDFLAGS += -lpthread -lrt
CFLAGS += -I./ -std=$(STD) -fPIC -finline-functions -w -pipe -fPIC -fwrapv -fdata-sections -ffunction-sections -O2
LDLIBS += -shared -Wl,-soname,$(NAME_SHARED_MODULE).$(VERSION)
