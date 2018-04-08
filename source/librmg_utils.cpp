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
                            std::cout << "O";
                        break;
                        case RMG_PATH:
                            std::cout << ".";
                        break;
                        case RMG_DOOR:
                            std::cout << "D";
                        break;
                        case RMG_STAIRS:
                            std::cout << "S";
                        break;
                        case RMG_FLOOR:
                            switch (_map.tile[(i * _map.w) + j].o)
                            {
                                case RMG_OBJECT_NONE:
                                    std::cout << " ";
                                    //std::cout << _map.tile[(i * _map.w) + j].r;
                                break;
                                case RMG_OBJECT_TREE:
                                    std::cout << "T";
                                break;
                                case RMG_OBJECT_AS_START:
                                    std::cout << "S";
                                break;
                                case RMG_OBJECT_AS_END:
                                    std::cout << "E";
                                break;
                                default:
                                    std::cout << ".";
                                break;
                            }
                        break;
                        default:
                            std::cout << "?";
                        break;
                    }
                }
                std::cout << std::endl;
            }
            uint16_t roomID = 0;
            std::cout << "Map seed: " << _map.seed << std::endl;
            std::cout << "Room Count: " << _map.roomCount << std::endl;
            //std::cout << "North: " << _map.room[roomID].exitN << std::endl;
            //std::cout << "South: " << _map.room[roomID].exitS << std::endl;
            //std::cout << "East: " << _map.room[roomID].exitE << std::endl;
            //std::cout << "West: " << _map.room[roomID].exitW << std::endl;
            //std::cout << "posXMax: " << _map.room[roomID].posXMax << std::endl;
            //std::cout << "posXMin: " << _map.room[roomID].posXMin << std::endl;
            //std::cout << "posYMax: " << _map.room[roomID].posYMax << std::endl;
            //std::cout << "posYMin: " << _map.room[roomID].posYMin << std::endl;
            //std::cout << "pos X: " << _map.room[roomID].x << std::endl;
            //std::cout << "pos Y: " << _map.room[roomID].y << std::endl;
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
        uint32_t tileT = 0;
        for (uint16_t k = 0; k < _map.pass; k++)
        {
            for (uint16_t i = 1; i < _map.h-1; i++)
            {
                for (uint16_t j = 1; j < _map.w-1; j++)
                {
                    tileT = (i * _map.w) + j;
                    if (_map.tile[tileT].d == RMG_FLOOR)
                    {
                        // Remove single horizontal floor tiles
                        // ???
                        // XOX
                        // ???
                        //--------------------
                        if ((_map.tile[tileT+1].d == RMG_WALL)
                         && (_map.tile[tileT-1].d == RMG_WALL))
                        {
                            _map.tile[tileT+1].d = RMG_FLOOR;
                            _map.tile[tileT-1].d = RMG_FLOOR;
                            _map.tile[tileT+1].r = _map.tile[tileT].r;
                            _map.tile[tileT-1].r = _map.tile[tileT].r;
                        }
                        // Remove single vertical floor tiles
                        // ?X?
                        // ?O?
                        // ?X?
                        //--------------------
                        if ((_map.tile[tileT+_map.w].d == RMG_WALL)
                         && (_map.tile[tileT-_map.w].d == RMG_WALL))
                        {
                            _map.tile[tileT+_map.w].d = RMG_FLOOR;
                            _map.tile[tileT-_map.w].d = RMG_FLOOR;
                            _map.tile[tileT+_map.w].r = _map.tile[tileT].r;
                            _map.tile[tileT-_map.w].r = _map.tile[tileT].r;
                        }
                    }
                    if (_map.tile[tileT].d == RMG_WALL)
                    {
                        // Remove single horizontal wall tiles
                        // ???
                        // OXO
                        // ???
                        //--------------------
                        if ((_map.tile[tileT+1].d == RMG_FLOOR)
                         && (_map.tile[tileT-1].d == RMG_FLOOR))
                        {
                            _map.tile[tileT].d = RMG_FLOOR;
                            _map.tile[tileT].r = _map.tile[tileT+1].r;
                        }
                        // Remove single vertical wall tiles
                        // ?O?
                        // ?X?
                        // ?O?
                        //--------------------
                        if ((_map.tile[tileT+_map.w].d == RMG_FLOOR)
                         && (_map.tile[tileT-_map.w].d == RMG_FLOOR))
                        {
                            _map.tile[tileT].d = RMG_FLOOR;
                            _map.tile[tileT].r = _map.tile[tileT+_map.w].r;
                        }
                        // Remove diagonal right wall tiles
                        // O?X
                        // ?X?
                        // X?O
                        //--------------------
                        if ((_map.tile[tileT+_map.w+1].d == RMG_WALL)
                         && (_map.tile[tileT+_map.w-1].d == RMG_FLOOR)
                         && (_map.tile[tileT-_map.w+1].d == RMG_FLOOR)
                         && (_map.tile[tileT-_map.w-1].d == RMG_WALL))
                        {
                            _map.tile[tileT+_map.w-1].d = RMG_WALL;
                            _map.tile[tileT-_map.w+1].d = RMG_WALL;
                            _map.tile[tileT+_map.w].d = RMG_WALL;
                            _map.tile[tileT-_map.w].d = RMG_WALL;
                            _map.tile[tileT+1].d = RMG_WALL;
                            _map.tile[tileT-1].d = RMG_WALL;
                        }
                        // Remove diagonal left wall tiles
                        // X?O
                        // ?X?
                        // O?X
                        //--------------------
                        if ((_map.tile[tileT+_map.w+1].d == RMG_FLOOR)
                         && (_map.tile[tileT+_map.w-1].d == RMG_WALL)
                         && (_map.tile[tileT-_map.w+1].d == RMG_WALL)
                         && (_map.tile[tileT-_map.w-1].d == RMG_FLOOR))
                        {
                            _map.tile[tileT+_map.w+1].d = RMG_WALL;
                            _map.tile[tileT-_map.w-1].d = RMG_WALL;
                            _map.tile[tileT+_map.w].d = RMG_WALL;
                            _map.tile[tileT-_map.w].d = RMG_WALL;
                            _map.tile[tileT+1].d = RMG_WALL;
                            _map.tile[tileT-1].d = RMG_WALL;
                        }
                    }
                }
            }
        }
    }

} // namespace rmg
