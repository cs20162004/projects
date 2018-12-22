//Alisheri Anvar
#include <iostream>
#include <list>
#include <stdexcept>
using namespace std;


template <typename Elem>
class  LinkedBinaryTree 
{
protected:
	struct Node 
	{
		Elem elt;
		Node* par;
		Node* left;
		Node* right;
		Node() : elt(), par(NULL), left(NULL), right(NULL) {}
	};
public:
	class Position
	{
	private:
		Node* v;
	public:
		Position(Node* _v = NULL) : v(_v) {}
		Elem& operator*() const 	{ return v->elt; }
		Position left() const 		{ return Position(v->left);}
		Position right() const 		{ return Position(v->right);}
		Position parent() const 	{ return Position(v->par);}
		bool isRoot() const		{return v->par == NULL;}
		bool isExternal() const 	{return v->left == NULL && v->right == NULL;}
		bool isNull() const 	{ return v == NULL; }


		friend class LinkedBinaryTree;
	};
	typedef std::list<Position> PositionList;

public:
	LinkedBinaryTree()
	: _root(NULL), n(0) {}
	int size() const	{ return n;}
	bool empty() const	{ return size() == 0;}
	Position root() const 	{
		//if (n == 0)		throw runtime_error("The tree is empty.");
		return Position(_root);
	}
	
	void addRoot() {
		if (n > 0)	throw runtime_error("The tree already has root.");
		_root = new Node; 
		n = 1;
	}
	void expandExternal(const Position& p){
		Node* k = p.v;
		if (p.isNull())	throw runtime_error("The node is NULL");
		if (k->left != NULL || k->right != NULL)	throw runtime_error("The node is internal.");
		k->left = new Node;
		k->left->par = k;
		k->right = new Node;
		k->right->par = k;
		n += 2;
	}
	Position removeAboveExternal(const Position& p){
		if (p.isNull())		throw runtime_error("The node is NULL.");
		Node*w = p.v;
		if (w == _root)		throw runtime_error("The node is a root.");
		if (w->left != NULL || w->right != NULL)	throw runtime_error("The node is internal.");
		if (w->par == NULL)		{ delete w; n--;}
		else {
			Node*k = w->par;
			Node* sib = (w == k->left ? k->right : k->left);
			if (k == _root)	{
				_root = sib;
				sib->par = NULL;
			}
			else {
				Node* gpar = k->par;
				if (k == gpar->left) gpar->left = sib;
				else gpar->right = sib;
				sib->par = gpar;
			}
			delete w; 	delete k;
			n -= 2;
			return Position(sib);
		}
	}
	PositionList positions() const{
		PositionList pl;
		preorder (_root, pl);
		return PositionList(pl);
	}
	int height() const {
		if (root().isNull())	throw runtime_error("The tree is empty.");
		int k = findheight(root());
		/*
		PositionList nodes = this->positions();
		typename std::list<Position>::iterator q;
		for (q = nodes.begin(); q != nodes.end(); ++q){
			if (q->isExternal())
				k = max(k, depth(*q));
		}*/
		return k;
	}
	void attachLeftSubtree(const Position& p, LinkedBinaryTree& subtree){
		if(subtree.root().isNull())		throw runtime_error("The subtree is NULL.");
		if (!p.left().isNull())	throw runtime_error("The left child is not empty.");
		Node* k = p.v; Node* w = subtree.root().v;
		k->left = w;
		w->par = k;
		n += subtree.size();
	}
	void attachRightSubtree(const Position& p, LinkedBinaryTree& subtree){
		if(subtree.root().isNull())		throw runtime_error("The subtree is NULL.");
		if (!p.right().isNull())	throw runtime_error("The right child is not empty.");
		Node* k = p.v; Node* w = subtree.root().v;
		k->right = w;
		w->par = k;
		n += subtree.size();
	}
	void removeSubtree(const Position& p){
		if (p.isNull())	throw runtime_error("The subtree is already empty.");
		if (!p.isNull()){
			removeSubtree(p.left());
			removeSubtree(p.right());
			this->n = this->n - 1;
			Node* w = p.v;
			if (w != _root){
			Node* k = w->par;
			if (w == k->left)		k->left = NULL;
			else k->right = NULL;}
			
			delete w;
			
		}

	}

protected:
	void preorder(Node* v, PositionList& pl) const{
		if (v != 0){
		pl.push_back(Position(v));
		if (v->left != NULL)
			preorder(v->left, pl);
		if (v->right != NULL)
			preorder(v->right, pl);}
	}
	int findheight(Position root) const{
		int left, right;
		if (root.isNull()) 	return -1;
		left = findheight(root.left());
		right = findheight(root.right());
		return max(left, right) + 1;
	}
	

	/*int depth(const Position& p) const{
		if (p.isRoot())		return 0;
		else return (1 + depth(p.parent()));	
	}*/
private:
	Node* _root;
	int n;

};
