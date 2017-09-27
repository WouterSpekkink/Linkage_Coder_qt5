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
  This dialog will be used for the user to input two indexes, which the program will
  then attempt to jump to.
*/

#include "../include/IndexDialog.h"
#include <QIntValidator>
#include <sstream>

// Initialize some constants
const QString IndexDialog::RELPAST = QString("Present to past");
const QString IndexDialog::RELFUTURE = QString("Past to present");

IndexDialog::IndexDialog(QWidget *parent, std::vector <std::vector <std::string> >::size_type size, const QString direction) : QDialog(parent) {
  // First we construct our dialog's entities.
  submittedDir = direction;
  informativeLabel = new QLabel(tr("Select the source and target index to jump to:"));
  sourceLabel = new QLabel(tr("Source:"));
  targetLabel = new QLabel(tr("Target:"));
  sourceText = "";
  targetText = "";
  sourceField = new QLineEdit();
  targetField = new QLineEdit();
  max = 1;
  submittedSize = size;
  sourceField->setValidator(new QIntValidator(1, submittedSize, this));
  targetField->setValidator(new QIntValidator(1, 1, this));
  goButton = new QPushButton(tr("Go"));
  goButton->setEnabled(false); // We only activate this button if there is something in the fields.
  cancelButton = new QPushButton(tr("Cancel"));

  // We connect the signals to the appropriate slots.
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(sourceField, SIGNAL(textChanged(const QString &)), this, SLOT(setSourceText(const QString &)));
  connect(targetField, SIGNAL(textChanged(const QString &)), this, SLOT(setTargetText(const QString &)));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> labelLayout = new QHBoxLayout;
  labelLayout->addWidget(informativeLabel);

  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  fieldLayout->addWidget(sourceLabel);
  fieldLayout->addWidget(sourceField);
  fieldLayout->addWidget(targetLabel);
  fieldLayout->addWidget(targetField);

  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(goButton);
  buttonLayout->addWidget(cancelButton);
  
  mainLayout->addLayout(labelLayout);
  mainLayout->addLayout(fieldLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Jump to index"));
  setFixedHeight(sizeHint().height());
  // And this finished the constructor.
}

void IndexDialog::setSourceText(const QString newSource) {
  sourceText = newSource.toStdString();
  // Let us immediately check if we should active the go Button.
  if (submittedDir == RELPAST) {
    std::istringstream ss(sourceText);
    ss >> max;
    targetField->setValidator(new QIntValidator(1, max, this));
  } else if (submittedDir == RELFUTURE) {
    std::istringstream ss(sourceText);
    ss >> max;
    targetField->setValidator(new QIntValidator(1, submittedSize - max + 1, this));
  }
  evaluateIndexes();
}

void IndexDialog::setTargetText(const QString newTarget) {
  targetText = newTarget.toStdString();
  // Let us immediately check if we should active the go Button.
  evaluateIndexes();
}

void IndexDialog::evaluateIndexes() {
  if (sourceText != "" && targetText != "") {
    goButton->setEnabled(true);
  } else {
    goButton->setEnabled(false);
  }
}

void IndexDialog::go() {
  this->close();
}

void IndexDialog::cancel() {
  sourceText = "0";
  targetText = "0";
  this->close();
}

std::string IndexDialog::getSourceText() {
  return sourceText;
}

std::string IndexDialog::getTargetText() {
  return targetText;
}
