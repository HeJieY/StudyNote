#include<memory>
#include<iostream>

//RAII:Resource Acquisition Ls Initialization->获取资源初始化后交给另一个对象智能管理(自动释放资源,异常,锁,指针)
//	1.auto_ptr/auto_ptr_ref:拷贝是一种管理权转移,也就是说:当把ptr1给ptr2时(使用auto_ptr)
//本意时ptr1和ptr2共同管理,但这里会把ptr1的管理的资源直接交给ptr2,就导致了ptr1的悬空
//	2.unique_ptr:唯一指针,不支持拷贝,只支持移动可以使用move将资源继续交换也就是资源的移动
//	3.shared_ptr:可以拷贝可以移动
//原理:引用计数来记录资源被试用的情况,当最后一个指向它消失后,便将资源进行释放,使用use_count可以看使用的情况
//此外,他们都重载了operator bool(允许将自定义类转化为bool值)
// shared_ptr再某些特殊情况下可能管理的指针和存储的指针不是一个,具体情况再看
//	4.weak_ptr:不支持RAII,解决的时shared_ptr的循环引用问题
//	5.如果是数组的话要这么写unique_ptr<Data[]>,shared_ptr后面支持这么使用了,老版本使用的是
//shared_ptr<Date>(new Date[10],一个仿函数>,这个叫做定制删除器,释放资源的时候会掉调用对应的仿函数,还有一些情况,
//资源不是指针,譬如是一个FILE*,可以是用仿函数+close关闭,这个部分unique_ptr把它写成了模板参数
//unique_ptr<Data,decltype(del)> uq3(new Data[5],del);decltype的作用是进行推导,自动推导出相应的类型
//	make_shared->底层自己new,是一个可变阐述模板	auto sp = make_shared<class ?>(?,?);

//模拟实现的要点
//	shared_ptr
//	1.引用计数只能用指针来表示,static是不行的,析构的时候对引用计数--,直到0的时候可以进行资源的释放
//	2.拷贝的时候记得对引用计数进行++
//	3.关于赋值operator=:
//		对于被赋值的对象,要先对其计数进行--,是否为0再决定是否释放资源,当然还要对赋值对象的引用计数++
//		自己对自己赋值,要直接判断智能指针所指向的资源,而不是变量名是否相等
//	4.关于删除器:可以使用function来包装一下,并且可以用来解决数组初始化问题
// 
//关于shared_ptr的循环引用问题
//	譬如当一个list中的Node,next和prev互相指向的时候
//	指针N1指向N2,N2中的prev指向N1,N1中的next指向N2,当主函数执行完毕N1释放,计数-1,还剩1,N2释放结束-1还剩1,结束了
//	有一个智能指针的成员互相指向,就会出现循环引用->weak_ptr
//关于weak_ptr的实现                                                                                                                                                                   	
