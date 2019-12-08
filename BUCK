cxx_binary(
    name = "demo",
    header_namespace = "",
    srcs = glob([
        "src/*.c",
    ]),
    headers = subdir_glob(
        [
            ("include", "*.h"),
        ],
    ),
    compiler_flags = [
        "-std=c11",
        "-Wall",
        "-Werror",
        "-Wno-unused",
    ],
    deps = [
        ":corange",
    ],
)

prebuilt_cxx_library (
    name = "corange",
    soname = "libcorange.so",
    shared_lib = "lib/libcorange.so",
    static_lib = "lib/libcorange.a",
    header_dirs = [
        "external/include",
    ],
    deps = [
        ":sdl2",
        ":sdl2_mixer",
        ":sdl2_net",
    ],
)

prebuilt_cxx_library (
    name = "sdl2",
    shared_lib = "lib/libSDL2.dylib",
    static_lib = "lib/libSDL2.a",
    header_dirs = [
        "external/include",
    ],
)

prebuilt_cxx_library (
    name = "sdl2_mixer",
    shared_lib = "lib/libSDL2_mixer.dylib",
    static_lib = "lib/libSDL2_mixer.a",
    header_dirs = [
        "external/include",
    ],
)

prebuilt_cxx_library (
    name = "sdl2_net",
    shared_lib = "lib/libSDL2_net.dylib",
    static_lib = "lib/libSDL2_net.a",
    header_dirs = [
        "external/include",
    ],
)
