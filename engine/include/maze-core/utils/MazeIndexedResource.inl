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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template <typename T>
    IndexedResource<T>::IndexedResource()
    {
        m_resourceId = GenerateNewResourceId(static_cast<T*>(this));
    }

    //////////////////////////////////////////
    template <typename T>
    IndexedResource<T>::~IndexedResource()
    {
        ReleaseResourceId(m_resourceId);
    }

    //////////////////////////////////////////
    template <typename T>
    T* IndexedResource<T>::GetResource(ResourceId _id)
    {
        if ((Size)_id.getIndex() >= s_resources.size())
            return nullptr;

        Size index = (Size)_id.getIndex();
        return s_resources[index];
    }

    //////////////////////////////////////////
    template <typename T>
    ResourceId IndexedResource<T>::GenerateNewResourceId(T* _ptr)
    {
        if (!s_freeResourceIndices.empty())
        {
            ResourceId id = s_freeResourceIndices.top();
            s_freeResourceIndices.pop();
            s_resources[(Size)id.getIndex()] = _ptr;
            return id;
        }

        ResourceId id((S8)s_resources.size(), 0);
        s_resources.push_back(_ptr);
        return id;
    }

    //////////////////////////////////////////
    template <typename T>
    void IndexedResource<T>::ReleaseResourceId(ResourceId _id)
    {
        s_resources[(Size)_id.getIndex()] = nullptr;
        _id.incrementGeneration();
        s_freeResourceIndices.push(_id);
    }
    
} // namespace Maze
//////////////////////////////////////////
