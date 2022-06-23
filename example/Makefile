-include config.mk

NAME_MODULE = $(EXAMPLE_DIR)/example

OBJ += 	$(EXAMPLE_DIR)/main.o \
		$(EXAMPLE_DIR)/task_list.o \
		$(EXAMPLE_DIR)/task_1.o \
		$(EXAMPLE_DIR)/task_2.o 

all: $(NAME_MODULE)

$(EXAMPLE_DIR)/%.o: %.c
	@echo CC $< 
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/libak.la
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/libak.la
	
clean:
	@echo rm -rf $(EXAMPLE_DIR)/$(OBJ)
	@rm -rf $(OBJ)
	@rm -rf $(NAME_MODULE)