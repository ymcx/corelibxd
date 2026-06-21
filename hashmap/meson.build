project(
  'hashmap',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

cc = meson.get_compiler('c')
m_dep = cc.find_library('m')

sources = [
  'src/hashmap.c',
  'src/main.c',
  'src/test.c',
]

exe = executable(
  'hashmap',
  sources,
  dependencies : m_dep,
  install : true,
)

test('basic', exe)
