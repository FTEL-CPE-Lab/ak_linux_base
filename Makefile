-include Config.mk

all: create build_lib example

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

.PHONY: build_lib
build_lib:
	@make -f $(SRC_DIR)/Makefile.mk all

.PHONY: example
example:
	@make -f $(EXAMPLE_DIR)/Makefile.mk all

.PHONY: clean
clean:
	@make -f $(EXAMPLE_DIR)/Makefile.mk clean
	@make -f $(SRC_DIR)/Makefile.mk clean

.PHONY: install
install: all
	@install -d $(PREFIX)/include/$(HEADER_DIR)
	@install -d $(PREFIX)/lib
	@make -f $(SRC_DIR)/Makefile.mk install