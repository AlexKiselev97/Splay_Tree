#include <algorithm>

#ifndef SET_ON_SPLAY_TREE_
#define SET_ON_SPLAY_TREE_

class Splay_tree
{
	struct Node {
		long sum_ = 0;
		long key_ = -1;
		Node* left_ = nullptr;
		Node* right_ = nullptr;
		Node* parent_ = nullptr;
		Node(long key, Node* parent) : key_(key), parent_(parent), sum_(key) {}
		void updateSum();
	};

	Node* root_ = nullptr;

	void splay(Node* x);
	void insert_(long key, Node* root);
	Node* find_(long key, Node* root);
	void print_(Node* root);
	void remove_(Node* x);
	void zig(Node* x, Node* p);
	void zag(Node* x, Node* p);
	Node* merge(Node* l, Node* r);
	Node* maxInTree(Node* root);
	std::pair<Node*, Node*> split(Node* root, long key);
public:
	void insert(long key);
	void remove(long key);
	bool find(long key);
	void print() { print_(root_); }
	long sum(long l, long r);
};

#endif // !SET_ON_SPLAY_TREE_
