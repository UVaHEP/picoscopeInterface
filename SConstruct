import os 

env = Environment()
include = Dir('#/include')
picoscopeInclude = Dir('/opt/picoscope/include')
env = Environment(LIBS=['ps6000'], CCFLAGS='-std=c++11 -Wl,rpath=./', CPPPATH=[include, picoscopeInclude], LIBPATH=['/opt/picoscope/lib', './lib'])

Export('env')

SConscript(['src/SConscript'])
#SConscript(['test/SConscript'])








