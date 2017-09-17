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
  ==NOTES==
  The main purpose of this class is to provide the user with a dialog, through which
  the user can interact with the program.
 */


#include <QtWidgets/QWidget>
#include "../include/MainDialog.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <sstream>

// Initializing some constants
const QString MainDialog::SEPDEFAULT = QString("-Select delimiter-");
const QString MainDialog::EVENTDEFAULT = QString("-Select column-");
const QString MainDialog::RELDEFAULT = QString("-Select direction-");
const QString MainDialog::RELPAST = QString("Present to past");
const QString MainDialog::RELFUTURE = QString("Past to present");

// This is the constructor for this class
MainDialog::MainDialog(QWidget *parent) : QDialog(parent) {
  /* 
     DataInterface is a class that interfaces with the data that is supplied 
     by the user via an imported csv-file, or by loading a save file that the 
     user created earlier. The DataInterface also handles the creation of new files
     based on the activities of the user. The main dialog owns an instance of 
     DataInterface, and let's it do all the heavy lifting int erms of reading, 
     storing and writing data useful to the user.
  */
  dataInterface = new DataInterface();
  /*
    Logger is a class that only stores logs, and writes them to the disk as soon
    as the user closes the program.
   */
  logger = new Logger();
  titleLabel = new QLabel(tr("<h3>Import new data</h3>")); // As instruction to user what to do here.
  titleLabel->setAlignment(Qt::AlignHCenter);
  openFile = new QPushButton(tr("Select file")); // Button to select filename
  sepSelector = new QComboBox(this); // Combobox to select delimiter for columns.
  sepSelector->setEnabled(false); // This is initially disabled.
  initializeSeps();
  sep = SEPDEFAULT;
  importFile = new QPushButton(tr("Import data from file")); // Button to start importing files.
  importFile->setEnabled(false); // This is initially disabled.
  optionsLabel = new QLabel(tr("<h3>Saving, loading and exit</h3>"));
  saveSessionButton = new QPushButton(tr("Save current session"));
  saveSessionButton->setEnabled(false); // This is initially disabled.
  loadSessionButton = new QPushButton(tr("Load previous session")); // Button to load previous session.
  importCodesButton = new QPushButton(tr("Import existing codes")); // Button to import codes from older version of dataset.
  importCodesButton->setEnabled(false); // This is initially disabled.
  exitButton = new QPushButton(tr("Exit program")); // The exit button
  exportLabel = new QLabel(tr("<h3>Export coded linkages</h3>"));
  selectIDLabel = new QLabel(tr("Select an ID column"));
  writeLinkagesButton = new QPushButton(tr("Export linkages"));
  writeLinkagesButton->setEnabled(false);
  compareCodingButton = new QPushButton(tr("Compare coding"));
  eventSelectorLabel = new QLabel(tr("Column with incident descriptions:"));
  eventSelector = new QComboBox(this); // Combobox to select the column with the event descriptions.
  eventSelector->addItem(EVENTDEFAULT); // The other options will be added later.
  eventSelector->setEnabled(false); // This is initially disabled.
  selectedEventColumn = EVENTDEFAULT;
  relationshipDirLabel =  new QLabel(tr("Relationship direction:"));
  relationshipDirSelector = new QComboBox(this);
  relationshipDirSelector->addItem(RELDEFAULT);
  relationshipDirSelector->addItem(RELPAST);
  relationshipDirSelector->addItem(RELFUTURE);
  relationshipDirSelector->setEnabled(false);
  relationshipDirection = RELDEFAULT;
  relationshipDescLabel = new QLabel(tr("Relationship description:"));
  relationshipDescriber = new QLineEdit();
  relationshipDescriber->setMaxLength(15);
  relationshipDescriber->setEnabled(false);
  relationshipDescription = "";
  relationshipInfoLabel = new QLabel(tr("<b>Relationship type:<b>"));
  relationshipReporter = new QLabel("");
  startCodingButton = new QPushButton(tr("Start coding")); // Selected if user starts new session.
  startCodingButton->setEnabled(false); // This is initially disabled.
  eventsLabelLeft = new QLabel(tr("<b>Source<b>"));
  eventsLabelMiddle = new QLabel(tr("<b>Incidents to be linked<b>"));
  eventsLabelRight = new QLabel(tr("<b>Target<b>"));
  sourceFlagLabel = new QLabel("");
  toggleSourceFlagButton = new QPushButton("Toggle flag for source");
  nextSourceFlagButton = new QPushButton("Next flagged source");
  prevSourceFlagButton = new QPushButton("Previous flagged source");
  toggleSourceFlagButton->setEnabled(false);
  nextSourceFlagButton->setEnabled(false);
  prevSourceFlagButton->setEnabled(false);
  targetFlagLabel = new QLabel("");
  toggleTargetFlagButton = new QPushButton("Toggle flag for target");
  nextTargetFlagButton = new QPushButton("Next flagged target");
  prevTargetFlagButton = new QPushButton("Previous flagged target");
  toggleTargetFlagButton->setEnabled(false);
  nextTargetFlagButton->setEnabled(false);
  prevTargetFlagButton->setEnabled(false);
  sourceEvent = new QTextEdit(); // This will have the description of the source event.
  sourceEvent->setReadOnly(true); // We only want to read text, and not edit it.
  targetEvent = new QTextEdit(); // This will have the description of the target event.
  targetEvent->setReadOnly(true); // We only want to read text, and not edit it.
  memoLabel = new QLabel(tr("Memo:"));
  memoText = new QLineEdit();
  otherColLeft = new QTextEdit();
  otherColLeft->setReadOnly(true);
  otherColRight = new QTextEdit();
  otherColRight->setReadOnly(true);
  nextTargetButton = new QPushButton(tr("Next target"));
  prevTargetButton = new QPushButton(tr("Previous target"));
  toggleLinkButton = new QPushButton(tr("Toggle Link"));
  linkLabel = new QLabel(tr(""));
  nextSourceButton = new QPushButton(tr("Next Source"));
  prevSourceButton = new QPushButton(tr("Previous Source"));
  leftInfoLabel = new QLabel(tr("<b>Left additional column:<b>"));
  rightInfoLabel = new QLabel(tr("<b>Right additional column:<b>"));
  nextLeftColumnButton = new QPushButton(tr("Next field"));
  prevLeftColumnButton = new QPushButton(tr("Previous field"));
  nextRightColumnButton = new QPushButton(tr("Next field"));
  prevRightColumnButton = new QPushButton(tr("Previous field"));
  leftSelector= new QComboBox();
  leftSelector->addItem("");
  rightSelector= new QComboBox();
  rightSelector->addItem("");
  jumpToIndexesButton = new QPushButton(tr("Jump to..."));
  jumpToIndexesButton->setEnabled(false);
  
  setWorkButtons(false);

  /* 
     Here all the signals through which the main dialog communicates with itself and with other classes.
     The first one calls a function that opens a dialog to select an input file.
  */
  connect(openFile, SIGNAL(clicked()), this, SLOT(getFile()));
  /*
    This handles any changes in the selection of the column delimiter.
    It calls a function that sets the delimiter to what was chosen by the user.
  */
  connect(sepSelector, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setSep(const QString &)));
  /* 
     This signal is fired if the user clicks the import button. 
     It calls a function of the main dialog that determines what function to
     call from the instantiated DataInterface Classe. Once this is determined, 
     one of the next two signals is fired. These pass the  necessary information 
     to the instantiated DataInterface class and tell it to import the file.  
  */
  connect(importFile, SIGNAL(clicked()), this, SLOT(readNewData()));
  // After the import of a new file is finished, some new options become available.
  connect(dataInterface, SIGNAL(importFinished()), this, SLOT(enableOptions()));
  // A function to import codes from an older version of the dataset being coded.
  connect(importCodesButton, SIGNAL(clicked()), this, SLOT(importCodes()));
  // The functions that process loaded data.
  connect(dataInterface, SIGNAL(loadFinished(const QString &, const QString &, const QString &,
					     const QString &, const QString &, const QString &)),
	  this, SLOT(processLoad(const QString &, const QString &, const QString &,
				 const QString &, const QString &, const QString &)));
  connect(saveSessionButton, SIGNAL(clicked()), this, SLOT(saveSession()));
  // The load session button signal to load a previous session.
  connect(loadSessionButton, SIGNAL(clicked()), this, SLOT(loadSession()));
  // The exit button signal to close the program.
  connect(eventSelector, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setEventColumn(const QString &)));
  connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
  /* 
     The direction of the relationship selected by the user should influence the
     behaviour of the program.
  */
  connect(relationshipDirSelector, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setRelationshipDirection(const QString &)));
  /* 
     The following signal processes the relationship description offered by
     the user.
  */
  connect(relationshipDescriber, SIGNAL(textChanged(const QString &)),
	  this, SLOT(checkRelationshipDescription(const QString &)));
  // Clicking the startCodingButton should set in motion the process of coding.
  connect(startCodingButton, SIGNAL(clicked()), this, SLOT(startCoding()));
  // This button navigates to the next target event.
  connect(nextTargetButton, SIGNAL(clicked()), this, SLOT(navigateNextTarget()));
  // And this button navigates to the previous target.
  connect(prevTargetButton, SIGNAL(clicked()), this, SLOT(navigatePreviousTarget()));
  // This button navigates to the next source event.
  connect(nextSourceButton, SIGNAL(clicked()), this, SLOT(navigateNextSource()));
  // And this button navigates to the previous source.
  connect(prevSourceButton, SIGNAL(clicked()), this, SLOT(navigatePreviousSource()));
  // The next four buttons allow the user to scroll through other fields.
  connect(nextLeftColumnButton, SIGNAL(clicked()), this, SLOT(scrollLeftColForward()));
  connect(prevLeftColumnButton, SIGNAL(clicked()), this, SLOT(scrollLeftColBackward()));
  connect(nextRightColumnButton, SIGNAL(clicked()), this, SLOT(scrollRightColForward()));
  connect(prevRightColumnButton, SIGNAL(clicked()), this, SLOT(scrollRightColBackward()));
  // The next two comboboxes do the same, but in another way.
  connect(leftSelector, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setLeftColumn(const QString &)));
  connect(rightSelector, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setRightColumn(const QString &)));
  // The next button toggles the link for the current pair of events.
  connect(toggleLinkButton, SIGNAL(clicked()), this, SLOT(toggleLink()));
  // We need to do some final business before the program exits.
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
  // The next button starts the process for writing linkages to a file.
  connect(writeLinkagesButton, SIGNAL(clicked()), this, SLOT(writeLinkages()));
  // The next button opens a separate dialog where the user can compare codings from two sessions.
  connect(compareCodingButton, SIGNAL(clicked()), this, SLOT(compareCoding()));
  // The next couple of buttons deal with flagging events and navigating flagged events.
  connect(toggleSourceFlagButton, SIGNAL(clicked()), this, SLOT(toggleSourceFlag()));
  connect(nextSourceFlagButton, SIGNAL(clicked()), this, SLOT(navigateNextSourceFlag()));
  connect(prevSourceFlagButton, SIGNAL(clicked()), this, SLOT(navigatePreviousSourceFlag()));
  connect(toggleTargetFlagButton, SIGNAL(clicked()), this, SLOT(toggleTargetFlag()));
  connect(nextTargetFlagButton, SIGNAL(clicked()), this, SLOT(navigateNextTargetFlag()));
  connect(prevTargetFlagButton, SIGNAL(clicked()), this, SLOT(navigatePreviousTargetFlag()));
  // The next button allows the user to jump directly to some user-selected indexes.
  connect(jumpToIndexesButton, SIGNAL(clicked()), this, SLOT(jumpToIndexes()));
  // The next signal fires when the user changes something in the memo field.
  connect(memoText, SIGNAL(textChanged(const QString &)), 
	  this, SLOT(setMemo(const QString &)));
  /*
    Below the layout is created, which consists out of several building blocks.
    All the buttons and other widgets are layed out here.
  */
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> topLeftLayout = new QVBoxLayout();
  topLeftLayout->addWidget(titleLabel);
  topLeftLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
  //topLeftLayout->setContentsMargins(150,0,150,0);

  QPointer<QHBoxLayout> topLeftLayoutOne = new QHBoxLayout;
  topLeftLayoutOne->addWidget(openFile);
  topLeftLayoutOne->addWidget(sepSelector);
  topLeftLayoutOne->setSpacing(10);
  QPointer<QHBoxLayout>topLeftLayoutTwo = new QHBoxLayout;
  topLeftLayoutTwo->addWidget(importFile);
  QPointer<QHBoxLayout>topLeftLayoutThree = new QHBoxLayout;
  topLeftLayoutThree->addWidget(importCodesButton);
  topLeftLayout->addLayout(topLeftLayoutOne);
  topLeftLayout->addLayout(topLeftLayoutTwo);
  topLeftLayout->addLayout(topLeftLayoutThree);
  QPointer<QVBoxLayout> topMiddleLayout = new QVBoxLayout;
  topMiddleLayout->addWidget(optionsLabel);
  topMiddleLayout->addWidget(saveSessionButton);
  topMiddleLayout->addWidget(loadSessionButton);
  topMiddleLayout->addWidget(exitButton);
  topMiddleLayout->setAlignment(Qt::AlignHCenter);
  topMiddleLayout->setContentsMargins(150,0,150,0);
  QPointer<QVBoxLayout> topRightLayout = new QVBoxLayout;
  topRightLayout->addWidget(exportLabel);
  exportLabel->setAlignment(Qt::AlignHCenter);
  topRightLayout->addWidget(writeLinkagesButton);
  topRightLayout->addWidget(compareCodingButton);
  topRightLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
  topLayout->addLayout(topLeftLayout);
  topLayout->addLayout(topMiddleLayout);
  topLayout->addLayout(topRightLayout);
    
  QPointer<QHBoxLayout> optionsLayoutTop = new QHBoxLayout;
  optionsLayoutTop->addWidget(eventSelectorLabel);
  optionsLayoutTop->addWidget(eventSelector);
  optionsLayoutTop->addWidget(relationshipDirLabel);
  optionsLayoutTop->addWidget(relationshipDirSelector);
  optionsLayoutTop->addWidget(relationshipDescLabel);
  optionsLayoutTop->addWidget(relationshipDescriber);
  optionsLayoutTop->addWidget(startCodingButton);
  optionsLayoutTop->setAlignment(Qt::AlignLeft);
  QPointer<QHBoxLayout> textFieldsLabelLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> tFLabelLeft = new QHBoxLayout;
  tFLabelLeft->addWidget(prevSourceButton);
  tFLabelLeft->addWidget(eventsLabelLeft);
  eventsLabelLeft->setFixedWidth(120);
  eventsLabelLeft->setAlignment(Qt::AlignCenter);
  tFLabelLeft->addWidget(sourceFlagLabel);
  sourceFlagLabel->setFixedWidth(20);
  tFLabelLeft->addWidget(nextSourceButton);
  tFLabelLeft->setAlignment(Qt::AlignHCenter);
  QPointer<QHBoxLayout> tFLabelMid = new QHBoxLayout;
  tFLabelMid->addWidget(eventsLabelMiddle);
  tFLabelMid->setAlignment(Qt::AlignHCenter);
  QPointer<QHBoxLayout> tFLabelRight = new QHBoxLayout;
  tFLabelRight->addWidget(prevTargetButton);
  tFLabelRight->addWidget(eventsLabelRight);
  eventsLabelRight->setFixedWidth(120);
  eventsLabelRight->setAlignment(Qt::AlignCenter);
  tFLabelRight->addWidget(targetFlagLabel);
  targetFlagLabel->setFixedWidth(20);
  tFLabelRight->addWidget(nextTargetButton);
  tFLabelRight->setAlignment(Qt::AlignHCenter);
  textFieldsLabelLayout->addLayout(tFLabelLeft);
  textFieldsLabelLayout->addLayout(tFLabelMid);
  textFieldsLabelLayout->addLayout(tFLabelRight);

  QPointer<QHBoxLayout> textFieldsTopLayout = new QHBoxLayout; 
  textFieldsTopLayout->addWidget(sourceEvent);
  QPointer<QVBoxLayout> linkButtonLayout = new QVBoxLayout;
  linkButtonLayout->addWidget(relationshipInfoLabel);
  linkButtonLayout->addWidget(relationshipReporter);
  relationshipReporter->setAlignment(Qt::AlignHCenter);
  relationshipReporter->setStyleSheet("color: red");
  linkButtonLayout->addWidget(linkLabel);
  linkLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  linkLabel->setStyleSheet("color: blue");
  linkButtonLayout->addWidget(toggleLinkButton);
  linkButtonLayout->addWidget(jumpToIndexesButton);
  linkButtonLayout->setContentsMargins(0,10,0,10);
  linkButtonLayout->setAlignment(Qt::AlignHCenter);
  textFieldsTopLayout->addLayout(linkButtonLayout);
  textFieldsTopLayout->addWidget(targetEvent);

  QPointer<QHBoxLayout> flaggingLayout = new QHBoxLayout;
  flaggingLayout->addWidget(toggleSourceFlagButton);
  flaggingLayout->addWidget(prevSourceFlagButton);
  flaggingLayout->addWidget(nextSourceFlagButton);
  QPointer<QFrame> flaggingSep = new QFrame();
  flaggingSep->setFrameShape(QFrame::HLine);
  flaggingLayout->addWidget(flaggingSep);
  flaggingLayout->addWidget(toggleTargetFlagButton);
  flaggingLayout->addWidget(prevTargetFlagButton);
  flaggingLayout->addWidget(nextTargetFlagButton);
  
  QPointer<QHBoxLayout> textFieldsMiddleLayout = new QHBoxLayout;
  textFieldsMiddleLayout->addWidget(memoLabel);
  textFieldsMiddleLayout->addWidget(memoText);

  QPointer<QHBoxLayout> textFieldsBottomLayout = new QHBoxLayout;
  textFieldsBottomLayout->addWidget(otherColLeft);
  QPointer<QVBoxLayout> extraButtonLayout = new QVBoxLayout;
  extraButtonLayout->addWidget(leftInfoLabel);
  leftInfoLabel->setAlignment(Qt::AlignHCenter);
  extraButtonLayout->addWidget(leftSelector);
  extraButtonLayout->addWidget(nextLeftColumnButton);
  extraButtonLayout->addWidget(prevLeftColumnButton);
  leftSelector->setEditable(true);
  leftSelector->lineEdit()->setAlignment(Qt::AlignHCenter);
  leftSelector->lineEdit()->setReadOnly(true);
  extraButtonLayout->addWidget(rightInfoLabel);
  rightInfoLabel->setAlignment(Qt::AlignHCenter);
  extraButtonLayout->addWidget(rightSelector);
  extraButtonLayout->addWidget(nextRightColumnButton);
  extraButtonLayout->addWidget(prevRightColumnButton);

  rightSelector->setEditable(true);
  rightSelector->lineEdit()->setAlignment(Qt::AlignHCenter);
  rightSelector->lineEdit()->setReadOnly(true);
  extraButtonLayout->setAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
  textFieldsBottomLayout->addLayout(extraButtonLayout);
  textFieldsBottomLayout->addWidget(otherColRight);

  QPointer<QVBoxLayout> workTextLayout = new QVBoxLayout;
  workTextLayout->addLayout(optionsLayoutTop);
  QPointer<QFrame> optionsSeparator = new QFrame();
  optionsSeparator->setFrameShape(QFrame::HLine);
  workTextLayout->addWidget(optionsSeparator);
  workTextLayout->addLayout(textFieldsLabelLayout);
  workTextLayout->addLayout(textFieldsTopLayout);
  QPointer<QFrame> textFieldsHorizontalSep = new QFrame();
  textFieldsHorizontalSep->setFrameShape(QFrame::HLine);
  QPointer<QFrame> textFieldsHorizontalSepTwo = new QFrame();
  textFieldsHorizontalSepTwo->setFrameShape(QFrame::HLine);
  workTextLayout->addLayout(flaggingLayout);
  workTextLayout->addWidget(textFieldsHorizontalSep);
  workTextLayout->addLayout(textFieldsMiddleLayout);
  workTextLayout->addWidget(textFieldsHorizontalSepTwo);
  workTextLayout->addLayout(textFieldsBottomLayout);
      
  QPointer<QFrame> topLine = new QFrame();
  topLine->setFrameShape(QFrame::HLine);
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout; 
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(topLine);
  mainLayout->addLayout(workTextLayout);
  setLayout(mainLayout);
  setWindowTitle(tr("Linkage Coder"));
  resize(1200,800);
  // And that finishes the constructor.
}

