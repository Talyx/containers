#ifndef UTILS_NODE_HPP_
#define UTILS_NODE_HPP_

#include <iostream>
#include "./utils.hpp"
//========================================================================================================
// node
//========================================================================================================
namespace ft {
template <typename T>
struct Node {

 public:
	typedef T	value_type;

	Node*		parent;
	Node*		left;
	Node*		right;
	value_type	data;

//=========================================================
// constructor/destructor
//=========================================================

	Node(): parent(NULL), left(NULL), right(NULL) {}

	Node(const value_type &val, Node* parent = NULL, Node* left = NULL,
		Node* right = NULL): data(val), parent(parent), left(left),
		right(right) { }

	Node(const Node & o): parent(o.parent), left(o.left),
		right(o.right), data(o.data) { }
	~Node() {}

	Node &operator=(const Node &o) {
		if (this == &o) return *this;

		parent = o.parent;
		left = o._left;
		right = o.right;
		data = o.data;
		return (*this);
	}

	bool operator==(const Node &o) {
		return (data == o.data);
	}
};
} // namespace ft

#endif // UTILS_NODE_HPP_