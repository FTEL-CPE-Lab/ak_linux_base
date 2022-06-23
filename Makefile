-include config.mk
-include src/Makefile.mk

NAME_SHARE_MODULE = libak.so
NAME_STATIC_MODULE = libak.la

all: create $(OBJ_DIR)/$(NAME_SHARE_MODULE) $(OBJ_DIR)/$(NAME_STATIC_MODULE) example

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@echo CC $<
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/$(NAME_SHARE_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS) 
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/$(NAME_STATIC_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(AR) rcs $@ $^
	@$(AR) rcs $@ $^

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
	@echo install -D $(OBJ_DIR)/$(NAME_SHARE_MODULE) $(PREFIX)/lib
	@echo install -D $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(PREFIX)/lib
	@install -d $(PREFIX)/include/$(HEADER_DIR)
	@install -d $(PREFIX)/lib
	@install -D $(HEADER_DIR)/*.h $(PREFIX)/include/$(HEADER_DIR)
	@install -D $(OBJ_DIR)/$(NAME_SHARE_MODULE) $(PREFIX)/lib
	@install -D $(OBJ_DIR)/$(NAME_STATIC_MODULE) $(PREFIX)/lib
	@cd $(PREFIX)/lib && ln -sf $(NAME_SHARE_MODULE) $(NAME_SHARE_MODULE).$(VERSION)