// Private slots are listed below.

// I am reusing this a couple of times, so it makes sense to write a function for it.
void MainDialog::initializeSeps() {
  sepSelector->clear();
  sepSelector->addItem(SEPDEFAULT);
  sepSelector->addItem(",");
  sepSelector->addItem(";");
  sepSelector->addItem(":");
  sepSelector->addItem("|");
}
  
// This gets the filename, using a dialog that requires the user to select a file.
void MainDialog::getFile() {
  if (saveSessionButton->isEnabled() == true) { // Perhaps unclear, but this is only possible if a session is currently running.
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->addButton(QMessageBox::Cancel);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("WARNING: session in progress!");
    warningBox->setInformativeText("You will lose all progress in the current session if you load a new file without saving first.");
    if (warningBox->exec() == QMessageBox::Ok) {
      QString file = QFileDialog::getOpenFileName(this, tr("Open File"),"","Comma Delimited Files(*.csv)");
      if(!file.trimmed().isEmpty()) {
	fileName = file;
	initializeSeps();
	sepSelector->setEnabled(true); // We want the user to use this option only after selecting a file.
      } else {
	sepSelector->setEnabled(false);
      }
    }
  } else {
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),"","Comma Delimited Files(*.csv)");
    if(!file.trimmed().isEmpty()) {
      fileName = file;
      initializeSeps();
      sepSelector->setEnabled(true); // We want the user to use this option only after selecting a file.
    } else {
      sepSelector->setEnabled(false);
    }
  }
}

