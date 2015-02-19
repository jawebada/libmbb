SUBDIRS = mbb examples tools
if HAVE_RUBY
SUBDIRS += tests
endif
nobase_include_HEADERS = mbb/debug.h mbb/hsm.h mbb/queue.h mbb/test.h mbb/timer_common.h mbb/timer_periodic.h mbb/types.h
if HAVE_LIBEV
nobase_include_HEADERS += mbb/timer_ev.h 
endif
nobase_doc_DATA = README.md docs/Debug.md docs/HSM.md docs/Queue.md docs/Test.md docs/mbb.png examples/debugging.c examples/monostable.c examples/pelican.c tests/test_hsm.c tests/test_queue.c
EXTRA_DIST = README.md LICENSE.txt docs examples/keyboard.inc examples/periodic.inc tests/test_hsm.c tests/test_queue.c
