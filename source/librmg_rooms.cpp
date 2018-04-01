
#include "librmg.hpp"

namespace rmg
{

    static void mapFindRoom(sMap &_map, uint32_t i)
    {
        if ((!_map.tile[i].p) && (_map.tile[i].d == RMG_FLOOR))
        {
            _map.tile[i].p = true;
            _map.tile[i].r = _map.roomCount;
            mapFindRoom(_map, i+1);
            mapFindRoom(_map, i-1);
            mapFindRoom(_map, i+_map.w);
            mapFindRoom(_map, i-_map.w);
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
                if ((!_map.tile[i].p) && (_map.tile[i].d == RMG_FLOOR) && (_map.tile[i].r == 0))
                {
                    _map.roomCount++;
                    mapFindRoom(_map, i);
                }
            }
        }
    }

    static void mapDiscardMinRooms(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            for (uint16_t i = 1; i < _map.roomCount+1; i++)
            {
                uint32_t tileCount = 0;
                for (uint32_t j = 0; j < _map.tileCount; j++)
                {
                    if (_map.tile[j].r == i)
                        tileCount++;
                }
                if (tileCount <= _map.roomAreaMin)
                {
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
    }

    static void mapRoomSizeLocation(sMap &_map)
    {
        if ((_map.roomCount > 0) && (_map.room != nullptr))
        {
            for (uint16_t i = 1; i < _map.roomCount+1; i++)
            {
                _map.room[i].posXMin = _map.w;
                _map.room[i].posXMax = 0;
                _map.room[i].posYMin = _map.h;
                _map.room[i].posYMax = 0;
                for (uint32_t j = 0; j < _map.h; j++)
                {
                    for (uint32_t k = 0; k < _map.w; k++)
                    {
                        if (_map.tile[(j * _map.w) + k].r == i)
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
            mapRoomSizeLocation(_map);
        }
    }

} // namespace rmg
