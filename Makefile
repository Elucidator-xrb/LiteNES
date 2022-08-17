CC      := /home/lxb/buildroot-2020.02.6/output/host/bin/mipsel-buildroot-linux-musl-gcc
CFLAGS  := -MMD -g -I./include -Wall -Werror --sysroot /home/lxb/buildroot-2020.02.6/output/staging
LDFLAGS := --sysroot /home/lxb/buildroot-2020.02.6/output/staging

CFILES  := $(shell find src -name "*.c")
OBJS    := $(CFILES:src/%.c=build/%.o)

build/%.o: src/%.c
	@echo + CC $< "->" $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
 
litenes: $(OBJS)
	@echo + LD "->" $@
	@$(CC) $(OBJS) $(LDFLAGS) -o litenes -Iinclude

-include $(patsubst %.o, %.d, $(OBJS))

.PHONY: clean

clean:
	rm -rf litenes build/
