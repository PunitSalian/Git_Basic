cc_library (
    name = "compression_intf",
    srcs = ["Compression_intf.h"],
    deps = [
        "//common:error",
        "//common:result",
    ],
    visibility = ["//visibility:public"],
)

cc_library (
    name = "zlib_mock",

    srcs = [
        "ZlibTest.h",
    ],

    deps = [
        ":compression_intf",
    ],  

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "zlib",
    srcs = [
        "Zlib.cpp",
    ],
    hdrs = [
        "Zlib.h"
    ],
    deps = [
        ":compression_intf",
        "@zlib//:zlib",
    ],
    visibility = ["//visibility:public"],
)

cc_test (
    name = "zlib_test",

    srcs = [
        "ZlibTest.cpp",
    ],

    deps = [
        ":zlib",
        "@googletest//:gtest_main",
    ],

    visibility = [
        "//visibility:public",
    ]
)