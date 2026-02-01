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
#include "MazeCSharpEditorToolsHeader.hpp"
#include "maze-plugin-csharp-editor-tools/property-drawers/MazePDCSharpList.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerCSharpList
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerCSharpList, PropertyDrawerVector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerCSharpList);

    //////////////////////////////////////////
    PropertyDrawerCSharpList::PropertyDrawerCSharpList()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerCSharpList::~PropertyDrawerCSharpList()
    {

    }

    //////////////////////////////////////////
    PropertyDrawerCSharpListPtr PropertyDrawerCSharpList::Create(
        MonoType* _monoType,
        DataBlock const& _dataBlock)
    {
        PropertyDrawerCSharpListPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerCSharpList, object, init(_monoType, _dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerCSharpList::init(
        MonoType* _monoType,
        DataBlock const& _dataBlock)
    {
        CString monoTypeName = mono_type_get_name(_monoType);

        CStringSpan monoTypeBaseName = MonoHelper::GetMonoGenericClassFirstGenericArgumentName(monoTypeName);
        MAZE_ERROR_RETURN_VALUE_IF(monoTypeBaseName.size() == 0, false, "Invalid generic class - %s", monoTypeName);

        Char monoTypeBaseNameBuffer[256];
        memcpy_s(monoTypeBaseNameBuffer, sizeof(monoTypeBaseNameBuffer), monoTypeBaseName.ptr(), monoTypeBaseName.size());
        monoTypeBaseNameBuffer[monoTypeBaseName.size()] = 0;
        HashedCString monoTypeBaseNameHCS(monoTypeBaseNameBuffer);
        
        ClassUID childPropertyClassUID = MonoEngine::GetCppTypeBindingClassUID(monoTypeBaseNameHCS);
        MAZE_ERROR_RETURN_VALUE_IF(childPropertyClassUID == 0, false, "Undefined binding for monoType=%s", monoTypeBaseNameBuffer);

        if (!PropertyDrawerVector::init(childPropertyClassUID, _dataBlock))
            return false;

        return true;
    }




} // namespace Maze
//////////////////////////////////////////
