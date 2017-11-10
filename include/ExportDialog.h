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
  See ExportDialog.cpp for more details on what this class and its methods do.  

*/

#ifndef EXPORTDIALOG_H  
#define EXPORTDIALOG_H

#include <QObject>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include "../include/DataInterface.h"

class ExportDialog : public QDialog {
  Q_OBJECT
  
public:
  ExportDialog(QWidget *parent=0, DataInterface *interface = new DataInterface, const QString &relDescription = QString());
  ~ExportDialog() {};

private slots:
  void closeDialog();
  void exportData();
  
private:
  QPointer<DataInterface> dataInterface;
  QPointer<QLabel> informativeLabel;
  QPointer<QLabel> incidentsLabel;
  QPointer<QLabel> incidentIdLabel;
  QPointer<QLabel> incidentPropertiesLabel;
  
  QVector<QPointer<QCheckBox> > incidentPropertiesVector;

  QPointer<QPushButton> closeButton;
  QPointer<QPushButton> exportButton;

  QString description;
};

#endif
