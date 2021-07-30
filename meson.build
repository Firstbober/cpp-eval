project('cpp-eval', 'cpp',
	version: '1.0.0',
	default_options: [
		'cpp_std=c++2a',
		'build.cpp_std=c++2a'
	])

deps = [
	meson.get_compiler('cpp').find_library('dl', required: true)
]

sources = [
	'src/main.cc'
]

prog_python = import('python').find_installation('python3')
eval_gen = generator(prog_python,
                output  : '@BASENAME@.cc',
                arguments : [meson.source_root() + '/cpp-eval.py', '@INPUT@', '@OUTPUT@'])

generated_sources = eval_gen.process(sources)

executable(
	'cpp-eval',
	generated_sources,
	dependencies: deps,
	include_directories: include_directories('src'),
	install: true,
)