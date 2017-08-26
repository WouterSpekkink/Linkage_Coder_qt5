/*
 Copyright 2017 Wouter Spekkink 
 Authors : Wouter Spekkink <wouterspekkink@gmail.com> 
 Website : http://www.wouterspekkink.org

 This file is part of the Linkage Coder Tool.

 DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.

 Copyright 2017 Wouter Spekkink. All rights reserved.

 The Linkage Coder Tool is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 The Linkage Coder Tool is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with the Linkage Coder Tool.  If not, see <http://www.gnu.org/licenses/>.

*/ 

/*
  ===NOTES===
  This dialog is shown when the user selects an older file to import codes from.
  The dialog is used only to allow the user to select which columns should match between 
  the old file, and the file currently being worked on. If the data in the selected columns
  match, then the program assumes that the corresponding rows of data are identical.

*/

#include "../include/ImportCodesDialog.h"
#include <vector>
#include <string>
#include <QPointer>

// This is the default constructor, which I don't actually use.
ImportCodesDialog::ImportCodesDialog(QWidget *parent) : QDialog(parent) {}

// This is the main constructor
ImportCodesDialog::ImportCodesDialog(QWidget *parent, const QVector<QString> Qheader) : QDialog(parent) {

  /* 
     First we create this dialog's entities.
     There will be just one label with text, and one button to exit.
     The rest will be dynamically created tick boxes, the number of which depends on the number of columns.
  */
  informativeLabel = new QLabel(tr("Select the columns that should match:"));
  selectButton = new QPushButton(tr("Select and Close"));
  markNewLabel = new QLabel(tr("Check box below to mark new dataset entries:"));
  markNew = new QCheckBox(tr("Mark new entries"));
  marked = false;
  
  // We use this to create our checkboxes, based on the number of common columns in the vector passed to the function.
  QVectorIterator<QString> it(Qheader);
  while (it.hasNext()) {
    QPointer<QCheckBox> tempBox = new QCheckBox(it.next(), this);
    columns.push_back(tempBox);
  }

  // Now we set up the signal for the one button we have.
  connect(selectButton, SIGNAL(clicked()), this, SLOT(selectAndClose()));

  // Now we create our layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(informativeLabel);
  QVectorIterator<QPointer<QCheckBox> > cI(columns);
  while (cI.hasNext()) {
    mainLayout->addWidget(cI.next());
  }
  QPointer<QFrame> horizontalSep = new QFrame();
  horizontalSep->setFrameShape(QFrame::HLine);

  mainLayout->addWidget(horizontalSep);
  mainLayout->addWidget(markNewLabel);
  mainLayout->addWidget(markNew);
  mainLayout->addWidget(selectButton);

  setLayout(mainLayout);
  setWindowTitle(tr("Set matching columns"));
  setFixedHeight(sizeHint().height());
  // The constructor is now finished.
}

// This is the function triggered by clicking the only button of the dialog.
void ImportCodesDialog::selectAndClose() {
  // We simply push the names of the checked boxes to a vector that we use for storage.
  QVectorIterator<QPointer<QCheckBox> > cI(columns);
  cI.toFront();
  while (cI.hasNext()) {
    QPointer<QCheckBox> tempBox = cI.next();
    if (tempBox->checkState() == Qt::Checked) {
      columnNames.push_back(tempBox->text());
    }
  }
  if (markNew->checkState() == Qt::Checked) {
    marked = true;
  }
  // Then we clean up and close the dialog.
  qDeleteAll(columns);
  columns.clear();
  this->close();
}

// This is to recover the stored column names after closure of the dialog.
QVector<QString> ImportCodesDialog::getColumnNames() {
  return columnNames;
}

bool ImportCodesDialog::getMarked() {
  return marked;
}



