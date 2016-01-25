/*
 * This file is part of BBC Graphics Viewer.
 * Copyright © 2016 by the authors - see the AUTHORS file for details.
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

#include <QWidget>

#include "BbcScreen.h"

class BbcScreenWidget : public QWidget
{
    Q_OBJECT

public:
    const int dispWidth = 640;  // Width to display the BBC screen
    const int defHeight = 512;  // default height of "screen" (client area)
    const int heightRatio = 2;  // Ratio to multiply height of screen for display

    explicit BbcScreenWidget(QWidget *parent = 0);
    void setScreen(BbcScreen *screen);
    bool saveAs(QString fileName);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void pixelCallback(int x, int y, uint8_t colour);

    QImage image;
};
