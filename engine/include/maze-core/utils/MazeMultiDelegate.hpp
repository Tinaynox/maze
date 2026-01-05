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
#if (!defined(_MazeMultiDelegate_hpp_))
#define _MazeMultiDelegate_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "MazeDelegate.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Forward Declaration
    class IMultiDelegate;


    //////////////////////////////////////////
    // Define
#if (MAZE_DEBUG)
#   define MAZE_DEBUG_DELEGATES (0)
#   define MAZE_USE_DELEGATES_PROTECTION (1)
#endif

    //////////////////////////////////////////
    // Class MultiDelegateCallbackReceiver
    //
    //////////////////////////////////////////
    class MAZE_CORE_API MultiDelegateCallbackReceiver
    {
#if (MAZE_USE_DELEGATES_PROTECTION)
    public:    

        //////////////////////////////////////////
        friend class IMultiDelegate;

    public:

        //////////////////////////////////////////
        MultiDelegateCallbackReceiver();

        //////////////////////////////////////////
        virtual ~MultiDelegateCallbackReceiver();

    protected:

        //////////////////////////////////////////
        void subscribeDelegate(IMultiDelegate* delegate);

        //////////////////////////////////////////
        void unsubscribeDelegate(IMultiDelegate* delegate);


    private:
        FastVector<IMultiDelegate*> m_delegates;
#endif
    };


    //////////////////////////////////////////
    // Class IMultiDelegate
    //
    //////////////////////////////////////////
    class MAZE_CORE_API IMultiDelegate
    {
#if (MAZE_USE_DELEGATES_PROTECTION)
    public:

        //////////////////////////////////////////
        IMultiDelegate();

        //////////////////////////////////////////
        virtual ~IMultiDelegate();


        //////////////////////////////////////////
        void subscribeDelegate(MultiDelegateCallbackReceiver* _receiver);

        //////////////////////////////////////////
        void unsubscribeDelegate(MultiDelegateCallbackReceiver* _receiver);

    protected:

        //////////////////////////////////////////
        void unsubscribeAllReceivers();

    public:
    
        FastVector<MultiDelegateCallbackReceiver*> m_receivers;
#endif
    };


    //////////////////////////////////////////
    // Class MultiDelegate
    //
    //////////////////////////////////////////
    template <class ...TArgs>
    class MultiDelegate
        : public IMultiDelegate
    {
    protected:

        //////////////////////////////////////////
        using DelegateType = Delegate<void, TArgs...>;
        using DelegatesList = FastVector<DelegateType>;
        using DelegateListErase = DelegateType* (DelegatesList::*)(DelegateType*);

        template <class C>
        using DelegateTypeMemberPair = typename DelegateType::template MemberPair<C>;
        


    public:

        //////////////////////////////////////////
        MultiDelegate(bool _unordered = false)
            : m_delegatesListErase(
                _unordered ? (DelegateListErase)&DelegatesList::eraseUnordered
                           : (DelegateListErase)&DelegatesList::erase
            )
        {
    
        }

        //////////////////////////////////////////
        ~MultiDelegate()
        {
        }

        //////////////////////////////////////////
        inline void subscribe(
            DelegateType const& _delegate,
            bool _updateDelegatesList = true)
        {
#if ((MAZE_DEBUG) && (MAZE_DEBUG_DELEGATES))
            for (DelegateType& d : m_delegatesList)
            {
                MAZE_ERROR_IF(d == _delegate, "Trying to add same delegate twice!");
            }
            m_delegatesList.push_back(_delegate);
#else
            m_delegatesList.emplace_back(_delegate);
#endif
            
            if MAZE_CONSTEXPR17 (_updateDelegatesList)
                updateDelegatesList();
        }

        //////////////////////////////////////////
        template <typename C>
        inline void subscribe(
            C* _object,
            void(C::* const _method)(TArgs...),
            bool _updateDelegatesList = true)
        {
            static_assert(std::is_base_of<MultiDelegateCallbackReceiver,C>::value, "C must derive from MultiDelegateCallbackReceiver");

#if ((MAZE_DEBUG) && (MAZE_DEBUG_DELEGATES))
            for (DelegateType& d : m_delegatesList)
            {
                if (d.isEqual(_object, _method))
                {
                    MAZE_ERROR("Trying to add same delegate twice!");
                }
            }
#endif

#if (MAZE_USE_DELEGATES_PROTECTION)
            subscribeDelegate(_object);
#endif

            subscribe(DelegateType::From(_object, _method), _updateDelegatesList);
        }

        //////////////////////////////////////////
        template <typename TFunctor>
        inline void subscribe(
            TFunctor&& _f,
            bool _updateDelegatesList = true)
        {
            
#if ((MAZE_DEBUG) && (MAZE_DEBUG_DELEGATES))
            for (DelegateType& d : m_delegatesList)
            {
                MAZE_ERROR_IF(d == _f, "Trying to add same delegate twice!");
            }
            m_delegatesList.emplace_back(std::move(_f));
#else
            m_delegatesList.emplace_back(std::move(_f));
#endif

            if MAZE_CONSTEXPR17 (_updateDelegatesList)
                updateDelegatesList();
        }

        //////////////////////////////////////////
        inline void subscribe(
            void(*const _function)(TArgs...),
            bool _updateDelegatesList = true)
        {
            subscribe(DelegateType::From(_function), _updateDelegatesList);
        }

        //////////////////////////////////////////
        inline bool unsubscribe(
            DelegateType const& _delegate,
            bool _updateDelegatesList = false)
        {
            bool result = false;
            for (auto it = m_delegatesList.begin(); it != m_delegatesList.end(); ++it)
            {
                if (*it == _delegate)
                {
                    it->reset();
                    result = true;
                    
                    if (_updateDelegatesList && m_callLock == 0)
                        ((&m_delegatesList)->*m_delegatesListErase)(it);
                    else
                        m_containsEmptyDelegates = false;
                    break;
                }
            }

            if MAZE_CONSTEXPR17 (_updateDelegatesList)
                updateDelegatesList();

            return result;
        }

        //////////////////////////////////////////
        template <typename C>
        inline bool unsubscribe(
            C* _object,
            bool _updateDelegatesList = false)
        {
            static_assert(std::is_base_of<MultiDelegateCallbackReceiver,C>::value, "C must derive from MultiDelegateCallbackReceiver");

            bool result = false;
            for (auto it = m_delegatesList.begin(); it != m_delegatesList.end(); )
            {
                if (it->getObjectAddress() == _object)
                {
                    it->reset();
                    result = true;
                    
                    if (m_callLock == 0)
                    {
                        it = ((&m_delegatesList)->*m_delegatesListErase)(it);
                    }
                    else
                    {
                        m_containsEmptyDelegates = true;
                        ++it;
                    }

#if (MAZE_USE_DELEGATES_PROTECTION)
                    if (result)
                        unsubscribeDelegate(_object);
#endif
                }
                else
                {
                    ++it;
                }
            }

            if MAZE_CONSTEXPR17 (_updateDelegatesList)
                 updateDelegatesList();

            return result;
        }

        //////////////////////////////////////////
        template <typename C>
        inline bool unsubscribe(
            C* _object,
            void(C::* const _method)(TArgs...),
            bool _updateDelegatesList = false)
        {
            static_assert(std::is_base_of<MultiDelegateCallbackReceiver,C>::value, "C must derive from MultiDelegateCallbackReceiver");

            bool result = false;
            for (auto it = m_delegatesList.begin(); it != m_delegatesList.end(); ++it)
            {
                if (it->isEqual(_object, _method))
                {
                    it->reset();
                    result = true;
                    
                    if (_updateDelegatesList && m_callLock == 0)
                        ((&m_delegatesList)->*m_delegatesListErase)(it);
                    else
                        m_containsEmptyDelegates = true;

#if (MAZE_USE_DELEGATES_PROTECTION)
                    if (result)
                        unsubscribeDelegate(_object);
#endif

                    break;
                }
            }



            if MAZE_CONSTEXPR17 (_updateDelegatesList)
                updateDelegatesList();

            return result;
        }

        //////////////////////////////////////////
        inline bool unsubscribe(
            void(*const _function)(TArgs...),
            bool _updateDelegatesList = false)
        {
            bool result = false;
            for (auto it = m_delegatesList.begin(); it != m_delegatesList.end(); ++it)
            {
                if (it->isEqual(_function))
                {
                    it->reset();
                    result = true;
                    
                    if (_updateDelegatesList && m_callLock == 0)
                        ((&m_delegatesList)->*m_delegatesListErase)(it);
                    else
                        m_containsEmptyDelegates = true;
                    break;
                }
            }

            if MAZE_CONSTEXPR17 (_updateDelegatesList)
                if (!result)
                    updateDelegatesList();

            return result;
        }

        //////////////////////////////////////////
        template <
            typename TFunctor,
            typename = std::enable_if_t<
                !std::is_pointer_v<std::decay_t<TFunctor>> &&
                !std::is_same_v<std::decay_t<TFunctor>, DelegateType>
            >
        >
        bool unsubscribe(
            TFunctor&& _f,
            bool _updateDelegatesList = false)
        {
            MAZE_ERROR("Functors unsubscribing is forbidden!");

            return false;
        }

        //////////////////////////////////////////
        void operator()(TArgs... _args)
        {
            Maze::Size delegatesCount = m_delegatesList.size();
            if (delegatesCount == 0)
                return;
            
            m_containsEmptyDelegates = false;

            ++m_callLock;
            for (Maze::Size i = 0; i < delegatesCount;)
            {
                auto& delegate = m_delegatesList[i];

                if (!delegate)
                {
                    ((&m_delegatesList)->*m_delegatesListErase)(m_delegatesList.begin() + i);
                    --delegatesCount;
                }
                else
                {
                    delegate(::std::forward<TArgs>(_args)...);
                    ++i;
                }
            }
            --m_callLock;
        }

        //////////////////////////////////////////
        inline void updateDelegatesList()
        {
            Maze::Size delegatesCount = m_delegatesList.size();
            if (delegatesCount == 0)
                return;

            if (!m_containsEmptyDelegates)
                return;

            if (m_callLock > 0)
                return;
            
            for (Maze::Size i = 0; i < delegatesCount;)
            {
                auto& delegate = m_delegatesList[i];

                if (!delegate)
                {
                    ((&m_delegatesList)->*m_delegatesListErase)(m_delegatesList.begin() + i);
                    --delegatesCount;
                }
                else
                {
                    ++i;
                }
            }

            m_containsEmptyDelegates = false;
        }

        //////////////////////////////////////////
        inline bool empty() const { return m_delegatesList.empty(); }

        //////////////////////////////////////////
        inline void clear()
        { 
            for (DelegateType& d : m_delegatesList)
                d.reset();
            m_delegatesList.clear();

#if (MAZE_USE_DELEGATES_PROTECTION)
            unsubscribeAllReceivers();
#endif
        }

    protected:
        DelegatesList m_delegatesList;
        DelegateListErase m_delegatesListErase;
        bool m_containsEmptyDelegates = false;
        S32 m_callLock = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMultiDelegate_h_
//////////////////////////////////////////
