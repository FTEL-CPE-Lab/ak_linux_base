-include config.mk
-include src/Makefile.mk

NAME_MODULE=libak.so

all: create $(OBJ_DIR)/$(NAME_MODULE) example

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@echo CC $<
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CC) -o $@ $^ $(CFLAGS)
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

.PHONY: example
example:
	@make -f $(EXAMPLE_DIR)/Makefile all

.PHONY: clean
clean:
	@echo rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)
	@make -f $(EXAMPLE_DIR)/Makefile clean

.PHONY: install
install: all
	@echo install -D $(HEADER_DIR)/*.h $(PREFIX)/include
	@echo install -d $(OBJ_DIR)/$(NAME_MODULE) $(PREFIX)/lib
	@install -d $(PREFIX)/include/$(HEADER_DIR)
	@install -d $(PREFIX)/lib
	@install -D $(HEADER_DIR)/*.h $(PREFIX)/include/$(HEADER_DIR)
	@install -D $(OBJ_DIR)/$(NAME_MODULE) $(PREFIX)/lib
	@cd $(PREFIX)/lib && ln -sf $(NAME_MODULE) $(NAME_MODULE).$(VERSION)