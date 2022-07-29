-include Config.mk
CFLAGS += -s -Wl,--gc-sections -Wl,--print-gc-sections

NAME_MODULE = $(EXAMPLE_DIR)/example
NAME_STATIC_MODULE = libak.la

OBJ += 	$(EXAMPLE_DIR)/main.o \
		$(EXAMPLE_DIR)/task_list.o \
		$(EXAMPLE_DIR)/task_1.o \
		$(EXAMPLE_DIR)/task_2.o 

all: $(NAME_MODULE)

$(EXAMPLE_DIR)/%.o: %.c
	@echo CC 	$^
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK EXAMPLE ----------
	@echo CC 	$^ -o $@
	@$(CC) -o $@ $^ $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(CFLAGS) $(LDFLAGS)
	
clean:
	@echo rm -rf $(EXAMPLE_DIR)/$(OBJ)
	@rm -rf $(OBJ)
	@rm -rf $(NAME_MODULE)