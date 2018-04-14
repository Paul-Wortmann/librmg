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

    static void genM1_internal(sMap &_map)
    {
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].d = RMG_BASE_WALL;
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
                    if (mapGetFloorArea(_map) > (_map.tileCount / _map.floorAreaMin))
                        i = _map.pass;
                }
            }
            mapInitRooms(_map);
            mapConnectRooms(_map);
            //mapRemoveAnomalies(_map);
        }
    }

} // namespace rmg

/*

#include "map_generator_M1.hpp"

bool isWallTile(sGenerationData &_data, int32_t _tile)
{
    if ((_tile < 0) || (_tile >= int32_t(_data.tileCount-1)))
        return false;
    return (_data.tile[_tile] == eTile::WALL);
}

bool map_gen_maze_check_tile(sGenerationData &_data, int32_t _tile, eDirection _directionBias)
{
    if ((_tile < 0) || (_tile >= int32_t(_data.tileCount-1)))
        return false;
    if ((_tile / _data.x) == 0 || (_tile / _data.x) == _data.y-1 || (_tile % _data.x) == 0 || (_tile % _data.x) == _data.x-1)
        return false;
    if (_data.tile[_tile] == eTile::FLOOR)
        return false;
    for (uint16_t wall_count = 1; wall_count <= _data.wallWidth; wall_count++)
    {
        if (_directionBias == eDirection::UP)
        {
            if (!isWallTile(_data, _tile-wall_count))
                return false;
            if (!isWallTile(_data, _tile+wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x*wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x-wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x+wall_count))
                return false;
        }
        else if (_directionBias == eDirection::DOWN)
        {
            if (!isWallTile(_data, _tile-wall_count))
                return false;
            if (!isWallTile(_data, _tile+wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x*wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x-wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x+wall_count))
                return false;
        }
        else if (_directionBias == eDirection::LEFT)
        {
            if (!isWallTile(_data, _tile-wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x*wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x-wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x*wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x-wall_count))
                return false;
        }
        else if (_directionBias == eDirection::RIGHT)
        {
            if (!isWallTile(_data, _tile+wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x*wall_count))
                return false;
            if (!isWallTile(_data, _tile-_data.x+wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x*wall_count))
                return false;
            if (!isWallTile(_data, _tile+_data.x+wall_count))
                return false;
        }
    }
    return true;
}

void map_gen_maze(sGenerationData &_data, uint32_t _tile, eDirection _directionBias)
{
    _data.tile[_tile] = eTile::FLOOR;
    uint32_t tiles_available = 1;
    eDirection tileDirection = eDirection::NONE;
    while (tiles_available > 0)
    {
        tiles_available = 0;
        uint32_t tile_up = _tile - _data.x;
        bool tile_up_ok = map_gen_maze_check_tile(_data, tile_up, eDirection::UP);
        if (tile_up_ok) tiles_available++;

        uint32_t tile_down = _tile + _data.x;
        bool tile_down_ok = map_gen_maze_check_tile(_data, tile_down, eDirection::DOWN);
        if (tile_down_ok) tiles_available++;

        uint32_t tile_right = _tile + 1;
        bool tile_right_ok = map_gen_maze_check_tile(_data, tile_right, eDirection::RIGHT);
        if (tile_right_ok) tiles_available++;

        uint32_t tile_left = _tile - 1;
        bool tile_left_ok = map_gen_maze_check_tile(_data, tile_left, eDirection::LEFT);
        if (tile_left_ok) tiles_available++;

        if (_directionBias == eDirection::NONE)
            tileDirection = (eDirection)((_data.rmg_rand() % 4) + 1);
        else
        {
            if (((_data.rmg_rand() % (4 + _data.directionBias_Threshold)) + 1) > 4)
                tileDirection = _directionBias;
            else
                tileDirection = (eDirection)((_data.rmg_rand() % 4) + 1);
        }
        if (tileDirection == eDirection::UP)
        {
            if (tile_up_ok)
            {
                tiles_available--;
                tile_up_ok = false;
                map_gen_maze(_data, tile_up, _directionBias);
            }
        }
        else if (tileDirection == eDirection::DOWN)
        {
            if (tile_down_ok)
            {
                tiles_available--;
                tile_down_ok = false;
                map_gen_maze(_data, tile_down, _directionBias);
            }
        }
        else if (tileDirection == eDirection::RIGHT)
        {
            if (tile_right_ok)
            {
                tiles_available--;
                tile_right_ok = false;
                map_gen_maze(_data, tile_right, _directionBias);
            }
        }
        else if (tileDirection == eDirection::LEFT)
        {
            if (tile_left_ok)
            {
                tiles_available--;
                tile_left_ok = false;
                map_gen_maze(_data, tile_left, _directionBias);
            }
        }
        else
            tiles_available--;
    }
}

bool add_room(sGenerationData &_data, sRoom &_room)
{
    _room.x = _data.roomMax_x + _data.rmg_rand() % (_data.x - (_data.roomMax_x * 2));
    _room.y = _data.roomMax_y + _data.rmg_rand() % (_data.y - (_data.roomMax_y * 2));
    _room.w = _data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x - _data.roomMin_x);
    _room.h = _data.roomMin_y + _data.rmg_rand() % (_data.roomMax_y - _data.roomMin_y);
    for (uint16_t i = _room.y-1-(uint16_t)_room.h/2.0f; i < _room.y+1+(uint16_t)_room.h/2.0f; i++)
    {
        for (uint16_t j = _room.x-1-(uint16_t)_room.w/2.0f; j < _room.x+1+(uint16_t)_room.w/2.0f; j++)
        {
            if (_data.tile[(i * _data.x) + j] == eTile::FLOOR)
                return false;
        }
    }
    for (uint16_t i = _room.y-(uint16_t)_room.h/2.0f; i < _room.y+(uint16_t)_room.h/2.0f; i++)
    {
        for (uint16_t j = _room.x-(uint16_t)_room.w/2.0f; j < _room.x+(uint16_t)_room.w/2.0f; j++)
        {
            _data.tile[(i * _data.x) + j] = eTile::FLOOR;
        }
    }
    return true;
}

bool connect_room(sGenerationData &_data, sRoom &_room)
{
    eAxis directionAxis = (abs(_room.x - _data.x) > abs(_room.y - _data.y)) ? eAxis::X : eAxis::Y;
    eDirection directionBias = (directionAxis == eAxis::X) ? ((_room.x > _data.x/2) ? eDirection::RIGHT : eDirection::LEFT) : ((_room.y > _data.y/2) ? eDirection::DOWN : eDirection::UP);
    bool foundWall = false;
    bool pathFound = false;
    uint32_t pathPos = (_room.y * _data.x) + _room.x;
    uint16_t pathMax = (directionAxis == eAxis::X) ? (abs(_data.x - _room.x) - 1) : (abs(_data.y - _room.y) - 1);
    for (uint16_t i = 0; i < pathMax; i++)
    {
        if (!pathFound)
        {
            if ((!foundWall)&&(_data.tile[pathPos] == eTile::WALL))
                foundWall = true;
            if ((foundWall)&&(_data.tile[pathPos] == eTile::FLOOR))
                pathFound = true;
            if ((foundWall)&&(_data.tile[pathPos] == eTile::WALL))
                _data.tile[pathPos] = eTile::FLOOR;
            pathPos = (directionAxis == eAxis::X) ? ((directionBias == eDirection::RIGHT) ? pathPos-1 : pathPos+1) : ((directionBias == eDirection::DOWN) ? pathPos-_data.x : pathPos+_data.x);
        }
    }
    return true;
}

void mapGenerator_M1(sGenerationData &_data)
{
    _data.tileCount = _data.x * _data.y;
    _data.tile = new eTile[_data.tileCount];
    for (uint32_t i = 0; i < _data.tileCount; i++)
        _data.tile[i] = eTile::WALL;
    uint16_t no_of_rooms = 2+ _data.rmg_rand() % 4;
    sRoom *room = new sRoom[no_of_rooms];
    for (uint16_t i = 0; i < no_of_rooms; i++)
        while (!add_room(_data, room[i]));
    map_gen_maze(_data, ((_data.x/2) + (_data.y/2*_data.x)), _data.directionBias);
    for (uint16_t i = 0; i < no_of_rooms; i++)
        connect_room(_data, room[i]);

    delete[] room;

}
*/
