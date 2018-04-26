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

    void splitRoom(sMap &_map, const uint16_t _roomID)
    {
        bool splitX = (_map.room[_roomID].w > ((_map.roomRadiusMin + _map.roomBorder) * 2));
        bool splitY = (_map.room[_roomID].h > ((_map.roomRadiusMin + _map.roomBorder) * 2));
        if (!splitX && !splitY)
        {
            _map.room[_roomID].p = false;
            return;
        }
        else
        {
            uint16_t newRoomID = _map.roomCount;
            sRoom *tRoom = new sRoom[_map.roomCount];
            for (uint16_t i = 0; i < _map.roomCount; i++)
            {
                tRoom[i].hidden = _map.room[i].hidden;
                tRoom[i].secret = _map.room[i].secret;
                tRoom[i].type = _map.room[i].type;
                tRoom[i].p = _map.room[i].p;
                tRoom[i].posXMin = _map.room[i].posXMin;
                tRoom[i].posXMax = _map.room[i].posXMax;
                tRoom[i].posYMin = _map.room[i].posYMin;
                tRoom[i].posYMax = _map.room[i].posYMax;
                tRoom[i].x = _map.room[i].x;
                tRoom[i].y = _map.room[i].y;
                tRoom[i].w = _map.room[i].w;
                tRoom[i].h = _map.room[i].h;
                tRoom[i].exitN = _map.room[i].exitN;
                tRoom[i].exitS = _map.room[i].exitS;
                tRoom[i].exitE = _map.room[i].exitE;
                tRoom[i].exitW = _map.room[i].exitW;
            }
            delete _map.room;
            _map.roomCount++;
            _map.room = new sRoom[_map.roomCount];
            for (uint16_t i = 0; i < _map.roomCount-1; i++)
            {
                _map.room[i].hidden = tRoom[i].hidden;
                _map.room[i].secret = tRoom[i].secret;
                _map.room[i].type = tRoom[i].type;
                _map.room[i].p = tRoom[i].p;
                _map.room[i].posXMin = tRoom[i].posXMin;
                _map.room[i].posXMax = tRoom[i].posXMax;
                _map.room[i].posYMin = tRoom[i].posYMin;
                _map.room[i].posYMax = tRoom[i].posYMax;
                _map.room[i].x = tRoom[i].x;
                _map.room[i].y = tRoom[i].y;
                _map.room[i].w = tRoom[i].w;
                _map.room[i].h = tRoom[i].h;
                _map.room[i].exitN = tRoom[i].exitN;
                _map.room[i].exitS = tRoom[i].exitS;
                _map.room[i].exitE = tRoom[i].exitE;
                _map.room[i].exitW = tRoom[i].exitW;
            }
            if (splitX && splitY)
                (_map.room[_roomID].w > _map.room[_roomID].h) ? (splitY = !splitY) : (splitX = !splitX);
            if (splitX)
            {
                _map.room[newRoomID].p = true;
                _map.room[newRoomID].y = _map.room[_roomID].y;
                _map.room[newRoomID].h = _map.room[_roomID].h;
                _map.room[newRoomID].posYMin = _map.room[_roomID].posYMin;
                _map.room[newRoomID].posYMax = _map.room[_roomID].posYMax;
                uint16_t splitMaxX = _map.room[_roomID].w - ((_map.roomRadiusMin + _map.roomBorder) * 2);
                uint16_t splitDeltaX = rand() % splitMaxX;
                _map.room[_roomID].w = _map.roomRadiusMin + _map.roomBorder + (splitMaxX - splitDeltaX);
                _map.room[newRoomID].w = _map.roomRadiusMin + _map.roomBorder + splitDeltaX;
                _map.room[_roomID].x = _map.room[_roomID].posXMin + (_map.room[_roomID].w / 2);
                _map.room[newRoomID].x = _map.room[_roomID].posXMax - (_map.room[newRoomID].w / 2);
                _map.room[newRoomID].posXMin = _map.room[_roomID].posXMin + _map.room[_roomID].w;
                _map.room[newRoomID].posXMax = _map.room[_roomID].posXMax;
                _map.room[_roomID].posXMax = _map.room[_roomID].posXMin + _map.room[_roomID].w - 1;
            }
            if (splitY)
            {
                _map.room[newRoomID].p = true;
                _map.room[newRoomID].x = _map.room[_roomID].x;
                _map.room[newRoomID].w = _map.room[_roomID].w;
                _map.room[newRoomID].posXMin = _map.room[_roomID].posXMin;
                _map.room[newRoomID].posXMax = _map.room[_roomID].posXMax;
                uint16_t splitMaxY = _map.room[_roomID].h - ((_map.roomRadiusMin + _map.roomBorder) * 2);
                uint16_t splitDeltaY = rand() % splitMaxY;
                _map.room[_roomID].h = _map.roomRadiusMin + _map.roomBorder + (splitMaxY - splitDeltaY);
                _map.room[newRoomID].h = _map.roomRadiusMin + _map.roomBorder + splitDeltaY;
                _map.room[_roomID].y = _map.room[_roomID].posYMin + (_map.room[_roomID].h / 2);
                _map.room[newRoomID].y = _map.room[_roomID].posYMax - (_map.room[newRoomID].h / 2);
                _map.room[newRoomID].posYMin = _map.room[_roomID].posYMin + _map.room[_roomID].h;
                _map.room[newRoomID].posYMax = _map.room[_roomID].posYMax;
                _map.room[_roomID].posYMax = _map.room[_roomID].posYMin + _map.room[_roomID].h - 1;
            }
            if (tRoom != nullptr)
                delete tRoom;
        }
    }

    void subdivideMap(sMap &_map)
    {
        uint16_t availableRooms = 1;
        while (availableRooms != 0)
        {
            availableRooms = 0;
            for (uint16_t i = 0; i < _map.roomCount; i++)
            {
                if (_map.room[i].p)
                {
                    splitRoom(_map, i);
                    i = _map.roomCount;
                }
            }
            for (uint16_t i = 0; i < _map.roomCount; i++)
                if (_map.room[i].p)
                    availableRooms++;
        }
    }

    void fillRooms(sMap &_map)
    {
        for (uint32_t k = 0; k < _map.roomCount; k++)
        {
            for (uint32_t i = _map.room[k].posYMin; i < _map.room[k].posYMax; i++)
            {
                for (uint32_t j = _map.room[k].posXMin; j < _map.room[k].posXMax; j++)
                {
                    _map.tile[(i * _map.w) + j].b = RMG_BASE_FLOOR;
                }
            }
        }
    }

    static void genD2_internal(sMap &_map)
    {
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].b = RMG_BASE_WALL;
        if (_map.room != nullptr)
            delete _map.room;
        _map.roomCount = 1;
        _map.room = new sRoom[_map.roomCount];
        _map.room[0].p = true;
        _map.room[0].x = (_map.w-2) / 2;
        _map.room[0].y = (_map.h-2) / 2;
        _map.room[0].w = (_map.w-1);
        _map.room[0].h = (_map.h-1);
        _map.room[0].posXMin = 1;
        _map.room[0].posXMax = _map.room[0].w;
        _map.room[0].posYMin = 1;
        _map.room[0].posYMax = _map.room[0].h;
        subdivideMap(_map);
        fillRooms(_map);
    }

    void genD2(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            _map.generationAlgorithm = RMG_GEN_D2;
            _map.tileCount = _map.w * _map.h;
            bool seeded = !(_map.seed == 0);
            if (seeded)
            {
                srand (_map.seed);
                genD2_internal(_map);
            }
            else
            {
                for (uint16_t i = 0; i < _map.pass; i++)
                {
                    _map.seed = time(NULL);
                    srand (_map.seed);
                    genD2_internal(_map);
                    if ((mapGetFloorArea(_map) > (_map.tileCount / _map.floorAreaMin)) && (_map.roomCount >= _map.roomMin))
                        i = _map.pass;
                }
            }
            mapInitRooms(_map);
            mapConnectRooms(_map);
            //mapRemoveAnomalies(_map);
        }
    }

} // namespace rmg
