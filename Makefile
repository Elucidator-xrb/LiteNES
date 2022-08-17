CC      := gcc
CFLAGS  := -MMD -g -I./include -Wall -Werror
LDFLAGS := -lallegro -lallegro_main -lallegro_primitives

CFILES  := $(shell find src -name "*.c")
OBJS    := $(CFILES:src/%.c=build/%.o)

build/%.o: src/%.c
	@echo + CC $< "->" $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
 
litenes: $(CFILES)
	@echo + LD "->" $@
	@$(CC) $(CFILES) $(LDFLAGS) -o litenes -Iinclude

-include $(patsubst %.o, %.d, $(OBJS))

.PHONY: clean

clean:
	rm -rf litenes build/
