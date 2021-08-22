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
#include "MazeParticlesHeader.hpp"
#include "maze-particles/MazeParticleSystem3DZone.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ParticleSystem3DZoneType);
    
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneEmissionPoint,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, direction, Vec3DF::c_unitZ, getDirection, setDirection));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneSphere,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, scale, Vec3DF::c_one, getScale, setScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radius, 1.0f, getRadius, setRadius));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneHemisphere,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, scale, Vec3DF::c_one, getScale, setScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radius, 1.0f, getRadius, setRadius));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneTorus,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, scale, Vec3DF::c_one, getScale, setScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radius, 0.5f, getRadius, setRadius),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, torusRadius, 1.0f, getTorusRadius, setTorusRadius),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radiusThickness, 1.0f, getRadiusThickness, setRadiusThickness));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneCone,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, scale, Vec3DF::c_one, getScale, setScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radius, 0.5f, getRadius, setRadius),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, angle, 0.0f, getAngle, setAngle),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, length, 0.5f, getLength, setLength));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneBox,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, scale, Vec3DF::c_one, getScale, setScale));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneCircle,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, position, Vec3DF::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, scale, Vec3DF::c_one, getScale, setScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radius, 1.0f, getRadius, setRadius));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DZoneData,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneSphere, sphere, ParticleSystem3DZoneSphere(), getSphere, setSphere),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneHemisphere, hemisphere, ParticleSystem3DZoneHemisphere(), getHemisphere, setHemisphere),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneTorus, torus, ParticleSystem3DZoneTorus(), getTorus, setTorus),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneCone, cone, ParticleSystem3DZoneCone(), getCone, setCone),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneBox, box, ParticleSystem3DZoneBox(), getBox, setBox),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneCircle, circle, ParticleSystem3DZoneCircle(), getCircle, setCircle));
    
    
} // namespace Maze
//////////////////////////////////////////
