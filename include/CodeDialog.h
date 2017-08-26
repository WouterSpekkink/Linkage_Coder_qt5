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
  See CodeDialog.cpp for more details on what this class and its methods do.

 */
   
#ifndef CODEDIALOG_H
#define CODEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <string>
#include "../include/DataInterface.h"

class CodeDialog : public QDialog {
  Q_OBJECT
  
public:
  CodeDialog(QWidget *parent=0);
  ~CodeDialog() {};
			       
private slots:
  void getFileOne(); // To obtain the file name of the first file.
  void getFileTwo(); // To obtain the file name of the second file.
  void compareFiles(); // This runs the comparison process.
  void writeResults(); // A function that can be used to write the results to a text file.
  void setButtons(const bool status);

private:
  // We need two data interfaces to be able to compare two files
  QPointer<DataInterface> dataOne;
  QPointer<DataInterface> dataTwo;

  QPointer<QLabel> titleLabel;
  QPointer<QLabel> fileSelectOneLabel;
  QPointer<QLabel> fileNameOneLabel; // To report to the user which file he selected.
  QPointer<QLabel> fileSelectTwoLabel;
  QPointer<QLabel> fileNameTwoLabel; // To report to the user which file he selected.
  
  QPointer<QLabel> totalDyadsLabel; // Only a label for the dialog. 
  QPointer<QLabel> totalDyads; // The total of possible dyads in the files. 
  QPointer<QLabel> numbLinksOneLabel; // Only a label for the dialog.
  QPointer<QLabel> numbLinksOne; // Number of links in file one.
  QPointer<QLabel> numbLinksTwoLabel; // Only a label for the dialog.
  QPointer<QLabel> numbLinksTwo; // Number of links in file two.
  QPointer<QLabel> positiveMatchesLabel; // Only a label for the dialog.
  QPointer<QLabel> positiveMatches; // Reports the absolute number of links that match (only counting 1's in the matrix).
  QPointer<QLabel> negativeMatchesLabel; // Only a label for the dialog.
  QPointer<QLabel> negativeMatches; // Reports the number of links that do not have a match in the other file.

  // I am not sure how much sense it makes to show a relative number for the level of agreement.
  // Such a measure would probably not be very robust.
  //QPointer<QLabel> percentageAgreementLabel; // Only a label for the dialog.
  //QPointer<QLabel> percentageAgreement; // Reports the percentage of agreement (only counting 1's in the matrix).

  // Then we have a number of buttons to operate the dialog.
  QPointer<QPushButton> openFileOneButton; 
  QPointer<QPushButton> openFileTwoButton;
  QPointer<QPushButton> compareFilesButton;
  QPointer<QPushButton> writeResultsButton; 
  QPointer<QPushButton> closeDialogButton;

  // We need to be able to access the file names in different functions, so we store them.
  QString fileNameOne;
  QString fileNameTwo;

  // We also need to store some basic details that we derive from the files. We use integers for that.
  int dyadsTotal;
  int linkagesOne;
  int linkagesTwo;
  int positive;
  int negative;

  // These indexes keep track of the linkages that are matched or unmatched between two selected files.
  std::vector <std::vector <int> > matchedIndexes;
  std::vector <std::vector <int> > unmatchedIndexes;

};

#endif
