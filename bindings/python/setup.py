import os
import sys

from setuptools import find_packages, setup, Extension

import pybind11


include_dirs = [pybind11.get_include()]
library_dirs = []

def _get_long_description():
    readme_path = "README.md"
    with open(readme_path) as readme_file:
        return readme_file.read()

def _maybe_add_library_root(lib_name, header_only=False):
  if "%s_ROOT" % lib_name in os.environ:
    root = os.environ["%s_ROOT" % lib_name]
    include_dirs.append("%s/include" % root)
    if not header_only:
      lib_dir = "%s/lib64" % root
      if not os.path.isdir(lib_dir):
        lib_dir = "%s/lib" % root
      library_dirs.append(lib_dir)

_maybe_add_library_root("TOKENIZER")

cflags = ["-std=c++11", "-fvisibility=hidden"]
if sys.platform == "darwin":
    cflags.append("-mmacosx-version-min=10.9")

ldflags = []
if sys.platform == "darwin":
    ldflags.append("-Wl,-rpath,/usr/local/lib")

tokenizer_module = Extension(
    "pyonmttok._ext",
    sources=["pyonmttok/Python.cc"],
    extra_compile_args=cflags,
    extra_link_args=ldflags,
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=["OpenNMTTokenizer"])

setup(
    name="pyonmttok",
    version="1.23.0",
    license="MIT",
    description="OpenNMT tokenization library",
    long_description=_get_long_description(),
    long_description_content_type="text/markdown",
    author="OpenNMT",
    author_email="guillaume.klein@systrangroup.com",
    url="http://opennmt.net",
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3 :: Only",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Topic :: Text Processing :: Linguistic",
        "Topic :: Software Development :: Libraries :: Python Modules"
    ],
    project_urls={
        "Forum": "http://forum.opennmt.net/",
        "Source": "https://github.com/OpenNMT/Tokenizer/"
    },
    packages=find_packages(),
    python_requires=">=3.5",
    setup_requires=["pytest-runner"],
    tests_require=["pytest"],
    ext_modules=[tokenizer_module]
)
