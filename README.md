# game_concept

A *work in progress* roguelike game written entirely in C.

## Using the software
### Prerequisites
Install `raylib` and its dependencies
```
brew install glfw
git submodule update --init --recursive raylib
cd raylib/src
make
```

### Building
```
make
```
If you want to compile with debug symbols and [asan](https://clang.llvm.org/docs/AddressSanitizer.html)
```
make debug
```

### Running
```
./demo
```
