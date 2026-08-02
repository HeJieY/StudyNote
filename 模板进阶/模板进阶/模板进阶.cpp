#include<stdio.h>
#include<array>
#include<iostream>
#include<string>
using namespace std;
//	1.非模板类型参数:不是模板类型参数,它的作用是在定义类时可以传入一个常量,此外模板参数的缺省只能
//从右往左进行缺省
//		应用:静态数组创建比如array类型
//	2.类的特化(全特化,偏特化)
// 
//template<class T = int, size_t N = 20>
//class Test
//{
//private:
//	int _num[N];
//};
//int main()
//{
//	Test<int, 10> a;
//	Test<> b;
//
//	return 0;
//}


//int main()
//{
//	std::array<int,10> a;
//	int arr[9];
	//	c语言arr采取抽查机制,第一只能查写读是检查不到的,第二是抽查机制:在数组最后标记几个位置被修改
	//表示改过了报错,但是,如果越界过远是检查不出来的.
	//	c++的array对象与c语言的arr解引用有本质区别,array本质是调用operator[]是一个函数,可以在函数内部检查
//}

//template<typename T1,typename T2>
//bool compare(const T1& v1, const T2& v2)//这里const修饰的是v1这个变量,也就是新创建的变量
//{
//	cout << typeid(v1).name() << endl;
//	cout << (size_t)v1 <<" " << (size_t)v2 << endl;
//	return (size_t)v1 > (size_t)v2;
//}
// 
//  const int* x;	修饰的是int*指向的内容
//	int const *x;   修饰的是int*指向的内容
//	int* const x;   修饰的是x 这个指针变量不可再变


//	compare<int*>(const int*& v1,const int*& v2);  const在*左边修饰的是指针指向的内容不可改变,要把
//const移动到内部,int*& 相当于对一个指针取了别名;
//特化
//template<>
//bool compare<int*,char*>(int* const& v1,char* const& v2)
//{
//	return *v1 > *v2;
//}
//偏特化:函数模板不支持类模板才支持
//template<class T1,class T2>
//class A
//{
//public:
//	A(T1 v1,T2 v2)
//	{
//		cout << "使用了全特化" << endl;
//	}
//};
//
//template<class T>
//class A<int,T>
//{
//public:
//	A(int v1,T v2)
//	{
//		cout << "使用了偏特化" << endl;
//	}
//};
//int main()
//{
//	//栈由高地址向低地址生长,堆由低地址向高地址生长
//
//	int x = 3;
//	int y = 2;
//	int ret1 = compare(x, y);
//	int ret2 = compare(&x, &y);
//	int ret3 = compare(&x, y);
//	printf("%d", ret1);
//
//	printf("%d", ret2);
//	return 0;
//}

//int main()
//{
//	A<int, int> a(1,2);
//	A<char, int> b('1', 3);
//}
// 

//继承部分
class Person
{
public:


protected:
	int _age;
	string _name;
};

class Student:public Person
{
public:

protected:


};
int main()
{
	
}
