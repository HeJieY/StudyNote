#pragma once
#include<cstdio>
#include<iostream>
#include<string>
#include<cstring>

//struct默认继承public,工程文件中,struct默认只存数据,同时访问权限也是public;
template<typename K>
struct BSTNode
{
	BSTNode<K>* _left;
	BSTNode<K>* _right;
	K _val;
	//	这里使用const,那么根据引用是对变量取别名本质是指针,外部如果直接填写一个1(如insert(1)),会报错,加上const就可以了,为什么?
	//这是c++开的特列,c++认为临时的短期变量不配有修改的权限,此外,这个临时变量的生命周期是整个函数.
	BSTNode(const K& val):
		_left(nullptr),
		_right(nullptr),
		_val(val)
	{

	}

	~BSTNode()
	{
			
	}
};




template<typename K>
class BSTree
{

	typedef BSTNode<K> Node;
public:
	BSTree()
		:_root(nullptr)
	{

	}
	bool insert(K val)
	{
		if (_root == nullptr)
		{
			_root = new Node(val);
			return true;
		}
		Node* cur = _root;
		//递归写法
		//非递归写法
		while (true)
		{
			Node* parent = cur;
			if (val > cur->_val)
			{
				//cur = cur->_right;//这种写法是错的,表面上看cur指向了root的right地址,改变cur就是改变root的right,但是,这里的root
				//的right是一个nullptr,是0地址位,你对它进行操作,并不会对原来产生影响.
				parent = cur;
				cur = cur->_right;
			}
			else if (val == cur->_val)
			{
				return false;
			}
			else
			{
				//cur = cur->_left;
				parent = cur;
				cur = cur->_left;
			}
			if (cur == nullptr)
			{	
				if (val > parent->_val)
				{
					parent->_right = new Node(val);
				}
				else
				{
					parent->_left = new Node(val);
				}
				break;
			}
		}
	}
	void inOrderPrint()
	{
		_inOrderPrint(_root);
	}
	
	bool pop(const K& key)
	{
		//三种情况:
		//	删除的节点没有孩子:直接删除该节点
		//	删除的节点只有一个孩子:直接把孩子托付给被删除节点的父亲
		//	删除的节点有多个孩子:替代法,找左子树的最大值或者右子树的最小值(满足被删除节点大于左子树小于右子树)
		Node* pop = _root;
		Node* parent = _root;

		while (pop != nullptr)
		{
			if (key > pop->_val)
			{
				parent = pop;
				pop = pop->_right;
			}
			else if (key < pop->_val)
			{
				parent = pop;
				pop = pop->_left;
			}
			else if (key == pop->_val)
			{
				break;
			}
		}
		if (pop == nullptr)
		{
			return false;
		}	
		//第一种情况和第二种情况可以归在一起
		if (pop->_left == nullptr)
		{
			//左子树为空,右子树交给父亲
			if (parent->_left == pop)
			{
				parent->_left = pop->_right;
			}
			else
			{
				parent->_right = pop->_right;
			}
			delete pop;
		}
		else if (pop->_right == nullptr)
		{
			if (parent->_left == pop)
			{
				parent->_left = pop->_left;
			}
			else
			{
				parent->_right = pop->_left;
			}
			delete pop;
		}
		else
		{
			Node* minRight = pop->_right;
			Node* minParent = minRight;
			while (minRight->_left != nullptr)
			{
				minParent = minRight;
				minRight = minRight->_left;
			}
			//交换两个节点的值然后进行释放
			
			valSwap(minRight, pop);
			delete minRight;
			minParent->_left = nullptr;
		}
		
	}
	bool find(const K& key)
	{
		Node* cur = _root;
		while (cur != nullptr)
		{
			if (key > cur->_val)
			{
				cur = cur->_right;
			}
			else if (key < cur->_val)
			{
				cur = cur->_left;
			}
			else if (key == cur->_val)
			{
				return true;
			}
		}
		return false;
	}

private:
	Node* _root;

	bool valSwap(Node*& minRight, Node*& pop)
	{
		K tmp = pop->_val;
		pop->_val = minRight->_val;
		minRight->_val = tmp;
	}
	Node& leftOrRight(Node& parent,Node& pop)
	{
		if (parent->_left == pop)
		{
			return parent._left;
		}
		else
		{
			return parent->_right;
		}
	}

	bool _inOrderPrint(Node* root)
	{	
		if (root == nullptr)
		{
			return false;
		}
		_inOrderPrint(root->_left);
		printf("%d ", root->_val);
		_inOrderPrint(root->_right);
		return false;

	}

	bool _preOrderPrint(Node* root)
	{
		if (root == nullptr)
		{
			return false;
		}
		printf("%d ", root->_val);
		_preOrderPrint(root->_left);
		_preOrderPrint(root->_right);
		return false;
	}
	
	bool _postOrderPrint(Node* root)
	{
		if (root == nullptr)
		{
			return false;
		}
		_postOrderPrint(root->_left);
		_postOrderPrint(root->_right);
		printf("%d ", root->_val);
		return false;
	}
};
