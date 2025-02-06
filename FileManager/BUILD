cc_library (
    name = "filemanager_intf",

    srcs = [
        "FileManagerInterface.h",
    ],

    deps = [
        "//common:error",
        "//common:result",
    ],

    visibility = ["//visibility:public"],
)

cc_library(
    name = "filemanager",
    srcs = [
        "FileManager.inl",
    ],
    hdrs = [
        "FileManager.h",
    ],

    deps = [
        ":filemanager_intf",
    ],


    visibility = ["//visibility:public"],
)

cc_test (
    name = "filemanager_test",

    srcs = [
        "FileManagerTest.cpp",
    ],

    linkstatic = 1,

    deps = [
        ":filemanager",
        ":filemanager_intf",
        "@googletest//:gtest_main",
    ],

    visibility = ["//visibility:public"],
)