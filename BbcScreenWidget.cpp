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

#include "BbcScreenWidget.h"

#include <QPainter>

BbcScreenWidget::BbcScreenWidget(QWidget *parent) :
    QWidget(parent),
    image(1, 1, QImage::Format_RGB32)
{
    image.fill(Qt::black);
    setFixedSize(dispWidth, defHeight);
}

void BbcScreenWidget::setScreen(BbcScreen *screen)
{
    image = QImage(screen->getScreenWidth(), screen->getScreenHeight(), QImage::Format_Indexed8);

    image.setColor(0, qRgb(0,   0,   0));
    image.setColor(1, qRgb(255, 0,   0));
    image.setColor(2, qRgb(0,   255, 0));
    image.setColor(3, qRgb(255, 255, 0));
    image.setColor(4, qRgb(0,   0,   255));
    image.setColor(5, qRgb(255, 0,   255));
    image.setColor(6, qRgb(0,   255, 255));
    image.setColor(7, qRgb(255, 255, 255));

    for(unsigned char i = 8; i < PALETTE_SIZE; i++)
    {
        // Map flashing colours to grey
        image.setColor(i, qRgb(128, 128, 128));
    }

    setFixedSize(dispWidth, screen->getScreenHeight() * heightRatio);
    image.fill(0);

    using namespace std::placeholders;
    screen->draw(std::bind(&BbcScreenWidget::pixelCallback, this, _1, _2, _3));
    update();
}

void BbcScreenWidget::pixelCallback(int x, int y, unsigned long colour)
{
    image.setPixel(x, y, colour);
}

void BbcScreenWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(rect(), image, image.rect());
}

bool BbcScreenWidget::saveAs(QString fileName)
{
    return image.scaled(rect().size()).save(fileName);
}
