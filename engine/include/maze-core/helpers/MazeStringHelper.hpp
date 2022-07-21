//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#pragma once
#if (!defined(_MazeStringHelper_hpp_))
#define _MazeStringHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstdarg>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace StringHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(Char* _buffer, Size _buflen, CString _format, va_list _args);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WChar* _buffer, Size _buflen, CWString _format, va_list _args);


        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(Char* _buffer, Size _buflen, CString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WChar* _buffer, Size _buflen, CWString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(String& _buffer, CString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WString& _buffer, CWString _format, ...);


        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CString _format, va_list _args);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CWString _format, va_list _args);


        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CWString _format, ...);


        //////////////////////////////////////////
        inline void FormatVAString(CString _text, va_list& _arg, StdString& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        inline void FormatVAString(CString _text, va_list& _arg, String& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        inline void FormatVAString(CWString _text, va_list& _arg, StdWString& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        inline void FormatVAString(CWString _text, va_list& _arg, WString& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        template <
            class TElem,
            class TValue>
        inline TElem* UIntegralToBuff(TElem* _rnext, TValue _value)
        {
            #pragma warning(push)
            #pragma warning(disable: 4127) // Conditional expression is constant

            if (sizeof(TValue) > 4)
            {
                // For 64-bit numbers, work in chunks to avoid 64-bit divisions.
                while (_value > 0xFFFFFFFFU)
                {
                    auto value_chunk = static_cast<U32>(_value % 1000000000);
                    _value /= 1000000000;

                    for (S32 i = 0; i != 9; ++i)
                    {
                        *--_rnext = '0' + value_chunk % 10;
                        value_chunk /= 10;
                    }
                }
            }

            #pragma warning(pop)

            auto valueTrunc = static_cast<U32>(_value);

            do
            {
                *--_rnext = '0' + valueTrunc % 10;
                valueTrunc /= 10;
            }
            while (valueTrunc != 0);

            return _rnext;
        }


        //////////////////////////////////////////
        template <
            class TString,
            class TValue>
        inline TString IntegralToString(TValue const _value)
        {
            using TUValue = typename std::make_unsigned<TValue>::type;

            typename TString::value_type buff[21]; // -2^63 and 2^64 - 1, plus NUL
            typename TString::value_type* const buffEnd = std::end(buff);
            typename TString::value_type* rnext = buffEnd;

            auto uValue = static_cast<TUValue>(_value);

            if (_value < 0)
            {
                rnext = UIntegralToBuff(rnext, 0 - uValue);
                *--rnext = '-';
            }
            else
            {
                rnext = UIntegralToBuff(rnext, uValue);
            }

            return (TString(rnext, buffEnd));
        }

        //////////////////////////////////////////
        template <
            class TString,
            class TValue>
        inline TString FloatingToString(CString _format, TValue _value)
        {
            auto len = FormattedStringSize(_format, _value);
            TString str((Size)len + 1, '\0');
            FormatString(&str[0], (Size)len + 1, _format, _value);
            str.resize(len);
            return str;
        }

        //////////////////////////////////////////
        template <
            class TWString,
            class TValue>
        inline TWString FloatingToWString(CWString _format, TValue _value)
        {
            auto len = FormattedStringSize(_format, _value);
            TWString str((Size)len + 1, L'\0');
            FormatString(&str[0], (Size)len + 1, _format, _value);
            str.resize(len);
            return str;
        }

        //////////////////////////////////////////
        inline bool IsF32Number(StdString const& _s) noexcept
        {
            if (_s.empty())
                return false;

            StdString::const_iterator begin = _s.begin();

            if (*begin == '-')
                ++begin;

            S32 point = 1;
            return  std::find_if(
                begin,
                _s.end(),
                [&point](S8 _c)
            {
                if (_c == '.' && point == 1)
                {
                    point++;
                    return false;
                }
                return !std::isdigit(_c);
            }) == _s.end();
        }

        //////////////////////////////////////////
        inline bool IsF32Number(String const& _s) noexcept
        {
            if (_s.empty())
                return false;

            String::const_iterator begin = _s.begin();

            if (*begin == '-')
                ++begin;

            if (begin == _s.end())
                return false;

            S32 point = 1;
            return  std::find_if(
                begin,
                _s.end(),
                [&point](S8 _c)
            {
                if (_c == '.' && point == 1)
                {
                    point++;
                    return false;
                }
                return !std::isdigit(_c);
            }) == _s.end();
        }

        //////////////////////////////////////////
        inline bool StringToBool(CString _str) noexcept
        {
            return strcmp(_str, "1") == 0 ? true : false;
        }

        //////////////////////////////////////////
        inline bool StringToBool(String const& _str) noexcept
        {
            return (_str == "1");
        }

        //////////////////////////////////////////
        inline S8 StringToS8(CString _str) noexcept
        {
            return static_cast<S8>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline S8 StringToS8(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S8>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline S8 StringToS8(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S8>(std::stoi(_str.c_str()));
        }

        //////////////////////////////////////////
        inline U8 StringToU8(CString _str) noexcept
        {
            return static_cast<U8>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline U8 StringToU8(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U8>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline U8 StringToU8(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U8>(std::stoi(_str.c_str()));
        }

        //////////////////////////////////////////
        inline S16 StringToS16(CString _str) noexcept
        {
            return static_cast<S16>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline S16 StringToS16(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S16>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline S16 StringToS16(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S16>(std::stoi(_str.c_str()));
        }

        //////////////////////////////////////////
        inline U16 StringToU16(CString _str) noexcept
        {
            return static_cast<U16>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline U16 StringToU16(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U16>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline U16 StringToU16(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U16>(std::stoi(_str.c_str()));
        }

        //////////////////////////////////////////
        inline S32 StringToS32(CString _str) noexcept
        {
            return std::atoi(_str);
        }

        //////////////////////////////////////////
        inline S32 StringToS32(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : std::stoi(_str);
        }

        //////////////////////////////////////////
        inline S32 StringToS32(String const& _str) noexcept
        {
            return _str.empty() ? 0 : std::stoi(_str.c_str());
        }

        //////////////////////////////////////////
        inline U32 StringToU32(CString _str) noexcept
        {
            return static_cast<U32>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline U32 StringToU32(StdString const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U32>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline U32 StringToU32(String const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U32>(std::stoi(_str.c_str()));
        }


        //////////////////////////////////////////
        inline U64 StringToU64(CString _str) noexcept
        {
            return static_cast<U64>(std::atoll(_str));
        }

        //////////////////////////////////////////
        inline U64 StringToU64(StdString const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U64>(std::stoull(_str));
        }

        //////////////////////////////////////////
        inline U64 StringToU64(String const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U64>(std::stoull(_str.c_str()));
        }

        //////////////////////////////////////////
        inline S64 StringToS64(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S64>(std::atoll(_str.c_str()));
        }

        //////////////////////////////////////////
        inline F32 StringToF32(CString _str) noexcept
        {
            return _str ? static_cast<F32>(std::atof(_str)) : 0.0f;
        }

        //////////////////////////////////////////
        inline F32 StringToF32(StdString const& _str) noexcept
        {
            return _str.empty() ? 0.0f : std::stof(_str);
        }

        //////////////////////////////////////////
        inline F32 StringToF32(String const& _str) noexcept
        {
            if (_str.empty())
                return 0.0f;

            return std::stof(_str.c_str());
        }

        //////////////////////////////////////////
        inline F32 StringToF32Safe(String const& _str) noexcept
        {
            if (_str.empty() || !IsF32Number(_str))
                return 0.0f;

            return std::stof(_str.c_str());
        }

        //////////////////////////////////////////
        inline F64 StringToF64(CString _str) noexcept
        {
            return _str ? std::atof(_str) : 0.0;
        }

        //////////////////////////////////////////
        inline F64 StringToF64(StdString const& _str) noexcept
        {
            return _str.empty() ? 0.0 : std::stod(_str);
        }

        //////////////////////////////////////////
        inline F64 StringToF64(String const& _str) noexcept
        {
            return std::stod(_str.c_str());
        }

        //////////////////////////////////////////
        inline StdString BoolToStdString(bool _value) noexcept
        {
            return _value ? StdString("1") : StdString("0");
        }

        ////////////////////////////////////
        inline String BoolToString(bool _value) noexcept
        {
            return _value ? String("1") : String("0");
        }

        //////////////////////////////////////////
        inline StdString ToStdString(bool _value) noexcept { return BoolToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(bool _value) noexcept { return BoolToString(_value); }


        //////////////////////////////////////////
        inline StdString ToStdString(CString _value) noexcept { return _value ? StdString(_value)
                                                                              : StdString(); }

        //////////////////////////////////////////
        inline String ToString(CString _value) noexcept { return _value ? String(_value)
                                                                        : String(); }


        //////////////////////////////////////////
        inline StdString U8ToStdString(U8 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String U8ToString(U8 _value) noexcept
        {
            return IntegralToString<String>((S32)_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(U8 _value) noexcept { return U8ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(U8 _value) noexcept { return U8ToString(_value); }


        //////////////////////////////////////////
        inline StdString S32ToStdString(S32 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String S32ToString(S32 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(S32 _value) noexcept { return S32ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(S32 _value) noexcept { return S32ToString(_value); }

        //////////////////////////////////////////
        inline StdString U32ToStdString(U32 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String U32ToString(U32 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(U32 _value) noexcept { return U32ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(U32 _value) noexcept { return U32ToString(_value); }


        //////////////////////////////////////////
        inline StdString U64ToStdString(U64 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String U64ToString(U64 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(U64 _value) noexcept { return U64ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(U64 _value) noexcept { return U64ToString(_value); }

        //////////////////////////////////////////
        inline StdString S64ToStdString(S64 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String S64ToString(S64 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(S64 _value) noexcept { return S64ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(S64 _value) noexcept { return S64ToString(_value); }

        //////////////////////////////////////////
        inline StdString F32ToStdString(F32 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String F32ToString(F32 _value) noexcept
        {
            return FloatingToString<String>("%f", _value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(F32 _value) noexcept { return F32ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(F32 _value) noexcept { return F32ToString(_value); }

        //////////////////////////////////////////
        inline StdString F32ToStdString(F32 _value, S32 _precision)
        {
            S32 s32 = S32(_value);
            if (_value == F32(s32))
                return S32ToStdString(s32);

            StdStringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline String F32ToString(F32 _value, S32 _precision)
        {
            S32 s32 = S32(_value);
            if (_value == F32(s32))
                return S32ToString(s32);

            StringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline StdString ToStdString(F32 _value, S32 _precision) noexcept { return F32ToStdString(_value, _precision); }

        //////////////////////////////////////////
        inline String ToString(F32 _value, S32 _precision) noexcept { return F32ToString(_value, _precision); }

        //////////////////////////////////////////
        inline StdString F32ToStdStringFormatted(F32 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%f", _value);
            StdString result(buff);
            if (result.find('.') != StdString::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, StdString::npos);
                result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            }
            return result;
        }

        //////////////////////////////////////////
        inline String F32ToStringFormatted(F32 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%f", _value);
            String result(buff);

            if (result.find('.') != String::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, String::npos);
                result.erase(result.find_last_not_of('.') + 1, String::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline StdString F32ToStdStringFormatted(F32 _value, S32 _precision) noexcept
        {
            StdString result = F32ToStdString(_value, _precision);

            if (result.find('.') != StdString::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, StdString::npos);
                result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline String F32ToStringFormatted(F32 _value, S32 _precision) noexcept
        {
            String result = F32ToString(_value, _precision);

            if (result.find('.') != String::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, String::npos);
                result.erase(result.find_last_not_of('.') + 1, String::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline StdString F64ToStdString(F64 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String F64ToString(F64 _value) noexcept
        {
            return FloatingToString<String>("%f", _value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(F64 _value) noexcept { return F64ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(F64 _value) noexcept { return F64ToString(_value); }

        //////////////////////////////////////////
        inline StdString F64ToStdString(F64 _value, S32 _precision)
        {
            S64 s64 = S64(_value);
            if (_value == F64(s64))
                return S64ToStdString(s64);

            StdStringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline String F64ToString(F64 _value, S32 _precision)
        {
            S64 s64 = S64(_value);
            if (_value == F64(s64))
                return S64ToString(s64);

            StringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline StdString F64ToStdStringFormatted(F64 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%9.9f", _value);
            StdString result(buff);
            result.erase(result.find_last_not_of('0') + 1, StdString::npos);
            result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            return result;
        }

        //////////////////////////////////////////
        inline String F64ToStringFormatted(F64 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%9.9f", _value);
            String result(buff);
            result.erase(result.find_last_not_of('0') + 1, String::npos);
            result.erase(result.find_last_not_of('.') + 1, String::npos);
            return result;
        }

        //////////////////////////////////////////
        inline StdString F64ToStdStringFormatted(F64 _value, S32 _precision) noexcept
        {
            StdString result = F64ToStdString(_value, _precision);
            if (result.find('.') != StdString::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, StdString::npos);
                result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline String F64ToStringFormatted(F64 _value, S32 _precision) noexcept
        {
            String result = F64ToString(_value, _precision);
            if (result.find('.') != String::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, String::npos);
                result.erase(result.find_last_not_of('.') + 1, String::npos);
            }

            return result;
        }


#if (((MAZE_COMPILER == MAZE_COMPILER_CLANG) || (MAZE_COMPILER == MAZE_COMPILER_GNUC)) && (MAZE_PLATFORM != MAZE_PLATFORM_ANDROID))

        //////////////////////////////////////////
        inline StdString ToStdString(Size _value) noexcept { return ToStdString(static_cast<U64>(_value)); }

        //////////////////////////////////////////
        inline String ToString(Size _value) noexcept { return ToString(static_cast<U64>(_value)); }

        //////////////////////////////////////////
        inline StdString ToStdString(UnixTime _value) noexcept { return ToStdString(static_cast<U64>(_value)); }

        //////////////////////////////////////////
        inline String ToString(UnixTime _value) noexcept { return ToString(static_cast<U64>(_value)); }

    #endif


        //////////////////////////////////////////
        inline StdString ToStdString(String const& _str)
        {
            return StdString(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline String ToString(StdString const& _str)
        {
            return String(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline StdString ToStdString(String&& _str)
        {
            return StdString(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline String ToString(StdString&& _str)
        {
            return String(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline StdString ToStdString(StdString const& _str)
        {
            return _str;
        }

        //////////////////////////////////////////
        inline String ToString(String const& _str)
        {
            return _str;
        }

        //////////////////////////////////////////
        inline void SplitWords(StdString const& _line, Vector<StdString>& _words, S8 _separator = ' ') noexcept
        {
            _words.clear();
            Size pos = 0;
            Size prevPos = 0;

            while ((pos = _line.find_first_of(_separator, pos)) != StdString::npos)
            {
                _words.push_back(
                    _line.substr(prevPos, pos - prevPos));
                prevPos = ++pos;
            }

            _words.push_back(_line.substr(prevPos, pos - prevPos));
        }

        //////////////////////////////////////////
        inline void SplitWords(String const& _line, Vector<String>& _words, S8 _separator = ' ') noexcept
        {
            _words.clear();
            Size pos = 0;
            Size prevPos = 0;

            while ((pos = _line.find_first_of(_separator, pos)) != String::npos)
            {
                _words.emplace_back(
                    _line.substr(prevPos, pos - prevPos));
                prevPos = ++pos;
            }

            _words.push_back(_line.substr(prevPos, pos - prevPos));
        }

        //////////////////////////////////////////
        inline void SplitWords(WString const& _line, Vector<WString>& _words, WChar _separator = ' ') noexcept
        {
            _words.clear();
            Size pos = 0;
            Size prevPos = 0;

            while ((pos = _line.find_first_of(_separator, pos)) != String::npos)
            {
                _words.emplace_back(
                    _line.substr(prevPos, pos - prevPos));
                prevPos = ++pos;
            }

            _words.push_back(_line.substr(prevPos, pos - prevPos));
        }

        //////////////////////////////////////////
        inline bool IsNumber(StdString const& _s) noexcept
        {
            return !_s.empty()
                && std::find_if(
                    _s.begin(),
                    _s.end(),
                    [](S8 _c)
                    {
                        return !std::isdigit(_c);
                    }) == _s.end();
        }

        //////////////////////////////////////////
        inline bool IsNumber(String const& _s) noexcept
        {
            return !_s.empty()
                && std::find_if(
                    _s.begin(),
                    _s.end(),
                    [](S8 _c)
                    {
                        return !std::isdigit(_c);
                    }) == _s.end();
        }

        //////////////////////////////////////////
        inline void TrimString(StdString& _str, bool _left = true, bool _right = true) noexcept
        {
            static StdString const delims = " \t\r\n";

            if (_right)
                _str.erase(_str.find_last_not_of(delims) + 1);
            if (_left)
                _str.erase(0, _str.find_first_not_of(delims));
        }

        //////////////////////////////////////////
        inline void TrimString(String& _str, bool _left = true, bool _right = true) noexcept
        {
            static String const delims = " \t\r\n";

            if (_right)
                _str.erase(_str.find_last_not_of(delims) + 1);
            if (_left)
                _str.erase(0, _str.find_first_not_of(delims));
        }

        //////////////////////////////////////////
        inline void ReplaceSubstring(StdString& _s, StdString const& _search, StdString const& _replace) noexcept
        {
            for (Size pos = 0, l = _replace.length(); ; pos += l)
            {
                // Locate the substring to replace
                pos = _s.find(_search, pos);
                if (pos == String::npos)
                    break;

                // Replace by erasing and inserting
                _s.erase(pos, _search.length());
                _s.insert(pos, _replace);
            }
        }

        //////////////////////////////////////////
        inline void ReplaceSubstring(String& _s, String const& _search, String const& _replace) noexcept
        {
            for (Size pos = 0, l = _replace.length(); ; pos += l)
            {
                // Locate the substring to replace
                pos = _s.find(_search, pos);
                if (pos == String::npos)
                    break;

                // Replace by erasing and inserting
                _s.erase(pos, _search.length());
                _s.insert(pos, _replace);
            }
        }

        //////////////////////////////////////////
        inline void ReplaceSubstring(WString& _s, WString const& _search, WString const& _replace) noexcept
        {
            for (Size pos = 0, l = _replace.length(); ; pos += l)
            {
                // Locate the substring to replace
                pos = _s.find(_search, pos);
                if (pos == WString::npos)
                    break;

                // Replace by erasing and inserting
                _s.erase(pos, _search.length());
                _s.insert(pos, _replace);
            }
        }

        //////////////////////////////////////////
        inline void RemoveSubstring(StdString& _s, StdString const& _search) noexcept
        {
            for (Size pos = 0; ; pos += 1)
            {
                pos = _s.find(_search, pos);
                if (pos == StdString::npos)
                    break;

                _s.erase(pos, _search.length());
            }
        }

        //////////////////////////////////////////
        inline void RemoveSubstring(String& _s, String const& _search) noexcept
        {
            for (Size pos = 0; ; pos += 1)
            {
                pos = _s.find(_search, pos);
                if (pos == String::npos)
                    break;

                _s.erase(pos, _search.length());
            }
        }

        //////////////////////////////////////////
        inline void RemoveSubstring(WString& _s, WString const& _search) noexcept
        {
            for (Size pos = 0; ; pos += 1)
            {
                pos = _s.find(_search, pos);
                if (pos == String::npos)
                    break;

                _s.erase(pos, _search.length());
            }
        }

        //////////////////////////////////////////
        inline void RemoveSymbols(StdString& _s, StdString const& _symbols) noexcept
        {
            for (Size i = 0, in = _symbols.size(); i < in; ++i)
                _s.erase(std::remove(_s.begin(), _s.end(), _symbols[i]), _s.end());
        }

        //////////////////////////////////////////
        inline void RemoveSymbols(String& _s, String const& _symbols) noexcept
        {
            for (Size i = 0, in = _symbols.size(); i < in; ++i)
                _s.erase(std::remove(_s.begin(), _s.end(), _symbols[i]), _s.end());
        }

        //////////////////////////////////////////
        template <typename FloatType = F32>
        inline FloatType ParseFloatFast(CString _text) noexcept
        {
            CString p = _text;

            if (!*p || *p == '?')
                return 0;
            S32 s = 1;

            while (*p == ' ')
                p++;

            if (*p == '-')
            {
                s = -1;
                p++;
            }

            FloatType acc = 0;
            while (*p >= '0' && *p <= '9')
                acc = acc * 10 + *p++ - '0';

            if (*p == '.')
            {
                FloatType k = (FloatType)0.1;
                p++;
                while (*p >= '0' && *p <= '9')
                {
                    acc += (*p++ - '0') * k;
                    k *= (FloatType)0.1;
                }
            }

            return s * acc;
        }

        //////////////////////////////////////////
        inline StdString ToLower(StdString const& _text) noexcept
        {
            StdString copy = _text;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
            return copy;
        }

        //////////////////////////////////////////
        inline String ToLower(String const& _text) noexcept
        {
            String copy = _text;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
            return copy;
        }

        //////////////////////////////////////////
        inline StdString ToUpper(StdString const& _text) noexcept
        {
            StdString copy = _text;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
            return copy;
        }

        //////////////////////////////////////////
        inline String ToUpper(String const& _text) noexcept
        {
            String copy = _text;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
            return copy;
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(StdString const& _value, StdString const& _start) noexcept
        {
            if (_start.size() > _value.size())
                return false;

            return std::equal(_start.begin(), _start.end(), _value.begin());
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(String const& _value, String const& _start) noexcept
        {
            if (_start.size() > _value.size())
                return false;

            return std::equal(_start.begin(), _start.end(), _value.begin());
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(WString const& _value, WString const& _start) noexcept
        {
            if (_start.size() > _value.size())
                return false;

            return std::equal(_start.begin(), _start.end(), _value.begin());
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(CString _value, CString _start) noexcept
        {
            return (strncmp(_value, _start, strlen(_start)) == 0);
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(CWString _value, CWString _start) noexcept
        {
            return (wcsncmp(_value, _start, wcslen(_start)) == 0);
        }

        //////////////////////////////////////////
        inline bool IsEndsWith(StdString const& _value, StdString const& _ending) noexcept
        {
            if (_ending.size() > _value.size())
                return false;

            return std::equal(_ending.rbegin(), _ending.rend(), _value.rbegin());
        }

        //////////////////////////////////////////
        inline bool IsEndsWith(String const& _value, String const& _ending) noexcept
        {
            if (_ending.size() > _value.size())
                return false;

            return std::equal(_ending.rbegin(), _ending.rend(), _value.rbegin());
        }

        //////////////////////////////////////////
        inline bool IsEndsWith(WString const& _value, WString const& _ending) noexcept
        {
            if (_ending.size() > _value.size())
                return false;

            return std::equal(_ending.rbegin(), _ending.rend(), _value.rbegin());
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline void StringToObjectPtr(SharedPtr<TClass>& _value, String const& _str)
        {
            void* ptr;
            std::stringstream ss;
            ss << std::hex << _str;
            ss >> ptr;

            TClass* object = reinterpret_cast<TClass*>(ptr);
            if (object)
                _value = object->template cast<TClass>();
            else
                _value.reset();
        }

        //////////////////////////////////////////
        #define MAZE_FORMAT_VA_STRING(__text, __destString)                         \
        {                                                                           \
            va_list ___args;                                                        \
            va_start(___args, __text);                                              \
            Maze::StringHelper::FormatVAString(__text, ___args, __destString);      \
            va_end(___args);                                                        \
        }


    } // namespace StringHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeStringHelper_hpp_
