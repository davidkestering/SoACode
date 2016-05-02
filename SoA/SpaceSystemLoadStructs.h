///
/// SpaceSystemLoadStructs.h
/// Seed of Andromeda
///
/// Created by Benjamin Arnold on 20 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Structs for SpaceSystem initialization
///

#pragma once

#ifndef SpaceSystemLoadStructs_h__
#define SpaceSystemLoadStructs_h__

struct PlanetGenData;

#include <Vorb/graphics/gtypes.h>
#include <Vorb/io/Keg.h>
#include <Vorb/ecs/Entity.h>

enum class SpaceBodyGenerationType {
    NONE,
    PLANET,
    STAR,
    GAS_GIANT
};
KEG_TYPE_DECL(SpaceBodyGenerationType);

enum class SpaceBodyType {
    NONE,
    BARYCENTER,
    STAR,
    PLANET,
    DWARF_PLANET,
    MOON,
    DWARF_MOON,
    ASTEROID,
    COMET
};
KEG_TYPE_DECL(SpaceBodyType);

enum class TrojanType {
    NONE,
    L4,
    L5
};
KEG_TYPE_DECL(TrojanType);

struct AtmosphereProperties {
    f32 kr = 0.0025f;
    f32 km = 0.0020f;
    f32 g = -0.99f;
    f32 scaleDepth = 0.25f;
    f32v3 waveLength = f32v3(0.65, 0.57, 0.475);
};
KEG_TYPE_DECL(AtmosphereProperties);

struct PlanetRingProperties {
    f32 innerRadius = 0.0f;
    f32 outerRadius = 0.0f;
    f32 aTilt = 0.0f;
    f32 lNorth = 0.0f;
    nString colorLookup = "";
};
KEG_TYPE_DECL(PlanetRingProperties);

struct CloudsProperties {
    f32v3 color = f32v3(1.0f, 1.0f, 1.0f);
    f32v3 scale = f32v3(1.0f, 1.5f, 1.0f);
    float density = 0.0f;
};
KEG_TYPE_DECL(CloudsProperties);

struct SystemBodyProperties {
    nString name = ""; ///< Name of the body, doubles as parent name during parsing
    nString path = ""; ///< Path to directory
    nString ref = ""; ///< Orbital period reference body

    SystemBodyProperties* parent;
    std::vector<SystemBodyProperties*> children;

    vecs::EntityID entity = 0;

    // TODO(Ben): Figure out a way to exclude these
    bool isBaryCalculated = false; ///< Used by barycenters
    bool hasComputedRef = false; ///< True when it has computed trojan and t with ref body

    // Type information
    SpaceBodyType type = SpaceBodyType::NONE; ///< Specific type of object
    TrojanType trojan = TrojanType::NONE;
    
    SpaceBodyGenerationType genType = SpaceBodyGenerationType::NONE; ///< How its generated
    void* genTypeProperties = nullptr; ///< genType specific data

    // Orbit Properties
    f64 mass = 0.0;
    f64 e = 0.0; ///< Shape of orbit, 0-1
    f64 t = 0.0; ///< Period of a full orbit in sec
    f64 a = 0.0; ///< Start mean anomaly in deg
    f64 n = 0.0; ///< Longitude of the ascending node in deg
    f64 p = 0.0; ///< Longitude of the periapsis in deg
    f64 i = 0.0; ///< Inclination in deg
    f64 RA = 0.0; ///< Right ascension relative to sol
    f64 dec = 0.0; ///< Declination relative to sol
    f64 dist = 0.0; ///< Distance from sol
    f64 td = 1.0; ///< Reference body period divisor
    f64 tf = 1.0; ///< Reference body period factor
    f64 minor = 0.0; ///< Semi-minor of the ellipse in KM
    f64 major = 0.0; ///< Semi-minor of the ellipse in KM

    f32 aTilt = 0.0; ///< Axial Tilt
    f32 lNorth = 0.0; ///< Longitude of the north

    // General properties
    f64 diameter = 0.0;
    f64 density = 0.0;
    f64 rotationalPeriod = 0.0;

    // Barycenter binary components
    Array<const char*> comps;
};
KEG_TYPE_DECL(SystemBodyProperties);

struct PlanetProperties {
    PlanetGenData* planetGenData = nullptr;
    AtmosphereProperties atmosphere;
    CloudsProperties clouds;
};
KEG_TYPE_DECL(PlanetProperties);

struct StarProperties {
    f64 surfaceTemperature = 0.0; ///< temperature in kelvin
};
KEG_TYPE_DECL(StarProperties);

struct GasGiantProperties {
    f32 oblateness = 0.0;
    nString colorMap = "";
    AtmosphereProperties atmosphere;
    Array<PlanetRingProperties> rings;
};
KEG_TYPE_DECL(GasGiantProperties);

#endif // SpaceSystemLoadStructs_h__
