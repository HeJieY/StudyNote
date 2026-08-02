#include"BSTree.h"
int main()
{
	BSTree<int> bsTree;
	bsTree.insert(8);
	bsTree.insert(3);
	bsTree.insert(1);
	bsTree.insert(10);
	bsTree.insert(1);
	bsTree.insert(6);
	bsTree.insert(4);
	bsTree.insert(7);
	bsTree.insert(14);
	bsTree.insert(13);

	
	bsTree.inOrderPrint();
	std::cout << std::endl;
	std::cout << bsTree.find(9) << std::endl;
	return 0;
	
}
