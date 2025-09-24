# BUILD file for the x11_blazeface project.
#
# This file defines how to build the application from its source code.

# Library that contains all LiteRT-related logic.
cc_library(
    name = "libfacelitert",
    srcs = ["litert_face_model.cc"],
    hdrs = ["litert_face_model.h"],  # changed: avoid name collision with external header
    # Allow dynamic linking by disabling static linking for this library.
    linkstatic = False,
    visibility = ["//visibility:public"],
    deps = [
        "@litert//litert/cc:litert_api_with_dynamic_runtime",
    ],
)

# X11 binary that links against the FaceLiteRT library.
cc_binary(
    name = "x11_blazeface",
    srcs = ["main.cc"],
    deps = [
        ":libfacelitert",
        "@xorg//:x11",
        "@xorg//:xext",
        "@xorg//:xfixes",
    ],
)
