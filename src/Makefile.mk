-include Config.mk

NAME_SHARED_MODULE = libak.so
NAME_STATIC_MODULE = libak.a
CFLAGS	+= -I./$(SRC_DIR)

VPATH += ./$(SRC_DIR)

OBJ += $(OBJ_DIR)/ak.o
OBJ += $(OBJ_DIR)/message.o
OBJ += $(OBJ_DIR)/timer.o
OBJ += $(OBJ_DIR)/fsm.o
OBJ += $(OBJ_DIR)/tsm.o
OBJ += $(OBJ_DIR)/sys_dbg.o

all: $(OBJ_DIR)/$(NAME_SHARED_MODULE).$(VERSION) $(OBJ_DIR)/$(NAME_STATIC_MODULE)

$(OBJ_DIR)/%.o: %.c
	@echo CC 	$<
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/$(NAME_SHARED_MODULE).$(VERSION): $(OBJ)
	@echo ---------- START BUILD STATIC LIBRARY ----------
	@echo CC 	$^ -o $@
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/$(NAME_STATIC_MODULE): $(OBJ)
	@echo ---------- START BUILD SHARED LIBRARY ----------
	@echo AR 	$^ rcs $@ 
	@$(AR) rcs $@ $^

.PHONY: clean
clean:
	@echo rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

.PHONY: install
install:
	@echo install -D $(HEADER_DIR)/*.h $(PREFIX)/include
	@echo install -D $(OBJ_DIR)/$(NAME_SHARED_MODULE).$(VERSION) $(PREFIX)/lib
	@echo install -D $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(PREFIX)/lib
	@install -D $(HEADER_DIR)/*.h $(PREFIX)/include/$(HEADER_DIR)
	@install -D $(OBJ_DIR)/$(NAME_SHARED_MODULE).$(VERSION) $(PREFIX)/lib
	@install -D $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(PREFIX)/lib
	@cd $(PREFIX)/lib && ln -sf $(NAME_SHARED_MODULE).$(VERSION) $(NAME_SHARED_MODULE)