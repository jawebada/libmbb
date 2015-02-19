noinst_PROGRAMS = debugging pelican
if HAVE_LIBEV
noinst_PROGRAMS += monostable
endif
CPPFLAGS = -I..
LDADD = ../mbb/libmbb.a
monostable_LDADD = -lev $(LDADD)
