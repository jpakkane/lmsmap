std = executable('stdmaptest', 'stdmaptest.cpp')
test('stdmap', std)

lms = executable('lmsmaptest', 'lmsmaptest.cpp',
include_directories : inc)
test('lmsmap', lms)

executable('stdperf', 'stdperf.cpp')
executable('lmsperf', 'lmsperf.cpp',
include_directories : inc)
