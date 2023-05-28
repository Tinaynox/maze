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
#include "maze-core/utils/MazeMultiDelegate.hpp"


////////////////////////////////////
namespace Maze
{


////////////////////////////////////
#if (MAZE_USE_DELEGATES_PROTECTION)


    //////////////////////////////////////////
    // Class MultiDelegateCallbackReceiver
    //
    //////////////////////////////////////////
    MultiDelegateCallbackReceiver::MultiDelegateCallbackReceiver()
    {
        
    }

    //////////////////////////////////////////
    MultiDelegateCallbackReceiver::~MultiDelegateCallbackReceiver()
    {
        for (Size i = 0, in = m_delegates.size(); i != in; ++i)
        {
            IMultiDelegate* multiDelegate = m_delegates[i];
            MAZE_UNUSED(multiDelegate);
            MAZE_ERROR("MultiDelegate is not unsubscribed!");
        }
    }

    //////////////////////////////////////////
    void MultiDelegateCallbackReceiver::subscribeDelegate(IMultiDelegate* _delegate)
    {
        m_delegates.push_back(_delegate);
    }

    //////////////////////////////////////////
    void MultiDelegateCallbackReceiver::unsubscribeDelegate(IMultiDelegate* _delegate)
    {
        FastVector<IMultiDelegate*>::iterator it = m_delegates.begin();
        FastVector<IMultiDelegate*>::iterator end = m_delegates.end();

        for (; it != end; ++it)
        {
            if ((*it) == _delegate)
            {
                m_delegates.eraseUnordered(it);
                return;
            }
        }

        MAZE_ERROR("Delegate is not found!");
    }


    //////////////////////////////////////////
    // Class IMultiDelegate
    //
    //////////////////////////////////////////
    IMultiDelegate::IMultiDelegate()
    {
        
    }

    //////////////////////////////////////////
    IMultiDelegate::~IMultiDelegate()
    {
        unsubscribeAllReceivers();
    }

    //////////////////////////////////////////
    void IMultiDelegate::unsubscribeAllReceivers()
    {
        FastVector<MultiDelegateCallbackReceiver*>::iterator it = m_receivers.begin();
        FastVector<MultiDelegateCallbackReceiver*>::iterator end = m_receivers.end();

        for (; it != end; ++it)
            (*it)->unsubscribeDelegate(this);
        m_receivers.clear();
    }

    //////////////////////////////////////////
    void IMultiDelegate::subscribeDelegate(MultiDelegateCallbackReceiver* _receiver)
    {
        _receiver->subscribeDelegate(this);

        m_receivers.push_back(_receiver);
    }

    //////////////////////////////////////////
    void IMultiDelegate::unsubscribeDelegate(MultiDelegateCallbackReceiver* _receiver)
    {
        _receiver->unsubscribeDelegate(this);

        FastVector<MultiDelegateCallbackReceiver*>::iterator it = m_receivers.begin();
        FastVector<MultiDelegateCallbackReceiver*>::iterator end = m_receivers.end();

        for (; it != end; ++it)
        {
            if ((*it) == _receiver)
            {
                m_receivers.eraseUnordered(it);
                return;
            }
        }

        MAZE_ERROR("Delegate is not found!");
    }


#endif // (MAZE_USE_DELEGATES_PROTECTION)
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
