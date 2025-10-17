CC := gcc
CFLAGS := -O2 -Wall -Wextra -std=c11
LDFLAGS :=          # flags passed before objects (e.g. -L...)
LDLIBS := -lcrypto  # libraries passed after objects

SRCDIR := Scripts
BINDIR := binaries

SOURCES := $(wildcard $(SRCDIR)/*.c)
# only build sources that contain a `main` definition to avoid linker errors
SRCS_WITH_MAIN := $(shell grep -El "int[[:space:]]+main" $(SOURCES) 2>/dev/null || true)
OBJ := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%,$(SRCS_WITH_MAIN))

.PHONY: all clean run

all: $(BINDIR) $(OBJ)

ifeq ($(strip $(SRCS_WITH_MAIN)),)
$(warning No source files with `main` found in $(SRCDIR) - nothing to build)
endif

$(BINDIR):
	@mkdir -p "$(BINDIR)"

# build each C source into an executable inside binaries/ with the same base name
$(BINDIR)/%: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o "$@" "$<" $(LDLIBS)

clean:
	@rm -rf "$(BINDIR)"

# convenience: run a specific binary. Usage: make run NAME=<executable>
run:
	@if [ -z "$(NAME)" ]; then echo "Usage: make run NAME=<executable>"; exit 1; fi
	@./"$(BINDIR)/$(NAME)"
