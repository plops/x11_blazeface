# BUILD file for the x11_blazeface project.
#
# This file defines how to build the application from its source code.

# Library that contains all LiteRT-related logic.
cc_library(
    name = "liblitert",
    srcs = ["litert_model.cc"],
    hdrs = ["litert_model.h"],
    # Allow dynamic linking by disabling static linking for this library.
    linkstatic = False,
    visibility = ["//visibility:public"],
    deps = [
        "@litert//litert/cc:litert_api_with_dynamic_runtime",
    ],
)

# X11 binary that links against the LiteRT library.
cc_binary(
    name = "x11_blazeface",
    srcs = ["main.cc"],
    deps = [
        ":liblitert",
        "@xorg//:x11",
        "@xorg//:xext",
        "@xorg//:xfixes",
    ],
)
