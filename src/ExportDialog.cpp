/*
 Copyright 2017 Wouter Spekkink 
 Authors : Wouter Spekkink <wouterspekkink@gmail.com> 
 Website : http://www.wouterspekkink.org

 This file is part of the Event Coder Tool.

 DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.

 Copyright 2017 Wouter Spekkink. All rights reserved.

 The Event Coder Tool is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 The Event Coder Tool is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with the Event Coder Tool.  If not, see <http://www.gnu.org/licenses/>.

*/ 

/*
  ===NOTES===
  This dialog will be used by the user to export data to output files.

*/

#include "../include/ExportDialog.h"


ExportDialog::ExportDialog(QWidget *parent, DataInterface *interface, const QString &relDescription)
  : QDialog(parent) {
  // First we initialise variables en set up the dialog's entities.
  dataInterface = interface;
  description = relDescription;
  informativeLabel = new QLabel(tr("<h3>Export options</h3>"));
  incidentsLabel = new QLabel(tr("<h4>Incident export options:</h4>"));

  QVector<QString> Qheader;
  std::vector<std::string>::iterator it;
  for (it = dataInterface->header.begin(); it != dataInterface->header.end(); it++) {
    std::string tempString = *it;
    Qheader.push_back(QString::fromStdString(tempString));
  }
  
  incidentPropertiesLabel = new QLabel(tr("Select incident properties:"));

  QVectorIterator<QString> it2(Qheader);
  while (it2.hasNext()) {
    QPointer<QCheckBox> tempBox = new QCheckBox(it2.next(), this);
    incidentPropertiesVector.push_back(tempBox);
  }
  
  closeButton = new QPushButton(tr("Close"));
  exportButton = new QPushButton(tr("Export Data"));
  
  // Then we connect the signals
  connect(exportButton, SIGNAL(clicked()), this, SLOT(exportData()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));

  // Then we create the layout
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(informativeLabel);
  informativeLabel->setAlignment(Qt::AlignHCenter);

  QPointer<QFrame> topLine = new QFrame();
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  mainLayout->addWidget(incidentsLabel);
  incidentsLabel->setAlignment(Qt::AlignHCenter);
  mainLayout->addWidget(incidentPropertiesLabel);
  
  QVectorIterator<QPointer<QCheckBox> > cI(incidentPropertiesVector);
  while (cI.hasNext()) {
    mainLayout->addWidget(cI.next());
  }
  
  QPointer<QFrame> bottomLine = new QFrame();
  bottomLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(bottomLine);

  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(closeButton);
  optionsLayout->addWidget(exportButton);
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Export dialog");
  setFixedHeight(sizeHint().height());
  // And this finishes the constructor
}

void ExportDialog::closeDialog() {
  qDeleteAll(incidentPropertiesVector);
  incidentPropertiesVector.clear();
  this->close();
}

void ExportDialog::exportData() {
  QVector<QString> incidentProperties;
  QVectorIterator<QPointer<QCheckBox> > cI(incidentPropertiesVector);
  while (cI.hasNext()) {
    QPointer<QCheckBox> tempBox = cI.next();
    if (tempBox->checkState() == Qt::Checked) {
      incidentProperties.push_back(tempBox->text());
    }
  }
  dataInterface->writeLinkages(description, incidentProperties);
  this->close();
}