/* This function sets the delimiter with which columns in the input file are delimited.
   The program cannot distinguish between columns and multi-value cells if the delimiters 
   for both are the same. Therefore, the options for the multi-value delimiters always
   exclude the column delimiter that is selected. The options for the multi-value 
   delimiter are reset each time the user sets another column delimiter.

   We also reset other functions and interface options for transparency reasons.
*/
void MainDialog::setSep(const QString &selection) {
  sep = selection;
  if (sep != SEPDEFAULT) {
    importFile->setEnabled(true);
  } else {
    importFile->setEnabled(false);
  }
}

/* 
   This function fires a signal to request the instantiated DataInterface Class
   to start importing the selected file.
*/

void MainDialog::readNewData() {
  sourceRowIndex = 0;
  targetRowIndex = 0;
  columnIndex = 0;
  leftColumnIndex = 0;
  rightColumnIndex = 0;
  dataInterface->readFile(fileName, sep);
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "imported new data from " + fileName;
  logger->addToLog(newLog);
  sourceEvent->setText("");
  targetEvent->setText("");
  otherColLeft->setText("");
  otherColRight->setText("");
  memoText->setText("");
  relationshipDescriber->setText("");
}

/* 
   This function enables the part of the interface where the nodes and edges configuration is set.
   It also retrieves the possible options for source and target nodes from the DataInterface. 
*/
void MainDialog::enableOptions() {
  /* 
     We should get the list of column headers and use this list to populate the combo box that is used
     to identify the column with event descriptions.
  */
  std::vector<std::string>::iterator it;
  eventSelector->clear();
  eventSelector->addItem(EVENTDEFAULT);
  leftSelector->clear();
  rightSelector->clear();
  for (it = dataInterface->header.begin(); it != dataInterface->header.end(); it++) {
    QString currentHeader = QString::fromUtf8(it->c_str());
    eventSelector->addItem(currentHeader);
    leftSelector->addItem(currentHeader);
    rightSelector->addItem(currentHeader);
  }
  sepSelector->setEnabled(false);
  importFile->setEnabled(false);
  eventSelector->setEnabled(true);
  startCodingButton->setEnabled(false);
  setWorkButtons(false);
  sourceEvent->setText("");
  targetEvent->setText("");
  linkLabel->setText("");
  eventsLabelLeft->setText("<b>Source<b>");
  eventsLabelRight->setText("<b>Target<b>");
  relationshipReporter->setText("");
  relationshipDescriber->clear();
}

