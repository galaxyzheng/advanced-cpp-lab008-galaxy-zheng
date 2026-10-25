#include "project/binary_search_tree.h"

#include <algorithm>
#include <stdexcept>

BinarySearchTree::BinarySearchTree() : root_(nullptr), size_(0) {}

BinarySearchTree::~BinarySearchTree() {
    deleteSubtree(root_);
}

void BinarySearchTree::deleteSubtree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    deleteSubtree(node->left);
    deleteSubtree(node->right);
    delete node;
}

bool BinarySearchTree::insert(int value) {
    if (root_ == nullptr) { 
        root_ = new TreeNode(value); 
        size_++; 
        return true; 
    }
    TreeNode* cur = insertRecursive(root_, value);
    if (value == cur->value) return false; 
    if (value < cur->value) {
        cur->left = new TreeNode(value);
    } else {cur->right = new TreeNode(value); }
    size_++;
    return true;
}

bool BinarySearchTree::remove(int value) {
    if (root_ == nullptr) return false;  //not found: root
    TreeNode* cur = root_;
    if (root_->value == value) {
        if (root_->left == nullptr && root_->right == nullptr) root_ = nullptr; 
        if (root_->left != nullptr) {
            cur = cur->left; //move to find largest minimum
            if (cur->right == nullptr) {root_->value = cur->value; root_->left = cur->left;}
            else {
                while (cur->right->right != nullptr) {
                    cur = cur->right;
                } root_->value = cur->right->value;
                TreeNode* save = cur->right;
                cur->right = cur->right->left;
                cur = save;
            }
        } else if (root_->right != nullptr) {
            cur = cur->right; //move to find largest minimum
            if (cur->left == nullptr) {root_->value = cur->value; root_->right = cur->right;}
            else {
                while (cur->left->left != nullptr) {
                    cur = cur->left;
                } root_->value = cur->left->value;
                TreeNode* save = cur->left;
                cur->left = cur->left->right;
                cur = save;
            }
        }
        delete cur;
        size_--;
        return true;
    }
    cur = removeRecursive(root_, value);
    if (cur == nullptr) return false; //not found
    TreeNode* del = root_; //otherwise begin delete decision
    (value < cur->value) ? del = cur->left : del = cur->right;
    if (del->left == nullptr) {
        //if (del->right == nullptr) {
        //    (value < cur->value) ? cur->left == nullptr : cur->right == nullptr;
        //} else {
        (value < cur->value) ? cur->left = del->right : cur->right = del->right; //if statement is unnecessary, fully functional
        //}
    } else if (del->right == nullptr) {
        (value < cur->value) ? cur->left = del->left : cur->right = del->left;
    } else {
        cur = del; //move cursor to current del, current del will no longer be deleted
        del = del->left; //move to find largest minimum
        if (del->right == nullptr) {cur->value = del->value; cur->left = del->left;}
        else { 
            while (del->right->right != nullptr) {
                del = del->right;
            } cur->value = del->right->value; //copy maximum smaller
            cur = del; del = del->right; //advance to deletion area
            cur->right = del->left; //links existing del->left to replace del or sets to nullptr
        }
    }
    delete del;
    size_--;
    return true;
}

bool BinarySearchTree::search(int value) const {
    return searchRecursive(root_, value); //returns bool, but should normally return pointer?
}

bool BinarySearchTree::contains(int value) const {
    return searchRecursive(root_, value);
}

bool BinarySearchTree::empty() const noexcept {
    return size_ == 0;
}

std::size_t BinarySearchTree::size() const noexcept {
    return size_;
}

int BinarySearchTree::height() const {
    return heightRecursive(root_);
}

std::vector<int> BinarySearchTree::inorderTraversal() const {
    std::vector<int> result;
    inorderRecursive(root_, result);
    return result;
}

std::vector<int> BinarySearchTree::preorderTraversal() const {
    std::vector<int> result;
    preorderRecursive(root_, result);
    return result;
}

std::vector<int> BinarySearchTree::postorderTraversal() const {
    std::vector<int> result;
    postorderRecursive(root_, result);
    return result;
}

int BinarySearchTree::minimum() const {
    if (empty()) {
        throw std::logic_error("BST is empty");
    }
    return minimumRecursive(root_);
}

int BinarySearchTree::maximum() const {
    if (empty()) {
        throw std::logic_error("BST is empty");
    }
    return maximumRecursive(root_);
}

TreeNode* BinarySearchTree::insertRecursive(TreeNode* node, int value) {
    if (value == node->value) return node;
    if (value < node->value) {
        if (node->left == nullptr) {
            return node;
        } else { return insertRecursive(node->left, value); }
    } else {
        if (node->right == nullptr) {
            return node;
        } else { return insertRecursive(node->right, value); }
    }
}

TreeNode* BinarySearchTree::removeRecursive(TreeNode* node, int value) {
    if (node == nullptr) return nullptr; // no checks for null, so return in next iteration if null
    if (value < node->value) {
        if (node->left != nullptr && node->left->value == value) {
            return node;
        } else { return removeRecursive(node->left, value); }
    } else {
        if (node->right != nullptr && node->right->value == value) {
            return node;
        } else { return removeRecursive(node->right, value); }
    }
}

bool BinarySearchTree::searchRecursive(const TreeNode* node, int value) const {
    if (node == nullptr) return false; // no checks for null, so return in next iteration if null
    if (value == node->value) return true;
    return (value < node->value) ? searchRecursive(node->left, value) : searchRecursive(node->right, value);
}

void BinarySearchTree::inorderRecursive(const TreeNode* node, std::vector<int>& result) const { //!
    if (node == nullptr) return;
    if (node->left != nullptr) inorderRecursive(node->left, result);
    result.push_back(node->value);
    if (node->right != nullptr) inorderRecursive(node->right, result);
}

void BinarySearchTree::preorderRecursive(const TreeNode* node, std::vector<int>& result) const { //!
    if (node == nullptr) return;
    result.push_back(node->value);
    if (node->left != nullptr) preorderRecursive(node->left, result);
    if (node->right != nullptr) preorderRecursive(node->right, result);
}

void BinarySearchTree::postorderRecursive(const TreeNode* node, std::vector<int>& result) const { //!
    if (node == nullptr) return;
    if (node->left != nullptr) postorderRecursive(node->left, result);
    if (node->right != nullptr) postorderRecursive(node->right, result);
    result.push_back(node->value);
}

int BinarySearchTree::heightRecursive(const TreeNode* node) const {
    if (node == nullptr) return 0; // no checks for null, so return in next iteration if null
    //if (node->left == nullptr && node->right == nullptr) return 0; // 0 height for leaf
    int heightL = 1 + heightRecursive(node->left);
    int heightR = 1 + heightRecursive(node->right);
    return (heightL > heightR) ? heightL : heightR;
}

int BinarySearchTree::minimumRecursive(const TreeNode* node) const {
    return (node->left == nullptr) ? node->value : minimumRecursive(node->left);
}

int BinarySearchTree::maximumRecursive(const TreeNode* node) const {
    return (node->right == nullptr) ? node->value : maximumRecursive(node->right);
}
