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
#include "MazeCoreHeader.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace PlatformHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool TestSystem()
        {
            if (!TestClassInfo())
            {
                MAZE_ERROR("Test ClassInfo Failed.\n");
                return false;
            }

            if (!TestBaseTypes())
            {
                MAZE_ERROR("Test Base Types Failed.\n");
                return false;
            }

#if (MAZE_ENDIAN == MAZE_ENDIAN_LITTLE)
            if (!IsLittleEndian())
            {
                MAZE_ERROR("Endian test Failed.\n");
                return false;
            }
#else
            if (IsLittleEndian())
            {
                MAZE_ERROR("Endian test Failed.\n");
                return false;
            }
#endif

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool TestClassInfo()
        {
            String typeInfoQualifiedName = ClassInfo<Rotation2D>::QualifiedName();
            String typeInfoName = ClassInfo<Rotation2D>::Name();
        
            if (typeInfoQualifiedName != "Maze::Rotation2D")
            {
                MAZE_ERROR("ClassInfo<Rotation2D>::QualifiedName() = %s", typeInfoQualifiedName.c_str());
                return false;
            }

            if (typeInfoName != "Rotation2D")
            {
                MAZE_ERROR("ClassInfo<Rotation2D>::Name() = %s", typeInfoName.c_str());
                return false;
            }

            return true;
        }

#define MAZE_TEST_TYPE_SIZE(__type, __size)                                              \
{                                                                                        \
    if (sizeof(__type) != __size)                                                        \
    {                                                                                    \
        MAZE_ERROR("Type %s have size %d!", MAZE_STRINGIFY(__type), sizeof(__type));     \
        return false;                                                                    \
    }                                                                                    \
}

        //////////////////////////////////////////
        MAZE_CORE_API bool TestBaseTypes()
        {
            MAZE_TEST_TYPE_SIZE(S8, 1);
            MAZE_TEST_TYPE_SIZE(S16, 2);
            MAZE_TEST_TYPE_SIZE(S32, 4);
            MAZE_TEST_TYPE_SIZE(S64, 8);
        
            MAZE_TEST_TYPE_SIZE(U8, 1);
            MAZE_TEST_TYPE_SIZE(U16, 2);
            MAZE_TEST_TYPE_SIZE(U32, 4);
            MAZE_TEST_TYPE_SIZE(U64, 8);
    
            MAZE_TEST_TYPE_SIZE(F32, 4);
            MAZE_TEST_TYPE_SIZE(F64, 8);

        
            MAZE_TEST_TYPE_SIZE(Vec2DF, 2 * 4);
            MAZE_TEST_TYPE_SIZE(Vec2DS, 2 * 4);
            MAZE_TEST_TYPE_SIZE(Vec2DU, 2 * 4);
        
            MAZE_TEST_TYPE_SIZE(Vec3DF, 3 * 4);
            MAZE_TEST_TYPE_SIZE(Vec3DS, 3 * 4);
            MAZE_TEST_TYPE_SIZE(Vec3DU, 3 * 4);
        
        
            MAZE_TEST_TYPE_SIZE(Vec4DF, 4 * 4);
            MAZE_TEST_TYPE_SIZE(Vec4DS, 4 * 4);
            MAZE_TEST_TYPE_SIZE(Vec4DU, 4 * 4);
        
            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool IsLittleEndian()
        {
            union 
            {
                U32 i;
                S8 c[4];
            } bint = {0x01020304};

            if (bint.c[0] == 1)
            {
                return false;
            }
            else
            {
                return true;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ConstructEngineInfo()
        {
            String info;

            info += "[Engine Info]\n";
#if (MAZE_STATIC)
            info += "\tDynamic Linking: No\n";
#else
            info += "\tDynamic Linking: Yes\n";
#endif

#if (MAZE_PRODUCTION)
            info += "\Production mode: Yes\n";
#else
            info += "\Production mode: No\n";
#endif

#if (MAZE_USE_OPTICK)
            info += "\Profiler: Optick\n";
#else
            info += "\Profiler: None\n";
#endif

            return info;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ConstructApplicationInfo()
        {
            String info;

            info += "[Application Info]\n";
            info += String("\tCompiler: ") + MAZE_COMPILER_NAME + " (" + StringHelper::ToString(MAZE_COMPILER_VERSION) + ")" + "\n";
            info += String("\tC++ Standart: ") + MAZE_CPP_STANDARD_NAME + "\n";
            info += String("\tArchitecture: ") + MAZE_ARCH_SUFFIX_STR + "\n";
            info += String("\tPointer Size: ") + StringHelper::ToString((Maze::U32)sizeof(Size) * 8) + "\n";
            info += String("\tEndian: ") + ((MAZE_ENDIAN == MAZE_ENDIAN_LITTLE) ? "Little" : "Big") + "\n";

#if (MAZE_DEBUG)
            info += "\tConfiguration: Debug\n";
#else
            info += "\tConfiguration: Release\n";
#endif

            return info;
        }


    } // namespace PlatformHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
