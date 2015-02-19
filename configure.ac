AC_INIT([libmbb], [0.1], [jan.weil@web.de])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_RANLIB
AC_CHECK_LIB(ev, ev_version_major, [have_ev=true], [have_ev_false])
AM_CONDITIONAL([HAVE_LIBEV], [test x$have_ev = xtrue])
AC_CONFIG_FILES([
	Makefile
	mbb/Makefile
	examples/Makefile
])
AC_OUTPUT
