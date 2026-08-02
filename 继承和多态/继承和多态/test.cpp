#include<stdio.h>
#include<string>
#include<iostream>


using namespace std;
//class Person
//{
//public:
//	Person(string home)
//		:_home(home)
//	{
//		_age = 18;
//		_name = "ZhangSan";
//		cout << "Person()" << endl;
//	}
//	~Person()
//	{
//		cout << "~Person()" << endl;
//	}
//protected:
//	int _age;
//	string _name;
//	const string _home;
//};
//
//
//class Student :public Person
//{
//public:
//	Student()
//		//子类要调用父类的初始化构造必须要在初始化列表
//		:Person("child")
//	{
//		cout << "Student()" << endl;
//		_name = "childName";
//	}
//
//	~Student()//子类的析构函数会自动调用父类第析构函数,这是为了保证先子类后父类的析构顺序(同样构造函数必须保证先父后子)
//	//这是因为,子类中可能会存在访问父类的操作,为避免误操作,所以把他设置为最后自动调用,同时父类和子类的析构都叫destructure
//	{
//		cout << "~Student() " << endl;
//	}
//	void changeName()
//	{
//		_name = "ZhangSi";
//	}
//	const string& getFatherName()
//	{
//		return Person::_name;
//	}
//	const string& getName()
//	{	
//		return _name;
//	}
//	const string& getHome()
//	{
//		return _home;
//	}
//protected:
//	string _name;
//
//
//};
//int main()
//{
//	Student s;
//	cout << s.getName() << endl;
//	s.changeName();
//	cout << s.getName() << endl;
//	cout << s.getFatherName() << endl;
//	cout << s.getHome() << endl;
//	
//}

//关于多态
//1.动态/静态多态
//`	动态多态:指的是类的重写,这个是发生在运行时期的,具体表现为:编译时前不会生成函数执行代码,而是生成查虚表->在执行对应虚表函数的逻辑
//	静态多态:指的是重载和模板这一类,发生在编译时期,他们已经生成了固定执行代码
//2.动态/静态绑定:是动/静多态的实现,对于查虚函数表运行是才确认的,我们叫做动态绑定,对于编译时就确定的叫做静态绑定
//3.析构函数的虚化
//	当出现这种情况(B是A的子类):
//	A* p1 = new A;
//	A* p2 = new B;
//	delete p2;
//		释放p2时会去调用析构函数,但这个时A类啊,析构只会调用A的析构,为了解决这个问题,我们对要求父类的析构是必须重写的)
//这也是为什么要将类的析构函数底层命名为destructer就是为了能够触发多态!
//
//
//class Person
//{
//public:
//	string name;
//	virtual void getName()
//	{
//		cout << "Person" << endl;
//	}
//};
//
//class Student:public Person
//{
//public:
//	void getName()
//	{
//		cout << "Student" << endl;
//	}
//};
//void getName(Person& ptr)
//{
//	ptr.getName();
//}
//int main()
//{
//	Student s;
//	Person p;
//	//子类向父类转换时发生切片
//	getName(s);
//	getName(p);
//
//	return 0;
//}
//	这里用一个题目来深入理解多态
//1,多态不是单纯的将父类的成员拷给子类,可以想象为先创建了父类然后再补充了子类的成员,他们使用地址偏移来访问.
//2.多态重写的函数部分是函数的定义部分,也就是说函数的定义仍然沿用父类虚函数.
class A
{
public:
	virtual void func(int val = 1) { std::cout << "A->" << val << std::endl; }
	virtual void test() { func(); }
};
class B : public A
{
public:
	void func(int val = 0) { std::cout << "B->" << val << std::endl; }
};
int main(int argc, char* argv[])
{
	B* p = new B;
	p->test();
	//	答案是D,首先子类会去调用父类的成员函数test,然后test中调用了func(),func是虚函数,再编译时期是没有生成执行代码
	//只生成执行逻辑的,所以会去查虚函数表,触发多态,所以使用B中重写的fun,但是多态生效的范围是函数的定义,所以缺省参数任用父类的1
	return 0;
}