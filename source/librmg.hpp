/**
 * Copyright (C) Paul Wortmann, PhysHex Games, www.physhexgames.com
 * This file is part of "Librmg"
 *
 * "Librmg" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 only.
 *
 * "Librmg" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with "Librmg" If not, see <http://www.gnu.org/licenses/>.
 *
 * @author  Paul Wortmann
 * @email   physhex@gmail.com
 * @website www.physhexgames.com
 * @license GPL V2
 * @date 2018-04-01
 */

#ifndef __LIBRMG_HPP__
#define __LIBRMG_HPP__

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <dirent.h>

//---- Layer - Base ----//
#define RMG_BASE_WALL   0
#define RMG_BASE_FLOOR  1
#define RMG_BASE_LIQUID 2

//---- Layer - Object ----//
#define RMG_OBJECT_NONE      0
#define RMG_OBJECT_AS_START  1
#define RMG_OBJECT_AS_END    2
#define RMG_OBJECT_AS_PATH   3
#define RMG_OBJECT_DOOR      4
#define RMG_OBJECT_CHEST     5
#define RMG_OBJECT_CHEST_KEY 6
#define RMG_OBJECT_KEY       7
#define RMG_OBJECT_LEVER     8
#define RMG_OBJECT_TREE      9
#define RMG_OBJECT_STAIRS    10

//---- Layer - Event ----//
#define RMG_EVENT_NONE         0
#define RMG_EVENT_TRIGGER      1
#define RMG_EVENT_TRIGGERABLE  2
#define RMG_EVENT_PLAYER_GOAL  3
#define RMG_EVENT_SPAWN_NPC    4
#define RMG_EVENT_SPAWN_BOSS   5
#define RMG_EVENT_SPAWN_PLAYER 6

#define RMG_PATH_SL 0 // Straight Line
#define RMG_PATH_ND 1 // Ninety Degree angle lines
#define RMG_PATH_DW 2 // Drunken Walk
#define RMG_PATH_CC 3 // Curved Corner

#define RMG_NOROOM -1
#define RMG_NONE    0
#define RMG_NORTH   1
#define RMG_SOUTH   2
#define RMG_EAST    3
#define RMG_WEST    4

#define RMG_RANDOM 0
#define RMG_CIRCLE 1
#define RMG_SQUARE 2

#define RMG_GEN_C1 0 // Cave 1
#define RMG_GEN_C2 1 // Cave 2
#define RMG_GEN_D1 2 // Dungeon 1
#define RMG_GEN_D2 3 // Dungeon 2
#define RMG_GEN_M1 4 // Maze 1
#define RMG_GEN_T1 5 // Town 1

#define RMG_ROOM_EMPTY      0
#define RMG_ROOM_BRIDGE     1
#define RMG_ROOM_WELL       2
#define RMG_ROOM_BLACKSMITH 3
#define RMG_ROOM_STORE      4

#define RMG_AS_NONE   0
#define RMG_AS_START  1
#define RMG_AS_END    2
#define RMG_AS_OPEN   3
#define RMG_AS_CLOSED 4

#define RMG_AS_MOV_D  20
#define RMG_AS_MOV_S  10
#define RMG_AS_MOV_H  10

namespace rmg
{

    struct sPrefabTile
    {
        uint16_t b = RMG_BASE_FLOOR;
        uint16_t o = RMG_OBJECT_NONE;
        uint16_t e = RMG_EVENT_NONE;
    };

    struct sPrefab
    {
        uint32_t count = 0;
        sPrefabTile *tile = nullptr;
    };

    struct sPrefabData
    {
        uint32_t count = 0;
        std::string *filename = nullptr;
        sPrefab *prefab = nullptr;
    };

    struct sPath
    {
        uint32_t *path = nullptr; // array of path tile numbers
        uint32_t length = 0;
        uint32_t position = 0; // current position on path
        uint32_t sx = 0;
        uint32_t sy = 0;
        uint32_t ex = 0;
        uint32_t ey = 0;
    };

    struct sTile
    {
        bool     c = false; // completed flag
        uint16_t d = RMG_BASE_FLOOR;
        uint16_t o = RMG_OBJECT_NONE;
        uint16_t e = RMG_EVENT_NONE;
        uint16_t r = 0; // room number
        uint32_t x = 0;
        uint32_t y = 0;

        uint32_t H = 0; // Heuristic cost
        uint32_t G = 0; // Movement cost
        uint32_t F = 0; // Total cost
        uint32_t A = RMG_AS_NONE; // Attribute
        uint32_t L = RMG_AS_NONE; // List
        uint32_t P = 0; // Parent tile
    };

