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
    void stripSpaces(std::string &_stringIN)
    {
        std::string tempString = "";
        uint16_t stringLength = _stringIN.length();
        for (uint16_t i = 0; i < stringLength; i++)
        {
            if (_stringIN[i] != ' ')
                tempString += _stringIN[i];
        }
        _stringIN = tempString;
    }

    int32_t xmlGetSingleInt(const std::string &_string)
    {
        bool valueStart = false;
        bool valueEnd = false;
        std::string value = "";
        uint16_t stringLength = _string.length();
        for (uint16_t i = 0; i < stringLength; i++)
        {
            if (_string[i] == '<')
            {
                if (!valueEnd && valueStart)
                    valueEnd = true;
            }
            if (_string[i] == '>')
            {
                if (!valueEnd && !valueStart)
                    valueStart = true;
            }
            if (!valueEnd && valueStart && (_string[i] != '>'))
                value += _string[i];
        }
        return atol(value.c_str());
    }

    int32_t xmlGetIntValueCount(const std::string &_string)
    {
        uint16_t valueCount = 0;
        uint16_t stringLength = _string.length();
        for (uint16_t i = 0; i < stringLength; i++)
            if (_string[i] == ',')
                valueCount++;
        if (valueCount > 0)
            valueCount++;
        return valueCount;
    }

    int32_t xmlGetIntValue(const std::string &_string, const uint16_t &_pos)
    {
        bool valueStart = false;
        bool valueEnd = false;
        uint16_t valuePos = 0;
        std::string value = "";
        uint16_t stringLength = _string.length();
        for (uint16_t i = 0; i < stringLength; i++)
        {
            if (_string[i] == '<')
            {
                if (!valueEnd && valueStart)
                    valueEnd = true;
            }
            if (_string[i] == '>')
            {
                if (!valueEnd && !valueStart)
                    valueStart = true;
            }
            if (!valueEnd && valueStart && (_string[i] != '>'))
            {
                if (_string[i] == ',')
                    valuePos++;
                else
                {
                    if (_pos == valuePos)
                        value += _string[i];
                }
            }
        }
        return atol(value.c_str());
    }

    void prefabFreeAll(sMap &_map)
    {
        sPrefab *prefabTemp = _map.prefab;
        while (prefabTemp != nullptr)
        {
            sPrefab *prefabTempPrevious = prefabTemp;
            prefabTemp = prefabTemp->next;
            if (prefabTempPrevious->tile)
                delete[] prefabTempPrevious->tile;
            delete prefabTempPrevious;
        }
    }

    void prefabEventFreeAll(sMap &_map)
    {
        sEventTile *eventTemp = _map.event;
        while (eventTemp != nullptr)
        {
            sEventTile *eventTempPrevious = eventTemp;
            eventTemp = eventTemp->next;
            if (eventTempPrevious->triggerTileID)
                delete[] eventTempPrevious->triggerTileID;
            if (eventTempPrevious->targetTileID)
                delete[] eventTempPrevious->targetTileID;
            delete eventTempPrevious;
        }
    }

    bool prefabFind(sMap &_map)
    {
        _map.prefabCount = 0;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (_map.prefabPath.c_str())) != nullptr)
        {
            while ((ent = readdir (dir)) != nullptr)
            {
                if (ent->d_type == DT_REG)
                {
                    _map.prefabCount++;
                    prefabLoad(_map, ent->d_name);
                }
            }
            closedir (dir);
        }
        else
        {
            std::cout << "Error could not open directory." << std::endl;
            return EXIT_FAILURE;
        }
        return (_map.prefabCount > 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    void prefabLoad(sMap &_map, const std::string &_fileName)
    {
        // read file and parse
        std::string fileAndPath = _map.prefabPath;
        fileAndPath += "/";
        fileAndPath += _fileName;
        std::ifstream tFstream(fileAndPath.c_str(), std::ifstream::in);
        if (tFstream.is_open())
        {
            uint16_t eventCount = _map.eventCount;
            sPrefab *prefabTemp = _map.prefab;
            if (_map.prefab == nullptr)
            {
                _map.prefab = new sPrefab;
                prefabTemp = _map.prefab;
            }
            else
            {
                for (prefabTemp = _map.prefab; prefabTemp->next != nullptr; prefabTemp = prefabTemp->next);
                prefabTemp->next = new sPrefab;
                prefabTemp = prefabTemp->next;
            }
            prefabTemp->next = nullptr;
            prefabTemp->filename = _fileName;
            uint16_t currentLayer = RMG_LAYER_NONE;
            bool eventSection = false;
            std::string tData = "";
            uint32_t tLength = 0;
            uint32_t tLine = 0;
            sEventTile *tEvent = nullptr;
            while (tFstream.good())
            {
                tData = "";
                std::getline(tFstream, tData);
                stripSpaces(tData);
                tLength = tData.length();
                if (tLength > 3)
                {
                    if (tData.find("<width>") != std::string::npos)
                        prefabTemp->w = xmlGetSingleInt(tData);
                    if (tData.find("<height>") != std::string::npos)
                        prefabTemp->h = xmlGetSingleInt(tData);
                    if (tData.find("<layer_base>") != std::string::npos)
                    {
                        prefabTemp->tileCount = prefabTemp->w * prefabTemp->h;
                        if (prefabTemp->tile == nullptr)
                            prefabTemp->tile = new sPrefabTile[prefabTemp->tileCount];
                        tLine = 0;
                        currentLayer = RMG_LAYER_BASE;
                    }
                    if (tData.find("<layer_object>") != std::string::npos)
                    {
                        prefabTemp->tileCount = prefabTemp->w * prefabTemp->h;
                        if (prefabTemp->tile == nullptr)
                            prefabTemp->tile = new sPrefabTile[prefabTemp->tileCount];
                        tLine = 0;
                        currentLayer = RMG_LAYER_OBJECT;
                    }
                    if (tData.find("<layer_event>") != std::string::npos)
                    {
                        prefabTemp->tileCount = prefabTemp->w * prefabTemp->h;
                        if (prefabTemp->tile == nullptr)
                            prefabTemp->tile = new sPrefabTile[prefabTemp->tileCount];
                        tLine = 0;
                        currentLayer = RMG_LAYER_EVENT;
                    }
                    if (tData.find("<tiles>") != std::string::npos)
                    {
                        uint16_t valueCount = xmlGetIntValueCount(tData);
                        for (uint16_t i = 0; i< valueCount; i++)
                        {
                            if (currentLayer == RMG_LAYER_BASE)
                                prefabTemp->tile[(tLine * prefabTemp->w) + i].b = xmlGetIntValue(tData, i);
                            if (currentLayer == RMG_LAYER_OBJECT)
                                prefabTemp->tile[(tLine * prefabTemp->w) + i].o = xmlGetIntValue(tData, i);
                            if (currentLayer == RMG_LAYER_EVENT)
                                prefabTemp->tile[(tLine * prefabTemp->w) + i].e = xmlGetIntValue(tData, i) + eventCount;
                        }
                        tLine++;
                    }
                    if (tData.find("<event>") != std::string::npos)
                    {
                        currentLayer = RMG_LAYER_NONE;
                        eventSection = true;
                    }
                    if (tData.find("</event>") != std::string::npos)
                        eventSection = false;
                    if (eventSection)
                    {
                        if (tData.find("<ID>") != std::string::npos)
                        {
                            if (_map.event == nullptr)
                            {
                                _map.event = new sEventTile;
                                tEvent = _map.event;
                            }
                            else
                            {
                                for (tEvent = _map.event; tEvent->next != nullptr; tEvent = tEvent->next);
                                tEvent->next = new sEventTile;
                                tEvent = tEvent->next;
                            }
                            tEvent->next = nullptr;
                            tEvent->ID = eventCount + xmlGetSingleInt(tData);
                        }
                        if (tData.find("<enabled>") != std::string::npos)
                            tEvent->enabled = (xmlGetSingleInt(tData) == 1);
                        if (tData.find("<type>") != std::string::npos)
                            tEvent->type = xmlGetSingleInt(tData);
                        if (tData.find("<state>") != std::string::npos)
                            tEvent->state = xmlGetSingleInt(tData);
                        if (tData.find("<reTriggerable>") != std::string::npos)
                            tEvent->reTriggerable = (xmlGetSingleInt(tData) == 1);
                        if (tData.find("<triggerTileID>") != std::string::npos)
                        {
                            tEvent->triggerTileCount = xmlGetIntValueCount(tData);
                            for (uint16_t i = 0; i< tEvent->triggerTileCount; i++)
                                tEvent->triggerTileID[i] = xmlGetIntValue(tData, i);
                        }
                        if (tData.find("<targetTileID>") != std::string::npos)
                        {
                            tEvent->targetTileCount = xmlGetIntValueCount(tData);
                            for (uint16_t i = 0; i< tEvent->targetTileCount; i++)
                                tEvent->targetTileID[i] = xmlGetIntValue(tData, i);
                        }
                        if (tData.find("<timer>") != std::string::npos)
                            tEvent->timer = xmlGetSingleInt(tData);
                    }
                }
            }
            tFstream.close();
            std::cout << "Loaded prefab: " << fileAndPath << std::endl;
        }
        else
        {
            std::cout << "Error - Failed to open file: " << fileAndPath << std::endl;
        }
    }

// should find how many prefabs of the same size and randomly select one.... TODO
    void mapPrefabRooms(sMap &_map)
    {
        for (uint16_t i = 0; i < _map.roomCount; i++)
        {
            sPrefab * tempPrefab = _map.prefab;
            for (tempPrefab = _map.prefab; tempPrefab != nullptr; )
            {
                if ((_map.room[i].w == tempPrefab->w) && (_map.room[i].h == tempPrefab->h))
                {
                    std::cout << "Applying prefab: " << tempPrefab->filename << std::endl;
                    mapPrefabRoomRotateL90(*tempPrefab);
                    uint16_t sX = _map.room[i].x - (_map.room[i].w / 2);
                    uint16_t sY = _map.room[i].y - (_map.room[i].h / 2);
                    for (uint16_t k = 0; k < tempPrefab->h; k++)
                    {
                        for (uint16_t l = 0; l < tempPrefab->w; l++)
                        {
                            _map.tile[(_map.w * (k+sY)) + (l+sX)].b = tempPrefab->tile[(tempPrefab->w * k) + l].b;
                            _map.tile[(_map.w * (k+sY)) + (l+sX)].o = tempPrefab->tile[(tempPrefab->w * k) + l].o;
                            _map.tile[(_map.w * (k+sY)) + (l+sX)].e = tempPrefab->tile[(tempPrefab->w * k) + l].e;
                        }
                    }
                    tempPrefab = nullptr;
                }
                if (tempPrefab != nullptr)
                    tempPrefab = tempPrefab->next;
            }
        }
    }

    void mapPrefabRoomRotateFV(sPrefab &_prefab)
    {
        sPrefabTile *tile = new sPrefabTile[_prefab.tileCount];
        for (uint32_t i = 0; i < _prefab.tileCount; i++)
        {
            tile[i].b = _prefab.tile[i].b;
            tile[i].o = _prefab.tile[i].o;
            tile[i].e = _prefab.tile[i].e;
        }
        for (uint32_t i = 0; i < _prefab.tileCount; i++)
        {
            _prefab.tile[(_prefab.tileCount-1)-i].b = tile[i].b;
            _prefab.tile[(_prefab.tileCount-1)-i].o = tile[i].o;
            _prefab.tile[(_prefab.tileCount-1)-i].e = tile[i].e;
        }
        delete[] tile;
    }

    void mapPrefabRoomRotateR90(sPrefab &_prefab)
    {
        sPrefabTile *tile = new sPrefabTile[_prefab.tileCount];
        for (uint32_t i = 0; i < _prefab.tileCount; i++)
        {
            tile[i].b = _prefab.tile[i].b;
            tile[i].o = _prefab.tile[i].o;
            tile[i].e = _prefab.tile[i].e;
        }
        for (uint32_t i = 0; i < _prefab.h; i++)
        {
            for (uint32_t j = 0; j < _prefab.w; j++)
            {
                _prefab.tile[(i * _prefab.w) + j].b = tile[(j * _prefab.w) + ((_prefab.w-1)-i)].b;
                _prefab.tile[(i * _prefab.w) + j].o = tile[(j * _prefab.w) + ((_prefab.w-1)-i)].o;
                _prefab.tile[(i * _prefab.w) + j].e = tile[(j * _prefab.w) + ((_prefab.w-1)-i)].e;
            }
        }
        delete[] tile;
    }

    void mapPrefabRoomRotateL90(sPrefab &_prefab)
    {
        sPrefabTile *tile = new sPrefabTile[_prefab.tileCount];
        for (uint32_t i = 0; i < _prefab.tileCount; i++)
        {
            tile[i].b = _prefab.tile[i].b;
            tile[i].o = _prefab.tile[i].o;
            tile[i].e = _prefab.tile[i].e;
        }
        for (uint32_t i = 0; i < _prefab.h; i++)
        {
            for (uint32_t j = 0; j < _prefab.w; j++)
            {
                _prefab.tile[(i * _prefab.w) + j].b = tile[(i * _prefab.w) + ((_prefab.w-1)-j)].b;
                _prefab.tile[(i * _prefab.w) + j].o = tile[(i * _prefab.w) + ((_prefab.w-1)-j)].o;
                _prefab.tile[(i * _prefab.w) + j].e = tile[(i * _prefab.w) + ((_prefab.w-1)-j)].e;
            }
        }
        delete[] tile;
    }

} // namespace rmg
