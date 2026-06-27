project(
  'print',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

cc = meson.get_compiler('c')

dependencies = [
  cc.find_library('m')
]

sources = [
  'src/main.c',
  'src/test.c',
  'src/print.c',
]

exe = executable(
  'print',
  sources,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
