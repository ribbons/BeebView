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

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>

#include "About.h"
#include "BbcImageLoader.h"

#include "Beebview.h"
#include "ui_Beebview.h"

Beebview::Beebview(QStringList args, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Beebview),
    modesGroup(this),
    coloursGroup(this)
{
    ui->setupUi(this);
    QApplication::setWindowIcon(QIcon(":Graphics/BeebView.png"));

    for(int i = 0; i < 6 ; i++)
    {
        if(i == 3)
        {
            continue;
        }

        QAction *action = new QAction(tr("Mode &%1").arg(i), this);
        action->setCheckable(true);
        action->setData(i);

        modesGroup.addAction(action);
        ui->menuMode->addAction(action);
    }

    for(int i = 0; i < 8; i++)
    {
        QAction *action = new QAction(tr("Colour &%1").arg(i), this);
        action->setData(i);

        coloursGroup.addAction(action);
        ui->menuCycleColour->addAction(action);
    }

    QObject::connect(&modesGroup, &QActionGroup::triggered, this, &Beebview::modesGroup_triggered);
    QObject::connect(&coloursGroup, &QActionGroup::triggered, this, &Beebview::coloursGroup_triggered);
    QObject::connect(ui->actionExit, &QAction::triggered, this, &Beebview::close);

    image = new BbcScreenWidget();
    this->setCentralWidget(image);
    this->setFixedSize(this->sizeHint());
    this->UpdateInfo();

    QString fileName;
    char screenMode = -1;
    bool autoSave = false;

    // Process command line arguments
    args.removeFirst();

    foreach (const QString &arg, args)
    {
        if(arg == "--save")
        {
            autoSave = true;
        }
        else if(arg == "--mode0")
        {
            screenMode = 0;
        }
        else if(arg == "--mode1")
        {
            screenMode = 1;
        }
        else if(arg == "--mode2")
        {
            screenMode = 2;
        }
        else if(arg == "--mode4")
        {
            screenMode = 4;
        }
        else if(arg == "--mode5")
        {
            screenMode = 5;
        }
        else
        {
            // As this doesn't match any switches, assume it is a filename
            fileName = arg;
        }
    }

    if(!fileName.isEmpty())
    {
        LoadFile(fileName);

        // Set the mode if this was requested, and the image loaded okay
        if(screenMode != -1 && screen != NULL)
        {
            screen->setMode(screenMode);
            image->setScreen(screen);
            this->UpdateInfo();
        }
    }

    // Automatically save the file and exit if autoSave is true
    if(autoSave)
    {
        // Only attempt to save a file if one was loaded
        if(screen != NULL)
        {
            QFileInfo info(fileName);
            SaveAs(info.path() + QDir::separator() + info.completeBaseName() + ".bmp");
        }

        // Close the program
        QTimer::singleShot(0, this, &Beebview::close);
    }
}

void Beebview::on_menuFile_aboutToShow()
{
    ui->actionSaveAs->setEnabled(screen != NULL);
}

void Beebview::on_menuMode_aboutToShow()
{
    modesGroup.setEnabled(screen != NULL);

    if(screen != NULL)
    {
        int checkItem = -1;

        switch(screen->getMode())
        {
            case 0:
                checkItem = 0;
                break;
            case 1:
                checkItem = 1;
                break;
            case 2:
                checkItem = 2;
                break;
            case 4:
                checkItem = 3;
                break;
            case 5:
                checkItem = 4;
                break;
        }

        modesGroup.actions()[checkItem]->setChecked(true);
    }
}

