///
/// SystemBodyLoader.h
/// Seed of Andromeda
///
/// Created by Benjamin Arnold on 12 Jul 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Loads a system body
///

#pragma once

#ifndef SystemBodyLoader_h__
#define SystemBodyLoader_h__

#include "PlanetGenLoader.h"

#include <Vorb/VorbPreDecl.inl>

struct SoaState;
DECL_VIO(class IOManager);

struct SystemBodyProperties;


class SpaceSystemBodyLoader {
public:
    void init(vio::IOManager* iom);

    bool SpaceSystemBodyLoader::loadBody(const SoaState* soaState, const nString& filePath,
                                    SystemBodyProperties* body,
                                    vcore::RPCManager* glrpc = nullptr);

private:
    vio::IOManager* m_iom;
    PlanetGenLoader m_planetLoader;
};

#endif // SystemBodyLoader_h__
