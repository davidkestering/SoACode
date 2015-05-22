///
/// TerrainPatchMesher.h
/// Seed of Andromeda
///
/// Created by Benjamin Arnold on 3 Feb 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Creates terrain patch meshes
///

#pragma once

#ifndef TerrainPatchMesher_h__
#define TerrainPatchMesher_h__

#include <Vorb/graphics/gtypes.h>
#include "TerrainPatchMesh.h"

struct PlanetGenData;
class TerrainPatchMeshManager;

class TerrainPatchMesher {
public:
    TerrainPatchMesher(TerrainPatchMeshManager* meshManager,
                       PlanetGenData* planetGenData);
    ~TerrainPatchMesher();

    /// Generates mesh using heightmap
    /// @param mesh: The mesh handle
    /// @param startPos: Starting position
    /// @param cubeFace: The world cube face
    /// @param width: Width of the patch
    /// @param heightData: The heightmap data
    /// @param isSpherical: True when this is a spherical mesh
    void buildMesh(OUT TerrainPatchMesh* mesh, const f32v3& startPos, WorldCubeFace cubeFace,
                   float width, float heightData[PATCH_HEIGHTMAP_WIDTH][PATCH_HEIGHTMAP_WIDTH][4],
                   bool isSpherical);

private:

    /// Calculates temperature based on angle with equator
    /// @param range: The range to scale between
    /// @angle: Angle from equator
    /// @param baseTemp: Base temperature at equator
    ui8 calculateTemperature(float range, float angle, float baseTemp);

    /// Calculates humidity based on angle with equator
    /// @param range: The range to scale between
    /// @angle: Angle from equator
    /// @param baseTemp: Base humidity at equator
    ui8 calculateHumidity(float range, float angle, float baseHum);

    /// Builds the skirts for a patch
    void buildSkirts();

    /// Adds water at a given point
    /// @param z: Z position
    /// @Param x: X position
    /// @param heightData: The heightmap data
    void addWater(int z, int x, float heightData[PATCH_HEIGHTMAP_WIDTH][PATCH_HEIGHTMAP_WIDTH][4]);

    /// Tries to add a water vertex at a given spot if one doesn't exist
    /// @param z: Z position
    /// @param x: X position
    /// @param heightData: The heightmap data
    void tryAddWaterVertex(int z, int x, float heightData[PATCH_HEIGHTMAP_WIDTH][PATCH_HEIGHTMAP_WIDTH][4]);

    /// Tries to add a quad at a given spot if one doesnt exist
    /// @param z: Z position
    /// @param x: X position
    void tryAddWaterQuad(int z, int x);

    /// Generates an index buffer.
    /// @param ibo: The index buffer handle
    void generateIndices(OUT VGIndexBuffer& ibo);

    /// Computes angle with the equator based on the normal of position
    /// @param normal: Normalized position on sphere
    float computeAngleFromNormal(const f32v3& normal);

    static VGIndexBuffer m_sharedIbo; ///< Reusable CCW IBO

    // PATCH_WIDTH * 4 is for skirts
    static const int VERTS_SIZE = PATCH_SIZE + PATCH_WIDTH * 4; ///< Number of vertices per patch
    static TerrainVertex verts[VERTS_SIZE]; ///< Vertices for terrain mesh
    static WaterVertex waterVerts[VERTS_SIZE]; ///< Vertices for water mesh
    static ui16 waterIndexGrid[PATCH_WIDTH][PATCH_WIDTH]; ///< Caches water indices for reuse
    static ui16 waterIndices[PATCH_INDICES]; ///< Buffer of indices to upload
    static bool waterQuads[PATCH_WIDTH - 1][PATCH_WIDTH - 1]; ///< True when a quad is present at a spot

    PlanetGenData* m_planetGenData = nullptr; ///< Planetary data
    TerrainPatchMeshManager* m_meshManager = nullptr; ///< Manages the patch meshes

    /// Meshing helper vars
    int m_index;
    int m_waterIndex;
    int m_waterIndexCount;
    float m_vertWidth;
    float m_radius;
    i32v3 m_coordMapping;
    f32v3 m_startPos;
    f32v2 m_coordMults;
    bool m_isSpherical;
    WorldCubeFace m_cubeFace;
};

#endif // TerrainPatchMesher_h__