// This function will write the currently processed data into a file that can be loaded later.
void MainDialog::saveSession() {
  // First we will ask for the desired file name.
  QString QsaveFile = QFileDialog::getSaveFileName(this, tr("Save File"),"", tr("sav files (*.sav)"));
  if (!QsaveFile.trimmed().isEmpty()) {
    if (!QsaveFile.endsWith(".sav")) {
      QsaveFile.append(".sav");
    }
    dataInterface->writeSave(QsaveFile, sourceRowIndex, targetRowIndex,
			     columnIndex, relationshipDirection,
			     relationshipDescription, sep);
    QDateTime time = QDateTime::currentDateTime();
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "saved session as " + QsaveFile +
      " with [" + relationshipDirection + "], and [" + relationshipDescription + "], and with indexes (as seen by user): " +
      eventsLabelLeft->text() + " and " + eventsLabelRight->text() + ", and with indexes (as stored in machine): " +
      "source: " + QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
    logger->addToLog(newLog);
  }
}

// This function will load a previously saved file.
void MainDialog::loadSession() {
  // First we will ask for the desired file name.
  if (saveSessionButton->isEnabled() == true) { 
    QPointer<QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->addButton(QMessageBox::Cancel);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("WARNING: session in progress!");
    warningBox->setInformativeText("You will lose all progress in the current session if you load a session  without saving the current session first.");
    if (warningBox->exec() == QMessageBox::Ok) {
      QString QloadFile = QFileDialog::getOpenFileName(this, tr("Load File"),"", tr("sav files (*.sav)"));
      if (!QloadFile.trimmed().isEmpty()) {
	dataInterface->readSave(QloadFile);
	QDateTime time = QDateTime::currentDateTime();
	QString timeText = time.toString(Qt::TextDate);
	QString newLog = timeText + " - " + "loaded session from " + QloadFile;
	logger->addToLog(newLog);
      }
    }
  } else {
   QString QloadFile = QFileDialog::getOpenFileName(this, tr("Load File"),"", tr("sav files (*.sav)"));
   if (!QloadFile.trimmed().isEmpty()) {
     dataInterface->readSave(QloadFile);
     QDateTime time = QDateTime::currentDateTime();
     QString timeText = time.toString(Qt::TextDate);
     QString newLog = timeText + " - " + "loaded session from " + QloadFile;
     logger->addToLog(newLog);
   }
  }
}

