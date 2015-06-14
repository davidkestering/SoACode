#include "stdafx.h"
#include "ModelMesher.h"

#include "VoxelMatrix.h"
#include "VoxelModel.h"
#include "VoxelModelMesh.h"
#include "MarchingCubesCross.h"

#include <vector>

const f32v3 VOXEL_MODEL[24] = {
    f32v3(0, 1, 0),
    f32v3(0, 1, 1),
    f32v3(0, 0, 0),
    f32v3(0, 0, 1),

    f32v3(1, 1, 1),
    f32v3(1, 1, 0),
    f32v3(1, 0, 1),
    f32v3(1, 0, 0),

    f32v3(0, 0, 1),
    f32v3(1, 0, 1),
    f32v3(0, 0, 0),
    f32v3(1, 0, 0),

    f32v3(0, 1, 0),
    f32v3(1, 1, 0),
    f32v3(0, 1, 1),
    f32v3(1, 1, 1),

    f32v3(1, 1, 0),
    f32v3(0, 1, 0),
    f32v3(1, 0, 0),
    f32v3(0, 0, 0),

    f32v3(0, 1, 1),
    f32v3(1, 1, 1),
    f32v3(0, 0, 1),
    f32v3(1, 0, 1)
};

const ui32 VOXEL_INDICES[6] = {
    0, 2, 1,
    1, 2, 3
};

const i32v3 VOXEL_SIDES[6] = {
    i32v3(-1, 0, 0),
    i32v3(1, 0, 0),
    i32v3(0, -1, 0),
    i32v3(0, 1, 0),
    i32v3(0, 0, -1),
    i32v3(0, 0, 1),
};

VoxelModelMesh ModelMesher::createMesh(const VoxelModel* model) {
    std::vector<VoxelModelVertex> vertices;
    std::vector<ui32> indices;
    VoxelModelMesh rv;
    
    genMatrixMesh(model->getMatrix(), vertices, indices);

    if (indices.size() == 0) return rv;

    glGenVertexArrays(1, &rv.m_vao);
    glBindVertexArray(rv.m_vao);

    glGenBuffers(1, &rv.m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rv.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VoxelModelVertex), vertices.data(), GL_STATIC_DRAW);
    
    rv.m_indCount = indices.size();
    rv.m_triCount = (indices.size() * 2) / 6;
    glGenBuffers(1, &rv.m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rv.m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rv.m_indCount * sizeof(ui32), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(rv.m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, rv.m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rv.m_ibo);
    glBindVertexArray(0);
    // THIS CAUSES CRASH v v v
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return rv;
}

VoxelModelMesh ModelMesher::createMarchingCubesMesh(const VoxelModel* model) {
    std::vector<VoxelModelVertex> vertices;
    std::vector<ui32> indices;
    VoxelModelMesh rv;

    auto& matrix = model->getMatrix();

    f32v4* points = new f32v4[(matrix.size.x + 1) * (matrix.size.y + 1) * (matrix.size.z + 1)];
    int index = 0;
    for (i32 x = 0; x < matrix.size.x + 1; x++) {
        for (i32 y = 0; y < matrix.size.y + 1; y++) {
            for (i32 z = 0; z < matrix.size.z + 1; z++) {
                f32v4 vert(x, y, z, 0);
                vert.w = getMarchingPotential(matrix, x, y, z);
               
                points[x*(matrix.size.y + 1)*(matrix.size.z + 1) + y*(matrix.size.z + 1) + z] = vert;
                index++;
            }
        }
    }

    int numTriangles;       
    TRIANGLE* tris = MarchingCubesCross(matrix.size.x, matrix.size.y, matrix.size.z, 0.5f, points, numTriangles);

    f32v3 mainOffset(matrix.size.x / 2.0f, matrix.size.y / 2.0f, matrix.size.z / 2.0f);

    for (int i = 0; i < numTriangles; i++) {

        vertices.emplace_back(tris[i].p[0] - mainOffset, getColor(tris[i].p[0], matrix), tris[i].norm);
        vertices.emplace_back(tris[i].p[1] - mainOffset, getColor(tris[i].p[1], matrix), tris[i].norm);
        vertices.emplace_back(tris[i].p[2] - mainOffset, getColor(tris[i].p[2], matrix), tris[i].norm);
    }

    rv.m_triCount = numTriangles;
    glGenVertexArrays(1, &rv.m_vao);
    glBindVertexArray(rv.m_vao);

    glGenBuffers(1, &rv.m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rv.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VoxelModelVertex), vertices.data(), GL_STATIC_DRAW);

    rv.m_indCount = 0;
    glGenBuffers(1, &rv.m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rv.m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rv.m_indCount * sizeof(ui32), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(rv.m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, rv.m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rv.m_ibo);
    glBindVertexArray(0);
    // THIS CAUSES CRASH v v v
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return rv;
}

