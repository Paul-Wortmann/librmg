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

    static void genC2_internal(sMap &_map)
    {
        uint32_t blobCount = _map.density / _map.pass;
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].d = RMG_BASE_WALL;
        mapPerimeterWalls(_map);
        for (uint32_t i = 0; i < blobCount; i++)
        {
            uint32_t r = (rand() % (_map.roomRadiusMax - _map.roomRadiusMin)) + _map.roomRadiusMin;

            uint32_t x = (rand() % (_map.w - r - r)) + r;
            uint32_t y = (rand() % (_map.h - r - r)) + r;

            genCircleRoom(_map, x, y, r);
        }

    }

    void genC2(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.generationAlgorithm = RMG_GEN_C2;
            _map.tileCount = _map.w * _map.h;
            bool seeded = !(_map.seed == 0);
            if (seeded)
            {
                srand (_map.seed);
                genC2_internal(_map);
            }
            else
            {
                for (uint16_t i = 0; i < _map.pass; i++)
                {
                    _map.seed = time(NULL);
                    srand (_map.seed);
                    genC2_internal(_map);
                    mapInitRooms(_map);
                    mapRoomDiscardAllButLargest(_map);
                    mapRemoveAnomalies(_map);
                    if (mapGetFloorArea(_map) > (_map.tileCount * (_map.floorAreaMin / 100.0f)))
                        i = _map.pass;
                }
            }
        }
    }

} // namespace rmg
