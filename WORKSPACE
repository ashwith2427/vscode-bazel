load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "toolchains_llvm",
    sha256 = "e91c4361f99011a54814e1afbe5c436e0d329871146a3cd58c23a2b4afb50737",
    strip_prefix = "toolchains_llvm-1.0.0",
    canonical_id = "1.0.0",
    url = "https://github.com/bazel-contrib/toolchains_llvm/releases/download/1.0.0/toolchains_llvm-1.0.0.tar.gz",
)

load("@toolchains_llvm//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@toolchains_llvm//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version = "16.0.0",
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

http_archive(
    name = "bzlws",
    strip_prefix = "bzlws-f929e5380f441f50a77776d34a7df8cacdbdf986",
    url = "https://github.com/zaucy/bzlws/archive/f929e5380f441f50a77776d34a7df8cacdbdf986.zip",
    sha256 = "5bebb821b158b11d81dd25cf031b5b26bae97dbb02025df7d0e41a262b3a030b",
)

load("@bzlws//:repo.bzl", "bzlws_deps")
bzlws_deps()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_vulkan",
    remote = "https://github.com/jadarve/rules_vulkan.git",
    tag = "v0.0.6"
)

load("@rules_vulkan//vulkan:repositories.bzl", "vulkan_repositories")

vulkan_repositories()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

GLM_VERSION = "0.9.9.8"

http_archive(
    name = "glm",
    build_file = "@//third_party/glm:glm.BUILD",
    sha256 = "4605259c22feadf35388c027f07b345ad3aa3b12631a5a316347f7566c6f1839",
    strip_prefix = "glm-{}".format(GLM_VERSION),
    urls = ["https://github.com/g-truc/glm/archive/{}.zip".format(GLM_VERSION)],
)

GLFW_VERSION = "3.3.5"

http_archive(
    name = "glfw",
    build_file = "@//third_party/glfw:glfw.BUILD",
    sha256 = "a89bb6074bc12bc12fcd322dcf848af81b679ccdc695f70b29ca8a9aa066684b",
    strip_prefix = "glfw-{}".format(GLFW_VERSION),
    urls = ["https://github.com/glfw/glfw/archive/{}.zip".format(GLFW_VERSION)],
)

