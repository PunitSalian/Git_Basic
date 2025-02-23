cc_library (
    name = "blob_intf",

    srcs = [
        "Blob_intf.h",
    ],

    deps = [
        "//common:error",
        "//common:result",
    ],

    visibility = ["//visibility:public"],
)

cc_library (
    name = "blob",

    srcs = [
        "Blob.cpp",
    ],

    hdrs = [
        "Blob.h",
    ],

    deps = [
        "//storage/FileManager:filemanager_intf",
        "//utility/Sha:hash_intf",
        "//config:gitconfig",
        "//utility/Compression:compression_intf",
        ":blob_intf",

    ],

    visibility = ["//visibility:public"],
)

cc_library (
    name = "blob_mock",

    srcs = [
        "BlobTest.h",
    ],

    deps = [
        ":blob_intf",
    ],

    visibility = ["//visibility:public"],
)

cc_test (
    name = "blob_test",

    srcs = [
        "BlobTest.cpp"
    ],

    deps = [
        ":blob",
        "//config:gitconfig",
        "//utility/Sha:hash_mock",
        "//utility/Compression:zlib_mock",
        "//storage/FileManager:filemanager_mock",
        "@googletest//:gtest_main",
    ],

    visibility = ["//visibility:public"],
)