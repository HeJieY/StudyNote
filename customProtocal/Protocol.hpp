#pragma once
#include<iostream>
#include<jsoncpp/json/json.h>
#include<string>
#include<functional>
#include"log.hpp"
class Request
{
public:
    Request()
    {

    }
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
        return true;
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
using HandlerResponse = std::function<void(Response&)>;
using HandlerRequest =  std::function<Response(Request&)>;
const std::string gSeq = "\r\n";
class Protocol
{
public:
    Protocol()
    {
    
    }
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
        while (true)
        {
            printf("unPacket used\n");
            if (packet.empty())
                return 0;
            if (jsonStr == nullptr)
                return -1;
            auto pos = packet.find(gSeq);
            if (pos == std::string::npos)
                return 0;
            // pos;一定指向的是\r,substr是一个前闭后开的函数
            std::string lenStr = packet.substr(0, pos);
            auto len = std::stoi(lenStr); // 计算出有效报文的实际大小
            int total = lenStr.size() + gSeq.size() + len + gSeq.size();
            if (packet.size() < total)
               return 0;
            *jsonStr = packet.substr(pos + gSeq.size(), len);
            packet.erase(0, total);
            return 1;
        }
    }

    std::string parseRequest(std::string& inbuffer)
    {
        std::string outbuffer =  "";
        while(true)
        {
            printf("parseRequest used\n");
            std::string jsonStr;
            int ret  = unPacket(inbuffer,&jsonStr);
            if(ret < 0 )
            {
                LOG(DEBUG,"Invalid argument!\n");
                return outbuffer;
            }
            if(ret == 0)
            {
                printf("ret==0\n");
                packet(outbuffer);
            }
            Request req;
            req.deSerialize(jsonStr);
            std::cout <<  "this  is jsonStr:" << jsonStr <<  std::endl; 
            std::cout << "this is inbuffer" << inbuffer <<  std::endl;
            Response rsp;
            rsp = _handlerRequest(req);
            printf("答案是%d",rsp._result);
            rsp.serialize(&outbuffer);
            return packet(outbuffer);
        }
    }
   
    std::string parseResponse(std::string& inbuffer)
    {
        std::string outbuffer = "";
        while(true)
        {
            std::string jsonStr;
            int ret  = unPacket(inbuffer,&jsonStr);
            if(ret < 0 )
            {
                LOG(DEBUG,"Invalid argument!\n");
                return outbuffer;
            }
            if(ret == 0)
                return outbuffer;
            Response rsp;
            rsp.deSerialize(jsonStr);
            if (_handlerResponse)
                _handlerResponse(rsp);
        }
    }
private:
    std::string _protocolVersion;
    HandlerRequest _handlerRequest;
    HandlerResponse _handlerResponse;
};