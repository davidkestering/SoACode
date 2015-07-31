//
// ChunkAccessor.h
// Seed of Andromeda
//
// Created by Benjamin Arnold on 30 Jul 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//
// Summary:
// Does ref counting and fires events for chunk access
//

#pragma once

#ifndef ChunkAccessor_h__
#define ChunkAccessor_h__

#include "Chunk.h"
#include <unordered_map>

class ChunkHandle {
public:
    ChunkHandle aquire();
    void release();

    operator Chunk&() { return *chunk; }
    operator const Chunk&() const { return *chunk; }
    
    Chunk* chunk = nullptr;
};

class ChunkAccessor {
    friend class ChunkHandle;
public:
    void init(PagedChunkAllocator* allocator);
    void destroy();

    ChunkHandle aquire(ChunkID id);

private:
    ChunkHandle aquire(ChunkHandle& chunk);
    void release(ChunkHandle& chunk);

    std::unordered_map<ChunkID, ChunkHandle> m_chunkLookup;
    PagedChunkAllocator* m_allocator = nullptr;
};

#endif // ChunkAccessor_h__
