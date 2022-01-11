/*
 * Copyright © 1999-2000 David Robinson
 * Copyright © 2007-2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QActionGroup>
#include <QMainWindow>

#include <BbcScreen.h>

#include "BbcScreenWidget.h"

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
