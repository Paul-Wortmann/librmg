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

    void mapDraw(sMap &_map)
    {
        if (_map.tile != nullptr)
        {
            for (uint32_t i = 0; i < _map.h; i++)
            {
                for (uint32_t j = 0; j < _map.w; j++)
                {
                    switch (_map.tile[(i * _map.w) + j].d)
                    {
                        case RMG_WALL:
                            std::cout << ".";
                        break;
                        case RMG_PATH:
                            std::cout << "P";
                        break;
                        case RMG_DOOR:
                            std::cout << "D";
                        break;
                        case RMG_STAIRS:
                            std::cout << "S";
                        break;
                        case RMG_FLOOR:
                            //if (_map.tile[(i * _map.w) + j].r != 0)
                                std::cout << _map.tile[(i * _map.w) + j].r;
                                /*
                            else
                                std::cout << " ";
                                */
                        break;
                        default:
                            std::cout << "?";
                        break;
                    }
                }
                std::cout << std::endl;
            }
            uint16_t roomID = 3;
            std::cout << "Map seed: " << _map.seed << std::endl;
            std::cout << "Room Count: " << _map.roomCount << std::endl;
            std::cout << "North: " << _map.room[roomID].exitN << std::endl;
            std::cout << "South: " << _map.room[roomID].exitS << std::endl;
            std::cout << "East: " << _map.room[roomID].exitE << std::endl;
            std::cout << "West: " << _map.room[roomID].exitW << std::endl;
            std::cout << "posXMax: " << _map.room[roomID].posXMax << std::endl;
            std::cout << "posXMin: " << _map.room[roomID].posXMin << std::endl;
            std::cout << "posYMax: " << _map.room[roomID].posYMax << std::endl;
            std::cout << "posYMin: " << _map.room[roomID].posYMin << std::endl;
        }
    }

    void genCircle(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            float level_r = (((_map.w / 2.0) < (_map.h / 2.0)) ? (_map.w / 2.0) : (_map.h / 2.0));
            for (uint32_t i = 0; i < _map.h; i++)
            {
                for (uint32_t j = 0; j < _map.w; j++)
                {
                    _map.tile[(i * _map.w) + j].x = (j - (_map.w / 2.0f));
                    _map.tile[(i * _map.w) + j].y = (i - (_map.h / 2.0f));
                    _map.tile[(i * _map.w) + j].d = (((_map.tile[(i * _map.w) + j].x * _map.tile[(i * _map.w) + j].x) + (_map.tile[(i * _map.w) + j].y * _map.tile[(i * _map.w) + j].y)) < (level_r * level_r)) ? RMG_FLOOR : RMG_WALL;
                }
            }
        }
    }

    void mapPerimeterWalls(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint32_t i = 0; i < _map.h; i++)
            {
                for (uint32_t j = 0; j < _map.w; j++)
                {
                    if ((j == 0) || (j == _map.w -1) || (i == 0) || (i == _map.h -1))
                        _map.tile[(i * _map.w) + j].d = RMG_WALL;
                }
            }
        }
    }

    uint32_t mapGetFloorArea(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            uint32_t tileCount = 0;
            for (uint32_t j = 0; j < _map.tileCount; j++)
            {
                if (_map.tile[j].d == RMG_FLOOR)
                    tileCount++;
            }
            return tileCount;
        }
        return 0;
    }

    void mapRemoveAnomalies(sMap &_map)
    {

    }

} // namespace rmg
