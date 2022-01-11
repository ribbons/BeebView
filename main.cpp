/*
 * Copyright © 2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Beebview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Beebview mainWin(app.arguments());
    mainWin.show();

    return app.exec();
}
