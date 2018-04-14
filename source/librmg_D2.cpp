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

    static void genD2_internal(sMap &_map)
    {
        for (uint32_t i = 0; i < _map.tileCount; i++)
            _map.tile[i].d = RMG_BASE_WALL;
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

#include "map_generator_D1.hpp"

uint16_t mapGenerator_D1_build_subrooms(sRoomGenData *_room, sGenerationData &_data)
{
    if ((_room->ex > _data.x) || (_room->ey > _data.y))
    {
        _data.error = eError::ALGORITHM;
        return 0;
    }
    uint16_t room_count = 0;
    uint16_t size_x = _room->ex - _room->sx;
    uint16_t size_y = _room->ey - _room->sy;
    bool split_x = (size_x > ((_data.roomMax_x * 2.125f) + _data.wallWidth));
    bool split_y = (size_y > ((_data.roomMax_y * 2.125f) + _data.wallWidth));
    if ((split_x) && (split_y))
        ((_data.rmg_rand() % 100) > 50) ? split_x = false : split_y = false;
    if (split_x)
    {
        uint16_t split_pos = (_data.rmg_rand() % (size_x - (_data.roomMin_x * 2))) + _data.roomMin_x;
        if (_data.error == eError::NONE)
        {
            _room->left = new sRoomGenData;
            _room->left->sx = _room->sx;
            _room->left->ex = _room->left->sx+split_pos;
            _room->left->sy = _room->sy;
            _room->left->ey = _room->ey;
            _room->left->parent = _room;
            _room->left->direction = eDirection::LEFT;
            room_count += mapGenerator_D1_build_subrooms(_room->left, _data);
        }
        if (_data.error == eError::NONE)
        {
            _room->right = new sRoomGenData;
            _room->right->sx = _room->left->sx+split_pos;
            _room->right->ex = _room->ex;
            _room->right->sy = _room->sy;
            _room->right->ey = _room->ey;
            _room->right->parent = _room;
            _room->right->direction = eDirection::RIGHT;
            room_count += mapGenerator_D1_build_subrooms(_room->right, _data);
        }
    }
    if (split_y)
    {
        uint16_t split_pos = (_data.rmg_rand() % (size_y - (_data.roomMin_y * 2))) + _data.roomMin_y;
        if (_data.error == eError::NONE)
        {
            _room->left = new sRoomGenData;
            _room->left->sx = _room->sx;
            _room->left->ex = _room->ex;
            _room->left->sy = _room->sy;
            _room->left->ey = _room->left->sy+split_pos;
            _room->left->parent = _room;
            _room->left->direction = eDirection::LEFT;
            room_count += mapGenerator_D1_build_subrooms(_room->left, _data);
        }
        if (_data.error == eError::NONE)
        {
            _room->right = new sRoomGenData;
            _room->right->sx = _room->sx;
            _room->right->ex = _room->ex;
            _room->right->sy = _room->left->sy+split_pos;
            _room->right->ey = _room->ey;
            _room->right->parent = _room;
            _room->right->direction = eDirection::RIGHT;
            room_count += mapGenerator_D1_build_subrooms(_room->right, _data);
        }
    }
    if ((!split_x) && (!split_y))
    {
        _room->end_node = true;
        room_count += 1;
    }
    return room_count;
}

void mapGenerator_D1_genRooms(sRoomGenData *_room, sGenerationData &_data)
{
    uint16_t room_ID = 0;
    bool done = false;
    _data.room = new sRoomData[_data.roomCount];
    sRoomGenData *tempRoom = _room;
     while (!done)
     {
        if ((tempRoom->parent == nullptr) && (tempRoom->left == nullptr) && (tempRoom->right == nullptr))
            done = true;
        if (tempRoom->left != nullptr)
            tempRoom = tempRoom->left;
        else if (tempRoom->right != nullptr)
            tempRoom = tempRoom->right;
        if ((tempRoom->left == nullptr) && (tempRoom->right == nullptr))
        {
            if(tempRoom->end_node)
            {
                _data.room[room_ID].valid = true;
                _data.room[room_ID].w = tempRoom->ex - tempRoom->sx;
                _data.room[room_ID].h = tempRoom->ey - tempRoom->sy;
                uint16_t x = tempRoom->sx + (_data.room[room_ID].w/2);
                uint16_t y = tempRoom->sy + (_data.room[room_ID].h/2);
                _data.room[room_ID].position = (y * _data.x) + x;
                room_ID++;
            }
            if (tempRoom->parent != nullptr)
            {
                eDirection direction = tempRoom->direction;
                tempRoom = tempRoom->parent;
                if (direction == eDirection::LEFT)
                {
                    delete tempRoom->left;
                    tempRoom->left = nullptr;
                }
                if (direction == eDirection::RIGHT)
                {
                    delete tempRoom->right;
                    tempRoom->right = nullptr;
                }
            }
        }
     }
}

void mapGenerator_D1_genRoomTileData(sGenerationData &_data)
{
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        uint16_t sx = (_data.room[i].position % _data.x) - (_data.room[i].w / 2) +1;
        uint16_t ex = sx + _data.room[i].w -1;
        uint16_t sy = (_data.room[i].position / _data.x) - (_data.room[i].h / 2) +1;
        uint16_t ey = sy + _data.room[i].h -1;
        for (uint16_t j = sx; j < ex; j++)
        {
            for (uint16_t k = sy; k < ey; k++)
            {
                _data.tile[(k * _data.x) + j] = eTile::FLOOR;
            }
        }
    }
}

void mapGenerator_D1(sGenerationData &_data)
{
    _data.exitCount = 0;
    _data.tileCount = _data.x * _data.y;
    _data.tile = new eTile[_data.tileCount];
    for (uint16_t i = 0; i < _data.tileCount; i++)
        _data.tile[i] = eTile::WALL;
    sRoomGenData *room = new sRoomGenData;
    room->sx = 0;
    room->ex = _data.x-1;
    room->sy = 0;
    room->ey = _data.y-1;
    room->end_node = false;
    _data.roomCount = mapGenerator_D1_build_subrooms(room, _data);
    mapGenerator_D1_genRooms(room, _data);
    delete room;
    mapGenerator_D1_genRoomTileData(_data);
    mapGenerator_connectRooms_direct(_data);
}
*/
