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
#include "maze-core/MazeTypes.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/system/MazeThread.hpp"
#include "maze-core/system/std/MazeThread_std.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"


//////////////////////////////////////////
Maze::S32 SecondThreadEntry()
{
    Maze::String text = "Second Thread!";
    Maze::Debug::log << "Hello from " << text << Maze::endl;

    return 1;
}


//////////////////////////////////////////
Maze::S32 main(Maze::S32 _argc, Maze::S8 const* _argv[])
{

    Maze::Debug::log << Maze::PlatformHelper::ConstructPlatformInfo();
    Maze::Debug::log << Maze::PlatformHelper::ConstructEngineInfo();
    Maze::Debug::log << Maze::endl;

    Maze::Thread thread(SecondThreadEntry);
    thread.run();
    thread.wait();

    Maze::LogService::GetInstancePtr()->logFormatted("qwerty %d\n", 42);
    Maze::LogService::GetInstancePtr()->setLogFile("log.log");
    Maze::Debug::log << "123" << Maze::endl;

    Maze::Vec2DS a(42, 228);
    a = Maze::Vec2DS::c_zero;
    a += 55;
    a.y = 0;
    auto aa = a.squaredLength();
    Maze::Debug::log << aa << Maze::endl;


    Maze::Vec2DF b(42, 228);
    b = Maze::Vec2DF::c_zero;
    b += 33.5f;
    b.x = 0;
    auto bb = b.squaredLength();
    Maze::Debug::log << bb << Maze::endl;



    Maze::Vec3DF c(a);


    Maze::Mat3DF mm(Maze::Mat3DF::c_identity);
    Maze::Mat4DF mmm(Maze::Mat4DF::c_identity);

    Maze::Debug::log << a << Maze::endl;
    Maze::Debug::log << b << Maze::endl;
    Maze::Debug::log << c << Maze::endl;
    Maze::Debug::log << mm << Maze::endl;
    Maze::Debug::log << mmm << Maze::endl;

    Maze::AABB2D aabb(a, b);
    Maze::Debug::log << aabb << Maze::endl;

    Maze::Rect2DU rect = aabb.toRect<Maze::U32>();
    Maze::Debug::log << rect << Maze::endl;

    Maze::Debug::log << "Hello, world!" << Maze::endl;


    Maze::Char text[256];
    Maze::StringHelper::FormatString(text, 256, "Check %d! %s", 123, "456");
    Maze::Debug::log << text << Maze::endl;

    Maze::Debug::log << Maze::StringHelper::FormattedStringSize("Check %d! %s", 123, "456") << Maze::endl;

    MAZE_ERROR("Hello, error! [2]");

    std::cin.get();

    return 0;
}
