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

    static void genT1_internal(sMap &_map)
    {
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].d = RMG_BASE_FLOOR;
        bool seeded = (_map.seed != 0);
        if (seeded)
        {
            srand (_map.seed);
            genC1(_map);
        }
        else
        {
            while (_map.roomCount != 1)
            {
                _map.seed = time(NULL);
                srand (_map.seed);
                genC1(_map);
            }
        }
        for (uint32_t i = 0; i < _map.tileCount; i++)
        {
            if (_map.tile[i].d == RMG_BASE_WALL)
            {
                _map.tile[i].d = RMG_BASE_FLOOR;
                _map.tile[i].o = RMG_OBJECT_TREE;
            }
        }
        // generate town center
        // generate river
        // generate bridge
        // generate roads
        // generate buildings

   }

    void genT1(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.generationAlgorithm = RMG_GEN_T1;
            _map.tileCount = _map.w * _map.h;
            _map.roomCount = 0;
            genT1_internal(_map);
        }
    }

} // namespace rmg
