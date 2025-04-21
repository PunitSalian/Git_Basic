cc_binary (
    name = "my_git",

    srcs = [
        "main.cpp",
    ],

    deps = [
        ":command_intf",
        ":init_command",
        "//core:git_index",
        "//core:tree",
        "//storage/FileManager:filemanager",
        "//config:gitconfig",
        ":add_command",
        "@tclap//:tclap",
        ":commit_command",
       "//storage/Objectstore:blob",
       "//utility/Sha:sha1",
       "//utility/Compression:zlib",
    ],

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "command_intf",

    srcs = [
        "Command_intf.h",
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
    name = "init_command",

    srcs = [
        "InitCommand.cpp",
    ],

    hdrs =  [
        "InitCommand.h",
    ],

    deps = [
        ":command_intf",
        "//core:git_index_intf",
        "//storage/FileManager:filemanager_intf",
        "//config:gitconfig",
    ],

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "add_command",

    srcs = [
        "AddCommand.cpp",
    ],

    hdrs =  [
        "AddCommand.h",
    ],

    deps = [
        "//core:git_index_intf",
        ":command_intf",
        "//storage/FileManager:filemanager_intf",
        "//config:gitconfig",
        "//storage/Objectstore:blob_intf",
    ],

    visibility = [
        "//visibility:public",
    ],
)

cc_library (
    name = "commit_command",

    srcs = [
        "CommitCommand.cpp",
    ],

    hdrs =  [
        "CommitCommand.h",
    ],

    deps = [
        ":command_intf",
        "//core:tree",
        "//storage/FileManager:filemanager_intf",
        "//config:gitconfig",
        "//storage/Objectstore:blob_intf",
    ],

    visibility = [
        "//visibility:public",
    ],
)

