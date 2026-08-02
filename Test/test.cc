#include<stdio.h>
#include<string.h>
int main()
{
	//小端：低字节低地址,大端则相反,在这里，78是低位，所以p指针拿到的如果是低位的话也就是78的话，说明它是小端
	int x = 0x12345678;
	char* p = (char*)(&x);
	if (*p == 0x78)
	{
		printf("该系统是小端\n");
	}
	else
	{
		printf("该系统是大端\n");
	}
	return 0;
}