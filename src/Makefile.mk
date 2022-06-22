
CFLAGS	+= -I./src

VPATH += ./src

OBJ += $(OBJ_DIR)/ak.o
OBJ += $(OBJ_DIR)/message.o
OBJ += $(OBJ_DIR)/timer.o
OBJ += $(OBJ_DIR)/fsm.o
OBJ += $(OBJ_DIR)/tsm.o
OBJ += $(OBJ_DIR)/sys_dbg.o