void Beebview::on_menuCycleColour_aboutToShow()
{
    coloursGroup.setEnabled(screen != NULL);

    if(screen != NULL)
    {
        switch(screen->getMode())
        {
            case 0:
            case 4:
                coloursGroup.actions()[2]->setEnabled(false);
                coloursGroup.actions()[3]->setEnabled(false);
                coloursGroup.actions()[4]->setEnabled(false);
                coloursGroup.actions()[5]->setEnabled(false);
                coloursGroup.actions()[6]->setEnabled(false);
                coloursGroup.actions()[7]->setEnabled(false);
                break;
            case 1:
            case 5:
                coloursGroup.actions()[2]->setEnabled(true);
                coloursGroup.actions()[3]->setEnabled(true);
                coloursGroup.actions()[4]->setEnabled(false);
                coloursGroup.actions()[5]->setEnabled(false);
                coloursGroup.actions()[6]->setEnabled(false);
                coloursGroup.actions()[7]->setEnabled(false);
                break;
            case 2:
                coloursGroup.actions()[2]->setEnabled(true);
                coloursGroup.actions()[3]->setEnabled(true);
                coloursGroup.actions()[4]->setEnabled(true);
                coloursGroup.actions()[5]->setEnabled(true);
                coloursGroup.actions()[6]->setEnabled(true);
                coloursGroup.actions()[7]->setEnabled(true);
                break;
        }
    }
}

void Beebview::modesGroup_triggered(QAction *action)
{
    screen->setMode(action->data().toInt());
    image->setScreen(screen);
    this->UpdateInfo();
}

void Beebview::on_actionReportBug_triggered(bool checked)
{
    QDesktopServices::openUrl(QUrl("https://nerdoftheherd.com/tools/beebview/bug_report.php"));
}

void Beebview::on_actionHelp_triggered(bool checked)
{
    QDesktopServices::openUrl(QUrl("https://nerdoftheherd.com/tools/beebview/help/"));
}

void Beebview::on_actionAbout_triggered(bool checked)
{
    About *about = new About(this);
    about->show();
}

Beebview::~Beebview()
{
    delete ui;

    // Clean up the screen object if it exists
    if(screen != NULL)
    {
        delete screen;
        screen = NULL;
    }
}

void Beebview::coloursGroup_triggered(QAction *action)
{
    uint8_t colour = action->data().toInt();
    screen->setColour(colour, (screen->getColour(colour) + 1) % 8);
    image->setScreen(screen);
}

void Beebview::on_actionOpen_triggered(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
        QString(), tr("BBC Graphics Files (*.bbg);;All Files (*)"));

    if(!fileName.isNull())
    {
        LoadFile(fileName);
        this->UpdateInfo();
    }
}

void Beebview::LoadFile(QString fileName)
{
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("File Error"),
            tr("There was a problem opening the file \"%1\".").arg(fileName));

        return;
    }

    if(file.size() > BbcScreen::MAX_MEMSIZE)
    {
        QMessageBox::critical(this, tr("File Error"),
            tr("\"%1\" is too large to be a BBC graphics file.").arg(fileName));

        return;
    }

    // Clean up the old screen object if there is one
    if(screen != NULL)
    {
        delete screen;
        screen = NULL;
    }

    uint8_t *data = file.map(0, file.size());
    BbcImageLoader loader(data, file.size());

    screen = loader.LoadAuto();

    image->setScreen(screen);
    this->setFixedSize(this->sizeHint());

    QFileInfo info(fileName);
    currentFileTitle = info.completeBaseName(); // File name minus path and extension
}

void Beebview::UpdateInfo()
{
    QString title = tr("BBC Graphics Viewer");

    if(screen != NULL)
    {
        title.append(tr(" - %1  [MODE %2]")
            .arg(currentFileTitle, QString::number(screen->getMode())));
    }

    this->setWindowTitle(title);
}

void Beebview::on_actionSaveAs_triggered(bool checked)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
        QString(), tr("Portable Network Graphics (*.png);;Windows Bitmap (*.bmp)"));

    if(!fileName.isNull())
    {
        SaveAs(fileName);
    }
}

void Beebview::SaveAs(QString fileName)
{
    if(!image->saveAs(fileName))
    {
        QMessageBox::critical(this, tr("File Error"),
            tr("Unable to save to \"%1\", please check the name and try again.").arg(fileName));
    }
}
