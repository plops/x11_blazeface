# litert_ext.bzl - A module extension to manage LiteRT's legacy dependencies.

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def _litert_deps_impl(ctx):
    """
    This function re-declares the WORKSPACE dependencies needed by LiteRT.
    It's a simplified version, containing only what's necessary for the C++ API.
    """
    maybe(
        http_archive,
        name = "rules_shell",
        sha256 = "bc61ef94facc78e20a645726f64756e5e285a045037c7a61f65af2941f4c25e1",
        strip_prefix = "rules_shell-0.4.1",
        url = "https://github.com/bazelbuild/rules_shell/releases/download/v0.4.1/rules_shell-v0.4.1.tar.gz",
    )
    
    maybe(
        http_archive,
        name = "rules_ml_toolchain",
        sha256 = "40d1e8a2c2e91e0c8781bce80ade7fb34dface5861248d03cae88b24c38fb991",
        strip_prefix = "rules_ml_toolchain-02fe7225d62af3036a541aa48b6f7bc86089ea5d",
        urls = [
            "https://github.com/google-ml-infra/rules_ml_toolchain/archive/02fe7225d62af3036a541aa48b6f7bc86089ea5d.tar.gz",
        ],
    )

    # Note: We don't define @org_tensorflow directly. LiteRT's own WORKSPACE file
    # is complex. Instead, we let the @litert repo itself handle that part after
    # we provide the necessary rule definitions. This is a bit of a hybrid approach.
    # The key is providing the rules that LiteRT's BUILD files *load*.
    # The error was about @rules_cc, which is brought in by @rules_ml_toolchain's WORKSPACE.bzl
    # but let's be explicit.

    maybe(
        git_repository,
        name = "rules_cc",
        commit = "b017997b5e40639e443b715a371306a445f28453",
        remote = "https://github.com/bazelbuild/rules_cc.git",
    )


litert_deps = module_extension(
    implementation = _litert_deps_impl,
)