void MainDialog::processLoad(const QString &sourceIndex, const QString &targetIndex,
			     const QString &colIndex, const QString &relDirection,
			     const QString &relDescription, const QString &separator) {
  // Let's first set the appropriate variables.
  int sI = sourceIndex.toInt();
  int tI = targetIndex.toInt();
  int cI = colIndex.toInt();
  sourceRowIndex = sI;
  targetRowIndex = tI;
  columnIndex = cI;
  leftColumnIndex = 0;
  rightColumnIndex = 0;
  relationshipDirection = relDirection;
  relationshipDescription = relDescription;
  sep = separator;

  // Then we do an altered version of the start coding process.
  eventSelector->setEnabled(false);
  relationshipDirSelector->setEnabled(false);
  relationshipDescriber->setEnabled(false);
  startCodingButton->setEnabled(false);
  updateTexts(); 
  updateIndexIndicators();
  leftSelector->clear();
  rightSelector->clear();
  std::vector<std::string>::iterator it;
  for (it = dataInterface->header.begin(); it != dataInterface->header.end(); it++) {
    QString currentHeader = QString::fromUtf8(it->c_str());
    leftSelector->addItem(currentHeader);
    rightSelector->addItem(currentHeader);
  }

  relationshipDirSelector = new QComboBox(this);
  relationshipDirSelector->addItem(RELDEFAULT);
  relationshipDirSelector->addItem(RELPAST);
  relationshipDirSelector->addItem(RELFUTURE);
  relationshipDirSelector->setEnabled(false);

  int tempIndex = relationshipDirSelector->findText(relationshipDirection);
  relationshipDirSelector->setCurrentIndex(tempIndex);
  relationshipDescriber->setText(relationshipDescription);
  QString relText = "--[" + relationshipDescription + "]-->";
  relationshipReporter->setText(relText);
  
  setWorkButtons(true);

  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "data successfully loaded with [" + relationshipDirection +
    "], and [" + relationshipDescription + "], and with indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() + ", and with indexes (as stored in machine): " +
    "source: " + QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
  
}

void MainDialog::setEventColumn(const QString &selection) {
  selectedEventColumn = selection;
  // This event should probably also trigger the process for filling the text fields with content.
  if (selection != EVENTDEFAULT) {
    relationshipDirSelector->setEnabled(true);
    if (relationshipDirection != RELDEFAULT) {
      relationshipDescriber->setEnabled(true);
    }
    if (relationshipDescription != "") {
      startCodingButton->setEnabled(true);
    }
  } else {
    relationshipDirSelector->setEnabled(false);
    relationshipDescriber->setEnabled(false);
    startCodingButton->setEnabled(false);
  }
}

void MainDialog::setRelationshipDirection(const QString &selection) {
  relationshipDirection = selection;
  if (selection != RELDEFAULT && selectedEventColumn != EVENTDEFAULT) {
    relationshipDescriber->setEnabled(true);
  } else {
    relationshipDescriber->setEnabled(false);
    startCodingButton->setEnabled(false);
  }
}

void MainDialog::checkRelationshipDescription(const QString &text) {
  relationshipDescription = text;
  if (relationshipDescription != "" && selectedEventColumn != EVENTDEFAULT &&
      relationshipDirection != RELDEFAULT) {
    QString relText = "--[" + relationshipDescription + "]-->";
    relationshipReporter->setText(relText);
    startCodingButton->setEnabled(true);
  } else {
    startCodingButton->setEnabled(false);
    relationshipReporter->setText("");
  }
}

void MainDialog::startCoding() {
  /* 
     We first deactivate some controls that should not be changed during the coding process.
     This means, unfortunately, that the user has to start over when (s)he wishes to change something.
     However, this also forces the user to think well about what (s)he is doing. 
  */
  eventSelector->setEnabled(false);
  relationshipDirSelector->setEnabled(false);
  relationshipDescriber->setEnabled(false);
  startCodingButton->setEnabled(false);
  // Let's initialize some of the indexes we are working with.
  leftColumnIndex = 0;
  rightColumnIndex = 0;
  sourceRowIndex = 0;
  targetRowIndex = 0;
  // The column index can be set immediately.
  for (std::vector <std::string>::size_type i = 0; i != dataInterface->header.size(); i++) {
    if (dataInterface->header[i] == selectedEventColumn.toStdString()) {
      columnIndex = i;
      break;
    }
  }
  // What follows should depend on the direction of the relationship that was set.
  if (relationshipDirection == RELPAST) {
    // First we set the indexes of the source and target events
    sourceRowIndex = 1; // Should be the second event.
    targetRowIndex = 0; // Should be the first event.
  } else if (relationshipDirection == RELFUTURE) {
    // The sourceRowIndex is already set to 0, and we need to be at the beginning, so no change is required.
    targetRowIndex = sourceRowIndex + 1;
  }
  updateTexts(); // We update all text
  updateIndexIndicators(); // We also update the indicators (for users) of where in the dataset we are.
  // Labels of columns with additional info.
  // Now we want to give the user control over what happens next.
  setWorkButtons(true);
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "started new session with [" + relationshipDirection + "], and " + "[" + relationshipDescription + "]" ;
  logger->addToLog(newLog);
}

