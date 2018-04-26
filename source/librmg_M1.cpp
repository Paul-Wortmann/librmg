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

    static void genM1_maze(sMap &_map, const uint32_t &_tile)
    {
        uint16_t tileCount = 4;
        uint16_t loopCount = 0; // exit instead of getting stuck in the loop
        while (tileCount > 0)
        {
            tileCount = 0;
            uint32_t next_tile =_tile - _map.w;
            if (!_map.tile[next_tile].c)
                tileCount++;
            next_tile =_tile + _map.w;
            if (!_map.tile[next_tile].c)
                tileCount++;
            next_tile =_tile - 1;
            if (!_map.tile[next_tile].c)
                tileCount++;
            next_tile =_tile + 1;
            if (!_map.tile[next_tile].c)
                tileCount++;
            loopCount++;
            if (loopCount > (_map.pass * _map.directionBiasStrength))
                tileCount = 0;
            uint32_t randBase = 25;
            uint32_t direction = _map.directionBias;
            uint32_t directionRand = (direction == RMG_NONE) ? (rand() % (randBase * 4)) :  (rand() % (randBase * (4 + _map.directionBiasStrength)));
            if (directionRand < randBase)
                direction = RMG_NORTH;
            if ((directionRand > (randBase * 1)) && (directionRand < (randBase * 2)))
                direction = RMG_SOUTH;
            if ((directionRand > (randBase * 2)) && (directionRand < (randBase * 3)))
                direction = RMG_EAST;
            if ((directionRand > (randBase * 3)) && (directionRand < (randBase * 4)))
                direction = RMG_WEST;
            if (_map.directionBias != RMG_NONE)
                _map.directionBias = direction;
            next_tile =_tile - _map.w;
            if ((direction == RMG_NORTH) && (next_tile > 0) && (next_tile < _map.tileCount))
            {
                if (!_map.tile[next_tile].c)
                {
                    if ((!_map.tile[next_tile + 1].c) && (!_map.tile[next_tile - 1].c))
                    {
                        if ((_map.tile[next_tile - _map.w].b == RMG_BASE_WALL) && (_map.tile[next_tile - _map.w + 1].b == RMG_BASE_WALL) && (_map.tile[next_tile - _map.w - 1].b == RMG_BASE_WALL))
                        {
                            _map.tile[next_tile].c = true;
                            _map.tile[next_tile].b = RMG_BASE_FLOOR;
                            genM1_maze(_map, (next_tile));
                        }
                    }
                }
            }
            next_tile =_tile + _map.w;
            if ((direction == RMG_SOUTH) && (next_tile > 0) && (next_tile < _map.tileCount))
            {
                if (!_map.tile[next_tile].c)
                {
                    if ((!_map.tile[next_tile + 1].c) && (!_map.tile[next_tile - 1].c))
                    {
                        if ((_map.tile[next_tile + _map.w].b == RMG_BASE_WALL) && (_map.tile[next_tile + _map.w + 1].b == RMG_BASE_WALL) && (_map.tile[next_tile + _map.w - 1].b == RMG_BASE_WALL))
                        {
                            _map.tile[next_tile].c = true;
                            _map.tile[next_tile].b = RMG_BASE_FLOOR;
                            genM1_maze(_map, (next_tile));
                        }
                    }
                }
            }
            next_tile =_tile - 1;
            if ((direction == RMG_EAST) && (next_tile > 0) && (next_tile < _map.tileCount))
            {
                if (!_map.tile[next_tile].c)
                {
                    if ((!_map.tile[next_tile + _map.w].c) && (!_map.tile[next_tile - _map.w].c))
                    {
                        if ((_map.tile[next_tile - _map.w].b == RMG_BASE_WALL) && (_map.tile[next_tile - 1 + _map.w].b == RMG_BASE_WALL) && (_map.tile[next_tile - 1 - _map.w].b == RMG_BASE_WALL))
                        {
                            _map.tile[next_tile].c = true;
                            _map.tile[next_tile].b = RMG_BASE_FLOOR;
                            genM1_maze(_map, (next_tile));
                        }
                    }
                }
            }
            next_tile =_tile + 1;
            if ((direction == RMG_WEST) && (next_tile > 0) && (next_tile < _map.tileCount))
            {
                if (!_map.tile[next_tile].c)
                {
                    if ((!_map.tile[next_tile + _map.w].c) && (!_map.tile[next_tile - _map.w].c))
                    {
                        if ((_map.tile[next_tile - _map.w].b == RMG_BASE_WALL) && (_map.tile[next_tile + 1 + _map.w].b == RMG_BASE_WALL) && (_map.tile[next_tile + 1 - _map.w].b == RMG_BASE_WALL))
                        {
                            _map.tile[next_tile].c = true;
                            _map.tile[next_tile].b = RMG_BASE_FLOOR;
                            genM1_maze(_map, (next_tile));
                        }
                    }
                }
            }
        }
    }

    static void genM1_internal(sMap &_map)
    {
        // map preparation
        uint16_t roomShape = _map.roomShape;
        uint16_t roomCount = 0;
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].b = RMG_BASE_WALL;
        mapPerimeterWalls(_map);
        // add rooms
        for (uint32_t i = 0; i < _map.roomMax; i++)
        {
            if (roomCount < _map.roomMin)
            {
                uint32_t r = (rand() % (_map.roomRadiusMax - _map.roomRadiusMin)) + _map.roomRadiusMin;
                int32_t x = (rand() % (_map.w - r - r)) + r;
                int32_t y = (rand() % (_map.h - r - r)) + r;
                if ((x > 1) && (x < (int32_t)(_map.w-1)) && (y > 1) && (y < (int32_t)(_map.h-1)))
                {
                    if (_map.roomShape == RMG_RANDOM)
                    {
                        roomShape = ((rand() % 100) < 50) ? RMG_CIRCLE : RMG_SQUARE;
                    }
                    if (roomShape == RMG_CIRCLE)
                    {
                        if (genCircleRoomOK(_map, x, y, r + _map.roomBorder))
                        {
                            genCircleRoom(_map, x, y, r);
                            roomCount++;
                        }
                    }
                    if (roomShape == RMG_SQUARE)
                    {
                        if (genSquareRoomOK(_map, x, y, r + _map.roomBorder))
                        {
                            genSquareRoom(_map, x, y, r);
                            roomCount++;
                        }
                    }
                }
            }
        }
        mapInitRooms(_map);
        // set unmodifiable tiles.
        for (uint32_t i = 0; i < _map.h; i++)
            for (uint32_t j = 0; j < _map.w; j++)
                _map.tile[(i * _map.w) + j].c = ((i == 0) || (i == (_map.h-1)) || (j == 0) || (j == (_map.w-1)) || (_map.tile[(i * _map.w) + j].b != RMG_BASE_WALL));
        // maze.
        uint32_t randBase = 25;
        uint32_t direction = _map.directionBias;
        uint32_t directionRand = (direction == RMG_NONE) ? (rand() % (randBase * 4)) : (rand() % (randBase * (4 + _map.directionBiasStrength)));
        if (directionRand < randBase)
            direction = RMG_NORTH;
        if ((directionRand > (randBase * 1)) && (directionRand < (randBase * 2)))
            direction = RMG_SOUTH;
        if ((directionRand > (randBase * 2)) && (directionRand < (randBase * 3)))
            direction = RMG_EAST;
        if ((directionRand > (randBase * 3)) && (directionRand < (randBase * 4)))
            direction = RMG_WEST;
        if (_map.directionBias != RMG_NONE)
            _map.directionBias = direction;
        uint32_t startX = _map.w / 2;
        uint32_t startY = _map.h / 2;
        if (direction == RMG_NORTH)
            startY = 1;
        if (direction == RMG_SOUTH)
            startY = _map.h - 2;
        if (direction == RMG_EAST)
            startX = 1;
        if (direction == RMG_WEST)
            startX = _map.w - 2;
        uint32_t nextTile = (startY * _map.w) + startX;
        _map.tile[nextTile].b = RMG_BASE_FLOOR;
        _map.tile[nextTile].c = true;
        genM1_maze(_map, nextTile);
        // connect rooms
        for (uint32_t i = 0; i < _map.roomCount; i++)
        {
            uint16_t deltaX = abs ((_map.w / 2) - _map.room[i].x);
            uint16_t deltaY = abs ((_map.h / 2) - _map.room[i].y);
            direction = (deltaX > deltaY) ? ((_map.room[i].x < (_map.w / 2)) ? RMG_WEST : RMG_EAST) : ((_map.room[i].y < (_map.h / 2)) ? RMG_SOUTH : RMG_NORTH);
            uint32_t tile = 0;
            if (direction == RMG_NORTH)
            {
                tile = ((_map.room[i].y - ((_map.room[i].h+1) / 2)) * _map.w) + _map.room[i].x;
                _map.tile[tile].b = RMG_BASE_FLOOR;
                if (_map.tile[tile-_map.w].b == RMG_BASE_WALL)
                {
                    while (_map.tile[tile-_map.w].b == RMG_BASE_WALL)
                    {
                        tile -= _map.w;
                        _map.tile[tile].b = RMG_BASE_FLOOR;
                    }
                }
            }
            if (direction == RMG_SOUTH)
            {
                tile = ((_map.room[i].y + ((_map.room[i].h+3) / 2)) * _map.w) + _map.room[i].x;
                _map.tile[tile].b = RMG_BASE_FLOOR;
                if (_map.tile[tile+_map.w].b == RMG_BASE_WALL)
                {
                    while (_map.tile[tile+_map.w].b == RMG_BASE_WALL)
                    {
                        tile += _map.w;
                        _map.tile[tile].b = RMG_BASE_FLOOR;
                    }
                }
            }
            if (direction == RMG_EAST)
            {
                tile = ((_map.room[i].y * _map.w) + _map.room[i].x - ((_map.room[i].w+1) / 2));
                _map.tile[tile].b = RMG_BASE_FLOOR;
                if (_map.tile[tile-1].b == RMG_BASE_WALL)
                {
                    while (_map.tile[tile-1].b == RMG_BASE_WALL)
                    {
                        tile--;
                        _map.tile[tile].b = RMG_BASE_FLOOR;
                    }
                }
            }
            if (direction == RMG_WEST)
            {
                tile = ((_map.room[i].y * _map.w) + _map.room[i].x + ((_map.room[i].w+3) / 2));
                _map.tile[tile].b = RMG_BASE_FLOOR;
                if (_map.tile[tile+1].b == RMG_BASE_WALL)
                {
                    while (_map.tile[tile+1].b == RMG_BASE_WALL)
                    {
                        tile++;
                        _map.tile[tile].b = RMG_BASE_FLOOR;
                    }
                }
            }
        }
    }

    void genM1(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.generationAlgorithm = RMG_GEN_M1;
            _map.tileCount = _map.w * _map.h;
            bool seeded = !(_map.seed == 0);
            if (seeded)
            {
                srand (_map.seed);
                genM1_internal(_map);
            }
            else
            {
                for (uint16_t i = 0; i < _map.pass; i++)
                {
                    _map.seed = time(NULL);
                    srand (_map.seed);
                    genM1_internal(_map);
                    if ((mapGetFloorArea(_map) > (_map.tileCount / _map.floorAreaMin)) && (_map.roomCount >= _map.roomMin))
                        i = _map.pass;
                }
            }
        }
    }

} // namespace rmg
