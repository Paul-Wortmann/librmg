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

    bool genCircleRoomOK(sMap &_map, uint32_t _x, uint32_t _y, uint32_t _r)
    {
        bool returnValue = true;
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint32_t i = 0; i < _map.h; i++)
            {
                for (uint32_t j = 0; j < _map.w; j++)
                {
                    if ((((j - _x) * (j - _x)) + ((i - _y) * (i - _y))) < (_r * _r))
                        if (_map.tile[(i * _map.w) + j].d == RMG_FLOOR)
                            returnValue = false;
                }
            }
        }
        return returnValue;
    }

    void genCircleRoom(sMap &_map, uint32_t _x, uint32_t _y, uint32_t _r)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint32_t i = 0; i < _map.h; i++)
            {
                for (uint32_t j = 0; j < _map.w; j++)
                {
                    if ((((j - _x) * (j - _x)) + ((i - _y) * (i - _y))) < (_r * _r))
                        _map.tile[(i * _map.w) + j].d = RMG_FLOOR;
                }
            }
        }
    }

    bool genSquareRoomOK(sMap &_map, uint32_t _x, uint32_t _y, uint32_t _r)
    {
        bool returnValue = true;
        if (((_x-_r) <= 0) || ((_x+_r) >= _map.w) || ((_y-_r) <= 0) || ((_y+_r) >= _map.h))
            return false;
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint32_t i = _y-_r; i < _y+_r; i++)
            {
                for (uint32_t j = _x-_r; j < _x+_r; j++)
                {
                    if (_map.tile[(i * _map.w) + j].d == RMG_FLOOR)
                        returnValue = false;
                }
            }
        }
        return returnValue;
    }

    void genSquareRoom(sMap &_map, uint32_t _x, uint32_t _y, uint32_t _r)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint32_t i = _y-_r; i < _y+_r; i++)
            {
                for (uint32_t j = _x-_r; j < _x+_r; j++)
                {
                    _map.tile[(i * _map.w) + j].d = RMG_FLOOR;
                }
            }
        }
    }

    static void mapFindRoom(sMap &_map, uint32_t i)
    {
        if (i  < _map.tileCount)
        {
            if ((!_map.tile[i].p) && (_map.tile[i].d == RMG_FLOOR))
            {
                _map.tile[i].p = true;
                _map.tile[i].r = _map.roomCount;
                if (((i + 1) > 0) && ((i + 1) < _map.tileCount))
                    mapFindRoom(_map, i + 1);
                if (((i - 1) > 0) && ((i - 1) < _map.tileCount))
                    mapFindRoom(_map, i - 1);
                if (((i + _map.w) > 0) && ((i + _map.w) < _map.tileCount))
                    mapFindRoom(_map, i + _map.w);
                if (((i - _map.w) > 0) && ((i - _map.w) < _map.tileCount))
                    mapFindRoom(_map, i - _map.w);
            }
        }
    }

    void mapFindRooms(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.roomCount = 0;
            for (uint32_t i = 0; i < _map.tileCount; i++)
            {
                _map.tile[i].r = 0;
                _map.tile[i].p = false;
            }
            for (uint32_t i = 0; i < _map.tileCount; i++)
            {
                if ((!_map.tile[i].p) && (_map.tile[i].d == RMG_FLOOR))
                {
                    mapFindRoom(_map, i);
                    _map.roomCount++;
                }
            }
        }
    }

    static void mapDiscardMinRooms(sMap &_map)
    {
        uint16_t discardCount = 0;
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint16_t i = 0; i < _map.roomCount; i++)
            {
                uint32_t tileCount = 0;
                for (uint32_t j = 0; j < _map.tileCount; j++)
                {
                    if ((_map.tile[j].r == i) && (_map.tile[j].d == RMG_FLOOR))
                        tileCount++;
                }
                if (tileCount < (_map.roomRadiusMin * _map.roomRadiusMin))
                {
                    discardCount++;
                    for (uint32_t j = 0; j < _map.tileCount; j++)
                    {
                        if (_map.tile[j].r == i)
                        {
                            _map.tile[j].r = 0;
                            _map.tile[j].d = RMG_WALL;
                        }
                    }
                }
            }
        }
        _map.roomCount -= discardCount;
    }

    static void mapRoomSizeLocation(sMap &_map)
    {
        if ((_map.roomCount > 0) && (_map.room != nullptr))
        {
            for (uint16_t i = 0; i < _map.roomCount; i++)
            {
                _map.room[i].posXMin = _map.w;
                _map.room[i].posXMax = 0;
                _map.room[i].posYMin = _map.h;
                _map.room[i].posYMax = 0;
                for (uint32_t j = 0; j < _map.h; j++)
                {
                    for (uint32_t k = 0; k < _map.w; k++)
                    {
                        if ((_map.tile[(j * _map.w) + k].r == i) && (_map.tile[(j * _map.w) + k].d == RMG_FLOOR))
                        {
                            if (k < _map.room[i].posXMin)
                                _map.room[i].posXMin = k;
                            if (k > _map.room[i].posXMax)
                                _map.room[i].posXMax = k;
                            if (j < _map.room[i].posYMin)
                                _map.room[i].posYMin = j;
                            if (j > _map.room[i].posYMax)
                                _map.room[i].posYMax = j;
                        }
                    }
                }
                _map.room[i].w = _map.room[i].posXMax - _map.room[i].posXMin;
                _map.room[i].h = _map.room[i].posYMax - _map.room[i].posYMin;
                _map.room[i].x = (_map.room[i].w / 2) + _map.room[i].posXMin;
                _map.room[i].y = (_map.room[i].h / 2) + _map.room[i].posYMin;
            }
        }
    }


    uint32_t mapGetRoomArea(sMap &_map, uint16_t _r)
    {
        uint32_t returnValue = 0;
        if (_map.tile != nullptr)
        {
            for (uint32_t i = 0; i < _map.tileCount; i++)
                if ((_map.tile[i].r == _r) && (_map.tile[i].d == RMG_FLOOR))
                    returnValue++;
        }
        return returnValue;
    }

    void mapRoomDiscardAllButLargest(sMap &_map)
    {
        if ((_map.roomCount > 0) && (_map.room != nullptr) && (_map.tile != nullptr))
        {
            uint32_t roomSizeT = 0;
            uint32_t roomSize = 0;
            uint32_t roomNumber = 0;
            for (uint16_t i = 0; i < _map.roomCount; i++)
            {
                roomSizeT = mapGetRoomArea(_map, i);
                if (roomSizeT > roomSize)
                {
                    roomSize = roomSizeT;
                    roomNumber = i;
                }
            }
            for (uint32_t i = 0; i < _map.tileCount; i++)
            {
                if ((_map.tile[i].r != roomNumber) && (_map.tile[i].d == RMG_FLOOR))
                {
                    _map.tile[i].r = RMG_NOROOM;
                    _map.tile[i].d = RMG_WALL;
                }
                else
                    _map.tile[i].r = 0;
            }
        }
        _map.roomCount = 1;
        if (_map.room != nullptr)
            delete _map.room;
        _map.room = new sRoom[_map.roomCount];
        mapRoomSizeLocation(_map);
    }

    void mapInitRooms(sMap &_map)
    {
        mapFindRooms(_map);
        mapDiscardMinRooms(_map);
        mapFindRooms(_map);
        if (_map.roomCount > 0)
        {
            if (_map.room != nullptr)
                delete _map.room;
            _map.room = new sRoom[_map.roomCount];
            for (uint16_t i = 0; i < _map.roomCount; i++)
            {
                _map.room[i].exitE = RMG_NOROOM;
                _map.room[i].exitW = RMG_NOROOM;
                _map.room[i].exitS = RMG_NOROOM;
                _map.room[i].exitN = RMG_NOROOM;
            }
            mapRoomSizeLocation(_map);
        }
    }

} // namespace rmg
