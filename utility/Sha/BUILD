
cc_library (
    name = "hash_intf",
    srcs = ["Hash_intf.h"],
    visibility = ["//visibility:public"],
)

cc_library (
    name = "hash_mock",

    srcs = [
        "HashTest.h",
    ],

    deps = [
        ":hash_intf",
    ],

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "sha1",
    srcs = ["Sha1.cpp"],

    hdrs = [
        "Sha1.h",
    ],

    deps = [
        ":hash_intf",
        "@openssl//:crypto", 
        "@openssl//:ssl",
    ],

    visibility = ["//visibility:public"],
)

cc_test (
    name = "sha1_test",

    srcs = [
        "HashTest.cpp",
    ],

    deps = [
        ":sha1",
        "@googletest//:gtest_main",
    ],

    visibility = [
        "//visibility:public",
    ]
)