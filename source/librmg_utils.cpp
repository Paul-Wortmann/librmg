
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
                            std::cout << ".";
                        break;
                        case RMG_PATH:
                            std::cout << "P";
                        break;
                        case RMG_DOOR:
                            std::cout << "D";
                        break;
                        case RMG_STAIRS:
                            std::cout << "S";
                        break;
                        case RMG_FLOOR:
                            if (_map.tile[(i * _map.w) + j].r != 0)
                                std::cout << _map.tile[(i * _map.w) + j].r;
                            else
                                std::cout << " ";
                        break;
                        default:
                            std::cout << "?";
                        break;
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "Map seed: " << _map.seed << std::endl;
        }
    }

    void genCircle(sMap &_map)
    {
        if (_map.tile == nullptr)
            mapInit(_map);
        if (_map.tile != nullptr)
        {
            float level_r = (((_map.w / 2.0) < (_map.h / 2.0)) ? (_map.w / 2.0) : (_map.h / 2.0));
            for (uint32_t i = 0; i < _map.h; i++)
            {
                for (uint32_t j = 0; j < _map.w; j++)
                {
                    _map.tile[(i * _map.w) + j].x = (j - (_map.w / 2.0f));
                    _map.tile[(i * _map.w) + j].y = (i - (_map.h / 2.0f));
                    _map.tile[(i * _map.w) + j].d = (((_map.tile[(i * _map.w) + j].x * _map.tile[(i * _map.w) + j].x) + (_map.tile[(i * _map.w) + j].y * _map.tile[(i * _map.w) + j].y)) < (level_r * level_r)) ? RMG_FLOOR : RMG_WALL;
                }
            }
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

} // namespace rmg
