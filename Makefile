LIBMBB = mbb/libmbb.a
LIBMBB_SOURCES = $(wildcard mbb/*.c)
LIBMBB_OBJECTS = $(patsubst %.c,%.o,$(LIBMBB_SOURCES))

EXAMPLES_SOURCES = $(wildcard examples/*.c)
EXAMPLES = $(EXAMPLES_SOURCES:%.c=%)

CPPFLAGS ?= -I.
LDFLAGS ?= -Lmbb -lmbb

$(LIBMBB): $(LIBMBB_SOURCES) $(LIBMBB)($(LIBMBB_OBJECTS))

all: $(LIBMBB) $(EXAMPLES)

% : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS) -o $@ 

$(EXAMPLES): $(LIBMBB_SOURCES) $(EXAMPLES_SOURCES)

clean:
	rm -f $(LIBMBB)
	rm -f $(EXAMPLES)

