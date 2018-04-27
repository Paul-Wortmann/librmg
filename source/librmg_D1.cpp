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

#include "librmg.hpp"

namespace rmg
{

    static void genD1_internal(sMap &_map)
    {
        uint16_t roomShape = _map.roomShape;
        uint32_t roomMax = _map.density * _map.pass;
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].b = RMG_BASE_WALL;
        mapPerimeterWalls(_map);
        for (uint32_t i = 0; i < roomMax; i++)
        {
            uint32_t r = (rand() % (_map.roomRadiusMax - _map.roomRadiusMin)) + _map.roomRadiusMin;

            int32_t x = (rand() % (_map.w - r - r)) + r;
            int32_t y = (rand() % (_map.h - r - r)) + r;
            if ((x > 1) && (x < _map.w-1) && (y > 1) && (y < _map.h-1))
            {
                if (_map.roomShape == RMG_RANDOM)
                {
                    roomShape = ((rand() % 100) < 50) ? RMG_CIRCLE : RMG_SQUARE;
                }
                if (roomShape == RMG_CIRCLE)
                {
                    if (genCircleRoomOK(_map, x, y, r + _map.roomBorder))
                        genCircleRoom(_map, x, y, r);
                }
                if (roomShape == RMG_SQUARE)
                {
                    if (genSquareRoomOK(_map, x, y, r + _map.roomBorder))
                        genSquareRoom(_map, x, y, r);
                }
            }
        }
    }

    void genD1(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.generationAlgorithm = RMG_GEN_D1;
            _map.tileCount = _map.w * _map.h;
            bool seeded = !(_map.seed == 0);
            if (seeded)
            {
                srand (_map.seed);
                genD1_internal(_map);
            }
            else
            {
                for (uint16_t i = 0; i < _map.pass; i++)
                {
                    _map.seed = time(NULL);
                    srand (_map.seed);
                    genD1_internal(_map);
                    if ((mapGetFloorArea(_map) > (_map.tileCount / _map.floorAreaMin)) && (_map.roomCount >= _map.roomMin))
                        i = _map.pass;
                }
            }
            mapInitRooms(_map);
            mapPrefabRooms(_map);
            mapConnectRooms(_map);
            //mapRemoveAnomalies(_map);
        }
    }

} // namespace rmg
