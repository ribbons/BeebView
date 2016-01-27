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

#include <QActionGroup>
#include <QMainWindow>

#include "BbcScreenWidget.h"
#include "BbcScreen.h"

namespace Ui {
    class Beebview;
}

class Beebview : public QMainWindow
{
    Q_OBJECT

public:
    explicit Beebview(QStringList args, QWidget *parent = 0);
    ~Beebview();

private slots:
    void on_menuFile_aboutToShow();
    void on_menuMode_aboutToShow();
    void on_menuCycleColour_aboutToShow();
    void on_actionOpen_triggered(bool checked);
    void on_actionSaveAs_triggered(bool checked);
    void modesGroup_triggered(QAction *action);
    void coloursGroup_triggered(QAction *action);
    void on_actionReportBug_triggered(bool checked);
    void on_actionHelp_triggered(bool checked);
    void on_actionAbout_triggered(bool checked);

private:
    void LoadFile(QString fileName);
    void UpdateInfo();
    void SaveAs(QString fileName);

    Ui::Beebview *ui;
    QActionGroup modesGroup;
    QActionGroup coloursGroup;
    BbcScreenWidget *image;
    BbcScreen *screen = NULL;
    QString currentFileTitle;
};
