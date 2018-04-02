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

    void mapConnectRooms(sMap &_map)
    {
        // find neighbors
        uint32_t distanceN = _map.tileCount; // size larger that possible distance
        uint32_t distanceE = _map.tileCount; // size larger that possible distance
        uint32_t distanceW = _map.tileCount; // size larger that possible distance
        uint32_t distanceS = _map.tileCount; // size larger that possible distance
        uint32_t distanceT = 0; // temp distance
        for (uint16_t i = 0; i < _map.roomCount; i++)
        {
            distanceN = _map.tileCount;
            distanceE = _map.tileCount;
            distanceW = _map.tileCount;
            distanceS = _map.tileCount;
            distanceT = 0;
            for (uint16_t j = 0; j < _map.roomCount; j++)
            {
                if (i != j)
                {
                    distanceT = abs(_map.room[j].x - _map.room[i].x) + abs(_map.room[j].y - _map.room[i].y);
                    // left / east
                    if ((_map.room[j].posXMax < _map.room[i].posXMin) && (distanceT < distanceE))
                    {
                        distanceE = distanceT;
                        _map.room[i].exitE = j;
                        _map.room[j].exitW = i;
                    }
                    // right / west
                    if ((_map.room[j].posXMin > _map.room[i].posXMax) && (distanceT < distanceW))
                    {
                        distanceW = distanceT;
                        _map.room[i].exitW = j;
                        _map.room[j].exitE = i;
                    }
                    // up / north
                    if ((_map.room[j].posYMax < _map.room[i].posYMin) && (distanceT < distanceN))
                    {
                        distanceN = distanceT;
                        _map.room[i].exitN = j;
                        _map.room[j].exitS = i;
                    }
                    // down / south
                    if ((_map.room[j].posYMin > _map.room[i].posYMax) && (distanceT < distanceS))
                    {
                        distanceS = distanceT;
                        _map.room[i].exitS = j;
                        _map.room[j].exitN = i;
                    }
                }
            }
        }
        for (uint16_t i = 0; i < _map.roomCount; i++)
        {
            switch (_map.connectivityAlgorithm)
            {
                case RMG_PATH_SL:
                    if (_map.room[i].exitE > -1)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > -1)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > -1)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > -1)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitS);
                break;
                case RMG_PATH_ND:
                    if (_map.room[i].exitE > -1)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > -1)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > -1)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > -1)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitS);
                break;
                case RMG_PATH_DW:
                    if (_map.room[i].exitE > -1)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > -1)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > -1)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > -1)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitS);
                break;
                case RMG_PATH_CC:
                    if (_map.room[i].exitE > -1)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > -1)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > -1)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > -1)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitS);
                break;
                default:
                    // throw error / set flag
                break;
            }
        }
    }

    // Straight Line
    void mapConnectRooms_SL(sMap &_map, uint16_t _r1, uint16_t _r2)
    {
        uint16_t direction = RMG_NONE;
        if (_map.room[_r1].exitE == _r2)
            direction = RMG_EAST;
        if (_map.room[_r1].exitW == _r2)
            direction = RMG_WEST;
        if (_map.room[_r1].exitN == _r2)
            direction = RMG_NORTH;
        if (_map.room[_r1].exitS == _r2)
            direction = RMG_SOUTH;
        uint16_t psx = ((direction == RMG_EAST) || (direction == RMG_WEST)) ?  ((direction == RMG_EAST) ? _map.room[_r1].posXMin : _map.room[_r1].posXMax) : _map.room[_r1].x;
        uint16_t psy = ((direction == RMG_NORTH) || (direction == RMG_SOUTH)) ?  ((direction == RMG_NORTH) ? _map.room[_r1].posYMin : _map.room[_r1].posYMax) : _map.room[_r1].y;
        uint16_t pex = ((direction == RMG_EAST) || (direction == RMG_WEST)) ?  ((direction == RMG_WEST) ? _map.room[_r2].posXMin : _map.room[_r2].posXMax) : _map.room[_r2].x;
        uint16_t pey = ((direction == RMG_NORTH) || (direction == RMG_SOUTH)) ?  ((direction == RMG_SOUTH) ? _map.room[_r2].posYMin : _map.room[_r2].posYMax) : _map.room[_r2].y;
        std::cout << "X1: " << psx  << " Y1: " << psy << std::endl;
        std::cout << "X2: " << pex  << " Y2: " << pey << std::endl;
        if (direction == RMG_EAST)
        {
            while ((_map.tile[(psy * _map.w) + psx].d == RMG_WALL) && (psx < _map.room[_r1].posXMax))
                psx++;
            while ((_map.tile[(pey * _map.w) + pex].d == RMG_WALL) && (pex > _map.room[_r2].posXMin))
                pex--;
        }
        if (direction == RMG_WEST)
        {
            while ((_map.tile[(psy * _map.w) + psx].d == RMG_WALL) && (psx > _map.room[_r1].posXMin))
                psx--;
            while ((_map.tile[(pey * _map.w) + pex].d == RMG_WALL) && (pex < _map.room[_r2].posXMax))
                pex++;
        }
        if (direction == RMG_NORTH)
        {
            while ((_map.tile[(psy * _map.w) + psx].d == RMG_WALL) && (psy < _map.room[_r1].posYMax))
                psy++;
            while ((_map.tile[(pey * _map.w) + pex].d == RMG_WALL) && (pey > _map.room[_r2].posYMin))
                pey--;
        }
        if (direction == RMG_SOUTH)
        {
            while ((_map.tile[(psy * _map.w) + psx].d == RMG_WALL) && (psy > _map.room[_r1].posYMin))
                psy--;
            while ((_map.tile[(pey * _map.w) + pex].d == RMG_WALL) && (pey < _map.room[_r2].posYMax))
                pey++;
        }
        while ((psx != pex) || (psy != pey))
        {
            _map.tile[(psy * _map.w) + psx].d = (_map.tile[(psy * _map.w) + psx].d == RMG_WALL) ? RMG_PATH : _map.tile[(psy * _map.w) + psx].d;
            psx = (psx == pex) ? psx : ((psx > pex) ? --psx : ++psx);
            psy = (psy == pey) ? psy : ((psy > pey) ? --psy : ++psy);
        }

    }

    // Straight Lines, 90 degree angle
    void mapConnectRooms_ND(sMap &_map, uint16_t _r1, uint16_t _r2)
    {
        uint16_t r1x = (_map.room[_r1].x < _map.room[_r2].x) ? _map.room[_r1].x : _map.room[_r2].x;
        uint16_t r2x = (_map.room[_r1].x > _map.room[_r2].x) ? _map.room[_r1].x : _map.room[_r2].x;
        uint16_t r1y = (_map.room[_r1].y < _map.room[_r2].y) ? _map.room[_r1].y : _map.room[_r2].y;
        uint16_t r2y = (_map.room[_r1].y > _map.room[_r2].y) ? _map.room[_r1].y : _map.room[_r2].y;
        for (uint16_t i = r1x; i < r2x; i++)
        {

        }
    }

    // Drunken Walk
    void mapConnectRooms_DW(sMap &_map, uint16_t _r1, uint16_t _r2)
    {

    }

    // Curved Corner
    void mapConnectRooms_CC(sMap &_map, uint16_t _r1, uint16_t _r2)
    {

    }

} // namespace rmg
