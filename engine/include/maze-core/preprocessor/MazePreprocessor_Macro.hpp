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
#if (!defined(_MazePreprocessor_Macro_hpp_))
#define _MazePreprocessor_Macro_hpp_


//////////////////////////////////////////
// Macro operations
//
//////////////////////////////////////////
#define MAZE_EXPAND(__x)                         __x


//////////////////////////////////////////
#define MAZE_EXPAND_ARGS(...)                   MAZE_EXPAND(__VA_ARGS__)


//////////////////////////////////////////
// String value
#define MAZE_STRINGIFY(...)                       #__VA_ARGS__
#define MAZE_STRINGIFY_WIDE(...)                  L#__VA_ARGS__
#define MAZE_STRINGIFY_ARG(x)                     MAZE_STRINGIFY(x)
#define MAZE_STRINGIFY_ARG_WIDE(x)                MAZE_STRINGIFY_WIDE(x)


//////////////////////////////////////////
// Split special tokens
#define MAZE_MACRO_SPLIT_COMMA(__a, __b)                    __a,__b
#define MAZE_MACRO_SPLIT_COMMA3(__a, __b, __c)              __a,__b,__c
#define MAZE_MACRO_SPLIT_COMMA4(__a, __b, __c, __d)         __a,__b,__c,__d
#define MAZE_MACRO_SPLIT_POINT(__a, __b)                    __a.__b
#define MAZE_MACRO_SPLIT_POINT_STR(__a, __b)                MAZE_STRINGIFY(__a.__b)
#define MAZE_MACRO_SPLIT_SLASH(__a, __b)                    __a/__b
#define MAZE_MACRO_SPLIT_SLASH3(__a, __b, __c)              __a/__b/__c
#define MAZE_MACRO_SPLIT_SLASH4(__a, __b, __c, __d)         __a/__b/__c/__d
#define MAZE_MACRO_SPLIT_SLASH5(__a, __b, __c, __d, __e)    __a/__b/__c/__d/__e
#define MAZE_MACRO_SPLIT_SLASH_STR(__a, __b)                MAZE_STRINGIFY(__a/__b)
#define MAZE_MACRO_SPLIT_SLASH3_STR(__a, __b, __c)          MAZE_STRINGIFY(__a/__b/__c)
#define MAZE_MACRO_SPLIT_SLASH4_STR(__a, __b, __c, __d)     MAZE_STRINGIFY(__a/__b/__c/__d)
#define MAZE_MACRO_SPLIT_SLASH5_STR(__a, __b, __c, __d)     MAZE_STRINGIFY(__a/__b/__c/__d/__e)


//////////////////////////////////////////
// Macro combine
#define MAZE_CONCAT(__a, __b)                               __a __b
#define MAZE_MACRO_COMBINE(__a, __b)                        MAZE_MACRO_COMBINER(__a, __b)
#define MAZE_MACRO_COMBINER(__a, __b)                       __a##__b
#define MAZE_MACRO_COMBINE_STR(__a, __b)                    MAZE_MACRO_COMBINER_STR(__a, __b)
#define MAZE_MACRO_COMBINER_STR(__a, __b)                   MAZE_STRINGIFY(__a##__b)

#define MAZE_MACRO_COMBINE3(__a, __b, __c)                  MAZE_MACRO_COMBINER3(__a, __b, __c)
#define MAZE_MACRO_COMBINER3(__a, __b, __c)                 __a##__b##__c
#define MAZE_MACRO_COMBINE3_STR(__a, __b, __c)              MAZE_MACRO_COMBINER3_STR(__a, __b, __c)
#define MAZE_MACRO_COMBINER3_STR(__a, __b, __c)             MAZE_STRINGIFY(__a##__b##__c)

#define MAZE_MACRO_COMBINE4(__a, __b, __c, __d)             MAZE_MACRO_COMBINER4(__a, __b, __c, __d)
#define MAZE_MACRO_COMBINER4(__a, __b, __c, __d)            __a##__b##__c##__d
#define MAZE_MACRO_COMBINE4_STR(__a, __b, __c, __d)         MAZE_MACRO_COMBINER4_STR(__a, __b, __c, __d)
#define MAZE_MACRO_COMBINER4_STR(__a, __b, __c, __d)        MAZE_STRINGIFY(__a##__b##__c##__d)

#define MAZE_MACRO_COMBINE5(__a, __b, __c, __d, __e)        MAZE_MACRO_COMBINER5(__a, __b, __c, __d, __e)
#define MAZE_MACRO_COMBINER5(__a, __b, __c, __d, __e)       __a##__b##__c##__d##__e
#define MAZE_MACRO_COMBINE5_STR(__a, __b, __c, __d, __e)    MAZE_MACRO_COMBINER5_STR(__a, __b, __c, __d, __e)
#define MAZE_MACRO_COMBINER5_STR(__a, __b, __c, __d, __e)   MAZE_STRINGIFY(__a##__b##__c##__d##__e)
    

//////////////////////////////////////////
#define MAZE_BIT(__bit)                                     (1 << (__bit))
#define MAZE_BIT_S32(__bit)                                 (1 << (__bit))
#define MAZE_BIT_U32(__bit)                                 (1u << (__bit))
    
    
//////////////////////////////////////////
#define MAZE_MACRO_FUNC_CHOOSER16(__f1, __f2, __f3, __f4, __f5, __f6, __f7, __f8, __f9, __f10, __f11, __f12, __f13, __f14, __f15, __f16, __N, ...) __N
#define MAZE_MACRO_FUNC_RECOMPOSER16(__argsWithParentheses) MAZE_MACRO_FUNC_CHOOSER16 __argsWithParentheses
#define MAZE_MACRO_CHOOSE_FROM_ARG_COUNT(__arg, ...) MAZE_MACRO_FUNC_RECOMPOSER16((__VA_ARGS__, __arg##_16, __arg##_15, __arg##_14, __arg##_13, __arg##_12, __arg##_11, __arg##_10, __arg##_9, __arg##_8, __arg##_7, __arg##_6, __arg##_5, __arg##_4, __arg##_3, __arg##_2, __arg##_1,))


#endif // _MazePreprocessor_Macro_hpp_
//////////////////////////////////////////
