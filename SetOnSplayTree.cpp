#include <iostream>
#include <string>
#include <algorithm>
#include "SetOnSplayTree.h"

using namespace std;

int main()
{
	/* example program
	input the count of queries
	then:
	+ x   - insert x in set
	- x   - delete x from set
	? x   - check x in set
	s l r - sum of [l, r]
	*/
	int n;
	cin >> n;
	Splay_tree st;
	for (int i = 0; i < n; ++i)
	{
		char c;
		cin >> c;
		long key;
		cin >> key;
		if (c == '+')
		{
			st.insert(key);
		}
		else if (c == '-')
		{
			st.remove(key);
		}
		else if (c == '?')
		{
			cout << (st.find(key) ? "Found" : "Not found") << endl;
		}
		else
		{ // s
			long r;
			cin >> r;
			cout << st.sum(key, r) << endl; // sum of [l, r]
		}
	}
	cout << endl;
	return 0;
}

void Splay_tree::splay(Node * x)
{
	if (x == nullptr)
		return;

	if (x->parent_ != nullptr)
	{
		Node* p = x->parent_;
		if (p->parent_ == nullptr)
		{ // zig/zag
			if (p->left_ == x)
				zig(x, p);
			else
				zag(x, p);
		}
		else
		{
			Node* g = p->parent_;
			if (g->left_ == p && p->left_ == x)
			{ // zig-zig
				zig(p, g);
				zig(x, p);
			}
			else if (g->right_ == p && p->right_ == x)
			{ // zag-zag
				zag(p, g);
				zag(x, p);
			}
			else
			{
				if (p->left_ == x)
				{ // zig-zag
					zig(x, p);
					g->right_ = x;
					zag(x, g);
				}
				else
				{ // zag-zig
					zag(x, p);
					g->left_ = x;
					zig(x, g);
				}
			}
		}
		splay(x);
	}
	else
	{
		root_ = x;
	}
}

void Splay_tree::zig(Node * x, Node * p)
{
	p->left_ = x->right_;
	if (p->left_ != nullptr)
		p->left_->parent_ = p;
	x->right_ = p;
	x->parent_ = p->parent_;
	p->parent_ = x;
	if (x->parent_ != nullptr)
	{
		if (x->parent_->left_ == p)
			x->parent_->left_ = x;
		else
			x->parent_->right_ = x;
	}
	p->updateSum();
	x->updateSum();
}

void Splay_tree::zag(Node * x, Node * p)
{
	p->right_ = x->left_;
	if (p->right_ != nullptr)
		p->right_->parent_ = p;
	x->left_ = p;
	x->parent_ = p->parent_;
	p->parent_ = x;
	if (x->parent_ != nullptr)
	{
		if (x->parent_->left_ == p)
			x->parent_->left_ = x;
		else
			x->parent_->right_ = x;
	}
	p->updateSum();
	x->updateSum();
}

void Splay_tree::insert(long key)
{
	Node* f = find_(key, root_);
	if (f == nullptr || f->key_ != key) // unique insert
		insert_(key, root_);
	else
		splay(f); // just splay on top
}

void Splay_tree::insert_(long key, Node * root)
{
	if (root == nullptr) // empty tree
	{
		root_ = new Node(key, nullptr);
	}
	else
	{
		if (key < root->key_)
		{
			if (root->left_ == nullptr)
			{
				root->left_ = new Node(key, root);
				splay(root->left_);
			}
			else
				insert_(key, root->left_);
		}
		else
		{
			if (root->right_ == nullptr)
			{
				root->right_ = new Node(key, root);
				splay(root->right_);
			}
			else
				insert_(key, root->right_);
		}
	}
}

bool Splay_tree::find(long key)
{
	Node* x = find_(key, root_);
	splay(x);
	if (x == nullptr)
	{
		return false;
	}
	else
	{
		return x->key_ == key;
	}
}

Splay_tree::Node* Splay_tree::find_(long key, Node * root)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	else if (key == root->key_)
	{
		return root;
	}
	else
	{
		Node* f = nullptr;
		if (key < root->key_)
			f = find_(key, root->left_);
		else
			f = find_(key, root->right_);

		// if key not found - return max key in path
		if (f == nullptr ||
			(f->key_ < key && key < root->key_))
		{
			f = root;
		}

		return f;
	}
}

void Splay_tree::remove(long key)
{
	Node* x = find_(key, root_);
	if (x != nullptr && x->key_ == key) // key found
		remove_(x);
	else
		splay(x); // just splay on top
}

void Splay_tree::remove_(Node* x)
{
	if (x == nullptr)
		return;

	if (x->left_ == nullptr && x->right_ == nullptr && x->parent_ == nullptr)
	{ // root of tree
		delete x;
		root_ = nullptr;
	}
	else
	{
		splay(x);
		root_ = merge(x->left_, x->right_);
		delete x;
	}
}

Splay_tree::Node* Splay_tree::merge(Node* l, Node* r)
{
	if (l == nullptr && r == nullptr)
	{
		return nullptr;
	}
	else if (l == nullptr)
	{
		r->parent_ = nullptr;
		return r;
	}
	else if (r == nullptr)
	{
		l->parent_ = nullptr;
		return l;
	}
	else
	{
		Node* m = maxInTree(l);
		splay(m);
		m->right_ = r;
		r->parent_ = m;
		m->parent_ = nullptr;
		m->updateSum();
		return m;
	}
}

Splay_tree::Node* Splay_tree::maxInTree(Node* root)
{
	if (root == nullptr) // empty tree
		return nullptr;

	if (root->right_ != nullptr)
		return maxInTree(root->right_);
	else
		return root;
}

void Splay_tree::print_(Node* root)
{ // for console debug
	if (root == nullptr)
		return;

	print_(root->left_);
	cout << '(' << root->key_ << ":s=" << root->sum_ << ", p=" << root->parent_
		<< ", l=" << root->left_ << ", r=" << root->right_ << ", me=" << root << ")\n";
	print_(root->right_);
}

pair<Splay_tree::Node*, Splay_tree::Node*> Splay_tree::split(Node* root, long key)
{
	Node* x = find_(key, root);
	splay(x);
	if (x == nullptr) // empty tree
	{
		return { nullptr, nullptr };
	}
	else
	{
		if (x->key_ < key) // whole tree less than key
		{
			return { x, nullptr };
		}
		else
		{
			if (x->left_ != nullptr)
				x->left_->parent_ = nullptr;
			Node * less_x = x->left_;
			x->left_ = nullptr;
			x->updateSum();
			return { less_x, x };
		}
	}
}

long Splay_tree::sum(long l, long r)
{
	auto less_and_x = split(root_, l);
	if (less_and_x.second == nullptr) // range not found
	{
		root_ = less_and_x.first;
		return 0;
	}
	else
	{
		auto x_and_greater = split(less_and_x.second, r + 1);
		if (x_and_greater.first == nullptr) // range not found
		{
			root_ = merge(less_and_x.first, merge(x_and_greater.first, x_and_greater.second));
			return 0;
		}
		else
		{
			long sum = x_and_greater.first->sum_;
			root_ = merge(less_and_x.first, merge(x_and_greater.first, x_and_greater.second));
			return sum;
		}
	}
}

void Splay_tree::Node::updateSum()
{
	sum_ = key_;
	if (left_ != nullptr)
		sum_ += left_->sum_;
	if (right_ != nullptr)
		sum_ += right_->sum_;
}
