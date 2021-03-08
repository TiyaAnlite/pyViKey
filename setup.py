from distutils.core import setup, Extension

module1 = Extension('pyViKey',
                    sources = ['ViKeyWrapper.cpp'])

setup (name = 'pyViKey',
       version = '1.0',
       description = 'Python uses ViKey support library',
       ext_modules = [module1])