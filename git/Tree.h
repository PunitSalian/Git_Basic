#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "GitIndex_intf.h"
#include "storage/Objectstore/Blob_intf.h"
#include "storage/FileManager/FileManagerInterface.h"

namespace git
{
    struct TreeNode
    {
        std::string name;
        std::string hash;
        bool isBlob = false;

        // Only used for blob nodes
        std::vector<uint8_t> data;

        std::vector<TreeNode *> children;

        TreeNode() = default;
        explicit TreeNode(std::string n, bool blob = false) : name(std::move(n)), isBlob(blob) {}
    };

    class TreeBuilder
    {
        storage::Blob_intf &b_;
        storage::FileManagerInterface<std::filesystem::path> &f_;

    public:
        TreeBuilder(storage::Blob_intf &b, storage::FileManagerInterface<std::filesystem::path> &f)
            : b_(b), f_(f) {}

        Result<TreeNode *, ErrorCode> buildTree(const std::vector<IndexEntry> &entries);
        Result<std::string, ErrorCode> WriteTree(TreeNode *root);
    };
}