/*
  The following two functions navigate through target events based on the user instructions.
  Their exact operation should depend on the direction of the relationships set by the user.
*/
void MainDialog::navigateNextTarget() {
  if (relationshipDirection == RELPAST && targetRowIndex != 0) {
    targetRowIndex--;
  }  else if (relationshipDirection == RELPAST && sourceRowIndex < dataInterface->rowData.size() - 1) {
    sourceRowIndex++;
    targetRowIndex = sourceRowIndex - 1;
  } else if (relationshipDirection == RELFUTURE && targetRowIndex != dataInterface->rowData.size() - 1) {
    targetRowIndex++;
  } else if (relationshipDirection == RELFUTURE && targetRowIndex == dataInterface->rowData.size() - 1 &&
	     sourceRowIndex != dataInterface->rowData.size() - 2) {
    sourceRowIndex++; 
    targetRowIndex = sourceRowIndex + 1;
  }
  // If all the above if-statements fail, then nothing noticable happens, which is exactly what we want.
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::navigatePreviousTarget() {
  if (relationshipDirection == RELPAST && targetRowIndex != dataInterface->rowData.size() - 2
      && targetRowIndex != sourceRowIndex - 1) { 
    targetRowIndex++;
  } else if (relationshipDirection == RELPAST && targetRowIndex == sourceRowIndex - 1 && sourceRowIndex != 1) {
    sourceRowIndex--; //WERKT
    targetRowIndex = sourceRowIndex - 1;
  } else if (relationshipDirection == RELFUTURE && targetRowIndex != 1 && targetRowIndex != sourceRowIndex + 1) {
    targetRowIndex--;
  } else if (relationshipDirection == RELFUTURE && targetRowIndex == sourceRowIndex + 1 && sourceRowIndex != 0) {
    sourceRowIndex++; 
    targetRowIndex = sourceRowIndex + 1;
  }
  // If all the above if-statements fail, then nothing noticable happens, which is exactly what we want.
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

/*
  The following two functions navigate through source events based on the user instructions.
  Their exact operation should depend on the direction of the relationships set by the user.
*/
void MainDialog::navigateNextSource() {
  if (relationshipDirection == RELPAST && sourceRowIndex != dataInterface->rowData.size() - 1) {
    sourceRowIndex++;
    targetRowIndex = sourceRowIndex - 1;
  } else if (relationshipDirection == RELFUTURE && sourceRowIndex != dataInterface->rowData.size() - 2) {
    sourceRowIndex++;
    targetRowIndex = sourceRowIndex + 1;
  }
  // If all the above if-statements fail, then nothing noticable happens, which is exactly what we want.
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::navigatePreviousSource() {
  if (relationshipDirection == RELPAST && sourceRowIndex != 1) {
    sourceRowIndex--;
    targetRowIndex = sourceRowIndex - 1;
  } else if (relationshipDirection == RELFUTURE && sourceRowIndex != 0) {
    sourceRowIndex--;
    targetRowIndex = sourceRowIndex + 1;
  }
  // If all the above if-statements fail, then nothing noticable happens, which is exactly what we want.
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::toggleTargetFlag() {
  std::vector<bool>::size_type currentRow = targetRowIndex;
  dataInterface->targetFlagIndex[currentRow] = !dataInterface->targetFlagIndex[currentRow];
  if (dataInterface->targetFlagIndex[currentRow] == true) {
    targetFlagLabel->setText("(!)");
    QDateTime time = QDateTime::currentDateTime(); 
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "target flag set at index (as seen by user): " +
      eventsLabelLeft->text() + ", and as stored in machine: " +
      QString::number(currentRow);
    logger->addToLog(newLog);
  } else if (dataInterface->targetFlagIndex[currentRow] == false) {
    targetFlagLabel->setText("");
    QDateTime time = QDateTime::currentDateTime();
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "target flag removed at index (as seen by user): " +
      eventsLabelLeft->text() + ", and as stored in machine: " +
      QString::number(currentRow);
    logger->addToLog(newLog);
  }
}

void MainDialog::navigateNextTargetFlag() {
  std::vector<bool>::size_type currentRow = targetRowIndex;
  if (relationshipDirection == RELPAST && currentRow != 0) {
    bool indexFound = false;
    for (std::vector<bool>::size_type i = currentRow; i != 0; i--) {
      if (dataInterface->targetFlagIndex[i] == true) {
	indexFound = true;
	targetRowIndex = i;
	if (i != currentRow) {
	  break;
	}
      }
    }
    if ((!indexFound && dataInterface->targetFlagIndex[0] == true) ||
	(indexFound && targetRowIndex == currentRow && dataInterface->targetFlagIndex[0] == true)) {
      targetRowIndex = 0;
    }
  } else if (relationshipDirection == RELFUTURE && currentRow != dataInterface->targetFlagIndex.size()) {
    for (std::vector<bool>::size_type i = currentRow; i != dataInterface->targetFlagIndex.size(); i++) {
      if (dataInterface->targetFlagIndex[i] == true) {
	targetRowIndex = i;
	if (i != currentRow) {
	  break;
	}
      }
    }
  }
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::navigatePreviousTargetFlag() {
  std::vector<bool>::size_type currentRow = targetRowIndex;
  if (relationshipDirection == RELPAST && currentRow != sourceRowIndex - 1) {
    for (std::vector<bool>::size_type i = currentRow; i != sourceRowIndex; i++) {
      if (dataInterface->targetFlagIndex[i] == true) {
	targetRowIndex = i;
	if (i != currentRow) {
	  break;
	}
      }
    }
  } else if (relationshipDirection == RELFUTURE && currentRow != 0) {
    for (std::vector<bool>::size_type i = currentRow; i != sourceRowIndex; i--) {
      if (dataInterface->targetFlagIndex[i] == true) {
	targetRowIndex = i;
	if (i != currentRow) {
	  break;
	}
      }
    }
  }
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::toggleSourceFlag() {
  std::vector<bool>::size_type currentRow = sourceRowIndex;
  dataInterface->sourceFlagIndex[currentRow] = !dataInterface->sourceFlagIndex[currentRow];
  if (dataInterface->sourceFlagIndex[currentRow] == true) {
    sourceFlagLabel->setText("(!)");
    QDateTime time = QDateTime::currentDateTime(); 
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "source flag set at index (as seen by user): " +
      eventsLabelLeft->text() + ", and as stored in machine: " +
      QString::number(currentRow);
    logger->addToLog(newLog);
  } else if (dataInterface->sourceFlagIndex[currentRow] == false) {
    sourceFlagLabel->setText("");
    QDateTime time = QDateTime::currentDateTime();
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "source flag removed at index (as seen by user): " +
      eventsLabelLeft->text() + ", and as stored in machine: " +
      QString::number(currentRow);
    logger->addToLog(newLog);
  }
}

void MainDialog::navigateNextSourceFlag() {
  std::vector<bool>::size_type currentRow = sourceRowIndex;
  if (relationshipDirection == RELPAST && currentRow != dataInterface->rowData.size() - 1) {
    for (std::vector<bool>::size_type i = currentRow; i != dataInterface->sourceFlagIndex.size(); i++) {
      if (dataInterface->sourceFlagIndex[i] == true) {
	sourceRowIndex = i;
	targetRowIndex = sourceRowIndex - 1;
	if (i != currentRow) {
	  break;
	}
      }
    }
  } else if (relationshipDirection == RELFUTURE && currentRow != dataInterface->sourceFlagIndex.size()) {
    for (std::vector<bool>::size_type i = currentRow; i != dataInterface->sourceFlagIndex.size(); i++) {
      if (dataInterface->sourceFlagIndex[i] == true) {
	sourceRowIndex = i;
	targetRowIndex = sourceRowIndex + 1;
	if (i != currentRow) {
	  break;
	}
      }
    }
  }
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::navigatePreviousSourceFlag() {
  std::vector<bool>::size_type currentRow = sourceRowIndex;
  if (relationshipDirection == RELPAST && currentRow != 1) {
    for (std::vector<bool>::size_type i = currentRow; i != dataInterface->sourceFlagIndex.size(); i--) {
      if (dataInterface->sourceFlagIndex[i] == true) {
	sourceRowIndex = i;
	targetRowIndex = sourceRowIndex - 1;
	if (i != currentRow) {
	  break;
	}
      }
    }
  } else if (relationshipDirection == RELFUTURE && currentRow != 0) {
    bool indexFound = false;
    for (std::vector<bool>::size_type i = currentRow; i != 0; i--) {
      if (dataInterface->sourceFlagIndex[i] == true) {
	indexFound = true;
	sourceRowIndex = i;
	targetRowIndex = sourceRowIndex + 1;
	if (i != currentRow) {
	  break;
	}
      }
    }
    if ((!indexFound && dataInterface->sourceFlagIndex[0] == true) ||
	(indexFound && sourceRowIndex == currentRow && dataInterface->sourceFlagIndex[0] == true)) {
      sourceRowIndex = 0;
      targetRowIndex = sourceRowIndex + 1;
    }
  }
  updateTexts();
  updateIndexIndicators();
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
}

void MainDialog::updateIndexIndicators() {
  QString sourceIndicator = "";
  QString targetIndicator = "";
  if (relationshipDirection == RELPAST) {
    sourceIndicator = "<b>Source<b> (" +
      QString::number(sourceRowIndex) + "/" +
      QString::number(dataInterface->rowData.size() - 1) + ")";
    targetIndicator = "<b>Target<b> (" +
      QString::number(sourceRowIndex - targetRowIndex) + "/" +
      QString::number(sourceRowIndex) + ")";
  } else if (relationshipDirection == RELFUTURE) {
    sourceIndicator = "<b>Source<b> (" +
      QString::number(sourceRowIndex + 1) + "/" +
      QString::number(dataInterface->rowData.size() - 1) + ")";
    targetIndicator = "<b>Target<b> (" +
      QString::number(targetRowIndex - sourceRowIndex) + "/" +
      QString::number(dataInterface->rowData.size() - 1 - sourceRowIndex) + ")";
  }
  eventsLabelLeft->setText(sourceIndicator);
  eventsLabelRight->setText(targetIndicator);
}

void MainDialog::scrollLeftColForward() {
  if (leftColumnIndex != dataInterface->header.size() - 1) {
    leftColumnIndex++;
  } else {
    leftColumnIndex = 0;
  }
  updateTexts();
}

void MainDialog::scrollRightColForward() {
  if (rightColumnIndex != dataInterface->header.size() - 1) {
    rightColumnIndex++;
  } else {
    rightColumnIndex = 0;
  }
  updateTexts();
}

void MainDialog::scrollLeftColBackward() {
  if (leftColumnIndex != 0) {
    leftColumnIndex--;
  } else {
    leftColumnIndex = dataInterface->header.size() - 1;
  }
  updateTexts();
}

void MainDialog::scrollRightColBackward() {
  if (rightColumnIndex != 0) {
    rightColumnIndex--;
  } else {
    rightColumnIndex = dataInterface->header.size() - 1;
  }
  updateTexts();
}

void MainDialog::setLeftColumn(const QString &selection) {
  std::string column = selection.toStdString();
  for (std::vector <std::string>::size_type i = 0; i != dataInterface->header.size(); i++) {
    if (dataInterface->header[i] == column) {
      leftColumnIndex = i;
    }
  }
  if (saveSessionButton->isEnabled()) {
    updateTexts();
  }
}

void MainDialog::setRightColumn(const QString &selection) {
  std::string column = selection.toStdString();
  for (std::vector <std::string>::size_type i = 0; i != dataInterface->header.size(); i++) {
    if (dataInterface->header[i] == column) {
      rightColumnIndex = i;
    }
  }
  if (saveSessionButton->isEnabled()) {
    updateTexts();
  }
}

void MainDialog::updateTexts() {
  QString currentSourceText =
    QString::fromUtf8((dataInterface->rowData[sourceRowIndex][columnIndex]).c_str());
  sourceEvent->setText(currentSourceText);
  QString currentTargetText =
    QString::fromUtf8((dataInterface->rowData[targetRowIndex][columnIndex]).c_str());
  targetEvent->setText(currentTargetText);
  QString currentExtraLeftText =
    QString::fromUtf8((dataInterface->rowData[sourceRowIndex][leftColumnIndex]).c_str());
  otherColLeft->setText(currentExtraLeftText);
  QString currentExtraRightText =
    QString::fromUtf8((dataInterface->rowData[targetRowIndex][rightColumnIndex]).c_str());
  otherColRight->setText(currentExtraRightText);
  QString leftColumnLabel = QString::fromUtf8((dataInterface->header[leftColumnIndex]).c_str());
  QString rightColumnLabel = QString::fromUtf8((dataInterface->header[rightColumnIndex]).c_str());
  int index = leftSelector->findText(leftColumnLabel);
  leftSelector->setCurrentIndex(index);
  index = rightSelector->findText(rightColumnLabel);
  rightSelector->setCurrentIndex(index);
  if (dataInterface->linkages[sourceRowIndex][targetRowIndex] == true) {
    linkLabel->setText("<b>Linked</b>");
  } else {
    linkLabel->setText("");
  }
  std::vector<bool>::size_type currentRow = sourceRowIndex;
  if (dataInterface->sourceFlagIndex[currentRow] == true) {
    sourceFlagLabel->setText("(!)");
  } else {
    sourceFlagLabel->setText("");
  }
  currentRow = targetRowIndex;
  if (dataInterface->targetFlagIndex[currentRow] == true) {
    targetFlagLabel->setText("(!)");
  } else {
    targetFlagLabel->setText("");
  }

  QString currentMemo = 
    QString::fromUtf8((dataInterface->memos[sourceRowIndex][targetRowIndex]).c_str());
  memoText->blockSignals(true);
  memoText->setText(currentMemo);
  memoText->blockSignals(false);
}

void MainDialog::toggleLink() {
  std::vector<std::vector <bool> >::size_type linkRow = sourceRowIndex;
  std::vector <bool>::size_type linkCol = targetRowIndex;
  dataInterface->linkages[sourceRowIndex][targetRowIndex] =
    !dataInterface->linkages[sourceRowIndex][targetRowIndex];
  if (dataInterface->linkages[sourceRowIndex][targetRowIndex] == true) {
    linkLabel->setText("<b>Linked</b>");
    QDateTime time = QDateTime::currentDateTime();
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "new link created between (as seen by user): " +
      eventsLabelLeft->text() + " and " + eventsLabelRight->text() + ", and as stored in machine: " +
      "source: " + QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);
  } else {
    linkLabel->setText("");
    QDateTime time = QDateTime::currentDateTime();
    QString timeText = time.toString(Qt::TextDate);
    QString newLog = timeText + " - " + "link removed between (as seen by user): " +
      eventsLabelLeft->text() + " and " + eventsLabelRight->text() + ", and as stored in machine: " +
      "source: " + QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
    logger->addToLog(newLog);
  }
}

void MainDialog::setMemo(const QString &text) {
  dataInterface->setMemo(text, sourceRowIndex, targetRowIndex);
}

// This sets  all the buttons in the main working section.
void MainDialog::setWorkButtons(const bool status) {
  saveSessionButton->setEnabled(status);
  nextTargetButton->setEnabled(status);
  prevTargetButton->setEnabled(status);
  toggleLinkButton->setEnabled(status);
  nextSourceButton->setEnabled(status);
  prevSourceButton->setEnabled(status);
  prevLeftColumnButton->setEnabled(status);
  nextLeftColumnButton->setEnabled(status);
  prevRightColumnButton->setEnabled(status);
  nextRightColumnButton->setEnabled(status);
  leftSelector->setEnabled(status);
  rightSelector->setEnabled(status);
  memoText->setEnabled(status);
  toggleSourceFlagButton->setEnabled(status);
  nextSourceFlagButton->setEnabled(status);
  prevSourceFlagButton->setEnabled(status);
  toggleTargetFlagButton->setEnabled(status);
  nextTargetFlagButton->setEnabled(status);
  prevTargetFlagButton->setEnabled(status);
  importCodesButton->setEnabled(status);
  jumpToIndexesButton->setEnabled(status);
  writeLinkagesButton->setEnabled(status);
}

void MainDialog::writeLinkages() {
  ExportDialog *exportDialog = new ExportDialog(this, dataInterface, relationshipDescription);
  exportDialog->exec();
  delete exportDialog;
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "exported linkages to ./export/ folder.";
  logger->addToLog(newLog);

}

/*
  This function is used to import codes from an earlier save.
  This can be useful if the user updated the file that was being coded (e.g., adding new incidents,
  removing incidents, or adding/removing columns), but wants to continue coding based on an earlier
  version of that file.

  The function basically triggers a process in which the program compares the two files and then decides
  which codes should be imported to the new session.
*/
void MainDialog::importCodes() {
  // First we will ask for the desired file name.
  QPointer<QMessageBox> warningBox = new QMessageBox;
  warningBox->addButton(QMessageBox::Ok);
  warningBox->addButton(QMessageBox::Cancel);
  warningBox->setIcon(QMessageBox::Warning);
  warningBox->setText("WARNING: session in progress!");
  warningBox->setInformativeText("You will lose all progress in the current session if you update the source file without saving the current session first.");
  if (warningBox->exec() == QMessageBox::Ok) {
    QString QloadFile = QFileDialog::getOpenFileName(this, tr("Import Codes"),"", tr("sav files (*.sav)"));
    if (!QloadFile.trimmed().isEmpty()) {
      dataInterface->importCodes(QloadFile, relationshipDirection, relationshipDescription);
      QDateTime time = QDateTime::currentDateTime();
      QString timeText = time.toString(Qt::TextDate);
      QString newLog = timeText + " - " + "updated codes from " + QloadFile;
      logger->addToLog(newLog);
    }
  }
  updateTexts();
  updateIndexIndicators();
}

// This function opens a dialog that allows the user to select indexes to jump to.
void MainDialog::jumpToIndexes() {
  // We create a dialog that the user can use to select an index.
  IndexDialog *indexer = new IndexDialog(0); // A dialog where these columns will be selected.
  indexer->deleteLater(); // We need to be able to access this after closure.
  indexer->exec(); // Open te dialog.

  // Now we get the source and index.
  std::string selectedSourceIndex = indexer->getSourceText();
  std::string selectedTargetIndex = indexer->getTargetText();

  // Now we need to convert the index to a number.
  unsigned int sourceInt = 0;
  unsigned int targetInt = 0;
  std::istringstream (selectedSourceIndex) >> sourceInt;
  std::istringstream (selectedTargetIndex) >> targetInt;
  // Now we can delete the dialog.
  delete indexer;
  
  /* 
     If the indexes are 0, then the user selected the cancel button,
     or input an illegal index.
  */
  if (!(sourceInt == 0 || targetInt == 0)) {   
    if (relationshipDirection == RELPAST) {
      // We should first check if the selected source index is legal.
      if (sourceInt > 0 && sourceInt <= dataInterface->rowData.size() - 1) {
	sourceRowIndex = sourceInt;
	targetRowIndex = sourceRowIndex - 1; // Default to this if target turns out to be illegal.
	// We should then check if the selected target index is legal.
	if (targetInt > 0 && targetInt <= sourceRowIndex) {
	  targetRowIndex = (targetInt - sourceRowIndex) * -1;
	}
      }
    } else if (relationshipDirection == RELFUTURE) {
      if (sourceInt > 0 && sourceInt <= dataInterface->rowData.size() - 1) {
	// We should first check if the selected source index is legal.
	sourceRowIndex = sourceInt - 1;
	targetRowIndex = sourceRowIndex + 1; // Default to this if target turns out to be illegal.
	// We should then check if the selected target index is legal.
	if (targetInt > 0 && targetInt <= (dataInterface->rowData.size() - 1 - sourceRowIndex)) {
	  targetRowIndex = targetInt + sourceRowIndex;
	}
      }
    }
  }

  // If all the above if-statements fail, then nothing noticable happens, which is exactly what we want.
  updateTexts();
  updateIndexIndicators();
  
  // And we log the event.
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString newLog = timeText + " - " + "new indexes (as seen by user): " +
    eventsLabelLeft->text() + " and " + eventsLabelRight->text() +
    ", and new indexes (as stored in machine): " + "source: " +
    QString::number(sourceRowIndex) + " and target: " + QString::number(targetRowIndex);
  logger->addToLog(newLog);

}

void MainDialog::compareCoding() {
  /*
    The code comparison dialog can be used to compare the codings of two files that
    have already been processed. This allows for some basic checks of coder agreement.    
  */
  codeComparisonDialog = new CodeDialog;
  codeComparisonDialog->exec();
}

void MainDialog::finalBusiness() {
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QString logName = "log_" + timeText + ".txt";
  QString path = QCoreApplication::applicationDirPath();
  path.append("/logs/");
  if (!QDir(path).exists()) {
    QDir().mkdir(path);
  }
  path.append(logName);
  logger->writeLog(path);
  delete logger;
  delete dataInterface;
}





