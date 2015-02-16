import os.path

env = Environment(CPPPATH='.', LIBPATH='mbb')
env.Library('mbb/mbb', Glob('mbb/*.c'))

examples = Glob('examples/*.c', True, False, True)

conf = Configure(env)
if not conf.CheckHeader('termios.h'):
	examples.remove('examples/pelican.c')
	examples.remove('examples/monostable.c')

for ex in examples:
	env.Program(ex, LIBS=['mbb'])
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
