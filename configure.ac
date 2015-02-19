AC_INIT(Embedded Building Bricks, 0.1, jan.weil@web.de, libmbb)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_RANLIB
AC_CHECK_PROG(have_ruby, ruby, yes, no)
if test x$have_ruby != xyes; then
	AC_MSG_WARN([Ruby was not found on your system, unit tests will not be compiled.])
fi
AC_CHECK_LIB(ev, ev_version_major, [have_ev=yes], [have_ev=no])
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_SYS_POSIX_TERMIOS
if test x$ac_cv_header_stdbool_h != xyes -o x$ac_cv_header_stdc != xyes; then
	AC_MSG_WARN([Your system lacks one of the standard types header.])
	AC_MSG_WARN([Please define MBB_SYSTEM_TYPES_HEADER to make standard types available.])
	AC_MSG_WARN([Have a look at mbb/types.h.])
fi
AM_CONDITIONAL([HAVE_TERMIOSH], [test x$ac_cv_sys_posix_termios = xyes])
AM_CONDITIONAL([HAVE_LIBEV], [test x$have_ev = xyes])
AM_CONDITIONAL([HAVE_RUBY], [test x$have_ruby = xyes])
AC_CONFIG_FILES([
	Makefile
	mbb/Makefile
	examples/Makefile
	tests/Makefile
	tools/Makefile
])
AC_OUTPUT
