/*
 * Copyright © 2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QDialog>

namespace Ui {
    class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:
    Ui::About *ui;
};
