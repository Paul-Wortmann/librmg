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
        uint32_t distanceT = 0; // temp distance
        uint32_t distanceN = _map.tileCount; // size larger than possible distance
        uint32_t distanceE = _map.tileCount; // size larger than possible distance
        uint32_t distanceW = _map.tileCount; // size larger than possible distance
        uint32_t distanceS = _map.tileCount; // size larger than possible distance
        // ray cast room finder
        for (uint16_t i = 0; i < _map.roomCount; i++)
        {
            //search horizontally
            for (uint16_t j = _map.room[i].posYMin; j < _map.room[i].posYMax; j++)
            {
                // right / west
                for (uint16_t k = _map.room[i].x; k < _map.w; k++)
                {
                    uint16_t r = _map.tile[(j * _map.w) + k].r;
                    if ((_map.tile[(j * _map.w) + k].b == RMG_BASE_FLOOR) && (r != i))
                    {
                        distanceT = sqrt(((_map.room[r].x - _map.room[i].x) * (_map.room[r].x - _map.room[i].x)) + ((_map.room[r].y - _map.room[i].y) * (_map.room[r].y - _map.room[i].y)));
                        if (distanceT < distanceW)
                        {
                            distanceW = distanceT;
                            _map.room[i].exitW = r;
                            _map.room[r].exitE = i;
                        }
                    }
                }
                // left / east
                for (uint16_t k = _map.room[i].x; k > 0; k--)
                {
                    uint16_t r = _map.tile[(j * _map.w) + k].r;
                    if ((_map.tile[(j * _map.w) + k].b == RMG_BASE_FLOOR) && (r != i))
                    {
                        distanceT = sqrt(((_map.room[r].x - _map.room[i].x) * (_map.room[r].x - _map.room[i].x)) + ((_map.room[r].y - _map.room[i].y) * (_map.room[r].y - _map.room[i].y)));
                        if (distanceT < distanceE)
                        {
                            distanceE = distanceT;
                            _map.room[i].exitE = r;
                            _map.room[r].exitW = i;
                        }
                    }
                }
            }
            //search vertically
            for (uint16_t k = _map.room[i].posXMin; k < _map.room[i].posXMax; k++)
            {
                // down / south
                for (uint16_t j = _map.room[i].y; j < _map.h; j++)
                {
                    if ((_map.tile[(j * _map.w) + k].b == RMG_BASE_FLOOR) && (_map.tile[(j * _map.w) + k].r != i))
                    {
                        distanceT = sqrt(((_map.room[j].x - _map.room[i].x) * (_map.room[j].x - _map.room[i].x)) + ((_map.room[j].y - _map.room[i].y) * (_map.room[j].y - _map.room[i].y)));
                        if (distanceT < distanceS)
                        {
                            distanceS = distanceT;
                            _map.room[i].exitS = _map.tile[(j * _map.w) + k].r;
                            _map.room[_map.tile[(j * _map.w) + k].r].exitN = i;
                        }
                    }
                }
                // up / north
                for (uint16_t j = _map.room[i].y; j > 0; j--)
                {
                    if ((_map.tile[(j * _map.w) + k].b == RMG_BASE_FLOOR) && (_map.tile[(j * _map.w) + k].r != i))
                    {
                        distanceT = sqrt(((_map.room[j].x - _map.room[i].x) * (_map.room[j].x - _map.room[i].x)) + ((_map.room[j].y - _map.room[i].y) * (_map.room[j].y - _map.room[i].y)));
                        if (distanceT < distanceN)
                        {
                            distanceN = distanceT;
                            _map.room[i].exitN = _map.tile[(j * _map.w) + k].r;
                            _map.room[_map.tile[(j * _map.w) + k].r].exitS = i;
                        }
                    }
                }
            }
        }
        // remove multiple connections to same neighbor
        for (uint16_t i = 0; i < _map.roomCount; i++)
        {
            for (uint16_t j = 0; j < _map.roomCount; j++)
            {
                uint16_t found = 0;
                if (_map.room[i].exitE == j)
                    found++;
                if (_map.room[i].exitW == j)
                {
                    if (found == 1)
                        _map.room[i].exitW = RMG_NOROOM;
                    else
                        found++;
                }
                if (_map.room[i].exitN == j)
                {
                    if (found == 1)
                        _map.room[i].exitN = RMG_NOROOM;
                    else
                        found++;
                }
                if (_map.room[i].exitS == j)
                {
                    if (found == 1)
                        _map.room[i].exitS = RMG_NOROOM;
                    else
                        found++;
                }
            }
        }
        // try to connect lonely rooms
        for (uint16_t i = _map.roomCount; i > 0; i--)
        {
            int32_t deltaX = 0;
            int32_t deltaY = 0;
            if ((_map.room[i].exitE == RMG_NOROOM) && (_map.room[i].exitW == RMG_NOROOM) && (_map.room[i].exitN == RMG_NOROOM) && (_map.room[i].exitS == RMG_NOROOM))
            {
                //std::cout << "Lonely room detected!" << std::endl;
                deltaX = _map.room[i].x - _map.room[i-1].x;
                deltaY = _map.room[i].y - _map.room[i-1].y;
                if (abs(deltaX) < abs(deltaY))
                {
                    if (deltaX > 0)
                    {
                        _map.room[i].exitE = i-1;
                        _map.room[i-1].exitW = i;
                    }
                    else
                    {
                        _map.room[i].exitW = i-1;
                        _map.room[i-1].exitE = i;
                    }
                }
                else
                {
                    if (deltaY > 0)
                    {
                        _map.room[i].exitN = i-1;
                        _map.room[i-1].exitS = i;
                    }
                    else
                    {
                        _map.room[i].exitS = i-1;
                        _map.room[i-1].exitN = i;
                    }
                }
            }
        }
        // apply room inter-connections
        for (uint16_t i = 0; i < _map.roomCount; i++)
        {
            switch (_map.connectivityAlgorithm)
            {
                case RMG_PATH_SL:
                    if (_map.room[i].exitE > i)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > i)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > i)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > i)
                        mapConnectRooms_SL(_map, i, _map.room[i].exitS);
                break;
                case RMG_PATH_ND:
                    if (_map.room[i].exitE > i)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > i)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > i)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > i)
                        mapConnectRooms_ND(_map, i, _map.room[i].exitS);
                break;
                case RMG_PATH_DW:
                    if (_map.room[i].exitE > i)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > i)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > i)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS > i)
                        mapConnectRooms_DW(_map, i, _map.room[i].exitS);
                break;
                case RMG_PATH_CC:
                    if (_map.room[i].exitE > i)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitE);
                    if (_map.room[i].exitW > i)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitW);
                    if (_map.room[i].exitN > i)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitN);
                    if (_map.room[i].exitS  > i)
                        mapConnectRooms_CC(_map, i, _map.room[i].exitS);
                break;
                default:
                    // throw error / set flag
                break;
            }
        }
        // use path finder to locate unreachable rooms and try connect them.
        if (_map.roomCount > 0)
        {
            for (uint16_t i = 0; i < _map.roomCount-1; i++)
            {
                int16_t r1Dir = RMG_NOROOM;
                int16_t r2Dir = RMG_NOROOM;
                sPath pathT;
                pathT.sx = _map.room[i].x;
                pathT.sy = _map.room[i].y;
                pathT.ex = _map.room[i+1].x;
                pathT.ey = _map.room[i+1].y;
                if (pathAS(_map, pathT) == false)
                {
                    if (_map.room[i].exitE)
                        r1Dir = RMG_EAST;
                    else if (_map.room[i].exitW)
                        r1Dir = RMG_WEST;
                    else if (_map.room[i].exitS)
                        r1Dir = RMG_SOUTH;
                    else if (_map.room[i].exitN)
                        r1Dir = RMG_NORTH;
                    if (_map.room[i+1].exitE)
                        r2Dir = RMG_EAST;
                    else if (_map.room[i+1].exitW)
                        r2Dir = RMG_WEST;
                    else if (_map.room[i+1].exitS)
                        r2Dir = RMG_SOUTH;
                    else if (_map.room[i+1].exitN)
                        r2Dir = RMG_NORTH;
                    // try connect
                    if (_map.connectivityAlgorithm == RMG_PATH_SL)
                        mapConnectRooms_SL(_map, i, i+1);
                    if (_map.connectivityAlgorithm == RMG_PATH_ND)
                        mapConnectRooms_ND(_map, i, i+1);
                    if (_map.connectivityAlgorithm == RMG_PATH_DW)
                        mapConnectRooms_DW(_map, i, i+1);
                    if (_map.connectivityAlgorithm == RMG_PATH_CC)
                        mapConnectRooms_CC(_map, i, i+1);
                    pathASFree(pathT);
                }
            }
        }
    }

    // Straight Line
    void mapConnectRooms_SL(sMap &_map, const uint16_t &_r1, const uint16_t &_r2)
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
        //std::cout << "X1: " << psx  << " Y1: " << psy << std::endl;
        //std::cout << "X2: " << pex  << " Y2: " << pey << std::endl;
        if (direction == RMG_EAST)
        {
            while ((_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL) && (psx < _map.room[_r1].posXMax))
                psx++;
            while ((_map.tile[(pey * _map.w) + pex].b == RMG_BASE_WALL) && (pex > _map.room[_r2].posXMin))
                pex--;
        }
        if (direction == RMG_WEST)
        {
            while ((_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL) && (psx > _map.room[_r1].posXMin))
                psx--;
            while ((_map.tile[(pey * _map.w) + pex].b == RMG_BASE_WALL) && (pex < _map.room[_r2].posXMax))
                pex++;
        }
        if (direction == RMG_NORTH)
        {
            while ((_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL) && (psy < _map.room[_r1].posYMax))
                psy++;
            while ((_map.tile[(pey * _map.w) + pex].b == RMG_BASE_WALL) && (pey > _map.room[_r2].posYMin))
                pey--;
        }
        if (direction == RMG_SOUTH)
        {
            while ((_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL) && (psy > _map.room[_r1].posYMin))
                psy--;
            while ((_map.tile[(pey * _map.w) + pex].b == RMG_BASE_WALL) && (pey < _map.room[_r2].posYMax))
                pey++;
        }
        while ((psx != pex) || (psy != pey))
        {
            if (_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL)
            {
                _map.tile[(psy * _map.w) + psx].b = RMG_BASE_FLOOR;
                if (_map.connectivityPadding > 0)
                {
                    for (uint16_t i = 0; i <= _map.connectivityPadding; i++)
                    {
                        if (_map.tile[(psy * _map.w) + psx + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w - i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w - i].b = RMG_BASE_FLOOR;
                    }
                }
            }
            psx = (psx == pex) ? psx : ((psx > pex) ? --psx : ++psx);
            psy = (psy == pey) ? psy : ((psy > pey) ? --psy : ++psy);
        }
    }

    // Straight Lines, 90 degree angle
    void mapConnectRooms_ND(sMap &_map, const uint16_t &_r1, const uint16_t &_r2)
    {
        uint16_t psx = _map.room[_r1].x;
        uint16_t psy = _map.room[_r1].y;
        uint16_t pex = _map.room[_r2].x;
        uint16_t pey = _map.room[_r2].y;

        while (psy != pey)
        {
            if (_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL)
            {
                _map.tile[(psy * _map.w) + psx].b = RMG_BASE_FLOOR;
                if (_map.connectivityPadding > 0)
                {
                    for (uint16_t i = 0; i <= _map.connectivityPadding; i++)
                    {
                        if (_map.tile[(psy * _map.w) + psx + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w - i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w - i].b = RMG_BASE_FLOOR;
                    }
                }
            }
            psy = (psy == pey) ? psy : ((psy > pey) ? --psy : ++psy);
        }
        while (psx != pex)
        {
            if (_map.tile[(psy * _map.w) + psx].b == RMG_BASE_WALL)
            {
                _map.tile[(psy * _map.w) + psx].b = RMG_BASE_FLOOR;
                if (_map.connectivityPadding > 0)
                {
                    for (uint16_t i = 0; i <= _map.connectivityPadding; i++)
                    {
                        if (_map.tile[(psy * _map.w) + psx + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx + _map.w - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx + _map.w - i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w + i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w + i].b = RMG_BASE_FLOOR;
                        if (_map.tile[(psy * _map.w) + psx - _map.w - i].b == RMG_BASE_WALL)
                            _map.tile[(psy * _map.w) + psx - _map.w - i].b = RMG_BASE_FLOOR;
                    }
                }
            }
            psx = (psx == pex) ? psx : ((psx > pex) ? --psx : ++psx);
        }
    }

    // Drunken Walk
    void mapConnectRooms_DW(sMap &_map, const uint16_t &_r1, const uint16_t &_r2)
    {

    }

    // Curved Corner
    void mapConnectRooms_CC(sMap &_map, const uint16_t &_r1, const uint16_t &_r2)
    {

    }

} // namespace rmg
