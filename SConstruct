import os.path

env = Environment(CPPPATH='.', LIBPATH='mbb', CCFLAGS="-Wall")
conf = Configure(env)

libev_available = conf.CheckLibWithHeader('ev', 'ev.h', 'c')

libmbb = Glob('mbb/*.c', True, False, True)
if not libev_available: libmbb.remove('mbb/timer_ev.c')
env.Library('mbb/mbb', libmbb)

termiosh_available = conf.CheckHeader('termios.h')

if termiosh_available:
	env.Program('examples/pelican.c', LIBS=['mbb'])
	if libev_available:
		env.Program('examples/monostable.c', LIBS=['mbb', 'ev'])

env['BUILDERS']['TestBuilder'] = Builder(action = 'tools/munt_main $SOURCE > $TARGET')

tests = Glob('tests/*.c', True, False, True)

test_aliases = []

for test in tests:
	test_main = "tests/build/" + os.path.basename(test.replace(".c", "_main.c"))
	test_out = test.replace(".c", "")
	env.TestBuilder(test_main, test)
	p = env.Program(test_out, [test_main], LIBS=['mbb'])
	a = Alias(test, [p], p[0].path)
	AlwaysBuild(a)
	test_aliases.append(a)

env.Clean(tests, "tests/build")

env.Alias('test', test_aliases)
