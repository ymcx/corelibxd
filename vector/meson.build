project(
  'vector',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

dependencies = [
]

sources = [
  'src/main.c',
  'src/test.c',
  'src/vector.c',
]

exe = executable(
  'vector',
  sources,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
