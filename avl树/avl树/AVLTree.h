#pragma once
#include<map>
#include<iostream>
#include<cstdio>
using namespace std;
template<class K,class V>
struct AVLTreeNode
{
	pair<K, V> _kv;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;//存在是为了方便查找祖先用来更新平衡因子
	int _bf;//平衡因子
	
	AVLTreeNode(const pair<K, V>& kv)
		:_kv(kv)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _bf(0)
	{

	}
};
template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	bool insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			return 1;
		}
		Node* cur = _root;
		Node* parent = cur;
		while (cur != nullptr)
		{
			if (kv.first > cur->_kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kv.first < cur->_kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return 0;
			}
		}
		cur = new Node(kv);
		cur->_parent = parent;
		if (parent->_kv.first > cur->_kv.first)
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}
		//平衡因子更新原则:插入左节点那么平衡因子减1,插入右节点平衡因子加1
		//平衡因子 = 右子树高度-左子树高度
		while (parent!=nullptr)
		{
			//对父亲节点的平衡因子进行修改 
			if (parent->_left = cur)
			{
				parent->_bf++;
			}
			else
			{
				parent->_bf--;
			}
			if (parent->_bf == 0)
			{
				//不会影响上层祖先
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				//会影响上层祖先,继续网上进行更新
				cur = cur->_parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				//进行旋转操作:让旋转的树从不平衡变得平衡,然后降低旋转树的高度 
				//	1.单右旋:失衡节点的左子树的右子树赋值给失衡节点的左子树.失衡节点变为原失衡节点左子树的右子树
				//原失衡节点的左子树变成根.
				if ()
				{
					
				}
			}
			else
			{
				assert(false);
			}

		}
		return 1;
	}
private:
	Node* _root = nullptr;
};
