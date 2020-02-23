# game_concept

A *work in progress* roguelike game written entirely in C.

## Using the software
### Prerequisites
Install `raylib` and its dependencies
```
brew install glfw
git clone https://github.com/raysan5/raylib.git
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
