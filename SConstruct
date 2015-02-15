env = Environment(CPPPATH='.', LIBPATH='mbb')
env.Library('mbb/mbb', Glob('mbb/*.c'))

examples = Glob('examples/*.c', True, False, True)

conf = Configure(env)
if not conf.CheckHeader('termios.h'):
	examples.remove('examples/pelican.c')
	examples.remove('examples/monostable.c')

for ex in examples:
	env.Program(ex, LIBS=['mbb'])
