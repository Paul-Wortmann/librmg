
#include "librmg.hpp"

namespace rmg
{

    void mapConnectRooms(sMap &_map)
    {
        switch (_map.connectivityAlgorithm)
        {
            case RMG_PATH_SL:
                mapConnectRooms_SL(_map, 1, 2);
            break;
            case RMG_PATH_ND:
                mapConnectRooms_ND(_map, 1, 2);
            break;
            case RMG_PATH_DW:
                mapConnectRooms_DW(_map, 1, 2);
            break;
            default:
                // throw error / set flag
            break;
        }
    }

    // Straight Line
    void mapConnectRooms_SL(sMap &_map, uint16_t _r1, uint16_t _r2)
    {

    }

    // Straight Lines, 90 degree angle
    void mapConnectRooms_ND(sMap &_map, uint16_t _r1, uint16_t _r2)
    {

    }

    // Drunken Walk
    void mapConnectRooms_DW(sMap &_map, uint16_t _r1, uint16_t _r2)
    {

    }

} // namespace rmg
