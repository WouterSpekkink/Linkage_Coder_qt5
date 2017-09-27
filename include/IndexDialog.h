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
  See IndexDialog.cpp for more details on what this class and its methods do.

*/

#ifndef INDEXDIALOG_H
#define INDEXDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QPointer>
#include <string>

class IndexDialog : public QDialog {
  friend class MainDialog;
  Q_OBJECT

public:
  IndexDialog(QWidget *parent = 0, std::vector <std::vector <std::string> >::size_type size = 0, const QString direction = "EMPTY");
  ~IndexDialog() {};

private slots:
  void go();
  void cancel();
  void setSourceText(const QString newSource);
  void setTargetText(const QString newTarget);
  void evaluateIndexes();
  std::string getSourceText();
  std::string getTargetText();

private:
  QPointer<QLabel> informativeLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> targetLabel;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> targetField;
  QPointer<QPushButton> goButton;
  QPointer<QPushButton> cancelButton;
  std::string sourceText;
  std::string targetText;

  QString submittedDir;
  
  std::vector <std::vector <std::string> >::size_type max;
  std::vector <std::vector <std::string> >::size_type submittedSize;
  
  static const QString RELPAST;
  static const QString RELFUTURE;
  
};

#endif
