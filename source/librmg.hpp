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

#define RMG_WALL   0
#define RMG_FLOOR  1
#define RMG_PATH   2
#define RMG_DOOR   3
#define RMG_STAIRS 4

#define RMG_PATH_SL 0 // Straight Line
#define RMG_PATH_ND 1 // Ninety Degree angle lines
#define RMG_PATH_DW 2 // Drunken Walk
#define RMG_PATH_CC 3 // Curved Corner

#define RMG_GEN_C1 0 // Cave 1

namespace rmg
{

    struct sTile
    {
        bool     p = false; // processed flag
        uint16_t d = RMG_FLOOR; // data
        uint16_t r = 0; // room number
        uint32_t x = 0;
        uint32_t y = 0;
    };

    struct sRoom
    {
        bool     hidden = false;
        bool     secret = false;
        bool     p = false; // processed flag
        uint16_t posXMin = 0;
        uint16_t posXMax = 0;
        uint16_t posYMin = 0;
        uint16_t posYMax = 0;
        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t w = 0;
        uint16_t h = 0;
        int16_t exitN = -1; // -1 for none, else connecting room ID
        int16_t exitS = -1;
        int16_t exitE = -1;
        int16_t exitW = -1;
    };

    struct sMap
    {
        sTile *tile = nullptr;
        sRoom *room = nullptr;
        uint32_t w = 100;
        uint32_t h = 100;
        uint32_t tileCount = 10000; // w * h
        uint64_t seed = 0; // seed, 0 for random seed
        uint16_t pass = 4; // general number of passes, smoothing etc...
        uint16_t density = 10; // percentage / 2, rough approximate
        uint16_t roomCount = 0;
        uint16_t roomAreaMax = 11 * 11; // max room area in tiles
        uint16_t roomAreaMin = 5 * 5; // min room area in tiles
        uint16_t floorAreaMin = 60; // percentage, min % floor area
        uint16_t generationAlgorithm = RMG_GEN_C1;
        uint16_t connectivityAlgorithm = RMG_PATH_SL;
        uint16_t connectivityComplexity = 50; // percentage, rand % connect neighbors
    };

    // --- librmg.cpp ---
    void mapInit(sMap &_map);
    void mapFree(sMap &_map);

    // --- librmg_rooms.cpp ---
    void mapFindRooms(sMap &_map);
    void mapInitRooms(sMap &_map);

    // --- librmg_utils.cpp ---
    void mapDraw(sMap &_map);
    void genCircle(sMap &_map);
    void mapPerimeterWalls(sMap &_map);
    uint32_t mapGetFloorArea(sMap &_map);
    void mapRemoveAnomalies(sMap &_map);

    // --- librmg_C1.cpp ---
    void genC1(sMap &_map);

    // --- librmg_connectivity.cpp ---
    void mapConnectRooms(sMap &_map);
    void mapConnectRooms_SL(sMap &_map, uint16_t _r1, uint16_t _r2);
    void mapConnectRooms_ND(sMap &_map, uint16_t _r1, uint16_t _r2);
    void mapConnectRooms_DW(sMap &_map, uint16_t _r1, uint16_t _r2);
    void mapConnectRooms_CC(sMap &_map, uint16_t _r1, uint16_t _r2);


} // namespace rmg

#endif // __LIBRMG_HPP__

