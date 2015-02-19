noinst_PROGRAMS = debugging 
if HAVE_TERMIOSH
noinst_PROGRAMS += pelican
if HAVE_LIBEV
noinst_PROGRAMS += monostable
endif
endif
AM_CPPFLAGS = -I..
LDADD = ../mbb/libmbb.a
monostable_LDADD = -lev $(LDADD)
