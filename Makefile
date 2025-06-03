MODE         ?= debug
BINARY_NAME  ?= balu

build-dir    := build
obj-dir      := $(build-dir)/$(MODE)/.obj-files
test-obj-dir := $(build-dir)/$(MODE)/.test-obj-files
binary       := $(build-dir)/$(MODE)/$(BINARY_NAME)
test         := $(build-dir)/$(MODE)/$(BINARY_NAME)-test
CC           := gcc


CFLAGS       += -Wall
CFLAGS       += -Wextra
CFLAGS       += -Wpedantic
CFLAGS       += -MMD
CFLAGS       += -std=c23

LDFLAGS      += -pthread
LDFLAGS      += -lm


ifeq ($(MODE), release)
	CFLAGS  += -O3
	CFLAGS  += -flto
	LFLAGS  += -flto
else ifeq ($(MODE), debug)
	CFLAGS  += -O0
	CFLAGS  += -g3
else
	$(error "Invalid MODE value: $(MODE). Use either 'release' or 'debug'.")
endif

CFLAGS      += -Isrc
CFLAGS      += -Isrc/common
CFLAGS      += -Isrc/config
CFLAGS      += -Isrc/networking
CFLAGS      += -Isrc/workerpool

test-CFLAGS += -IUnity/src
test-CFLAGS += -Itest/common
test-CFLAGS += -Itest/config
test-CFLAGS += -Itest/networking
test-CFLAGS += -Itest/workerpool

all: $(binary) $(test)

include src/common/Makefile
include src/config/Makefile
include src/networking/Makefile
include src/workerpool/Makefile

include test/common/Makefile
include test/config/Makefile
include test/networking/Makefile
include test/workerpool/Makefile

$(obj-dir)/main.o: src/main.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
-include $(obj-dir)/main.d

$(test-obj-dir)/test-main.o: test/test-main.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(test-CFLAGS) -c $< -o $@
-include $(test-obj-dir)/test-main.d

$(test-obj-dir)/unity.o: Unity/src/unity.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

test-objects += $(test-obj-dir)/unity.o

$(binary): $(objects) $(obj-dir)/main.o
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)

$(test): $(objects) $(test-objects) $(test-obj-dir)/test-main.o
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(objects)
	rm -f $(obj-dir)/main.o
	rm -f $(test-objects)
	rm -f $(test-obj-dir)/unity.o
	rm -f $(test-obj-dir)/test-main.o
	rm -f $(binary)
	rm -f $(test)
.PHONY: clean

distclean:
	rm -rf $(build-dir)
.PHONY: distclean
