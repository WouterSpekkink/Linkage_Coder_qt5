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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with the Linkage Coder Tool.  If not, see <http://www.gnu.org/licenses/>.  
*/

/*
  ===NOTES===
  See MainDialog.cpp for more details on what this class and its methods do.  

 */

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QDateTime>
#include "DataInterface.h"
#include "Logger.h"
#include "CodeDialog.h"
#include "IndexDialog.h"
#include "ExportDialog.h"
#include <vector>
#include <string>

class QPushButton;
class QComboBox;
class QLabel;
class QSpinBox;
class QCheckBox;
class QTextEdit;

class MainDialog : public QDialog {
  Q_OBJECT

public:
  MainDialog(QWidget *parent=0);

signals:
  void writeLog(const QString &entry);
				     
private slots:
  void initializeSeps();
  void getFile();
  void setSep(const QString &selection);
  void setEventColumn(const QString &selection);
  void readNewData();
  void enableOptions();
  void saveSession();
  void loadSession();
  void importCodes();
  void setRelationshipDirection(const QString &selection);
  void checkRelationshipDescription(const QString &text);
  void startCoding();
  void startAssistedCoding();
  void setWorkButtons(const bool status);
  void navigateNextTarget();
  void navigatePreviousTarget();
  void navigateNextSource();
  void navigatePreviousSource();
  void toggleSourceFlag();
  void toggleTargetFlag();
  void navigateNextSourceFlag();
  void navigatePreviousSourceFlag();
  void navigateNextTargetFlag();
  void navigatePreviousTargetFlag();
  void updateTexts();
  void updateIndexIndicators();
  void scrollLeftColForward();
  void scrollLeftColBackward();
  void scrollRightColForward();
  void scrollRightColBackward();
  void setLeftColumn(const QString &selection);
  void setRightColumn(const QString &selection);
  void setLink();
  void unsetLink();
  void processLoad(const QString &sourceIndex,
		   const QString &targetIndex,
		   const QString &colIndex,
		   const QString &relDirection,
		   const QString &relDescription,
		   const QString &separator,
		   const QString &type);
  void finalBusiness();
  void writeLinkages();
  void setMemo(const QString &text);
  void compareCoding();
  void jumpToIndexes();
  void previousLinked();
  void nextLinked();
  void setSourceFilter(const QString &text);
  void setTargetFilter(const QString &text);
  void previousSourceFiltered();
  void nextSourceFiltered();
  void previousTargetFiltered();
  void nextTargetFiltered();
  void findFuturePaths(std::vector<std::vector<bool>::size_type> *pIgnore, std::vector<bool>::size_type currentEvent);
  void findPastPaths(std::vector<std::vector<bool>::size_type> *pIgnore, std::vector<bool>::size_type currentEvent);
  
private:
  QPointer<DataInterface> dataInterface;
  QPointer<CodeDialog> codeComparisonDialog;
  QPointer<Logger> logger;
  QPointer<QLabel> titleLabel;
  QPointer<QLabel> loadLabel;
  QPointer<QLabel> importLabel;
  QPointer<QLabel> lowerLabel;
  QPointer<QLabel> warningSeps;
  QPointer<QLabel> eventsLabelLeft;
  QPointer<QLabel> eventsLabelMiddle;
  QPointer<QLabel> eventsLabelRight;
  QPointer<QLabel> eventSelectorLabel;
  QPointer<QLabel> optionsLabel;
  QPointer<QLabel> relationshipDirLabel;
  QPointer<QLabel> relationshipDescLabel;
  QPointer<QLabel> relationshipInfoLabel;
  QPointer<QLabel> relationshipReporter;
  QPointer<QLabel> linkLabel;
  QPointer<QLabel> leftInfoLabel;
  QPointer<QLabel> rightInfoLabel;
  QPointer<QLabel> exportLabel;
  QPointer<QLabel> selectIDLabel;
  QPointer<QLabel> memoLabel;
  QPointer<QLabel> sourceFlagLabel;
  QPointer<QLabel> targetFlagLabel;
  QPointer<QPushButton> openFile;
  QPointer<QPushButton> startCodingButton;
  QPointer<QPushButton> startAssistedCodingButton;
  QPointer<QPushButton> saveSessionButton;
  QPointer<QPushButton> loadSessionButton;
  QPointer<QPushButton> importCodesButton;
  QPointer<QPushButton> exitButton;
  QPointer<QPushButton> importFile;
  QPointer<QPushButton> nextTargetButton;
  QPointer<QPushButton> prevTargetButton;
  QPointer<QPushButton> setLinkButton;
  QPointer<QPushButton> unsetLinkButton;
  QPointer<QPushButton> nextSourceButton;
  QPointer<QPushButton> prevSourceButton;
  QPointer<QPushButton> toggleSourceFlagButton;
  QPointer<QPushButton> nextSourceFlagButton;
  QPointer<QPushButton> prevSourceFlagButton;
  QPointer<QPushButton> toggleTargetFlagButton;
  QPointer<QPushButton> nextTargetFlagButton;
  QPointer<QPushButton> prevTargetFlagButton;
  QPointer<QPushButton> nextLeftColumnButton;
  QPointer<QPushButton> prevLeftColumnButton;
  QPointer<QPushButton> nextRightColumnButton;
  QPointer<QPushButton> prevRightColumnButton;
  QPointer<QPushButton> writeLinkagesButton;
  QPointer<QPushButton> compareCodingButton;
  QPointer<QPushButton> jumpToIndexesButton;
  QPointer<QPushButton> previousLinkedButton;
  QPointer<QPushButton> nextLinkedButton;
  QPointer<QPushButton> previousSourceFilterButton;
  QPointer<QPushButton> nextSourceFilterButton;
  QPointer<QPushButton> previousTargetFilterButton;
  QPointer<QPushButton> nextTargetFilterButton;
  QPointer<QComboBox> sepSelector;
  QPointer<QComboBox> eventSelector;
  QPointer<QComboBox> relationshipDirSelector;
  QPointer<QComboBox> leftSelector;
  QPointer<QComboBox> rightSelector;
  QPointer<QTextEdit> sourceEvent;
  QPointer<QTextEdit> targetEvent;
  QPointer<QTextEdit> otherColLeft;
  QPointer<QTextEdit> otherColRight;
  QPointer<QLineEdit> memoText;
  QPointer<QLineEdit> relationshipDescriber;
  QPointer<QLineEdit> sourceFilterField;
  QPointer<QLineEdit> targetFilterField;
  QString fileName;
  QString selectedEventColumn;
  QString sep;
  QString relationshipDirection;
  QString relationshipDescription;
  QString currentSourceFilter;
  QString currentTargetFilter;
  QString codingType;
  static const QString SEPDEFAULT;
  static const QString EVENTDEFAULT;
  static const QString RELDEFAULT;
  static const QString RELPAST;
  static const QString RELFUTURE;
  static const QString MANUAL;
  static const QString ASSISTED;
  
  std::vector<std::string>::size_type columnIndex;
  std::vector<std::string>::size_type leftColumnIndex;
  std::vector<std::string>::size_type rightColumnIndex;
  std::vector<std::vector <std::string> >::size_type sourceRowIndex;
  std::vector<std::vector <std::string> >::size_type targetRowIndex;
};

#endif
