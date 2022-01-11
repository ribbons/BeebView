/*
 * Copyright © 2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QWidget>

#include <BbcScreen.h>

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
