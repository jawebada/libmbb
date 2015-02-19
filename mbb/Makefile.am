lib_LIBRARIES = libmbb.a
libmbb_a_SOURCES = debug.c hsm.c timer_periodic.c
if HAVE_LIBEV
libmbb_a_SOURCES += timer_ev.c
endif
libmbb_a_CPPFLAGS = -I..