    struct sRoom
    {
        bool     hidden = false;
        bool     secret = false;
        uint16_t type = RMG_ROOM_EMPTY;
        bool     p = false; // processed flag
        uint16_t posXMin = 0;
        uint16_t posXMax = 0;
        uint16_t posYMin = 0;
        uint16_t posYMax = 0;
        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t w = 0;
        uint16_t h = 0;
        int16_t exitN = RMG_NOROOM; // -1 for none, else connecting room ID
        int16_t exitS = RMG_NOROOM;
        int16_t exitE = RMG_NOROOM;
        int16_t exitW = RMG_NOROOM;
    };

    struct sMap
    {
        sTile *tile = nullptr;
        sRoom *room = nullptr;
        uint32_t w = 100;
        uint32_t h = 100;
        uint32_t tileCount = w * h; // w * h
        uint64_t seed = 0; // seed, 0 for random seed
        uint16_t pass = 4; // general number of passes, smoothing etc...
        uint16_t density = 10; // percentage / 2, rough approximate
        uint16_t roomCount = 0;
        uint16_t roomRadiusMax = 9; // max room radius
        uint16_t roomRadiusMin = 5; // min room radius
        uint16_t roomBorder = 4;
        uint16_t roomShape = RMG_SQUARE;
        uint16_t floorAreaMin = 60; // percentage, min % floor area
        uint16_t generationAlgorithm = RMG_GEN_D1;
        uint16_t connectivityAlgorithm = RMG_PATH_SL;
        uint16_t connectivityComplexity = 50; // percentage, rand % connect neighbors
        uint16_t connectivityPadding = 0; // Tiles to pad on each side of generated paths, roomBorder should be taken into consideration!
    };

    // --- librmg.cpp ---
    void mapInit(sMap &_map);
    void mapFree(sMap &_map);
    void mapGen(sMap &_map);

    // --- librmg_rooms.cpp ---
    bool genCircleRoomOK(sMap &_map, const uint32_t &_x, const uint32_t &_y, const uint32_t &_r);
    void genCircleRoom(sMap &_map, const uint32_t &_x, const uint32_t &_y, const uint32_t &_r);
    bool genSquareRoomOK(sMap &_map, const uint32_t &_x, const uint32_t &_y, const uint32_t &_r);
    void genSquareRoom(sMap &_map, const uint32_t &_x, const uint32_t &_y, const uint32_t &_r);
    void mapFindRooms(sMap &_map);
    uint32_t mapGetRoomArea(sMap &_map, const uint32_t &_r);
    void mapRoomDiscardAllButLargest(sMap &_map);
    void mapInitRooms(sMap &_map);

    // --- librmg_utils.cpp ---
    void mapDraw(sMap &_map);
    uint32_t mapGetFloorMin(sMap &_map);
    uint32_t mapGetFloorMax(sMap &_map);
    void mapPerimeterWalls(sMap &_map);
    uint32_t mapGetFloorArea(sMap &_map);
    void mapRemoveAnomalies(sMap &_map);

    // --- librmg_C1.cpp ---
    void genC1(sMap &_map);

    // --- librmg_C2.cpp ---
    void genC2(sMap &_map);

    // --- librmg_D1.cpp ---
    void genD1(sMap &_map);

    // --- librmg_D2.cpp ---
    void genD2(sMap &_map);

    // --- librmg_M1.cpp ---
    void genM1(sMap &_map);

    // --- librmg_T1.cpp ---
    void genT1(sMap &_map);

    // --- librmg_connectivity.cpp ---
    void mapConnectRooms(sMap &_map);
    void mapConnectRooms_SL(sMap &_map, const uint16_t &_r1, const uint16_t &_r2);
    void mapConnectRooms_ND(sMap &_map, const uint16_t &_r1, const uint16_t &_r2);
    void mapConnectRooms_DW(sMap &_map, const uint16_t &_r1, const uint16_t &_r2);
    void mapConnectRooms_CC(sMap &_map, const uint16_t &_r1, const uint16_t &_r2);

    // --- librmg_pathing.cpp ---
    void pathASFree(sPath &_path);
    bool pathAS(sMap &_map, sPath &_path);

    // --- librmg_prefab.cpp ---
    bool prefabFind(sPrefabData &_prefabData);

} // namespace rmg

#endif // __LIBRMG_HPP__

