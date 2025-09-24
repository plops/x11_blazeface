# WORKSPACE for x11_blazeface
#
# This file defines the external dependencies for the project. It has been
# modified to only include the components necessary for LiteRT (for AI)
# and X11 (for window capture).

workspace(name = "x11_blazeface")

# Correctly load repository rules from their respective .bzl files.
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:local.bzl", "new_local_repository")

# LiteRT: A lightweight, cross-platform runtime for TFLite models.
# This is the core AI component for face detection.
LITERT_REF = "499b99313d6cd7ec6552b93c0622662dec8eaf6d"
LITERT_SHA256 = "9ef8e120f803b6ff9e40299c5cf4216100dff14fc47771b41d86dc8bbb0b144d"

http_archive(
    name = "litert",
    sha256 = LITERT_SHA256,
    strip_prefix = "LiteRT-" + LITERT_REF,
    url = "https://github.com/google-ai-edge/LiteRT/archive/" + LITERT_REF + ".tar.gz",
)

# TensorFlow: LiteRT has a core dependency on TensorFlow.
# This pulls in the TensorFlow source code, which is required to build LiteRT.
TENSORFLOW_REF = "ec7c1948718638d4e63a68ce511c8425d930b7d9"
TENSORFLOW_SHA256 = "f67de8c5db765f11ffb7ef28bac151398e46f76c74b3d5c156bc894ec54b0bfb"

http_archive(
    name = "org_tensorflow",
    sha256 = TENSORFLOW_SHA256,
    strip_prefix = "tensorflow-" + TENSORFLOW_REF,
    url = "https://github.com/tensorflow/tensorflow/archive/" + TENSORFLOW_REF + ".tar.gz",
)

# Initialize the TensorFlow repository and its own dependencies (like Abseil, Protobuf, etc.).
# These macros are defined within the TensorFlow repository and are required by LiteRT.
load("@org_tensorflow//tensorflow:workspace3.bzl", "tf_workspace3")
tf_workspace3()

load("@org_tensorflow//tensorflow:workspace2.bzl", "tf_workspace2")
tf_workspace2()

load("@org_tensorflow//tensorflow:workspace1.bzl", "tf_workspace1")
tf_workspace1()

load("@org_tensorflow//tensorflow:workspace0.bzl", "tf_workspace0")
tf_workspace0()

# X11 Libraries: Required for capturing window contents on Linux.
#
# This rule makes the system-installed X11 libraries available to Bazel targets.
# It assumes a standard Linux environment where X11 headers and libraries are
# in /usr/include and the system's standard library paths.
new_local_repository(
    name = "xorg",
    path = "/usr",
    build_file_content = """
# BUILD file for system-provided X11 libraries.
# This allows other Bazel packages to depend on targets like "@xorg//:x11".
package(default_visibility = ["//visibility:public"])

# The core X11 protocol library (libX11)
cc_library(
    name = "x11",
    hdrs = glob(["include/X11/Xlib.h", "include/X11/Xutil.h"]),
    includes = ["include"],
    linkopts = ["-lX11"],
)

# The X Extension library (libXext)
cc_library(
    name = "xext",
    includes = ["include"],
    linkopts = ["-lXext"],
)

# The XFixes extension library (libXfixes)
cc_library(
    name = "xfixes",
    includes = ["include"],
    linkopts = ["-lXfixes"],
)
""",
)