color3 ModelMesher::getColor(const f32v3& pos, const VoxelMatrix& matrix) {
    i32v3 ipos(glm::round(pos));
    if (ipos.x >= matrix.size.x) ipos.x = matrix.size.x - 1;
    if (ipos.y >= matrix.size.y) ipos.y = matrix.size.y - 1;
    if (ipos.z >= matrix.size.z) ipos.z = matrix.size.z - 1;
    int x = ipos.x;
    int y = ipos.y;
    int z = ipos.z;

    int numColors = 0;
    i32v3 fColor(0);

    if (y > 0) {
        if (z > 0) {
            // Bottom back left
            if (x > 0) {
                color4 color = matrix.getColor(x - 1, y - 1, z - 1);
                if (color.a != 0 && !matrix.isInterior(x - 1, y - 1, z - 1)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
            // Bottom back right
            if (x < matrix.size.x) {
                color4 color = matrix.getColor(x, y - 1, z - 1);
                if (color.a != 0 && !matrix.isInterior(x, y - 1, z - 1)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
        }
        if (z < matrix.size.z) {
            // Bottom front left
            if (x > 0) {
                color4 color = matrix.getColor(x - 1, y - 1, z);
                if (color.a != 0 && !matrix.isInterior(x - 1, y - 1, z)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
            // Bottom front right
            if (x < matrix.size.x) {
                color4 color = matrix.getColor(x, y - 1, z);
                if (color.a != 0 && !matrix.isInterior(x, y - 1, z)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
        }

    }

    if (y < matrix.size.y) {
        if (z > 0) {
            // Top back left
            if (x > 0) {
                color4 color = matrix.getColor(x - 1, y, z - 1);
                if (color.a != 0 && !matrix.isInterior(x - 1, y, z - 1)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
            // Top back right
            if (x < matrix.size.x) {
                color4 color = matrix.getColor(x, y, z - 1);
                if (color.a != 0 && !matrix.isInterior(x, y, z - 1)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
        }
        if (z < matrix.size.z) {
            // Top front left
            if (x > 0) {
                color4 color = matrix.getColor(x - 1, y, z);
                if (color.a != 0 && !matrix.isInterior(x - 1, y, z)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
            // Top front right
            if (x < matrix.size.x) {
                color4 color = matrix.getColor(x, y, z);
                if (color.a != 0 && !matrix.isInterior(x, y, z)) {
                    numColors++;
                    fColor.r += color.r;
                    fColor.g += color.g;
                    fColor.b += color.b;
                }
            }
        }
    }
    if (numColors) {
        fColor /= numColors;
    }

    return color3(fColor.r, fColor.g, fColor.b);
}

f32 ModelMesher::getMarchingPotential(const VoxelMatrix& matrix, int x, int y, int z) {
    f32 potential = 0.0f;
    int filterSize = 2;
    int halfFilterSize = filterSize / 2;
    x -= halfFilterSize;
    y -= halfFilterSize;
    z -= halfFilterSize;
    for (int i = 0; i < filterSize; i++) {
        for (int j = 0; j < filterSize; j++) {
            for (int k = 0; k < filterSize; k++) {
                if (matrix.getColor(x + i, y + j, z + k).a != 0) {
                    potential += 1.0f;
                }
            }
        }
    }
   
    return potential / (filterSize * filterSize * filterSize);
}

void ModelMesher::genMatrixMesh(const VoxelMatrix& matrix, std::vector<VoxelModelVertex>& vertices, std::vector<ui32>& indices) {
    // TODO(Ben): Could be optimized
    f32v3 mainOffset(matrix.size.x / 2.0f, matrix.size.y / 2.0f, matrix.size.z / 2.0f);
    for(i32 i = 0; i < matrix.size.x; i++) {
        for(i32 j = 0; j < matrix.size.y; j++) {
            for(i32 k = 0; k < matrix.size.z; k++) {
                ColorRGBA8 voxel = matrix.getColor(i, j, k); // Get the current voxel's color
                if(voxel.a == 0) continue; // If the current voxel is invisible go to next voxel

                f32v3 offset = f32v3(i, j, k) - mainOffset; // Position of the current voxel in the model
                for(i32 face = 0; face < 6; face++) { // For each face of the voxel
                    if(matrix.getColor(i32v3(i, j, k) + VOXEL_SIDES[face]).a == 0) { // Check if the adjacent voxel is invisible
                        i32 indexStart = vertices.size(); // Get the position of the first vertex for this face
                        for(int l = 0; l < 4; l++) // Add the 4 vertices for this face
                            vertices.emplace_back(offset + VOXEL_MODEL[face * 4 + l], voxel.rgb, f32v3(VOXEL_SIDES[face]));
                        for(int l = 0; l < 6; l++) // Add the 6 indices for this face
                            indices.push_back(indexStart + VOXEL_INDICES[l]);
                    }
                }
            }
        }
    }
}