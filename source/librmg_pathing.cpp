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

    void pathASFree(sPath &_path)
    {
        if (_path.path != nullptr)
        {
            delete _path.path;
            _path.path = nullptr;
        }
    }

    static void pathAScalcTile(sMap &_map, sPath &_path, uint32_t _p, uint32_t _t)
    {
        if (_map.tile[_t].L == RMG_AS_CLOSED)
            return;
        if (_map.tile[_t].A == RMG_AS_START)
            return;
        if (_map.tile[_t].L == RMG_AS_OPEN)
        {
            uint32_t newG = (((_map.tile[_t].x == _map.tile[_p].x) || (_map.tile[_t].y == _map.tile[_p].y)) ? RMG_AS_MOV_S : RMG_AS_MOV_D);
            if ((_map.tile[_p].G + _map.tile[_t].G) >= (_map.tile[_p].G + newG))
                return;
        }
        _map.tile[_t].P = _p;
        _map.tile[_t].L = RMG_AS_OPEN;
        _map.tile[_t].H = (abs(_map.tile[_t].x - _path.ex) + abs(_map.tile[_t].y - _path.ey)) * RMG_AS_MOV_H;
        _map.tile[_t].G = _map.tile[_p].G + (((_map.tile[_t].x == _map.tile[_p].x) || (_map.tile[_t].y == _map.tile[_p].y)) ? RMG_AS_MOV_S : RMG_AS_MOV_D);
        _map.tile[_t].F = _map.tile[_t].H + _map.tile[_t].G;
    }

    static bool pathASinternal(sMap &_map, sPath &_path, uint32_t _p, uint32_t _t)
    {
        uint32_t tile = _t;
        if (_map.tile[_t].A == RMG_AS_END)
        {
            _map.tile[_t].P = _p;
            return true;
        }
        if (_map.tile[_t].L == RMG_AS_OPEN)
            _map.tile[_t].L = RMG_AS_CLOSED;
        tile = _t + 1;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t - 1;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t + _map.w;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t - _map.w;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t + _map.w + 1;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t + _map.w - 1;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t - _map.w + 1;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        tile = _t - _map.w - 1;
        if ((tile >= 0) && (tile < _map.tileCount))
            pathAScalcTile(_map, _path, _t, tile);
        uint32_t nextTileF = UINT32_MAX;
        uint32_t nextTile  = 0;
        bool     nextFound = false;
        for (uint32_t i = 0; i < _map.tileCount; i++)
        {
            if ((_map.tile[i].L == RMG_AS_OPEN) && (_map.tile[i].F < nextTileF))
            {
                nextTileF = _map.tile[i].F;
                nextTile = i;
                nextFound = true;
            }
        }
        if (nextFound)
            return pathASinternal(_map, _path, _t, nextTile);
        return false;
    }

    bool pathAS(sMap &_map, sPath &_path)
    {
        for (uint32_t i = 0; i < _map.h; i++)
        {
            for (uint32_t j = 0; j < _map.w; j++)
            {
                uint32_t tile = (i * _map.w) + j;
                _map.tile[tile].x = j;
                _map.tile[tile].y = i;
                _map.tile[tile].L = (_map.tile[tile].b == RMG_BASE_WALL) ? RMG_AS_CLOSED : RMG_AS_NONE;
                _map.tile[tile].A = RMG_AS_NONE;
            }
        }
        uint32_t st = (_path.sy * _map.w) + _path.sx;
        uint32_t et = (_path.ey * _map.w) + _path.ex;
        _map.tile[st].A = RMG_AS_START;
        _map.tile[st].L = RMG_AS_CLOSED;
        _map.tile[et].A = RMG_AS_END;
        _map.tile[st].H = 0;
        _map.tile[st].G = 0;
        _map.tile[st].F = 0;
        _map.tile[st].P = st;
        if (!pathASinternal(_map, _path, st, st))
            return false;
        _path.length = 0;
        //build path data
        uint32_t pathPosition = et;
        while (pathPosition != st)
        {
            pathPosition = _map.tile[pathPosition].P;
            _path.length++;
        }
        _path.path = new uint32_t[_path.length];
        pathPosition = et;
        uint32_t i = _path.length;
        while (pathPosition != st)
        {
            _path.path[--i] = pathPosition;
            pathPosition = _map.tile[pathPosition].P;
        }
        return true;
    }

} // namespace rmg












