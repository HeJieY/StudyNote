#pragma once
#include<iostream>

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
    bool  serialize(std::string* out)
    {
    }
    bool  deSerialize(std::string& in)
    {

    }
private:

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
    bool  serialize()
    {
    }
    bool  deSerialize()
    {

    }
private:
    int  _result;
    int _status;

};