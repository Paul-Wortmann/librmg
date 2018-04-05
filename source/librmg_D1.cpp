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
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].d = RMG_WALL;
        mapPerimeterWalls(_map);
        uint32_t rMax = sqrt(_map.roomAreaMax);
        uint32_t rMin = sqrt(_map.roomAreaMin);
        std::cout << "rMin: " << rMin << " rMax: " << rMax << std::endl;
        if (rMax > RAND_MAX);
        for (uint32_t i = 0; i < 10; i++)
        {
            uint32_t r = (rand() % (7)) + 1;

            uint32_t x = (rand() % (_map.w - r - r)) + r;
            uint32_t y = (rand() % (_map.h - r - r)) + r;
            if (genCircleRoomOK(_map, x, y, r + _map.roomBorder))
                genCircleRoom(_map, x, y, r);
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
                    if (mapGetFloorArea(_map) > (_map.tileCount / _map.floorAreaMin))
                        i = _map.pass;
                }
            }
            mapInitRooms(_map);
            mapRemoveAnomalies(_map);
            mapConnectRooms(_map);
        }
    }

} // namespace rmg
