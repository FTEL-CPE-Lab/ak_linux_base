-include config.mk

NAME_MODULE = $(EXAMPLE_DIR)/example
NAME_STATIC_MODULE = libak.la

OBJ += 	$(EXAMPLE_DIR)/main.o \
		$(EXAMPLE_DIR)/task_list.o \
		$(EXAMPLE_DIR)/task_1.o \
		$(EXAMPLE_DIR)/task_2.o 

all: $(NAME_MODULE)

$(EXAMPLE_DIR)/%.o: %.c
	@echo ---------- START BUILD OBJECT EXAMPLE ----------
	@echo $(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK EXAMPLE ----------
	@echo $(CC) -o $@ $^ $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(CFLAGS) $(LDFLAGS)
	@$(CC) -o $@ $^ $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(CFLAGS) $(LDFLAGS)
	
clean:
	@echo rm -rf $(EXAMPLE_DIR)/$(OBJ)
	@rm -rf $(OBJ)
	@rm -rf $(NAME_MODULE)