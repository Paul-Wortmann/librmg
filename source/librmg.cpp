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

    void mapInit(sMap &_map)
    {
        if (_map.tile != nullptr)
            mapFree(_map);
        if ((_map.tile == nullptr) && (_map.w > 0) && (_map.h > 0))
        {
            _map.tileCount = _map.h * _map.w;
            _map.tile = new rmg::sTile[_map.tileCount];
        }
    }

    void mapFree(sMap &_map)
    {
        if (_map.tile != nullptr)
        {
            delete [] _map.tile;
            _map.tile = nullptr;
        }
        if (_map.room != nullptr)
        {
            delete [] _map.room;
            _map.room = nullptr;
        }
    }

    void mapGen(sMap &_map)
    {
        switch (_map.generationAlgorithm)
        {
            case RMG_GEN_C1:
                genC1(_map);
            break;
            case RMG_GEN_C2:
                genC2(_map);
            break;
            case RMG_GEN_D1:
                genD1(_map);
            break;
        }
    }

} // namespace rmg
