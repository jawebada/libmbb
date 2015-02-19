SUFFIXES = .c _main.c
.c_main.c:
	$(top_srcdir)/tools/munt_main $< > $@

bin_PROGRAMS = test_hsm test_queue
nodist_test_hsm_SOURCES = test_hsm_main.c
nodist_test_queue_SOURCES = test_queue_main.c
AM_CPPFLAGS = -I$(top_srcdir)
LDADD = $(top_builddir)/mbb/libmbb.a
MOSTLYCLEANFILES = test_hsm_main.c test_queue_main.c
TESTS = $(bin_PROGRAMS)
