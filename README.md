# SemVer [![Build Status](https://travis-ci.org/progician/SemVer.svg?branch=master)](https://travis-ci.org/progician/SemVer)

A simple C++ library for handling semantic version numbers.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* CMake 3.13+
* Python 3 (optional for testing)
* Conan (optional to get dependencies)
* Catch2 2.6.0+ (you can install it manually, just make sure CMake will know about it)

### Installing

A step by step series of examples that tell you how to get a development env running

Check out the source code:

```
$ git clone https://github.com/progician/SemVer.git
$ cd SemVer
```

Create a virtual environment with Python 3 and install conan and pytest

```
$ python -m virtualenv -p python3 env
$ source env/bin/activate
$ pip install conan pytest
```

First time building, you can use conan the configure and build the project.

```
$ conan install -if build .
$ conan build -bf build . 
```

When iterating through the development I suggest just use your build tool, `make` or `ninja` for example

```
$ ninja -C build
```


## Running the tests

The project has two set of tests, the unit test suite and the external test for the semver-cli command using PyTest. The unit test suite runs as part of the build which means that any failure in the unit tests will result in a build failure outright. For the command line program testing, you can use the build system very simply: 

```
$ ninja -C build test
```