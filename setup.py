from distutils.core import setup, Extension

module = Extension("myModule", sources = ["compressExtension.c"])

setup(name="PackageName",
		version = "1.0",
		description="This is a package for myModule",
		ext_modules = [module])

