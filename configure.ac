AC_INIT([libmbb], [0.1], [jan.weil@web.de])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_RANLIB
AC_CHECK_PROG(have_ruby, ruby, yes, no)
AC_CHECK_LIB(ev, ev_version_major, [have_ev=yes], [have_ev=no])
AM_CONDITIONAL([HAVE_LIBEV], [test x$have_ev = xyes])
AM_CONDITIONAL([HAVE_RUBY], [test x$have_ruby = xyes])
AC_CONFIG_FILES([
	Makefile
	mbb/Makefile
	examples/Makefile
	tests/Makefile
])
AC_OUTPUT
