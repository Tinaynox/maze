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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsUI.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/system/MazeSystemCursor.hpp"
#include "maze-core/managers/MazeSystemCursorManager.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionTranslation.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    inline void UIElement2DSetFlags(Component* _component, S32 _flags)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UIElement2D>::UID(), "Component is not UIElement2D!");
        _component->castRaw<UIElement2D>()->setFlags(_flags);
    }

    //////////////////////////////////////////
    inline void UIElement2DGetFlags(Component* _component, S32& _outFlags)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UIElement2D>::UID(), "Component is not UIElement2D!");
        _outFlags = _component->castRaw<UIElement2D>()->getFlags();
    }

    //////////////////////////////////////////
    inline void UIElement2DSetFocused(Component* _component, bool _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UIElement2D>::UID(), "Component is not UIElement2D!");
        _component->castRaw<UIElement2D>()->setFocused(_value);
    }

    //////////////////////////////////////////
    inline void UIElement2DGetFocused(Component* _component, bool& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UIElement2D>::UID(), "Component is not UIElement2D!");
        _outValue = _component->castRaw<UIElement2D>()->getFocused();
    }

    //////////////////////////////////////////
    inline void UIElement2DSetPressed(Component* _component, bool _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UIElement2D>::UID(), "Component is not UIElement2D!");
        _component->castRaw<UIElement2D>()->setPressed(_value);
    }

    //////////////////////////////////////////
    inline void UIElement2DGetPressed(Component* _component, bool& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UIElement2D>::UID(), "Component is not UIElement2D!");
        _outValue = _component->castRaw<UIElement2D>()->getPressed();
    }


    //////////////////////////////////////////
    inline void UITweenTransitionAlphaSetHidden(Component* _component, bool _value, bool _resetProgress)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _component->castRaw<UITweenTransitionAlpha>()->setHidden(_value, _resetProgress);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaGetHidden(Component* _component, bool& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _outValue = _component->castRaw<UITweenTransitionAlpha>()->getHidden();
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaAddDelayTimer(Component* _component, F32 _time)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _component->castRaw<UITweenTransitionAlpha>()->addDelayTimer(_time);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaSetDelayTimer(Component* _component, F32 _time)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _component->castRaw<UITweenTransitionAlpha>()->setDelayTimer(_time);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaSetShowTime(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _component->castRaw<UITweenTransitionAlpha>()->setShowTime(_value);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaGetShowTime(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _outValue = _component->castRaw<UITweenTransitionAlpha>()->getShowTime();
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaSetHideKoef(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _component->castRaw<UITweenTransitionAlpha>()->setHideKoef(_value);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionAlphaGetHideKoef(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionAlpha>::UID(), "Component is not UITweenTransitionAlpha!");
        _outValue = _component->castRaw<UITweenTransitionAlpha>()->getHideKoef();
    }


    //////////////////////////////////////////
    inline void UITweenTransitionScaleSetHidden(Component* _component, bool _value, bool _resetProgress)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionScale>::UID(), "Component is not UITweenTransitionScale!");
        _component->castRaw<UITweenTransitionScale>()->setHidden(_value, _resetProgress);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionScaleGetHidden(Component* _component, bool& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionScale>::UID(), "Component is not UITweenTransitionScale!");
        _outValue = _component->castRaw<UITweenTransitionScale>()->getHidden();
    }


    //////////////////////////////////////////
    inline void UITweenTransitionTranslationSetHidden(Component* _component, bool _value, bool _resetProgress)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _component->castRaw<UITweenTransitionTranslation>()->setHidden(_value, _resetProgress);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationGetHidden(Component* _component, bool& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _outValue = _component->castRaw<UITweenTransitionTranslation>()->getHidden();
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationSetHiddenPosition(Component* _component, Vec2F const& _pos)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _component->castRaw<UITweenTransitionTranslation>()->setHiddenPosition(_pos);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationGetHiddenPosition(Component* _component, Vec2F& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _outValue = _component->castRaw<UITweenTransitionTranslation>()->getHiddenPosition();
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationSetShownPosition(Component* _component, Vec2F const& _pos)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _component->castRaw<UITweenTransitionTranslation>()->setShownPosition(_pos);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationGetShownPosition(Component* _component, Vec2F& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _outValue = _component->castRaw<UITweenTransitionTranslation>()->getShownPosition();
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationSetShowTime(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _component->castRaw<UITweenTransitionTranslation>()->setShowTime(_value);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationGetShowTime(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _outValue = _component->castRaw<UITweenTransitionTranslation>()->getShowTime();
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationSetHideKoef(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _component->castRaw<UITweenTransitionTranslation>()->setHideKoef(_value);
    }

    //////////////////////////////////////////
    inline void UITweenTransitionTranslationGetHideKoef(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<UITweenTransitionTranslation>::UID(), "Component is not UITweenTransitionTranslation!");
        _outValue = _component->castRaw<UITweenTransitionTranslation>()->getHideKoef();
    }


    //////////////////////////////////////////
    inline bool SystemCursorIsValid(S32 _systemCursorId)
    {
        SystemCursor* systemCursor = SystemCursor::GetResource(_systemCursorId);
        return !!systemCursor;
    }

    inline S32 SystemCursorGetOrLoad(MonoString* _name)
    {
        if (!SystemCursorManager::GetInstancePtr())
            return c_invalidResourceId;

        Char* cstr = mono_string_to_utf8(_name);
        S32 id = c_invalidResourceId;
        
        SystemCursorPtr const& systemCursor = SystemCursorManager::GetInstancePtr()->getOrLoadSystemCursor(cstr);
        if (systemCursor)
            id = systemCursor->getResourceId();

        mono_free(cstr);

        return id;
    }


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsUI()
    {
        // UIElement2D
        MAZE_UI_MONO_BIND_FUNC(UIElement2DSetFlags);
        MAZE_UI_MONO_BIND_FUNC(UIElement2DGetFlags);
        MAZE_UI_MONO_BIND_FUNC(UIElement2DSetFocused);
        MAZE_UI_MONO_BIND_FUNC(UIElement2DGetFocused);
        MAZE_UI_MONO_BIND_FUNC(UIElement2DSetPressed);
        MAZE_UI_MONO_BIND_FUNC(UIElement2DGetPressed);

        // UITweenTransitionAlpha
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaSetHidden);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaGetHidden);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaAddDelayTimer);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaSetDelayTimer);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaSetShowTime);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaGetShowTime);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaSetHideKoef);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionAlphaGetHideKoef);

        // UITweenTransitionScale
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionScaleSetHidden);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionScaleGetHidden);

        // UITweenTransitionTranslation
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationSetHidden);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationGetHidden);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationSetHiddenPosition);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationGetHiddenPosition);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationSetShownPosition);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationGetShownPosition);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationSetShowTime);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationGetShowTime);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationSetHideKoef);
        MAZE_UI_MONO_BIND_FUNC(UITweenTransitionTranslationGetHideKoef);

        // SystemCursor
        MAZE_UI_MONO_BIND_FUNC(SystemCursorIsValid);
        MAZE_UI_MONO_BIND_FUNC(SystemCursorGetOrLoad);
    }

} // namespace Maze
//////////////////////////////////////////
