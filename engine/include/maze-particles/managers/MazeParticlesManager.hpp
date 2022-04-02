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
#if (!defined(_MazeParticlesManager_hpp_))
#define _MazeParticlesManager_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ParticlesManager);


    //////////////////////////////////////////
    // Class ParticlesManager
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticlesManager
    {
    public:

        //////////////////////////////////////////
        virtual ~ParticlesManager();

        //////////////////////////////////////////
        static void Initialize(ParticlesManagerPtr& _particlesManager);


        //////////////////////////////////////////
        static inline ParticlesManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline ParticlesManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline Texture2DPtr const& getDefaultParticleTexture() const { return m_defaultParticleTexture; }

        //////////////////////////////////////////
        inline SpritePtr const& getDefaultParticleSprite() const { return m_defaultParticleSprite;}

        //////////////////////////////////////////
        inline MaterialPtr const& getDefaultParticleMaterial() const { return m_defaultParticleMaterial; }


        //////////////////////////////////////////
        void createBuiltinAssets();

    protected:

        //////////////////////////////////////////
        ParticlesManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static ParticlesManager* s_instance;

        Texture2DPtr m_defaultParticleTexture;
        SpritePtr m_defaultParticleSprite;
        MaterialPtr m_defaultParticleMaterial;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticlesManager_hpp_
//////////////////////////////////////////
