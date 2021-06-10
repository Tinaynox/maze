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
#include <android/log.h>
#include <android_native_app_glue.h>
#include <string>


////////////////////////////////////
extern int main(int argc, char const* argv[]);


////////////////////////////////////
void android_main(struct android_app* _state)
{
    // app_dummy();

    std::string stateArg = "--android_app=" + std::to_string((size_t)_state);
    const char* args[] = { stateArg.c_str() };
    main( 1, args );
}