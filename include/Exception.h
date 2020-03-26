/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once 
#include <string>

class Exception
{
private:
    std::string msg;
    std::wstring wmsg;
public:
    Exception(const std::string &Msg) : msg(Msg){}
    Exception(const std::wstring &Wmsg) : wmsg(Wmsg){}
    const std::string &What() const {return msg;}
    const std::wstring &WhatW() const {return wmsg;}
};

#define DECLARE_EXCEPTION(CLASS_NAME)\
struct CLASS_NAME : public Exception\
{\
    CLASS_NAME(const std::string &Msg) : Exception(Msg){}\
    CLASS_NAME(const std::wstring &Wmsg) : Exception(Wmsg){}\
};

#define DECLARE_CHILD_EXCEPTION(CLASS_NAME, BASE_CLASS_NAME)\
struct CLASS_NAME : public BASE_CLASS_NAME\
{\
    CLASS_NAME(const std::string &Msg) : BASE_CLASS_NAME(Msg){}\
    CLASS_NAME(const std::wstring &Wmsg) : BASE_CLASS_NAME(Wmsg){}\
};

DECLARE_EXCEPTION(IOException);