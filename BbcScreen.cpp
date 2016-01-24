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

#include <stdexcept>

#include "BbcScreen.h"

BbcScreen::BbcScreen(int screenMemSize)
{
    if(screenMemSize > MAX_MEMSIZE)
    {
        throw std::invalid_argument("Requested screen memory size too large");
    }

    screenMemSize_ = screenMemSize;
    screenStorage_ = new unsigned char[screenMemSize];

    // Initialise the screen memory to all zeros (to match BBC)
    for(int init = 0; init < screenMemSize; init++)
    {
        screenStorage_[init] = 0;
    }

    setMode(DEFAULT_MODE);
}

BbcScreen::~BbcScreen()
{
    // Free up the screen storage
    delete []screenStorage_;
}

void BbcScreen::setMode(int mode)
{
    mode = mode % 8;

    if(mode == 3 || mode == 6 || mode == 7)
    {
        throw std::invalid_argument("modes 3, 6 and 7 are not supported");
    }

    mode_ = (unsigned char)mode;
    int blocksInFile = screenMemSize_ / BLOCK_BYTES;

    switch(mode_)
    {
        case 0:
            screenWidth_ = BBC_WIDTH0;
            blockRows_ = blocksInFile / BBC_XBLKS0;
            break;
        case 1:
            screenWidth_ = BBC_WIDTH1;
            blockRows_ = blocksInFile / BBC_XBLKS1;
            break;
        case 2:
            screenWidth_ = BBC_WIDTH2;
            blockRows_ = blocksInFile / BBC_XBLKS2;
            break;
        case 4:
            screenWidth_ = BBC_WIDTH4;
            blockRows_ = blocksInFile / BBC_XBLKS4;
            break;
        case 5:
            screenWidth_ = BBC_WIDTH5;
            blockRows_ = blocksInFile / BBC_XBLKS5;
            break;
    }

    if(blockRows_ == 0) {
        // The screen memory doesn't have even one row of blocks
        throw std::runtime_error("Allocated screen too small");
    }

    // Calculate how tall the image is based on the number of rows of blocks.
    screenHeight_ = blockRows_ * BLOCK_BYTES;

    // Set the default colour mappings for the mode
    switch(mode)
    {
        case 0:
        case 4:
            palette_[0] = 0;
            palette_[1] = 7;
            break;
        case 1:
        case 5:
            palette_[0] = 0;
            palette_[1] = 1;
            palette_[2] = 3;
            palette_[3] = 7;
            break;
        case 2:
            for(unsigned char i = 0; i < 8; i++) {
                palette_[i] = i;
            }
            break;
    }
}

unsigned char BbcScreen::getMode()
{
    return mode_;
}

void BbcScreen::setScreenByte(int address, unsigned char byte)
{
    if(address >= screenMemSize_)
    {
        throw std::invalid_argument("address out of bounds");
    }

    screenStorage_[address] = byte;
}

int BbcScreen::getScreenWidth()
{
    return screenWidth_;
}

int BbcScreen::getScreenHeight()
{
    return screenHeight_;
}

void BbcScreen::setColour(unsigned char colour, unsigned char value)
{
    if(colour >= PALETTE_SIZE || colour < 0)
    {
        throw std::invalid_argument("Colour out of range");
    }

    if(value >= PALETTE_SIZE || value < 0)
    {
        throw std::invalid_argument("Value out of range");
    }

    palette_[colour] = value;
}

unsigned char BbcScreen::getColour(unsigned char colour)
{
    if(colour >= PALETTE_SIZE || colour < 0)
    {
        throw std::invalid_argument("Colour out of range");
    }

    return palette_[colour];
}

void BbcScreen::draw(DrawPixel callback)
{
    switch(mode_)
    {
        case 0:
        case 4:
            draw04(callback);
            break;
        case 1:
        case 5:
            draw15(callback);
            break;
        case 2:
            draw2(callback);
            break;
    }
}

// MODE 0 or MODE 4 picture
void BbcScreen::draw04(DrawPixel callback)
{
   int bit, i, j, k;
   unsigned char thisByte;
   unsigned int index;
   int nX = 0;
   int nY = 0;
   int nBlocks = BBC_XBLKS0;

   if(mode_ == 4) {
      nBlocks = BBC_XBLKS4;
   }

   int address = 0;

   for(k = 0; k < blockRows_; k++)
   {
       for(j = 0; j < nBlocks; j++)
       {
           for(i = 0; i < BLOCK_BYTES; i++)
           {
               thisByte = screenStorage_[address];

               for(bit = 0; bit < 8; bit++)
               {
                   index = thisByte & 1;
                   callback((nX+7)-bit, nY+i, getColour(index));
                   thisByte = thisByte >> 1;
               }

               address++;
           }

           nX = nX + 8;
       }

       nX = 0;
       nY = nY + 8;
   }
}

// MODE 1 or MODE 5 picture
void BbcScreen::draw15(DrawPixel callback)
{
   int i, j, k;
   unsigned char thisByte;
   unsigned char index;
   int nX = 0;
   int nY = 0;
   int nBlocks = BBC_XBLKS1;

   if(mode_ == 5) {
      nBlocks = BBC_XBLKS5;
   }

   int address = 0;

   for(k = 0; k < blockRows_; k++)
   {
       for(j = 0; j < nBlocks; j++)
       {
           for(i = 0; i < BLOCK_BYTES; i++)
           {
               thisByte = screenStorage_[address];

               index = ((thisByte >> 6) & 2) | ((thisByte >> 3) & 1);
               callback(nX, nY+i, getColour(index));

               index = ((thisByte >> 5) & 2) | ((thisByte >> 2) & 1);
               callback(nX+1, nY+i, getColour(index));

               index = ((thisByte >> 4) & 2) | ((thisByte >> 1) & 1);
               callback(nX+2, nY+i, getColour(index));

               index = ((thisByte >> 3) & 2) | (thisByte & 1);
               callback(nX+3, nY+i, getColour(index));

               address++;
           }

           nX = nX + 4;
       }
       nX = 0;
       nY = nY + 8;
   }
}

// MODE 2 picture
void BbcScreen::draw2(DrawPixel callback)
{
   int i, j, k;
   unsigned char thisByte;
   unsigned char index;
   int nX = 0;
   int nY = 0;

   int address = 0;

   for(k = 0; k < blockRows_; k++) {
      for(j = 0; j < BBC_XBLKS2; j++) {
         for(i = 0; i < BLOCK_BYTES; i++) {
            thisByte = screenStorage_[address];

            index = ((thisByte >> 4) & 8) | ((thisByte >> 3) & 4) | ((thisByte >> 2) & 2) | ((thisByte >> 1) & 1);
            callback(nX, nY+i, getColour(index));

            index = ((thisByte >> 3) & 8) | ((thisByte >> 2) & 4) | ((thisByte >> 1) & 2) | (thisByte  & 1);
            callback(nX+1, nY+i, getColour(index));

            address++;
         }
         nX = nX + 2;
      }
      nX = 0;
      nY = nY + 8;
   }
}
