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

    bool prefabFind(sMap &_map, sPrefabData &_prefabData)
    {
        _prefabData.count = 0;;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (_map.prefabPath.c_str())) != nullptr)
        {
            while ((ent = readdir (dir)) != nullptr)
            {
                if (ent->d_type == DT_REG)
                    _prefabData.count++;
            }
            rewinddir(dir);
            _prefabData.filename = new std::string[_prefabData.count];
            _prefabData.count = 0;;
            while ((ent = readdir (dir)) != nullptr)
            {
                if (ent->d_type == DT_REG)
                {
                    _prefabData.filename[_prefabData.count] = ent->d_name;
                    _prefabData.count++;
                }
            }
            closedir (dir);
            std::cout << _prefabData.filename[0] << std::endl;
        }
        else
        {
            std::cout << "Error could not open directory." << std::endl;
            return EXIT_FAILURE;
        }
        return (_prefabData.count > 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

} // namespace rmg
