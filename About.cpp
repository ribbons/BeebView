/*
 * Copyright © 2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QAbstractButton>

#include "About.h"
#include "ui_About.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->labelCopyright->setText(ui->labelCopyright->text() + BEEBVIEW_COMMIT_YEAR);
    ui->labelName->setText(ui->labelName->text() + BEEBVIEW_VERSION);

    QObject::connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &About::close);
}

About::~About()
{
    delete ui;
}
