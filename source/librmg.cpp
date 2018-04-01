
#include "librmg.hpp"

namespace rmg
{

    void mapInit(sMap &_map)
    {
        if (_map.tile != nullptr)
            mapFree(_map);
        if ((_map.tile == nullptr) && (_map.w > 0) && (_map.h > 0))
        {
            _map.tileCount = _map.h * _map.w;
            _map.tile = new rmg::sTile[_map.tileCount];
        }
    }

    void mapFree(sMap &_map)
    {
        if (_map.tile != nullptr)
        {
            delete _map.tile;
            _map.tile = nullptr;
        }
        if (_map.room != nullptr)
        {
            delete _map.room;
            _map.room = nullptr;
        }
    }

} // namespace rmg
