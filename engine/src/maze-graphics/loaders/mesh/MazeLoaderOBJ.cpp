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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/data/MazeSpan.hpp"
#include "maze-core/MazeTypes.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadOBJ(
        AssetFile const& _file,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadOBJ(fileData, _mesh, _props);
    }


    //////////////////////////////////////////
    // OBJ vertex is uniquely identified by its (position, uv, normal) index triple -
    // this is the key used to deduplicate/share vertices across faces when building
    // the indexed submesh. -1 means "component not present in the source face token".
    //////////////////////////////////////////
    struct ObjVertexKey
    {
        S32 posIndex = -1;
        S32 uvIndex = -1;
        S32 normalIndex = -1;

        inline bool operator==(ObjVertexKey const& _other) const
        {
            return    posIndex == _other.posIndex
                   && uvIndex == _other.uvIndex
                   && normalIndex == _other.normalIndex;
        }
    };

    //////////////////////////////////////////
    struct ObjVertexKeyHash
    {
        inline Size operator()(ObjVertexKey const& _key) const
        {
            U32 h = (U32)_key.posIndex;
            h = h * 486187739u + (U32)_key.uvIndex;
            h = h * 486187739u + (U32)_key.normalIndex;
            return (Size)h;
        }
    };


    //////////////////////////////////////////
    // Advances '_it' past the next whitespace-delimited token in [_it, _end).
    // Returns false (leaving '_it' at '_end') when no more tokens remain.
    //////////////////////////////////////////
    static inline bool NextToken(Char const*& _it, Char const* _end, ConstSpan<Char>& _outToken)
    {
        while (_it != _end && (*_it == ' ' || *_it == '\t'))
            ++_it;

        if (_it == _end)
            return false;

        Char const* start = _it;
        while (_it != _end && *_it != ' ' && *_it != '\t')
            ++_it;

        _outToken.set(start, (Size)(_it - start));
        return true;
    }

    //////////////////////////////////////////
    static inline ConstSpan<Char> GetFirstTokenSpan(Char const* _begin, Char const* _end)
    {
        Char const* it = _begin;
        ConstSpan<Char> token;
        NextToken(it, _end, token);
        return token;
    }

    //////////////////////////////////////////
    static inline ConstSpan<Char> GetTailSpan(Char const* _begin, Char const* _end)
    {
        Char const* it = _begin;
        ConstSpan<Char> firstToken;
        if (!NextToken(it, _end, firstToken))
            return ConstSpan<Char>(_begin, 0);

        while (it != _end && (*it == ' ' || *it == '\t'))
            ++it;

        Char const* tailEnd = _end;
        while (tailEnd > it && (*(tailEnd - 1) == ' ' || *(tailEnd - 1) == '\t'))
            --tailEnd;

        return ConstSpan<Char>(it, (Size)(tailEnd - it));
    }

    //////////////////////////////////////////
    static inline bool TokenEquals(ConstSpan<Char> _token, CString _keyword)
    {
        Size len = strlen(_keyword);
        return _token.size() == len && memcmp(_token.ptr(), _keyword, len) == 0;
    }

    //////////////////////////////////////////
    // OBJ indices are 1-based; negative indices are relative to the element count
    // seen so far (-1 = most recently defined element). 0/absent resolves to -1.
    //////////////////////////////////////////
    static inline S32 ResolveOBJIndex(S32 _raw, Size _poolSize)
    {
        if (_raw > 0)
            return _raw - 1;
        if (_raw < 0)
            return (S32)_poolSize + _raw;
        return -1;
    }

    //////////////////////////////////////////
    // Parses a single face-vertex reference token ("v", "v/vt", "v//vn" or "v/vt/vn").
    // Returns false if the (mandatory) position reference is missing or out of range.
    // Out-of-range optional uv/normal references are dropped (treated as absent)
    // rather than failing the whole face.
    //////////////////////////////////////////
    static bool ParseFaceVertexToken(
        ConstSpan<Char> _token,
        Size _positionsCount,
        Size _uvsCount,
        Size _normalsCount,
        ObjVertexKey& _outKey)
    {
        Char const* begin = _token.ptr();
        Char const* end = begin + _token.size();

        Char const* firstSlash = (Char const*)memchr(begin, '/', _token.size());
        Char const* posEnd = firstSlash ? firstSlash : end;

        if (posEnd == begin)
            return false;

        S32 rawPos = 0;
        StringHelper::ParseInteger<S32>(begin, posEnd, rawPos);
        S32 posIndex = ResolveOBJIndex(rawPos, _positionsCount);
        if (posIndex < 0 || (Size)posIndex >= _positionsCount)
            return false;

        S32 uvIndex = -1;
        S32 normalIndex = -1;

        if (firstSlash)
        {
            Char const* uvBegin = firstSlash + 1;
            Char const* secondSlash = (uvBegin < end) ? (Char const*)memchr(uvBegin, '/', end - uvBegin) : nullptr;
            Char const* uvEnd = secondSlash ? secondSlash : end;

            if (uvEnd > uvBegin)
            {
                S32 rawUV = 0;
                StringHelper::ParseInteger<S32>(uvBegin, uvEnd, rawUV);
                S32 idx = ResolveOBJIndex(rawUV, _uvsCount);
                if (idx >= 0 && (Size)idx < _uvsCount)
                    uvIndex = idx;
            }

            if (secondSlash && (secondSlash + 1) < end)
            {
                Char const* normBegin = secondSlash + 1;
                S32 rawNorm = 0;
                StringHelper::ParseInteger<S32>(normBegin, end, rawNorm);
                S32 idx = ResolveOBJIndex(rawNorm, _normalsCount);
                if (idx >= 0 && (Size)idx < _normalsCount)
                    normalIndex = idx;
            }
        }

        _outKey.posIndex = posIndex;
        _outKey.uvIndex = uvIndex;
        _outKey.normalIndex = normalIndex;
        return true;
    }

    //////////////////////////////////////////
    // Triangulates a simple (possibly concave, possibly non-planar) polygon given as an
    // ordered list of corner positions, via 2D ear-clipping projected onto the polygon's
    // dominant plane (found with Newell's method, so it works regardless of the polygon's
    // orientation in space). Emits corner-index triples (indices into '_positions') into
    // '_outTriangleCornerIndices'. The scratch vectors are caller-owned so repeated calls
    // (one per face in a file) don't reallocate.
    //////////////////////////////////////////
    static void TriangulatePolygon(
        Vector<Vec3F> const& _positions,
        Vector<S32>& _outTriangleCornerIndices,
        Vector<S32>& _scratchNext,
        Vector<S32>& _scratchPrev,
        Vector<Vec2F>& _scratch2D)
    {
        _outTriangleCornerIndices.clear();

        S32 n = (S32)_positions.size();
        if (n < 3)
            return;

        if (n == 3)
        {
            _outTriangleCornerIndices.push_back(0);
            _outTriangleCornerIndices.push_back(1);
            _outTriangleCornerIndices.push_back(2);
            return;
        }

        // Newell's method - robust polygon normal even for near-degenerate/non-planar input
        Vec3F normal = Vec3F::c_zero;
        for (S32 i = 0; i < n; ++i)
        {
            Vec3F const& curr = _positions[i];
            Vec3F const& next = _positions[(i + 1) % n];
            normal.x += (curr.y - next.y) * (curr.z + next.z);
            normal.y += (curr.z - next.z) * (curr.x + next.x);
            normal.z += (curr.x - next.x) * (curr.y + next.y);
        }

        // Project onto the two axes orthogonal to the dominant normal component
        F32 ax = Math::Abs(normal.x);
        F32 ay = Math::Abs(normal.y);
        F32 az = Math::Abs(normal.z);

        Size u, v;
        if (ax >= ay && ax >= az) { u = 1; v = 2; }
        else if (ay >= ax && ay >= az) { u = 2; v = 0; }
        else { u = 0; v = 1; }

        _scratch2D.resize(n);
        for (S32 i = 0; i < n; ++i)
            _scratch2D[i].set(_positions[i][u], _positions[i][v]);

        F32 signedArea2 = 0.0f;
        for (S32 i = 0; i < n; ++i)
        {
            Vec2F const& p0 = _scratch2D[i];
            Vec2F const& p1 = _scratch2D[(i + 1) % n];
            signedArea2 += p0.x * p1.y - p1.x * p0.y;
        }
        bool ccw = signedArea2 >= 0.0f;

        _scratchNext.resize(n);
        _scratchPrev.resize(n);
        for (S32 i = 0; i < n; ++i)
        {
            _scratchNext[i] = (i + 1) % n;
            _scratchPrev[i] = (i - 1 + n) % n;
        }

        S32 remaining = n;
        S32 current = 0;
        S32 noProgress = 0;

        while (remaining > 3)
        {
            S32 prev = _scratchPrev[current];
            S32 next = _scratchNext[current];

            Vec2F const& pPrev = _scratch2D[prev];
            Vec2F const& pCurr = _scratch2D[current];
            Vec2F const& pNext = _scratch2D[next];

            // Reflex vertices can never be ears
            F32 cross = (pCurr.x - pPrev.x) * (pNext.y - pCurr.y) - (pCurr.y - pPrev.y) * (pNext.x - pCurr.x);
            bool isEar = ccw ? (cross >= 0.0f) : (cross <= 0.0f);

            // No other remaining vertex may lie inside the candidate ear triangle
            if (isEar)
            {
                S32 probe = _scratchNext[next];
                while (probe != prev)
                {
                    if (probe != current && Math::IsPointInTriangle(_scratch2D[probe], pPrev, pCurr, pNext))
                    {
                        isEar = false;
                        break;
                    }
                    probe = _scratchNext[probe];
                }
            }

            if (isEar)
            {
                _outTriangleCornerIndices.push_back(prev);
                _outTriangleCornerIndices.push_back(current);
                _outTriangleCornerIndices.push_back(next);

                _scratchNext[prev] = next;
                _scratchPrev[next] = prev;
                --remaining;
                noProgress = 0;
                current = next;
            }
            else
            {
                current = next;
                ++noProgress;

                if (noProgress >= remaining)
                {
                    // Degenerate/self-intersecting polygon - fall back to a plain fan so we
                    // still emit valid (if imperfect) geometry instead of hanging or dropping it.
                    Debug::LogWarning("LoaderOBJ: degenerate polygon during triangulation, falling back to fan triangulation");

                    S32 a = current;
                    S32 b = _scratchNext[a];
                    S32 c = _scratchNext[b];
                    while (c != a)
                    {
                        _outTriangleCornerIndices.push_back(a);
                        _outTriangleCornerIndices.push_back(b);
                        _outTriangleCornerIndices.push_back(c);
                        b = c;
                        c = _scratchNext[c];
                    }
                    remaining = 0;
                    break;
                }
            }
        }

        if (remaining == 3)
        {
            S32 a = current;
            S32 b = _scratchNext[a];
            S32 c = _scratchNext[b];
            _outTriangleCornerIndices.push_back(a);
            _outTriangleCornerIndices.push_back(b);
            _outTriangleCornerIndices.push_back(c);
        }
    }


    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadOBJ(
        ByteBuffer const& _fileData,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        MAZE_PROFILE_EVENT("LoadOBJ");

        _mesh.clear();

        if (_fileData.getSize() == 0)
            return true;

        // Global OBJ attribute pools - face indices refer to these across the whole file,
        // regardless of submesh (o/g/usemtl) boundaries.
        Vector<Vec3F> positions;
        Vector<Vec2F> uvs;
        Vector<Vec3F> normals;

        // Current submesh being accumulated (deduplicated/indexed)
        Vector<Vec3F> outPositions;
        Vector<Vec2F> outUVs;
        Vector<Vec3F> outNormals;
        Vector<Vec3F> outNormalAccum;
        Vector<bool> outHasExplicitNormal;
        Vector<U16> outIndices;
        FlatHashMap<ObjVertexKey, U32, ObjVertexKeyHash> vertexCache;
        bool indexOverflowWarned = false;

        // Per-face scratch, reused across every "f" line in the file to avoid
        // reallocating on every face (this is the hottest part of the parser)
        Vector<ObjVertexKey> faceKeys;
        Vector<Vec3F> facePositions;
        Vector<S32> faceTriangles;
        Vector<S32> eclNext;
        Vector<S32> eclPrev;
        Vector<Vec2F> ecl2D;

        bool listening = false;
        String meshName;

        auto processCreateSubMesh =
            [&]()
            {
                // Finalize normals for vertices that had no explicit "vn" - average the
                // accumulated (area-weighted) face normals of every triangle touching them
                for (Size i = 0, in = outNormals.size(); i < in; ++i)
                {
                    if (outHasExplicitNormal[i])
                        continue;

                    if (outNormalAccum[i].squaredLength() > 1e-12f)
                    {
                        outNormalAccum[i].normalize();
                        outNormals[i] = outNormalAccum[i];
                    }
                }

                SubMeshPtr subMesh = SubMesh::Create();
                subMesh->setName(meshName);

                // Flip X for LHCS
                SubMeshHelper::FlipX(
                    subMesh->getRenderDrawTopology(),
                    outIndices,
                    &outPositions,
                    &outNormals,
                    nullptr);

                for (Vec3F& position : outPositions)
                    position *= _props.scale;

                subMesh->setIndices(&outIndices[0], outIndices.size());
                subMesh->setPositions(&outPositions[0], outPositions.size());
                subMesh->setNormals(&outNormals[0], outNormals.size());
                subMesh->setTexCoords(0, &outUVs[0], outUVs.size());

                // Generate tangents and bitangents
                Vector<Vec3F> tangents;
                Vector<Vec3F> bitangents;
                if (SubMeshHelper::GenerateTangentsAndBitangents(
                    &outIndices[0],
                    outIndices.size(),
                    &outPositions[0],
                    &outUVs[0],
                    &outNormals[0],
                    outPositions.size(),
                    tangents,
                    bitangents))
                {
                    subMesh->setTangents(&tangents[0], tangents.size());
                    subMesh->setBitangents(&bitangents[0], bitangents.size());
                }

                _mesh.addSubMesh(subMesh);

                // Cleanup
                outPositions.clear();
                outUVs.clear();
                outNormals.clear();
                outNormalAccum.clear();
                outHasExplicitNormal.clear();
                outIndices.clear();
                vertexCache.clear();
                indexOverflowWarned = false;
            };

        auto processOBJLine =
            [&](Char const* _begin, Char const* _end)
            {
                if (_begin == _end)
                    return;

                ConstSpan<Char> firstToken = GetFirstTokenSpan(_begin, _end);
                if (firstToken.empty())
                    return;

                if (TokenEquals(firstToken, "o") || TokenEquals(firstToken, "g"))
                {
                    ConstSpan<Char> tail = GetTailSpan(_begin, _end);

                    if (listening && !outIndices.empty() && !outPositions.empty() && !_props.mergeSubMeshes)
                        processCreateSubMesh();

                    listening = true;
                    meshName = tail.empty() ? String("unnamed") : String(tail.ptr(), tail.size());
                }
                else
                // Vertex Position
                if (TokenEquals(firstToken, "v"))
                {
                    ConstSpan<Char> tail = GetTailSpan(_begin, _end);
                    Char const* it = tail.ptr();
                    Char const* tailEnd = it + tail.size();

                    Vec3F position = Vec3F::c_zero;
                    ConstSpan<Char> tok;
                    S32 comp = 0;
                    while (comp < 3 && NextToken(it, tailEnd, tok))
                    {
                        F32 value = 0.0f;
                        StringHelper::ParseF32(tok.ptr(), tok.size(), value);
                        position[(Size)comp] = value;
                        ++comp;
                    }

                    if (comp == 3)
                        positions.push_back(position);
                }
                else
                // Vertex UV
                if (TokenEquals(firstToken, "vt"))
                {
                    ConstSpan<Char> tail = GetTailSpan(_begin, _end);
                    Char const* it = tail.ptr();
                    Char const* tailEnd = it + tail.size();

                    Vec2F uv = Vec2F::c_zero;
                    ConstSpan<Char> tok;
                    S32 comp = 0;
                    while (comp < 2 && NextToken(it, tailEnd, tok))
                    {
                        F32 value = 0.0f;
                        StringHelper::ParseF32(tok.ptr(), tok.size(), value);
                        uv[(Size)comp] = value;
                        ++comp;
                    }

                    if (comp == 2)
                        uvs.push_back(uv);
                }
                else
                // Vertex Normal
                if (TokenEquals(firstToken, "vn"))
                {
                    ConstSpan<Char> tail = GetTailSpan(_begin, _end);
                    Char const* it = tail.ptr();
                    Char const* tailEnd = it + tail.size();

                    Vec3F normal = Vec3F::c_zero;
                    ConstSpan<Char> tok;
                    S32 comp = 0;
                    while (comp < 3 && NextToken(it, tailEnd, tok))
                    {
                        F32 value = 0.0f;
                        StringHelper::ParseF32(tok.ptr(), tok.size(), value);
                        normal[(Size)comp] = value;
                        ++comp;
                    }

                    if (comp == 3)
                        normals.push_back(normal);
                }
                else
                // Face
                if (TokenEquals(firstToken, "f"))
                {
                    ConstSpan<Char> tail = GetTailSpan(_begin, _end);
                    Char const* it = tail.ptr();
                    Char const* tailEnd = it + tail.size();

                    faceKeys.clear();
                    facePositions.clear();

                    ConstSpan<Char> tok;
                    bool faceValid = true;
                    while (NextToken(it, tailEnd, tok))
                    {
                        ObjVertexKey key;
                        if (!ParseFaceVertexToken(tok, positions.size(), uvs.size(), normals.size(), key))
                        {
                            faceValid = false;
                            break;
                        }

                        faceKeys.push_back(key);
                        facePositions.push_back(positions[key.posIndex]);
                    }

                    if (!faceValid)
                    {
                        Debug::LogWarning("LoaderOBJ: skipping face with invalid/out-of-range vertex reference");
                        return;
                    }

                    if (facePositions.size() < 3)
                        return;

                    TriangulatePolygon(facePositions, faceTriangles, eclNext, eclPrev, ecl2D);

                    for (Size t = 0, tn = faceTriangles.size(); t < tn; t += 3)
                    {
                        S32 cornerIdx[3] = { faceTriangles[t], faceTriangles[t + 1], faceTriangles[t + 2] };
                        U32 slot[3];
                        bool overflow = false;

                        for (S32 c = 0; c < 3; ++c)
                        {
                            ObjVertexKey const& key = faceKeys[cornerIdx[c]];

                            auto cacheIt = vertexCache.find(key);
                            if (cacheIt != vertexCache.end())
                            {
                                slot[c] = cacheIt->second;
                                continue;
                            }

                            if (outPositions.size() >= 65535)
                            {
                                if (!indexOverflowWarned)
                                {
                                    Debug::LogWarning(
                                        "LoaderOBJ: submesh exceeds 65535 unique vertices, truncating "
                                        "(split the mesh or add usemtl/g boundaries to keep submeshes smaller)");
                                    indexOverflowWarned = true;
                                }
                                overflow = true;
                                break;
                            }

                            slot[c] = (U32)outPositions.size();
                            vertexCache.emplace(key, slot[c]);

                            outPositions.push_back(positions[key.posIndex]);
                            outUVs.push_back(key.uvIndex >= 0 ? SubMeshHelper::NormalizeUV(uvs[key.uvIndex]) : Vec2F::c_zero);

                            bool hasNormal = key.normalIndex >= 0;
                            outNormals.push_back(hasNormal ? normals[key.normalIndex] : Vec3F::c_zero);
                            outNormalAccum.push_back(Vec3F::c_zero);
                            outHasExplicitNormal.push_back(hasNormal);
                        }

                        if (overflow)
                            break;

                        outIndices.push_back((U16)slot[0]);
                        outIndices.push_back((U16)slot[1]);
                        outIndices.push_back((U16)slot[2]);

                        if (!outHasExplicitNormal[slot[0]] || !outHasExplicitNormal[slot[1]] || !outHasExplicitNormal[slot[2]])
                        {
                            Vec3F faceNormal = Math::GenTriangleNormal(
                                outPositions[slot[0]], outPositions[slot[1]], outPositions[slot[2]]);

                            if (!outHasExplicitNormal[slot[0]]) outNormalAccum[slot[0]] += faceNormal;
                            if (!outHasExplicitNormal[slot[1]]) outNormalAccum[slot[1]] += faceNormal;
                            if (!outHasExplicitNormal[slot[2]]) outNormalAccum[slot[2]] += faceNormal;
                        }
                    }
                }
                else
                // Material Name - splits the submesh, but material assignment itself isn't
                // wired up yet (mtllib is skipped below too)
                if (TokenEquals(firstToken, "usemtl"))
                {
                    if (!outIndices.empty() && !outPositions.empty() && !_props.mergeSubMeshes)
                        processCreateSubMesh();
                }
                else
                // Material Library
                if (TokenEquals(firstToken, "mtllib"))
                {
                    // Skip materials
                }
            };

        Char const* fileBegin = (Char const*)_fileData.getDataRO();
        Char const* fileEnd = fileBegin + _fileData.getSize();
        Char const* cursor = fileBegin;

        while (cursor < fileEnd)
        {
            Char const* lineEnd = (Char const*)memchr(cursor, '\n', fileEnd - cursor);
            Char const* nextCursor = lineEnd ? (lineEnd + 1) : fileEnd;
            Char const* trimmedEnd = lineEnd ? lineEnd : fileEnd;

            if (trimmedEnd > cursor && *(trimmedEnd - 1) == '\r')
                --trimmedEnd;

            processOBJLine(cursor, trimmedEnd);
            cursor = nextCursor;
        }

        if (!outIndices.empty() && !outPositions.empty())
        {
            // Create Mesh
            processCreateSubMesh();
        }

        return true;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsOBJFile(
        AssetFile const& _file)
    {
        return _file.getExtension() == "obj";
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsOBJFile(
        ByteBuffer const& _fileData)
    {
        return false;
    }

} // namespace Maze
//////////////////////////////////////////
