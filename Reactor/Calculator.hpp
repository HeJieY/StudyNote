#pragma once

#include "Protocol.hpp"
#include <iostream>
#include <string>

class Calculator
{
public:
    Response Execute(const Request &req)
    {
        Response resp;
        printf("%d %d %d = \n",req._x,req._op,req._y);
        switch (req._op)
        {
        case '+':
            resp._result = req._x + req._y;
            break;
        case '-':
            resp._result = req._x - req._y;
            break;
        case '*':
            resp._result = req._x * req._y;
            break;
        case '/':
        {
            if (req._y == 0)
                resp._status = 1; // div error
            else
                resp._result = req._x / req._y;
        }
        break;
        case '%':
        {
            if (req._y == 0)
                resp._status = 2; // mod error
            else
                resp._result = req._x % req._y;
        }
        break;
        default:
            resp._status = 3; // 非法操作
            break;
        }
        return resp;
    }
};