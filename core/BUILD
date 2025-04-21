cc_library (
    name = "git_index_intf",

    srcs = [
        "GitIndex_intf.h",
    ],

    deps = [
        "//common:error",
        "//common:result",
    ],

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "git_index",

    srcs = [
        "GitIndex.cpp",
    ],

    hdrs =  [
        "GitIndex.h",
    ],

    deps = [
        ":git_index_intf",
        "//storage/FileManager:filemanager_intf",
        "//config:gitconfig",
        "//utility/Sha:hash_intf",
    ],

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "tree",

    srcs = [
        "Tree.cpp",
    ],

    hdrs =  [
        "Tree.h",
    ],

    deps = [
        "//utility/Sha:hash_intf",
        ":git_index_intf",
        "//storage/Objectstore:blob_intf",
        "//storage/FileManager:filemanager_intf",
    ],

    visibility = [
        "//visibility:public",
    ],
)