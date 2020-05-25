# btree

![Build](https://github.com/dbargatz/btree/workflows/Build/badge.svg)

An implementation of a B-Tree in C which closely follows the implementation
described in CLRS<sup>[1](#clrs)</sup>.

## Differences

This implementation differs from the book in a few important ways:

* Values (referred to as "satellite information" in the book, p.488) are stored
in the node alongside keys and children.
* Inserting a duplicate key overwrites the existing value; there cannot be more
than one of a given key in the B-tree at a time.
* The `DISK_READ` and `DISK_WRITE` functions aren't implemented; it's assumed
that any reading or writing to storage is handled externally.

## Environment

The `./configure` script assumes the following:

* Debian-based Linux
* GCC 6.3.0/clang 4.0.1 or newer is installed
* Doxygen 1.8.11 or newer is installed
* Python 3.8.x is installed
* A version of pip3 is installed

The `./configure` script will install the following:

* Most recent `pip3`
* [Meson](https://mesonbuild.com/index.html) for compilation and running of
tests
* [Munit](https://nemequ.github.io/munit/#about) for the test framework

## Building

Normal debug build:

```bash
> ./configure
> meson build
```

Build with AddressSanitizer:

```bash
> rm -rf build/
> meson build -Db_sanitize=address
```

Generating Doxygen documentation (output to build/docs):

```bash
> rm -rf build/
> meson build
> cd build/ && ninja docs
```

## Testing

```bash
> meson test -v -C build/
```

Tests should take about 30-60 seconds to run. The `-v` option shows the munit
test output as it's generated; otherwise, the test output is hidden and will
appear to hang after building until the tests are complete.

<a name="clrs">1</a>: Cormen, Thomas H., et al. “Chapter 18: B-Trees.”
Introduction to Algorithms, Third ed., The MIT Press, 2009, pp. 484–504.
