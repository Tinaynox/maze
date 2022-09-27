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
#if (!defined(_MazeDelegate_hpp_))
#define _MazeDelegate_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/memory/MazeNedMemoryAllocator.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Delegate
    //
    //////////////////////////////////////////
    template <class TReturnType, class ...TArgs>
    class Delegate
    {
    public:

        //////////////////////////////////////////
        friend struct ::std::hash<Delegate>;


        //////////////////////////////////////////
        using DelegateClass = Delegate<TReturnType, TArgs...>;
        using StubFunction = TReturnType(*)(void*, TArgs&&...);
        using GetObjectAddressFunction = void*(*)(void*);
        using GetObjectMethodAddressFunction = void*(*)(void*);
        using FunctorStoreClearFunctionType = void(*)(void*);


        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(DelegateClass);


        //////////////////////////////////////////
        template <class C>
        using MemberPair = ::std::pair<C* const, TReturnType(C::* const)(TArgs...)>;

        //////////////////////////////////////////
        template <class C>
        using MemberPairConst = ::std::pair<C const* const, TReturnType(C::* const)(TArgs...) const>;


    private:

        //////////////////////////////////////////
        Delegate(void* const _object, StubFunction const _stubFunction) noexcept 
            : m_functorObject(_object)
            , m_stubFunction(_stubFunction)
            , m_object(_object)
        {
            
        }

    public:

        //////////////////////////////////////////
        Delegate()
            : m_stubFunction(nullptr)
            , m_functorObject(nullptr)
            , m_object(nullptr)
        {
            
        }

        //////////////////////////////////////////
        Delegate(Delegate const& _d) = default;

        //////////////////////////////////////////
        Delegate(Delegate&& _d) = default;


        //////////////////////////////////////////
        Delegate(::std::nullptr_t const) noexcept 
            : Delegate()
        {}


        //////////////////////////////////////////
        template <
            class C,
            typename = typename ::std::enable_if<::std::is_class<C>{}>::type>
        explicit Delegate(C const* const _object) noexcept 
            : m_functorObject(const_cast<C*>(_object))
            , m_object(_object)            
        {
            
        }

        //////////////////////////////////////////
        template <
            class C,
            typename = typename ::std::enable_if<::std::is_class<C>{}>::type>
        explicit Delegate(C const& _object) noexcept 
            : m_functorObject(const_cast<C*>(&_object))
            , m_object(_object)
        {
            
        }

        //////////////////////////////////////////
        template <class C>
        Delegate(C* const _object, TReturnType(C::* const _method)(TArgs...))
        {
            *this = From(_object, _method);
        }


        //////////////////////////////////////////
        template <class C>
        Delegate(C* const _object, TReturnType(C::* const _method)(TArgs...) const)
        {
            *this = From(_object, _method);
        }

        //////////////////////////////////////////
        template <class C>
        Delegate(C& _object, TReturnType(C::* const _method)(TArgs...))
        {
            *this = From(_object, _method);
        }

        //////////////////////////////////////////
        template <class C>
        Delegate(C const& _object, TReturnType(C::* const _method)(TArgs...) const)
        {
            *this = From(_object, _method);
        }

        //////////////////////////////////////////
        template <
            typename T,
            typename = typename ::std::enable_if <!::std::is_same<Delegate, typename ::std::decay<T>::type>{}> ::type>
        Delegate(T&& _functor) 
            : m_functorStore(
#if (1)
                Maze::NedMemoryAllocator::AllocateBytes(
                    sizeof(typename ::std::decay<T>::type)
#   if (MAZE_DEBUG_MEMORY)
                    , Maze::ClassInfo<Delegate<T>>::QualifiedName()
                    , __FILE__
                    , __LINE__
                    , __FUNCTION__
#   endif
                ),
#else
                operator new(sizeof(typename ::std::decay<T>::type)),
#endif
                FunctorStoreDeleteFunction<typename ::std::decay<T>::type>)
            , m_functorStoreSize(sizeof(typename ::std::decay<T>::type))
            {
                using FunctorType = typename ::std::decay<T>::type;

                new (m_functorStore.get()) FunctorType(::std::forward<T>(_functor));

                m_functorObject = m_functorStore.get();
                m_object = GetObjectAddress<FunctorType>(m_functorObject);
                

                m_stubFunction = StubFunctionFunctor<FunctorType>;
                m_functorStoreClearFunction = FunctorStoreClearFunction<FunctorType>;

#if (MAZE_DEBUG)
                m_debugFunctorName = ClassInfo<FunctorType>::QualifiedName();
#endif
            }

        //////////////////////////////////////////
        Delegate& operator=(Delegate const&) = default;

        //////////////////////////////////////////
        Delegate& operator=(Delegate&&) = default;

        //////////////////////////////////////////
        template <class C>
        Delegate& operator=(TReturnType(C::* const _rhs)(TArgs...))
        {
            return *this = From(static_cast<C*>(m_functorObject), _rhs);
        }

        //////////////////////////////////////////
        template <class C>
        Delegate& operator=(TReturnType(C::* const _rhs)(TArgs...) const)
        {
            return *this = From(static_cast<C const*>(m_functorObject), _rhs);
        }

        //////////////////////////////////////////
        Delegate& operator=(::std::nullptr_t const)
        {
            reset();
            return *this;
        }

        //////////////////////////////////////////
        template <
            typename T,
            typename = typename ::std::enable_if<!::std::is_same<Delegate, typename ::std::decay<T>::type>{}>::type>
        Delegate& operator=(T&& _functor)
        {
            using FunctorType = typename ::std::decay<T>::type;

            if ((sizeof(FunctorType)> m_functorStoreSize) || !m_functorStore.unique())
            {
                m_functorStore.reset(
#if (1)
                    Maze::NedMemoryAllocator::AllocateBytes(
                        sizeof(FunctorType)
#   if (MAZE_DEBUG_MEMORY)
                        , Maze::ClassInfo<Delegate<T>>::QualifiedName()
                        , __FILE__
                        , __LINE__
                        , __FUNCTION__
#   endif
                    ),
#else
                    operator new(sizeof(FunctorType)),
#endif
                    FunctorStoreDeleteFunction<FunctorType>);

                m_functorStoreSize = sizeof(FunctorType);
            }
            else
            {
                m_functorStoreClearFunction(m_functorStore.get());
            }

            new (m_functorStore.get()) FunctorType(::std::forward<T>(_functor));

            m_functorObject = m_functorStore.get();
            m_object = GetObjectAddress<FunctorType>(m_functorObject);
            

            m_stubFunction = StubFunctionFunctor<FunctorType>;
            m_functorStoreClearFunction = FunctorStoreClearFunction<FunctorType>;

#if (MAZE_DEBUG)
            m_debugFunctorName = ClassInfo<FunctorType>::QualifiedName();
#endif

            return *this;
        }

        //////////////////////////////////////////
        template <TReturnType(*const _function)(TArgs...)>
        static Delegate From() noexcept
        {
            return { nullptr, StubFunctionGlobal<_function> };
        }

        //////////////////////////////////////////
        template <class C, TReturnType(C::* const _method)(TArgs...)>
        static Delegate From(C* const _object) noexcept
        {
            return { _object, StubFunctionMethod<C, _method> };
        }

        //////////////////////////////////////////
        template <class C, TReturnType(C::* const _method)(TArgs...) const>
        static Delegate From(C const* const _object) noexcept
        {
            return { const_cast<C*>(_object), StubFunctionMethodConst<C, _method> };
        }

        //////////////////////////////////////////
        template <class C, TReturnType(C::* const _method)(TArgs...)>
        static Delegate From(C& _object) noexcept
        {
            return { &_object, StubFunctionMethod<C, _method> };
        }

        //////////////////////////////////////////
        template <class C, TReturnType(C::* const _method)(TArgs...) const>
        static Delegate From(C const& _object) noexcept
        {
            return { const_cast<C*>(&_object), StubFunctionMethodConst<C, _method> };
        }

        //////////////////////////////////////////
        template <typename TFunctor>
        static Delegate From(TFunctor&& _f)
        {
            return ::std::forward<TFunctor>(_f);
        }

        //////////////////////////////////////////
        static Delegate From(TReturnType(*const _function)(TArgs...))
        {
            return _function;
        }

        
        //////////////////////////////////////////
        template <class C>
        static Delegate From(
            C* const _object,
            TReturnType(C::* const _method)(TArgs...))
        {
            return MemberPair<C>(_object, _method);
        }

        //////////////////////////////////////////
        template <class C>
        static Delegate From(
            C const* const _object,
            TReturnType(C::* const _method)(TArgs...) const)
        {
            return MemberPairConst<C>(_object, _method);
        }

        //////////////////////////////////////////
        template <class C>
        static Delegate From(C& _object, TReturnType(C::* const _method)(TArgs...))
        {
            return MemberPair<C>(&_object, _method);
        }

        //////////////////////////////////////////
        template <class C>
        static Delegate From(
            C const& _object,
            TReturnType(C::* const _method)(TArgs...) const)
        {
            return MemberPairConst<C>(&_object, _method);
        }

        //////////////////////////////////////////
        void reset()
        {
            m_stubFunction = nullptr; 
            m_functorStore.reset();
            m_object = nullptr;
            
        }

        //////////////////////////////////////////
        void resetStubFunction() noexcept
        {
            m_stubFunction = nullptr;
        }

        //////////////////////////////////////////
        void swap(Delegate& _other) noexcept
        {
            ::std::swap(*this, _other);
        }

        //////////////////////////////////////////
        bool operator==(Delegate const& _rhs) const noexcept
        {
            return (m_functorObject == _rhs.m_functorObject) && (m_stubFunction == _rhs.m_stubFunction);
        }

        //////////////////////////////////////////
        bool operator!=(Delegate const& _rhs) const noexcept
        {
            return !operator==(_rhs);
        }

        //////////////////////////////////////////
        bool operator<(Delegate const& _rhs) const noexcept
        {
            return    (m_functorObject <_rhs.m_functorObject) 
                ||    ((m_functorObject == _rhs.m_functorObject) && (m_stubFunction <_rhs.m_stubFunction));
        }

        //////////////////////////////////////////
        bool operator==(::std::nullptr_t const) const noexcept
        {
            return !m_stubFunction;
        }

        //////////////////////////////////////////
        bool operator!=(::std::nullptr_t const) const noexcept
        {
            return m_stubFunction;
        }

        //////////////////////////////////////////
        explicit operator bool() const noexcept
        {
            return m_stubFunction;
        }

        //////////////////////////////////////////
        TReturnType operator()(TArgs... _args) const
        {
#if (MAZE_DEBUG)
            MAZE_ERROR_RETURN_VALUE_IF(!m_stubFunction, TReturnType(), "Function is null!");
#endif

            return m_stubFunction(m_functorObject, ::std::forward<TArgs>(_args)...);
        }

        //////////////////////////////////////////
        inline void* getObjectAddress() const 
        { 
            return m_object; 
        }
        

        //////////////////////////////////////////
        inline ::std::shared_ptr<void>& getFunctorStore() { return m_functorStore; }

        //////////////////////////////////////////
        inline ::std::size_t getFunctorStoreSize() { return m_functorStoreSize; }
        

        //////////////////////////////////////////
        template <typename C>
        inline bool isEqual(C* const _object, TReturnType(C::* const _method)(TArgs...))
        {
            if (!m_stubFunction)
                return false;

            if (m_functorStoreSize != sizeof(MemberPair<C>))
                return false;

            if (static_cast<MemberPair<C>*>(m_functorObject)->first != _object)
                return false;

            if (static_cast<MemberPair<C>*>(m_functorObject)->second != _method)
                return false;

            return true;
        }

        //////////////////////////////////////////
        inline bool isEqual(TReturnType(*const _function)(TArgs...))
        {
            if (!m_stubFunction)
                return false;

            if (m_functorStoreSize != sizeof(_function))
                return false;


            TReturnType(*const functorFunction)(TArgs...) = reinterpret_cast<TReturnType(*const)(TArgs...)>(*((Size*)m_functorObject));
            if (functorFunction != _function)
                return false;

            return true;
        }

    private:
    
        //////////////////////////////////////////
        template <class T>
        static void FunctorStoreDeleteFunction(void* const _p)
        {
            static_cast<T*>(_p)->~T();

#if (1)
            Maze::NedMemoryAllocator::DeallocateBytes(_p);
#else
#    if (MAZE_DEBUG_MEMORY)
            operator delete(_p);
#    endif
#endif
        }

        //////////////////////////////////////////
        template <class T>
        static void FunctorStoreClearFunction(void* const _p)
        {
            static_cast<T*>(_p)->~T();
        }

        //////////////////////////////////////////
        template <typename>
        struct IsMemberPair 
            : std::false_type
        {
        };

        //////////////////////////////////////////
        template <class C>
        struct IsMemberPair<::std::pair<C* const, TReturnType(C::* const)(TArgs...)>> 
            : std::true_type
        {
        };

        //////////////////////////////////////////
        template <typename>
        struct IsConstMemberPair 
            : std::false_type
        {
        };

        //////////////////////////////////////////
        template <class C>
        struct IsConstMemberPair<::std::pair<C const* const,
            TReturnType(C::* const)(TArgs...) const>> : std::true_type
        {
        };

        //////////////////////////////////////////
        template <TReturnType(*function)(TArgs...)>
        static TReturnType StubFunctionGlobal(void* const, TArgs&&... _args)
        {
            return function(::std::forward<TArgs>(_args)...);
        }

        //////////////////////////////////////////
        template <class C, TReturnType(C::*method)(TArgs...)>
        static TReturnType StubFunctionMethod(void* const _object, TArgs&&... _args)
        {
            return (static_cast<C*>(_object)->*method)(::std::forward<TArgs>(_args)...);
        }

        //////////////////////////////////////////
        template <class C, TReturnType(C::*method)(TArgs...) const>
        static TReturnType StubFunctionMethodConst(void* const _object, TArgs&&... _args)
        {
            return (static_cast<C const*>(_object)->*method)(::std::forward<TArgs>(_args)...);
        }


        //////////////////////////////////////////
        template <typename T>
        static typename ::std::enable_if<!(IsMemberPair<T>::value || IsConstMemberPair<T>::value), void*>::type
            GetObjectAddress(void* const _functorObject)
        {
            return static_cast<void*>(static_cast<T*>(_functorObject));
        }

        //////////////////////////////////////////
        template <typename T>
        static typename ::std::enable_if<!(IsMemberPair<T>::value || IsConstMemberPair<T>::value), TReturnType>::type
            StubFunctionFunctor(void* const _functorObject, TArgs&&... _args)
        {
            return (*static_cast<T*>(_functorObject))(::std::forward<TArgs>(_args)...);
        }


        //////////////////////////////////////////
        template <typename T>
        static typename ::std::enable_if<(IsMemberPair<T>::value || IsConstMemberPair<T>::value), void*>::type
            GetObjectAddress(void* const _functorObject)
        {
            return static_cast<void*>(static_cast<T*>(_functorObject)->first);
        }


        //////////////////////////////////////////
        template <typename T>
        static typename ::std::enable_if<(IsMemberPair<T>::value || IsConstMemberPair<T>::value), TReturnType>::type
            StubFunctionFunctor(void* const _functorObject, TArgs&&... _args)
        {
#if (MAZE_DEBUG)
            MAZE_ERROR_RETURN_VALUE_IF(!static_cast<T*>(_functorObject)->first, TReturnType(), "First is null!");
            MAZE_ERROR_RETURN_VALUE_IF(!static_cast<T*>(_functorObject)->second, TReturnType(), "Second is null!");
#endif

            return (static_cast<T*>(_functorObject)->first->*
                    static_cast<T*>(_functorObject)->second)(::std::forward<TArgs>(_args)...);
        }

    
    private:
        StubFunction m_stubFunction{};

        ::std::shared_ptr<void> m_functorStore;
        ::std::size_t m_functorStoreSize = 0;
        FunctorStoreClearFunctionType m_functorStoreClearFunction = nullptr;
        void* m_functorObject = nullptr;
        void* m_object = nullptr;
        
#if (MAZE_DEBUG)
        Char const* m_debugFunctorName = "";
#endif
    };


    //////////////////////////////////////////
    template <class TReturnType, class ...TArgs>
    inline Delegate<TReturnType, TArgs...> CreateDelegate(Delegate<TReturnType, TArgs...> const& _delegate)
    {
        return _delegate;
    }

    //////////////////////////////////////////
    template <class ...TArgs>
    inline Delegate<void, TArgs...> CreateDelegate(Delegate<void, TArgs...> const& _delegate)
    {
        return _delegate;
    }

    //////////////////////////////////////////
    template <class C, class TReturnType, class ...TArgs>
    inline Delegate<TReturnType, TArgs...> CreateDelegate(C* _object, TReturnType(C::* const _method)(TArgs...))
    {
        return Delegate<TReturnType, TArgs...>::From(_object, _method);
    }

    //////////////////////////////////////////
    template <class C, class ...TArgs>
    inline Delegate<void, TArgs...> CreateDelegate(C* _object, void(C::* const _method)(TArgs...))
    {
        return Delegate<void, TArgs...>::template From<C>(_object, _method);
    }

    //////////////////////////////////////////
    template <class TFunctor, class TReturnType, class ...TArgs>
    inline Delegate<TReturnType, TArgs...> CreateDelegate(TFunctor&& _f)
    {
        return Delegate<TReturnType, TArgs...>::From(::std::forward<TFunctor>(_f));
    }

    //////////////////////////////////////////
    template <class TFunctor, class ...TArgs>
    inline Delegate<void, TArgs...> CreateDelegate(TFunctor&& _f)
    {
        return Delegate<void, TArgs...>::From(::std::forward<TFunctor>(_f));
    }

    //////////////////////////////////////////
    template <class TReturnType, class ...TArgs>
    inline Delegate<TReturnType, TArgs...> CreateDelegate(TReturnType(*const _function)(TArgs...))
    {
        return Delegate<TReturnType, TArgs...>::From(_function);
    }

    //////////////////////////////////////////
    template <class ...TArgs>
    inline Delegate<void, TArgs...> CreateDelegate(void(*const _function)(TArgs...))
    {
        return Delegate<void, TArgs...>::From(_function);
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
namespace std
{
    //////////////////////////////////////////
    template <typename TReturnType, typename ...TArgs>
    struct hash<Maze::Delegate<TReturnType, TArgs...>>
    {
        size_t operator()(Maze::Delegate<TReturnType, TArgs...> const& _d) const noexcept
        {
            auto const seed(hash<void*>()(_d.m_functorObject));

            return hash<typename Maze::Delegate<TReturnType, TArgs...>::StubFunction>()(
                _d.m_stubFunction) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };


} // namespace std
//////////////////////////////////////////


#endif // _MazeDelegate_hpp_
//////////////////////////////////////////
