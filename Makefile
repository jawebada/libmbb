LIBMBB = mbb/libmbb.a
LIBMBB_SOURCES = $(wildcard mbb/*.c)
LIBMBB_OBJECTS = $(patsubst %.c,%.o,$(LIBMBB_SOURCES))

$(LIBMBB): $(LIBMBB)($(LIBMBB_OBJECTS))

clean:
	rm -f $(LIBMBB)

