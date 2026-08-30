#include<jsoncpp/json/json.h>
#include<iostream>
#include<string>
int main()
{
    //方法一
    // Json::Value root;
    // root["name"] = "张三";
    // root["sex"] = "男";
    // std::string s = root.toStyledString();
    // std::cout << s <<std::endl;
    //方法二
    // Json::Value root;
    // root["name"] = "zhangsan";
    // Json::StyledWriter writer; //为字符串添加风格
    // std::string s = writer.write(root);
    // Json::FastWriter fastWriter;//直接一行输出
    //反序列化
    Json::Reader reader;
    Json::Value root;
    reader.parse("zhangsan",root);
    //str中得数据会递交给root
     
    return 0;
}