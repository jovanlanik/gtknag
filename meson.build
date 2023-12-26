# gtknag
# Copyright (c) 2023 Jovan Lanik

project(
  'gtknag', 'c',
  license : 'GPLv3',
  default_options : ['c_std=c11'],
)

gtk = dependency('gtk+-3.0')
gtk_layer_shell = dependency('gtk-layer-shell-0')

dependencies = [ gtk, gtk_layer_shell ]

gtknag_sources = files([
  'gtknag.c',
  'source.c',
  'util.c',
])

executable(
  meson.project_name(),
  gtknag_sources,
  include_directories : include_directories('include'),
  dependencies : dependencies,
  install : true,
)
