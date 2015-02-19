SUBDIRS = mbb examples 
if HAVE_RUBY
SUBDIRS += tests
endif
nobase_include_HEADERS = mbb/debug.h mbb/hsm.h mbb/queue.h mbb/test.h mbb/timer_common.h mbb/timer_periodic.h mbb/types.h
if HAVE_LIBEV
nobase_include_HEADERS += mbb/timer_ev.h 
endif
EXTRA_DIST = README.md LICENSE.txt docs tools examples/keyboard.inc examples/periodic.inc
