#pragma once

template <class NODETYPE> class Tree;

template <class NODETYPE> class TreeNode
{
	friend class Tree<NODETYPE>;
public:
	explicit TreeNode(const NODETYPE &);
	NODETYPE get_data();
protected:
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent;
	NODETYPE data;
};

template<class NODETYPE>
TreeNode <NODETYPE>::TreeNode(const NODETYPE &data)
	: data(data)
{
	left = right = nullptr;
}

template <class NODETYPE>
NODETYPE TreeNode <NODETYPE>::get_data()
{
	return data;
}

template <class NODETYPE>
class Tree
{
public:
	Tree();
	int insert_node(const NODETYPE &);
	TreeNode<NODETYPE>* find_node(TreeNode<NODETYPE>*, const NODETYPE &);
	TreeNode<NODETYPE> *get_root();
private:
	TreeNode<NODETYPE> *root;
};

template<class NODETYPE>
Tree<NODETYPE>::Tree()
	: root(nullptr)
{
}

template<class NODETYPE>
int Tree<NODETYPE>::insert_node(const NODETYPE &x)
{
	TreeNode<NODETYPE>* n = new TreeNode<NODETYPE>(x);
	n->parent = n->left = n->right = nullptr;

	TreeNode<NODETYPE>* ptr = root;
	TreeNode<NODETYPE>* ptr1 = nullptr;

	while (ptr != nullptr) {
		ptr1 = ptr;
		if (x < ptr->get_data()) {
			ptr = ptr->left;
		}
		else {
			ptr = ptr->right;
		}
	}

	n->parent = ptr1;
	if (ptr1 == nullptr) {
		root = n;
	}
	else {
		if (x < ptr1->get_data())
			ptr1->left = n;
		else
			ptr1->right = n;
	}

	return 0;
}

template<class NODETYPE>
TreeNode<NODETYPE>* Tree<NODETYPE>::find_node(TreeNode<NODETYPE>* n,
	const NODETYPE & val)
{
	if (n == nullptr || val == n->get_data()) {
		return n;
	}

	if (val > n->get_data()) {
		return find_node(n->right, val);
	}

	return find_node(n->left, val);
}

template<class NODETYPE>
TreeNode<NODETYPE>* Tree<NODETYPE>::get_root()
{
	return root;
}
