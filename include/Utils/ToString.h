/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>

namespace Utils
{

template<class T>
inline std::string ToString(const T &Var)
{
    std::ostringstream sstrm;
    sstrm << Var;
    return sstrm.str();
}

template<>
inline std::string ToString(const std::wstring &Var)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.to_bytes(Var);
}

template<class T>
inline std::wstring ToWString(const T &Var)
{
    std::wstringstream sstrm;
    sstrm << Var;
    return sstrm.str();
}

template<>
inline std::wstring ToWString(const std::string &Var)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.from_bytes(Var);
}

template<class T, typename... Rest> 
inline std::string ToString(const T &Var, const Rest&... Args)
{
    std::ostringstream sstrm;
    sstrm << Var << ToString(Args...);
    return sstrm.str();
}

template<class T, typename... Rest> 
inline std::wstring ToWString(const T &Var, const Rest&... Args)
{
    std::wstringstream sstrm;
    sstrm << Var << ToWString(Args...);
    return sstrm.str();
}

}