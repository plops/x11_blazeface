# src/BUILD.bazel

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "blazeface_hermetic",
    srcs = ["main.cc"],
    deps = [
        # This target is now provided by the @litert defined in the WORKSPACE file.
        "@litert//litert/cc:litert_api_with_dynamic_runtime",

        # This dependency is transitively brought in by @litert, so it's available.
        "@com_google_absl//absl/types:span",

        # These hermetic libraries come from the BCR via MODULE.bazel.
        "@libx11//:libx11",
    ],
)
