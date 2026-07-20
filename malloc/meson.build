project(
  'malloc',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

sources = [
  'src/main.c',
  'src/test.c',
  'src/malloc.c',
]

exe = executable(
  'malloc',
  sources,
  install : true,
)

test('basic', exe)
