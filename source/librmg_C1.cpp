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

    static void genC1_internal(sMap &_map)
    {
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].d = RMG_BASE_FLOOR;
        mapPerimeterWalls(_map);
        uint32_t walls = (_map.tileCount / 100) * _map.density;
        for (uint16_t i = 0; i < walls; i++)
            _map.tile[rand() % _map.tileCount].d = RMG_BASE_WALL;
        for (uint16_t p = 0; p < _map.pass; p++)
        {
            for (uint32_t i = 1; i < _map.h-1; i++)
            {
                for (uint32_t j = 1; j < _map.w-1; j++)
                {
                    uint16_t tCount = 0;
                    if (_map.tile[(i * _map.w) + j].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j + 1].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j - 1].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j + _map.w].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j + _map.w + 1].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j + _map.w - 1].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j - _map.w].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j - _map.w + 1].d == RMG_BASE_WALL)
                        tCount++;
                    if (_map.tile[(i * _map.w) + j - _map.w - 1].d == RMG_BASE_WALL)
                        tCount++;
                    if (tCount >= 5)
                        _map.tile[(i * _map.w) + j].d = RMG_BASE_WALL;
                    else
                        _map.tile[(i * _map.w) + j].d = RMG_BASE_FLOOR;
                }
            }
        }
    }

    void genC1(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.generationAlgorithm = RMG_GEN_C1;
            _map.tileCount = _map.w * _map.h;
            bool seeded = !(_map.seed == 0);
            if (seeded)
            {
                srand (_map.seed);
                genC1_internal(_map);
            }
            else
            {
                for (uint16_t i = 0; i < _map.pass; i++)
                {
                    _map.seed = time(NULL);
                    srand (_map.seed);
                    genC1_internal(_map);
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
