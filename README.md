# btree

![Build](https://github.com/dbargatz/btree/workflows/Build/badge.svg)

An implementation of a B-Tree in C.

## Building

```bash
> ./configure
> meson setup build
> meson compile -C build/
```

## Testing

```bash
> meson test -v -C build/
```

Tests should take about 30-60 seconds to run. The `-v` option shows the munit
test output as it's generated; otherwise, the test output is hidden and will
appear to hang after building until the tests are complete.
