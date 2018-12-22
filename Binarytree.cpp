#include <iostream>
#include <stdexcept>
#include <list>
#include <stack>
#include <limits>
#include "LinkedBinaryTree.h"
#include "Symbol.h"
#include "prog.h"


LinkedBinaryTree<Symbol> parsePostfixExpression(list<string> tokens) {
		stack< LinkedBinaryTree<Symbol> > stackoftree;
		std::list<string>::iterator p; 
		LinkedBinaryTree<Symbol> rightsubtree;
		LinkedBinaryTree<Symbol> leftsubtree;
		LinkedBinaryTree<Symbol> tree;
		
		LinkedBinaryTree<Symbol> a;
		LinkedBinaryTree<Symbol> ext;
		
		Symbol sym; 
		for (p = tokens.begin(); p != tokens.end(); p++)
		{
			
			if (Symbol(*p).isConstant() || Symbol(*p).isVariable())	{ 
				
				ext = LinkedBinaryTree<Symbol>();
				ext.addRoot();
				*ext.root() = Symbol(*p);
				stackoftree.push(ext);

			}	
			else if (Symbol(*p).isOperator()){
				if (stackoftree.empty())	throw runtime_error("Invalid Expression.");
				rightsubtree = stackoftree.top(); 		
				stackoftree.pop();
				if (stackoftree.empty())	throw runtime_error("Invalid Expression.");
				leftsubtree = stackoftree.top(); 		
				stackoftree.pop();
				tree = LinkedBinaryTree<Symbol>();
				tree.addRoot();
				*tree.root() = Symbol(*p);
				tree.attachLeftSubtree(tree.root(), leftsubtree);
				tree.attachRightSubtree(tree.root(), rightsubtree);
				stackoftree.push(tree);
			}
			else throw runtime_error("Invalid Expression."); 

		}
		if (stackoftree.size() != 1)	throw runtime_error("Invalid Expression.");
		return stackoftree.top();

}





void inorder(LinkedBinaryTree<Symbol>::Position root){
	if (root.isNull())	return ;
	else 
	{
		if (!root.isExternal()) 	cout << "( ";
		inorder(root.left());
		cout << *root << " ";
		inorder(root.right());
		if (!root.isExternal()) 	cout << ") ";
	}
}
void print_inorder(const LinkedBinaryTree<Symbol>& tree) {
	

	inorder(tree.root());

}





void postorder(LinkedBinaryTree<Symbol>::Position root){
	if (root.isNull())	return ;
	else{
		postorder(root.left());
		postorder(root.right());
		cout << *root << " ";
	}
}
void print_postorder(const LinkedBinaryTree<Symbol>& tree) {
	postorder(tree.root());
}






int depth(const LinkedBinaryTree<Symbol>& tree, const LinkedBinaryTree<Symbol>::Position& pos){
	if (pos.isRoot())		return 0;
	else return (1 + depth(tree, pos.parent()));
}
int findmin(const LinkedBinaryTree<Symbol>& tree, const LinkedBinaryTree<Symbol>::Position& root, const Symbol& sym, int min){
	if (!root.isNull()){
		if (*root == sym){
			if (min >= depth(tree, root))	min = depth(tree,root);
		}else{
		min = findmin(tree, root.left(), sym, min);
		min = findmin(tree, root.right(), sym, min);}

		
	}
	return min;
}
int findMinimumDepth(const LinkedBinaryTree<Symbol>& tree, const Symbol& sym) {
	//if (sym.isConstant() || sym.isOperator())		throw runtime_error("The input is not a variable.");
	int min = tree.height();
	int k = 0;
	std::list<LinkedBinaryTree<Symbol>::Position>::iterator p;
	std::list<LinkedBinaryTree<Symbol>::Position> list;
	list = tree.positions();
	for (p = list.begin(); p != list.end(); p++)
		if (**p == sym)		k = 1;
	if (k == 0)		return -1;
	else 
		return findmin(tree, tree.root(), sym, min);
	
}





void subs(const LinkedBinaryTree<Symbol>::Position& pos, const Symbol& var, const Symbol& consta){
	if (!pos.isNull()){
		if (*pos == var)	*pos = consta;
		subs(pos.left(), var, consta);
		subs(pos.right(), var, consta);
	}
}
void substitute(const LinkedBinaryTree<Symbol>& tree, const Symbol& variable, const Symbol& constant) {
	subs(tree.root(), variable, constant);
}





void simplify(LinkedBinaryTree<Symbol>& tree, const LinkedBinaryTree<Symbol>::Position& root)
{
	if (!root.isNull() && !root.isExternal()){
		simplify(tree, root.left());
		simplify(tree, root.right());
		if ((*root.left()).isConstant() && (*root.right()).isConstant()){
			*root = (*root).compute((*root.left()).getConstant(), (*root.right()).getConstant());
			tree.removeSubtree(root.left());
			tree.removeSubtree(root.right());
		}

	}
}
void simplify_subtree(LinkedBinaryTree<Symbol>& tree) {
		
		simplify(tree, tree.root());


		/*cout << "FGSFDGSFD" << endl;
		LinkedBinaryTree<Symbol>::Position a = tree.root();
		cout << *a << endl;
		a = a.left();
		cout << *a;
		//simplify_subtree(tree); 
		//tree.root() = (tree.root()).right();
		//simplify_subtree(tree);
		
		LinkedBinaryTree<Symbol>::Position	rootd = tree.root();
		if ((*rootd.left()).isConstant() && (*rootd.right()).isConstant()){
			*rootd = (*rootd).compute((*rootd.left()).getConstant(), (*rootd.right()).getConstant());

		}

	}*/

}
