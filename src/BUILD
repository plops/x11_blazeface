# BUILD file for the x11_blazeface project.
#
# This file defines how to build the application from its source code.

# The cc_binary rule compiles and links the C++ source code to create an executable file.
cc_binary(
    # The 'name' is the name of the target and the resulting executable.
    name = "x11_blazeface",

    # 'srcs' lists the source files to be compiled.
    srcs = ["src/main.cc"],

    # 'deps' lists the libraries this target depends on.
    deps = [
        # LiteRT dependency for AI/face detection.
        # This specific target is the main C++ library provided by LiteRT.
        "@litert//:litert",

        # X11 dependencies for window capturing.
        # These targets were defined in the new_local_repository rule in the WORKSPACE file.
        "@xorg//:x11",
        "@xorg//:xext",
        "@xorg//:xfixes",
    ],
)