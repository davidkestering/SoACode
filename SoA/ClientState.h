//
// ClientState.h
// Seed of Andromeda
//
// Created by Benjamin Arnold on 11 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//
// Summary:
// State for the client only.
//

#pragma once

#ifndef ClientState_h__
#define ClientState_h__

#include "MainMenuSystemViewer.h"
#include "Camera.h"
#include "BlockTextureLoader.h"
#include "BlockTexturePack.h"
#include "ModPathResolver.h"

class DebugRenderer;
class MeshManager;

class ChunkMeshManager;

struct ClientState {
    ChunkMeshManager* chunkMeshManager = nullptr;
    // TODO(Ben): Commonstate
    DebugRenderer* debugRenderer = nullptr;
    MeshManager* meshManager = nullptr;
    MainMenuSystemViewer* systemViewer = nullptr;

    BlockTextureLoader blockTextureLoader;
    BlockTexturePack* blockTextures = nullptr;
    ModPathResolver texturePathResolver;

    // TODO(Ben): This is temporary?
    CinematicCamera spaceCamera; ///< The camera that looks at the planet from space
    Camera localCamera; ///< Camera for voxels and far terrain

    bool isNewGame = true;
    f64v3 startSpacePos = f64v3(0.0f); ///< Starting position of player entity
};

#endif // ClientState_h__