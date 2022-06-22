PREFIX:=/mnt/local
CC:=gcc
ROOT_DIR=$(shell pwd)
OBJ_DIR=build
HEADER_DIR=ak
EXAMPLE_DIR=example
LDFLAGS += -lm -lpthread -lrt
CFLAGS	+= -I./ -std=gnu99 -fPIC -finline-functions -pipe -fPIC -fwrapv -Wl,--gc-sections -Wl,--print-gc-sections -fdata-sections -ffunction-sections -O2
LDLIBS += -shared -Wl,-soname,$(NAME_MODULE)
VERSION=1