from setuptools import setup, Extension

module = Extension('range_module',
                   sources=['src/range.c'])

setup(name='range_module',
      version='1.0',
      description='This is a custom range module just for experimenting',
      ext_modules=[module])