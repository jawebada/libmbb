SUFFIXES = .c _main.c
.c_main.c:
	../tools/munt_main $< > $@

noinst_PROGRAMS = test_hsm test_queue
test_hsm_SOURCES = test_hsm_main.c
test_queue_SOURCES = test_queue_main.c
AM_CPPFLAGS = -I..
LDADD = ../mbb/libmbb.a
MOSTLYCLEANFILES = test_hsm_main.c test_queue_main.c
TESTS = $(noinst_PROGRAMS)
