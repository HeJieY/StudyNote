#pragma once
#include<iostream>
#include<jsoncpp/json/json.h>
#include<string>
#include<functional>
class Request
{
public:
    Request(int x,int y,char op)
        :_x(x)
        ,_y(y)
        ,_op(op)
    {

    }
    ~Request()
    {

    }
    bool serialize(std::string* out)
    {
        Json::Value root;
        root["left"] = _x;
        root["right"] = _y;
        root["op"] = _op;
        Json::StyledWriter writer;
        *out = writer.write(root);
        return true;
    }
    bool deSerialize(std::string& in)
    {
        Json::Value root;
        Json::Reader reader;
        if(!reader.parse(in,root))
        {
            //失败
            return false;
        }
        _x = root["left"].asInt();
        _y = root["right"].asInt();
        _op = root["op"].asInt();
    }
public:
    int _x;
    int _y;
    char _op;
};



class Response
{
public:
    Response()
    {

    }
    ~Response()
    {

    }
    bool serialize(std::string* out)
    {
        Json::Value root;
        root["result"] = _result;
        root["stutus"] = _status;
        Json::StyledWriter writer;
        *out = writer.write(root);
        return true;
    }
    bool deSerialize(const std::string& in)
    {
        Json::Value root;
        Json::Reader reader;
        if(!reader.parse(in,root))
        {
            //失败
            return false;
        }
        _result = root["result"].asInt();
        _status = root["status"].asInt();
        return true;
    }
public:
    int  _result;
    int _status;

};
using HandlerResponse = std::function<void(Request&)>;
using HandlerRequest =  std::function<Response&(Request&)>;
const std::string gSeq = "\r\n";
class Protocol
{
public:
    Protocol(HandlerRequest handler)
        :_protocolVersion("1.0")
        ,_handlerRequest(handler)
    {

    }
    Protocol(HandlerResponse handler)
        :_protocolVersion("1.0")
        ,_handlerResponse(handler)
    {

    }
    std::string packet(const std::string& str)
    {
        return std::to_string(str.size()) + gSeq +str + gSeq;
    }
    //ret < 0  : error | jsonStr == null
    //ret == 0 : no error | packet == null
    //ret > 0  : no error 
    int unPacket(std::string& packet,std::string* jsonStr)
    {
        if(packet.empty())
            return 0;
        if(jsonStr == nullptr)
            return -1;
        auto pos = packet.find(gSeq);
        if(pos == std::string::npos) 
            return 1;
        //pos;一定指向的是\r,substr是一个前闭后开的函数 
        std::string lenStr = packet.substr(0,pos);
        auto len = std::stoi(lenStr);//计算出有效报文的实际大小
        int total = lenStr.size() + gSeq.size() + len + gSeq.size();
        if(packet.size() < total)
            return 1;
        *jsonStr = packet.substr(pos+gSeq.size(),len);
        packet.erase(0,total);
        return 1;
    }

    void parseResponse()
    {

    }
    
    void parseRequest()
    {

    }
private:
    std::string _protocolVersion;
    HandlerRequest _handlerRequest;
    HandlerResponse _handlerResponse;
};