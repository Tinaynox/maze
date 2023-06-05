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
        inline void subscribe(DelegateType const& _delegate)
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
            
        }

        //////////////////////////////////////////
        template <typename C>
        inline void subscribe(C* _object, void(C::* const _method)(TArgs...))
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

            subscribe(DelegateType::From(_object, _method));
        }

        //////////////////////////////////////////
        template <typename TFunctor>
        inline void subscribe(TFunctor&& _f)
        {
            
#if ((MAZE_DEBUG) && (MAZE_DEBUG_DELEGATES))
            auto delegate = DelegateType::From(::std::forward<TFunctor>(_f));
            for (DelegateType& d : m_delegatesList)
            {
                MAZE_ERROR_IF(d == delegate, "Trying to add same delegate twice!");
            }
            m_delegatesList.push_back(delegate);
#else
            m_delegatesList.emplace_back(::std::forward<TFunctor>(_f));
#endif
            
        }

        //////////////////////////////////////////
        inline void subscribe(void(*const _function)(TArgs...))
        {
            subscribe(DelegateType::From(_function));
        }

        //////////////////////////////////////////
        inline bool unsubscribe(DelegateType const& _delegate)
        {
            for (DelegateType& d : m_delegatesList)
            {
                if (d == _delegate)
                {
                    d.reset();
                    return true;
                }
            }

            return false;
        }

        //////////////////////////////////////////
        inline bool unsubscribe(DelegateType& _delegate)
        {
            for (DelegateType& d : m_delegatesList)
            {
                if (d == _delegate)
                {
                    d.reset();
                    return true;
                }
            }

            return false;
        }        

        //////////////////////////////////////////
        template <typename C>
        inline bool unsubscribe(C* _object)
        {
            static_assert(std::is_base_of<MultiDelegateCallbackReceiver,C>::value, "C must derive from MultiDelegateCallbackReceiver");

            bool result = false;
            for (DelegateType& d : m_delegatesList)
            {
                if (d.getObjectAddress() == _object)
                {
                    d.reset();

#if (MAZE_USE_DELEGATES_PROTECTION)
                    unsubscribeDelegate(_object);
#endif

                    result = true;
                }
            }

            return result;
        }

        //////////////////////////////////////////
        template <typename C>
        inline bool unsubscribe(C* _object, void(C::* const _method)(TArgs...))
        {
            static_assert(std::is_base_of<MultiDelegateCallbackReceiver,C>::value, "C must derive from MultiDelegateCallbackReceiver");

            for (DelegateType& d : m_delegatesList)
            {
                if (d.isEqual(_object, _method))
                {
                    d.reset();

#if (MAZE_USE_DELEGATES_PROTECTION)
                    unsubscribeDelegate(_object);
#endif

                    return true;
                }
            }

            return false;
        }

        //////////////////////////////////////////
        inline bool unsubscribe(void(*const _function)(TArgs...))
        {
            for (DelegateType& d : m_delegatesList)
            {
                if (d.isEqual(_function))
                {
                    d.reset();
                    return true;
                }
            }

            return false;
        }

        //////////////////////////////////////////
        template <
            typename TFunctor,
            typename = typename ::std::enable_if<!(::std::is_pointer<TFunctor>{})>::type>
        bool unsubscribe(TFunctor&& _f)
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
        }

        //////////////////////////////////////////
        inline void updateDelegatesList()
        {
            Maze::Size delegatesCount = m_delegatesList.size();
            if (delegatesCount == 0)
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
        }

        //////////////////////////////////////////
        inline bool empty() const { return m_delegatesList.empty(); }

        //////////////////////////////////////////
        inline void clear()
        { 
            for (DelegateType& d : m_delegatesList)
                d.reset();

#if (MAZE_USE_DELEGATES_PROTECTION)
            unsubscribeAllReceivers();
#endif
        }

    protected:
        DelegatesList m_delegatesList;
        DelegateListErase m_delegatesListErase;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMultiDelegate_h_
//////////////////////////////////////////
