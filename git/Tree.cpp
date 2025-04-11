#include "Tree.h"
#include <algorithm>
#include <iostream>
#include "utility/Sha/Hash_intf.h"
Result<git::TreeNode *, ErrorCode> git::TreeBuilder::buildTree(const std::vector<IndexEntry> &c)
{
    git::TreeNode *root = new git::TreeNode();
    git::TreeNode *tmp = root;
    for (auto &entry : c)
    {
        // traverse the directory
        std::filesystem::path p(entry.file);
        std::cout << "buildTree next entry " << entry.file << std::endl;
        auto entry_file_size = f_.getFilesize(p);
        if (entry_file_size.is_err())
        {
            return entry_file_size.error();
        }

        std::vector<uint8_t> buff(entry_file_size.value()); // Reserve buffer with correct size
        auto res = f_.readFile(p, buff);
        if (res.is_err())
        {
            return res.error();
        }

        for (auto it = p.begin(); it != p.end(); ++it)
        {
            // find if this entry is already in the children
            auto ele = std::find_if(tmp->children.begin(), tmp->children.end(), [&](TreeNode *child)
                                    { return tmp->name == *it; });

            std::cout << "buildTree part " << *it << std::endl;
            // If it is in the children move the tmp node to point here.
            if (ele != tmp->children.end())
            {
                tmp = *ele;
                continue;
            }

            git::TreeNode *new_node = new git::TreeNode();

            // If there is no entry next to this, we reached a file.
            // set blob as true
            if (std::next(it) == p.end())
            {
                new_node->name = *it;
                std::span<const uint8_t, SHA_DIGEST_LENGTH> hashView(entry.stat_entry.sha1, SHA_DIGEST_LENGTH);

                new_node->hash = util::toHexString(hashView);
                new_node->isBlob = true;
                new_node->data = std::move(buff);
                tmp->children.push_back(new_node);
                std::cout << "buildTree  file" << *it << std::endl;

                break;
            }
            std::cout << "buildTree  dir" << *it << std::endl;

            // insert it to this nodes children and move to the next dir and make it as next node
            // set blob as false
            new_node->name = *it;
            new_node->isBlob = false;
            tmp->children.push_back(new_node);
            tmp = new_node;
        }
    }

    return root;
}

Result<std::string, ErrorCode> git::TreeBuilder::WriteTree(git::TreeNode *root)
{
    // base case file write a create a blob
    // return hash
    if (root->isBlob)
    {
        return root->hash;
    }

    // navigate all the children and recursively call
    std::vector<uint8_t> tree_object;
    for (auto &c : root->children)
    {
        auto res = git::TreeBuilder::WriteTree(c);
        std::string tree_entry = "";

        // Is blob
        if (c->isBlob)
        {
            // create a blob string line with hash and mode pushback in a vector
            tree_entry = "100644 ";
        }
        else
        {
            // create a blob string line with hash and mode pushback in a vector
            tree_entry = "40000 ";
        }
        tree_object.insert(tree_object.end(), tree_entry.begin(), tree_entry.end());
        // Insert Null as well
        tree_object.insert(tree_object.end(), c->name.begin(), c->name.end() + 1);

        auto sha = util::fromHexString(res.value());
        tree_object.insert(tree_object.end(), sha.begin(), sha.end());
    }

    auto res = b_.storeObject(std::move(tree_object), storage::Objecttype::TREE);

    if (res.is_err())
    {
        std::cerr << "WriteTree storeBlob Tree error " << static_cast<uint16_t>(res.error()) << std::endl;
        return res.error();
    }
    std::cout << "WriteTree hash for tree object for " << root->name << " = " << res.value() << std::endl;
    return res.value();
}