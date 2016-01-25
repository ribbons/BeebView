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

#include <fstream>

#include "About.h"
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
    std::ifstream file;
    file.open(fileName.toStdString().c_str(), std::ifstream::binary);

    if(!file)
    {
        QMessageBox::critical(this, tr("File Error"),
            tr("There was a problem opening the file \"%1\".").arg(fileName));
    }

    // Clean up the old screen object if there is one
    if(screen != NULL)
    {
        delete screen;
        screen = NULL;
    }

    // Assume the file is LdPic format, and attempt to load it like that
    if(!LoadLdPic(file))
    {
        // File was not in LdPic format, clean up and load it as a memory dump
        if(screen != NULL)
        {
            delete screen;
            screen = NULL;
        }

        file.seekg(0);
        LoadMemDump(file);
    }

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

void Beebview::LoadMemDump(std::ifstream &file)
{
    file.seekg(0, std::ifstream::end);
    int fileSize = file.tellg();
    screen = new BbcScreen(fileSize);

    file.seekg(0, std::ifstream::beg);

    int writeAddr = 0;

    for(;;)
    {
        int readVal = file.get();

        if(readVal == EOF)
        {
            break;
        }

        screen->setScreenByte(writeAddr, readVal);
        writeAddr++;
    }
}

bool Beebview::LoadLdPic(std::ifstream &file)
{
    uint8_t outValBitSize;
    uint8_t mode;
    uint8_t colMapping;
    uint8_t stepSize;
    uint8_t repCountBits;
    uint8_t readMode;
    uint8_t repeatCount;
    uint8_t valToRepeat;

    // Read the number of bits to read for each image byte
    if(!getBitsFromFile(file, 8, true, &outValBitSize))
    {
        return false;
    }

    if(outValBitSize == 0 || outValBitSize > 8)
    {
        // As this is the number of bits to read, it can't be zero, and it can't be
        // larger than eight, as that would overflow the single byte storage.
        return false;
    }

    // Read the mode from the file
    if(!getBitsFromFile(file, 8, false, &mode))
    {
        return false;
    }

    int memSize;

    switch(mode % 8)
    {
        case 0:
        case 1:
        case 2:
            memSize = BV_MEMSIZE012;
            break;
        case 4:
        case 5:
            memSize = BV_MEMSIZE45;
            break;
        default:
            // Modes 3, 6 & 7 are not supported
            return false;
    }

    // Initialise a new BbcScreen instance and set the mode
    screen = new BbcScreen(memSize);
    screen->setMode(mode);

    // Read the colour mappings from the file
    for(int8_t readPal = 15; readPal >= 0; readPal--)
    {
        if(!getBitsFromFile(file, 4, false, &colMapping))
        {
            return false;
        }

        screen->setColour(readPal, colMapping);
    }

    // Read the number of bytes to move forward by after each byte is written to memory
    if(!getBitsFromFile(file, 8, false, &stepSize))
    {
        return false;
    }

    if(stepSize == 0)
    {
        // This can't be zero, as only the first pixel would get written to
        return false;
    }

    // Fetch the number of bits to read for each repeat count
    if(!getBitsFromFile(file, 8, false, &repCountBits))
    {
        return false;
    }

    if(repCountBits == 0 || repCountBits > 8)
    {
        // As this is the number of bits to read, it can't be zero, and it can't be
        // larger than eight, as that would overflow the single byte storage.
        return false;
    }

    // Start in the highest step position and work backwards
    int address = stepSize - 1;
    int progPos = address;

    for(;;)
    {
        // The next bit of the file shows whether to read just a single
        // value, or to read the number of repeats and a value
        if(!getBitFromFile(file, false, &readMode))
        {
            // Unexpected end of file
            return false;
        }

        if(readMode == 0)
        {
            // Single value mode - only 1 repeat
            repeatCount = 1;
        }
        else
        {
            // Fetch the number of times the value should be repeated
            if(!getBitsFromFile(file, repCountBits, false, &repeatCount))
            {
                // Unexpected end of file
                return false;
            }

            if(repeatCount == 0)
            {
                // The value must be repeated at least once
                return false;
            }
        }

        // Now fetch the value itself
        if(!getBitsFromFile(file, outValBitSize, false, &valToRepeat))
        {
            // Unexpected end of file
            return false;
        }

        // Output the value(s) to the file
        while(repeatCount > 0)
        {
            screen->setScreenByte(address, valToRepeat);
            address += stepSize;

            // Reached the end of the address space, wrap around and
            // store the previous step's values now
            if(address >= memSize)
            {
                if(progPos == 0)
                {
                    if(repeatCount > 1)
                    {
                        // Repeats are still remaining but the memory is full
                        return false;
                    }

                    // All of the screen memory has now had data loaded to it
                    return true;
                }

                // Move back to start filling in the next step of values
                progPos--;
                address = progPos;
            }

            repeatCount--;
        }
    }
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

bool Beebview::getBitsFromFile(std::ifstream &file, int numBits, bool flushStore, uint8_t *fileBits)
{
    *fileBits = 0;
    uint8_t addBit;

    // Must be between 1 and 8 bits that have been asked for
    if(numBits < 1 || numBits > 8)
    {
        throw std::invalid_argument("numBits must be between 1 and 8");
    }

    for(int bitCount = 0; bitCount < 8; bitCount++)
    {
        // Shift the bits in the byte one place to the right
        *fileBits = *fileBits >> 1;

        if(bitCount < numBits)
        {
            if(!getBitFromFile(file, flushStore, &addBit))
            {
                // End of file
                return false;
            }

            if(flushStore)
            {
                // The store has now been flushed, so reset the flag
                flushStore = false;
            }

            // Insert the returned bit as the msb of the byte
            *fileBits = *fileBits | addBit << 7;
        }
    }

    return true;
}

bool Beebview::getBitFromFile(std::ifstream &file, bool flushStore, uint8_t *fileBit)
{
    static uint8_t byteStore;
    static int bitsLeft = 0;

    if(flushStore)
    {
        // Clear the count of remaining bits
        bitsLeft = 0;
    }

    if(bitsLeft == 0)
    {
        // Fetch a byte from the file
        int readVal = file.get();

        if(readVal == EOF)
        {
            // End of file
            return false;
        }

        byteStore = readVal;
        bitsLeft = 8;
    }

    // Fetch the leftmost bit
    *fileBit = (byteStore & 128) >> 7;

    // Shift the remaining bits one place left
    byteStore = byteStore << 1;

    // Decrement the bytes left counter
    bitsLeft --;

    return true;
}
