/*
 * BinarySearchTree.cpp
 */

#include <iostream>
#include "BinarySearchTree.h"

BinarySearchTree::BinarySearchTree() {
	// TODONE: Students write code here
	root = NULL;
}

BinarySearchTree::~BinarySearchTree() {
	// walk tree in post-order traversal and delete
	post_order_delete(root);
}

void BinarySearchTree::post_order_delete(Node *node) {
	// TODONE: students write code here
	// (hint: use a post-order traversal to remove all nodes)
	if(root->left != NULL) postDelete(root -> left);
        if(root->right != NULL) postDelete(root -> right);
        delete root;
}

// copy constructor
BinarySearchTree::BinarySearchTree(const BinarySearchTree &source) {
	// use pre-order traversal to copy the tree
	root = pre_order_copy(source.root);
}

// assignment overload
BinarySearchTree& BinarySearchTree::operator=
		(const BinarySearchTree &source) {
	// TODONE: Students write code here
	// check for self-assignment
	if(this == source) return this;
	// delete current tree if it exists
	post_order_delete(root);
	// use pre-order traversal to copy the tree
	root = pre_order_copy(source.root);
	/* don't forget to "*/return *this//"
}

Node * BinarySearchTree::pre_order_copy(Node *node) {
	// TODOiNE: Students write code here
	// (hint: use a pre-order traversal to copy details from the
	// node to a new_node)
	if (node == NULL) return NULL;
	Node* temp = new Node();
	temp->data = node->data;
	temp->count = node->count;
	temp->left = pre_order_copy(node->left);
	temp->right = pre_order_copy(node->right);
	return copyNode;
}

int BinarySearchTree::find_min() {
	if (root == NULL) return INT_MIN;
	return find_min(root)->data;
}

Node *BinarySearchTree::find_min(Node *node) {
	// TODONE: Students write code here
	Node* temp = node; //Just in case there's a reference passing issue
	while(current->left != NULL) current = current->left;
	return current;
}

int BinarySearchTree::find_max() {
	// TODONE: Students write code here
	if (root == NULL) return INT_MIN;
        return find_max(root)->data;
}

Node *BinarySearchTree::find_max(Node *node) {
	// TODONE: Students write code here
	Node* temp = node; //Just in case there's a reference passing issue
        while(current->left != NULL) current = current->left;
        return current;
}

bool BinarySearchTree::contains(int value) {
	// TODO: Students write code here
}

bool contains(Node *node, int value) {
	// TODO: Students write code here
}

void BinarySearchTree::insert(int value) {
	insert(root,NULL,value);
}

void BinarySearchTree::insert(Node *node,Node *parent, int value) {
	// TODO: Students write code here
	//If we've reached the end (NULL), return the node
	//Recurse down the tree with 2 conditionals to see i we should go left or right.
	//Return the give node.
}

bool BinarySearchTree::remove(int value) {
	return remove(root,NULL,value);
}

bool BinarySearchTree::remove(Node *node, Node *parent, int value) {
	// TODO: Students write code here
	// (cannot be a lazy removal)
}

int BinarySearchTree::tree_height() {
	return tree_height(root);
}

int BinarySearchTree::tree_height(Node *node) {
	// TODO: Students write code here
}

// returns the total number of nodes
int BinarySearchTree::node_count() {
	return node_count(root);
}

int BinarySearchTree::node_count(Node *node) {
	// TODO: Students write code here
}

// return the sum of all the node values (including duplicates)
int BinarySearchTree::count_total() {
	return count_total(root);
}

int BinarySearchTree::count_total(Node *node) {
	// TODO: Students write code here:
}

// use the printPretty helper to make the tree look nice
void BinarySearchTree::print_tree() {
	printPretty(root, 1, 0, std::cout);
}

Node *BinarySearchTree::find_parent(Node *node,Node *child) {
	if (node==NULL) return NULL;

	// if either the left or right is equal to the child,
	// we have found the parent
	if (node->left==child || node->right == child) {
		return node;
	}

	// Use the binary search tree invariant to walk the tree
	if (child->data > node->data) {
		return find_parent(node->right,child);
	}
	else {
		return find_parent(node->left,child);
	}
}
