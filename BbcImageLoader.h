/*
 * This file is part of BBC Graphics Viewer.
 * Copyright © 2003-2016 by the authors - see the AUTHORS file for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <fstream>

#include "BbcScreen.h"

class BbcImageLoader
{
public:
    BbcImageLoader(uint8_t *data, int size);
    BbcScreen* LoadAuto();
    BbcScreen* LoadMemDump();
    BbcScreen* LoadLdPic();

private:
    bool getBitFromFile(bool flushStore, uint8_t *fileBit);
    bool getBitsFromFile(int numBits, bool flushStore, uint8_t *fileBits);

    uint8_t *data;
    int size;
    int pos = 0;
};