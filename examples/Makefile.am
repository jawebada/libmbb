bin_PROGRAMS = debugging 
if HAVE_TERMIOSH
bin_PROGRAMS += pelican
if HAVE_LIBEV
bin_PROGRAMS += monostable
endif
endif
AM_CPPFLAGS = -I..
LDADD = ../mbb/libmbb.a
monostable_LDADD = -lev $(LDADD)
