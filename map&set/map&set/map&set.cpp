#include<stdio.h>
#include<iostream>
#include<set>
#include<map>
#include<functional>
//1.set底层是红黑树是一个具有保持平衡功效的搜索二叉树
//2.set是key树,set英文名是集合,表示的是有限不重复
//3.set默认是走less逻辑的,遍历走中序遍历,所以默认遍历就是从小到大
//4.效率是logN
void test_set1()
{
	std::set<int> set = { 1,7,4,6,2,8 };
	std::set<int>::iterator it = set.begin();
	while (it != set.end())
	{
		
		std::cout << *it << " ";
		it++;
	}
}
void test_set2()
{
	std::set<int> set = { 1,7,4,3,6,2,8,9,12,17 };
	auto it1 = set.lower_bound(3);//找到第一个不小于3的迭代器
	auto it2 = set.upper_bound(9);//upper_bound找到第一个大于9的迭代器,这里就是说返回的是大于9的下一个迭代器;
	//	所有容器迭代器涉及时都为左闭右开区间,原因时迭代器不可以比较大小只可以看是否相等,这就意味着
	//只能使用!=这种比较方式,也就是无法到达it2;
	while (it1 != it2)
	{
		std::cout << *it1 << " ";
		it1++;
	}
	
}
void test_map1()
{
	std::map < std::string, std::string> map = { {"left","左边"} };
	map.insert(std::make_pair("const", "常量"));
	std::pair<std::string, std::string > pair("sort", "排序");
	map.insert(pair);
	map.insert({ "string","字符串" });//隐式类型转化
	//key不可以修改,value可以修改,不会影响树的结构
}
int main()
{
	//test_set1();
	//test_set2();

	return 0;
}