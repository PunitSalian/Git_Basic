cc_library (
    name = "gitconfig_intf",

    srcs = [
        "GitConfig_intf.h",
    ],

    visibility = ["//visibility:public"],
)


cc_library (
    name = "gitconfig",
    
    srcs = [
        "GitConfig.cpp",
    ],

    hdrs = [
        "GitConfig.h",
    ],

    deps = [
        ":gitconfig_intf",
    ],

    visibility = ["//visibility:public"],
)