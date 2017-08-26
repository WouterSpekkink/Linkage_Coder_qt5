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
  See ImportCodesDialog.cpp for more details on what this class and its methods do.

*/

#ifndef IMPORTCODESDIALOG_H
#define IMPORTCODESDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QPointer>
#include <vector>
#include <string>

class ImportCodesDialog : public QDialog {
  Q_OBJECT

public:
  ImportCodesDialog(QWidget *parent=0);
  ImportCodesDialog(QWidget *parent=0, QVector<QString> = QVector<QString>());
  ~ImportCodesDialog() {};

  QVector<QString> getColumnNames();
  bool getMarked();				   
			 
private slots:
  
  void selectAndClose();
  
private:
  QPointer<QLabel> informativeLabel;
  QVector<QPointer<QCheckBox> > columns;
  QPointer<QPushButton> selectButton;
  QVector<QString> columnNames;
  QPointer<QLabel> markNewLabel;
  QPointer<QCheckBox> markNew;
  bool marked;
  
};

#endif
  
