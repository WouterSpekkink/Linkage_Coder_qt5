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
  `==NOTES==
  This class is basically a dialog that the user can use to compare two coded files with
  each other, checking for coder agreement. Given the somewhat special nature of the data
  the measures for comparison are very basic. The user is shown the total number of possible
  linkages (which is very high, and the number of actual linkages will typically be low), the
  number of linkages coded in each files, the absolute number of linkages that match, and the 
  absolute number of linkages that do not match.

  The indexes of the matched and unmatched linkages can also be found in a file that the user
  can export with a button on this dialog.
 */

#include "../include/CodeDialog.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QFileInfo>
#include <vector>
#include <fstream>

CodeDialog::CodeDialog(QWidget *parent) : QDialog(parent) {
  // We first initalize all the variables of this dialog.

  // We allow for the comparison of two coded files. These are stored in data interfaces.
  dataOne = new DataInterface();
  dataTwo = new DataInterface();

  titleLabel = new QLabel(tr("<h1>Code comparison dialog</h1>")); // The title of the dialog.
  fileSelectOneLabel = new QLabel(tr("<strong>Select file one</strong>")); // Just a label
  fileNameOneLabel = new QLabel(""); // This label will show which file the user selected.
  openFileOneButton = new QPushButton(tr("Open file")); // Button to select file one.
  fileSelectTwoLabel = new QLabel(tr("<strong>Select file two</strong>")); // Just a label.
  fileNameTwoLabel = new QLabel(""); //  This label will show which second file the user selected.
  openFileTwoButton = new QPushButton(tr("Open file")); // Button to select file two.
  openFileTwoButton->setEnabled(false); // We will only activate this if file one was selected first.
  compareFilesButton = new QPushButton(tr("Compare files")); // This starts the process of comparing codes.
  compareFilesButton->setEnabled(false); // We only want to activate this when two files have been selected.

  // We always initially reset the file names to empty strings..
  fileNameOne = ""; 
  fileNameTwo = "";

  // Now follow the labels that we use to report some basic results of the comparison to the user.
  totalDyadsLabel = new QLabel(tr("Total possible number of linkages: "));
  totalDyads = new QLabel(""); // Initially, these are all set to empty strings.
  numbLinksOneLabel = new QLabel(tr("Number of linkages in file one: "));
  numbLinksOne = new QLabel("");
  numbLinksTwoLabel = new QLabel(tr("Number of linkages in file two: "));
  numbLinksTwo = new QLabel("");
  positiveMatchesLabel = new QLabel(tr("Number of matched linkages: "));
  positiveMatches = new QLabel("");
  negativeMatchesLabel = new QLabel(tr("Number of unmatched linkages: "));
  negativeMatches = new QLabel("");

  // The write results button can be used to create a file where more details of the comparison can be found.
  writeResultsButton = new QPushButton(tr("Write detailed results to file"));
  writeResultsButton->setEnabled(false); // Only after getting results.

  // Simple button to close the dialog.
  closeDialogButton = new QPushButton(tr("Close dialog"));

  // We deactive certain buttons at first (see below for more details).
  setButtons(false);

  // We link all the button signals here
  connect(openFileOneButton, SIGNAL(clicked()), this, SLOT(getFileOne()));
  connect(openFileTwoButton, SIGNAL(clicked()), this, SLOT(getFileTwo()));
  connect(compareFilesButton, SIGNAL(clicked()), this, SLOT(compareFiles()));
  connect(writeResultsButton, SIGNAL(clicked()), this, SLOT(writeResults()));
  connect(closeDialogButton, SIGNAL(clicked()), this, SLOT(close()));
  
  // We create the layout for the dialog
  QPointer<QVBoxLayout> topLayout = new QVBoxLayout;
  topLayout->addWidget(titleLabel);
  QPointer<QHBoxLayout> fileSelectLayoutHori = new QHBoxLayout;
  QPointer<QVBoxLayout> fileSelectLayoutVerti = new QVBoxLayout;
  QPointer<QVBoxLayout> fileSelectLayoutLeft = new QVBoxLayout;
  QPointer<QVBoxLayout> fileSelectLayoutRight = new QVBoxLayout;
  fileSelectLayoutLeft->addWidget(fileSelectOneLabel);
  fileSelectLayoutLeft->addWidget(fileNameOneLabel);
  fileSelectLayoutLeft->addWidget(openFileOneButton);
  fileSelectLayoutRight->addWidget(fileSelectTwoLabel);
  fileSelectLayoutRight->addWidget(fileNameTwoLabel);
  fileSelectLayoutRight->addWidget(openFileTwoButton);
  fileSelectLayoutHori->addLayout(fileSelectLayoutLeft);
  fileSelectLayoutHori->addLayout(fileSelectLayoutRight);
  fileSelectLayoutVerti->addLayout(fileSelectLayoutHori);
  fileSelectLayoutVerti->addWidget(compareFilesButton);
  topLayout->addLayout(fileSelectLayoutVerti);

  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> dyadsLayout = new QHBoxLayout;
  dyadsLayout->addWidget(totalDyadsLabel);
  dyadsLayout->addWidget(totalDyads);
  totalDyads->setAlignment(Qt::AlignRight);
  QPointer<QHBoxLayout> numbLinksOneLayout = new QHBoxLayout;
  numbLinksOneLayout->addWidget(numbLinksOneLabel);
  numbLinksOneLayout->addWidget(numbLinksOne);
  numbLinksOne->setAlignment(Qt::AlignRight);
  QPointer<QHBoxLayout> numbLinksTwoLayout = new QHBoxLayout;
  numbLinksTwoLayout->addWidget(numbLinksTwoLabel);
  numbLinksTwoLayout->addWidget(numbLinksTwo);
  numbLinksTwo->setAlignment(Qt::AlignRight);
  QPointer<QHBoxLayout> positiveMatchesLayout = new QHBoxLayout;
  positiveMatchesLayout->addWidget(positiveMatchesLabel);
  positiveMatchesLayout->addWidget(positiveMatches);
  positiveMatches->setAlignment(Qt::AlignRight);
  QPointer<QHBoxLayout> negativeMatchesLayout = new QHBoxLayout;
  negativeMatchesLayout->addWidget(negativeMatchesLabel);
  negativeMatchesLayout->addWidget(negativeMatches);
  negativeMatches->setAlignment(Qt::AlignRight);
  middleLayout->addLayout(dyadsLayout);
  middleLayout->addLayout(numbLinksOneLayout);
  middleLayout->addLayout(numbLinksTwoLayout);
  middleLayout->addLayout(positiveMatchesLayout);
  middleLayout->addLayout(negativeMatchesLayout);

  QPointer<QVBoxLayout> bottomLayout = new QVBoxLayout;
  bottomLayout->addWidget(writeResultsButton);
  bottomLayout->addWidget(closeDialogButton);

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  QPointer<QFrame> topLine = new QFrame();
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);
  mainLayout->addLayout(middleLayout);
  QPointer<QFrame> bottomLine = new QFrame();
  bottomLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(bottomLine);
  mainLayout->addLayout(bottomLayout);
  setLayout(mainLayout);
  setWindowTitle(tr("Codes Comparison"));
  resize(500,400);
  // And this finishes the constructor.
}

// This is a simple function to set the status of two buttons.
void CodeDialog::setButtons(const bool status) {
  compareFilesButton->setEnabled(status);
  writeResultsButton->setEnabled(status);
}

// This function is used to load file one.
void CodeDialog::getFileOne() {
  // If the compareFilesButton is enabled, it means that a previous session was running.
  if (compareFilesButton->isEnabled() == true) { 
    QPointer <QMessageBox> warningBox = new QMessageBox; // We warn the user about this.
    warningBox->addButton(QMessageBox::Ok);
    warningBox->addButton(QMessageBox::Cancel);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("WARNING: opening a new file will reset this dialog!");
    if (warningBox->exec() == QMessageBox::Ok) {
      setButtons(false); // We reset some buttons.
      fileNameOne = ""; // We also reset the file names.
      fileNameTwo = "";
      // And then we get the file. 
      QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Save Files(*.sav)");
      if (!file.trimmed().isEmpty()) {
	fileNameOne = file; 
	QFileInfo fileInfOne(fileNameOne);
	fileNameOneLabel->setText(fileInfOne.fileName()); // To show user which file was selected.
	openFileTwoButton->setEnabled(true); // Now we allow for a second file to be loaded.
      }
    }
  } else {
    // Same procedure, without the warning box.
    fileNameOne = "";
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Save Files(*.sav)");
    if (!file.trimmed().isEmpty()) {
      fileNameOne = file;
      QFileInfo fileInfOne(fileNameOne);
      fileNameOneLabel->setText(fileInfOne.fileName());
      openFileTwoButton->setEnabled(true);
    }
  }
}

// This function is similar to the previous function, but simpler.
void CodeDialog::getFileTwo() {
  setButtons(false); // Reset some buttons.
  fileNameTwo = ""; // Reset the second file name.
  // And then we load the file, similarly to the procedure in the previous function.
  QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Save Files(*.sav)");
  if (!file.trimmed().isEmpty()) {
    fileNameTwo = file;
    QFileInfo fileInfTwo(fileNameTwo); 
    fileNameTwoLabel->setText(fileInfTwo.fileName());
  }
  if (fileNameOne != "" && fileNameTwo != "") {
    compareFilesButton->setEnabled(true); // Now we can active the button that starts the comparison.
  }
}

// This function is used to compare the two selected files.
void CodeDialog::compareFiles() {
  // First we read the data from the two files.
  dataOne->readSave(fileNameOne);
  dataTwo->readSave(fileNameTwo);
  // If the data are not the same, then we should abort and inform the user.
  if (dataOne->rowData.size() != dataTwo->rowData.size()) {
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("ERROR: The two selected files are not of the same dataset!");
    warningBox->exec();
    // We should also abort if the relationship direction is not the same.
  } else if (dataOne->relDir != dataTwo->relDir) {
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("ERROR: The direction of the relationship is different for the two files!");
    warningBox->exec();
    // If none of the above happens, we can continue.
  } else { 
    if (dataOne->relDesc != dataTwo->relDesc) {
      // The relationship descripion may differ. This is not a major issue, but still relevant to report to the user.
      QPointer <QMessageBox> warningBox = new QMessageBox;
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("WARNING: The relationship description differs for the two files!");
      warningBox->exec();
    }

    // We make sure that some variables are (re)set to 0.
    dyadsTotal = 0;
    linkagesOne = 0;
    linkagesTwo = 0;
    positive = 0;
    negative = 0;

    // And we also make sure that our indexes are clean.
    matchedIndexes.clear();
    unmatchedIndexes.clear();     

    // Now we walk through the save files.
    for (std::vector <std::vector <bool> >::size_type i = 0; i != dataOne->linkages.size(); i++) {
      std::vector <bool> currentRow = dataOne->linkages[i];
      for (std::vector <bool>::size_type j = 0; j != currentRow.size(); j++) {
	if (i != j) {
	  dyadsTotal++; // We can add a dyad at each pass.
	  // Only count linkages if they are present, for each file separately.
	  if (dataOne->linkages[i][j] == true) {
	    linkagesOne++;
	  }
	  if (dataTwo->linkages[i][j] == true) {
	    linkagesTwo++;
	  }
	  // If both files have the same linkage, then we count a match.
	  if (dataOne->linkages[i][j] == true && dataTwo->linkages[i][j] == true) {
	    positive++;
	    std::vector <int> tempIndex; // We use this to identify where the match occurred.
	    tempIndex.push_back(i); // Index of the source.
	    tempIndex.push_back(j); // Index of the target.
	    matchedIndexes.push_back(tempIndex); // store the indexes as a pair.
	  }
	  // If either of the files has a linkage that the other doesn't have, we count a mismatch.
	  if (dataOne->linkages[i][j] == true && dataTwo->linkages[i][j] == false) {
	    negative++;
	    std::vector <int> tempIndex; // We use this to identify where the mismatch occurred.
	    tempIndex.push_back(i); // Index of the source.
	    tempIndex.push_back(j); // Index of the target.
	    tempIndex.push_back(0); // The linkage did occur in the source, not in the target.
	    unmatchedIndexes.push_back(tempIndex); // store the indexes as a pair
	  } else if (dataOne->linkages[i][j] == false && dataTwo->linkages[i][j] == true) {
	    negative++;
	    std::vector <int> tempIndex;
	    tempIndex.push_back(i); // Index of the source.
	    tempIndex.push_back(j); // Index of the target.
	    tempIndex.push_back(1); // The linkage did occur in the target, not in the source.
	    unmatchedIndexes.push_back(tempIndex);
	  }
	}
      }
    }
    dyadsTotal /= 2; // One half of the matrix will always be 0;
    // Now we immediately report some information to the user in the dialog.
    totalDyads->setText(QString::number(dyadsTotal));
    numbLinksOne->setText(QString::number(linkagesOne));
    numbLinksTwo->setText(QString::number(linkagesTwo));
    positiveMatches->setText(QString::number(positive));
    negativeMatches->setText(QString::number(negative));
    writeResultsButton->setEnabled(true); // And we allow the user to write all results to a file.
  }
}

// This function writes the result of a comparison to a text file.
void CodeDialog::writeResults() {
  QString QfileName = QFileDialog::getSaveFileName(this, tr("Export Results"),"", tr("text files (*.txt)"));
  
  if (!QfileName.trimmed().isEmpty()) {
    if (!QfileName.endsWith(".txt")) {
      QfileName.append(".txt");
    }
    const std::string fileName = QfileName.toStdString();
    std::ofstream fileOut(fileName.c_str());

    if (!fileOut.is_open()) {
      QPointer<QMessageBox> errorBox =  new QMessageBox;
      errorBox->setText(tr("<b>ERROR: Cannot write file</b>"));
      errorBox->setInformativeText("It appears impossible to open the file to write data.");
      errorBox->exec();
      return;
    }

    fileOut << "==Results of code comparison 'Linkage Coder'==\n"; // A standard header for these files.
    // We want to record the file names in the results.
    QFileInfo fileInfOne(fileNameOne);  
    std::string fileOne = fileInfOne.fileName().toStdString();
    fileOut << "Name file one: " << "\t\t" <<  fileOne << "\n";
    QFileInfo fileInfTwo(fileNameTwo); 
    std::string fileTwo = fileInfTwo.fileName().toStdString();
    fileOut << "Name file two: " << "\t\t" <<  fileTwo << "\n\n";

    // Then we report some details that we also reported in the dialogue.
    fileOut << "Total number of possible linkages: " << "\t\t" << dyadsTotal << "\n";
    fileOut << "Number of linkages in file one: " << "\t\t" << linkagesOne << "\n";
    fileOut << "Number of linkages in file two: " << "\t\t" << linkagesTwo << "\n";
    fileOut << "Number of matched linkages: " << "\t\t\t" << positive << "\n";
    fileOut << "Number of unmatched linkages: " << "\t\t\t" << negative << "\n\n";

    // Some additional details, about the direction and description of the relationships.
    fileOut << "Relationship direction: " << dataOne->relDir << "\n";
    fileOut << "Relationship description file one: " << dataOne->relDesc << "\n";
    fileOut << "Relationship description file two: " << dataTwo->relDesc << "\n\n";

    /*
      Then follow two lists. The first list is of the matched linkages, identified by their index.
      The second list is of the unmatched linkages, also identified by their index. 

      The indexes should always be the same as those seen by the user in the Linkage Coder Tool.
      This allows the user to use information from the text file to find those linkages that were not matched,
      and this facilitates exploration of where coder disagreement occurred.
    */
    fileOut << "=Indexes of matched linkages=\n";
    std::vector <std::vector <int> >::iterator positiveIt;
    for (positiveIt = matchedIndexes.begin(); positiveIt != matchedIndexes.end(); positiveIt++) {
      std::vector <int> currentPair = *positiveIt;
      /* 
	 The code for writing the indexes of the linkages looks a bit complicated. This is because we 
	 need to make sure that the indexes match what the user sees when using the Linkage Coder Tool.
      */
      if (dataOne->relDir == "Present to past") {
	fileOut << "source: " << "(" << currentPair[0] << " / " <<
	  dataOne->rowData.size() - 1 << ")" << "\t" << "target: " << "("  <<
	  currentPair[0] - currentPair[1] << " / "  << currentPair[0] << ")\n";
      } else if (dataOne->relDir == "Past to present") {
	fileOut << "source: " << "(" << currentPair[0] + 1 << " / " << dataOne->rowData.size() - 1 <<
	  ")" << "\t" << "target: " << "(" << currentPair[1] - currentPair[0] << " / " <<
	  dataOne->rowData.size() - 1 - currentPair[0] << ")\n";
      }
    }
    // Almost exactly the same as the above, but for unmatched linkages.
    fileOut << "\n=Indexes of unmatched linkages=\n";
    std::vector <std::vector <int> >::iterator negativeIt;
    for (negativeIt = unmatchedIndexes.begin(); negativeIt != unmatchedIndexes.end(); negativeIt++) {
      std::vector <int> currentPair = *negativeIt;
      // In this case, we also want to report in which file the linkage occurred, and in which file it was missing.
      std::string report = "";
      if (currentPair[2] == 0) {
	report = "Link in " + fileOne + " not present in " + fileTwo + ".";
      } else if (currentPair[2] == 1) {
	report = "Link in " + fileTwo + " not present in " + fileOne + ".";
      }
      if (dataOne->relDir == "Present to past") {
	fileOut << "source: " << "(" << currentPair[0] << " / " <<
	  dataOne->rowData.size() - 1 << ")" << "\t" << "target: " << "("  <<
	  currentPair[0] - currentPair[1] << " / "  << currentPair[0] << ")" <<
	  "\t" << report << "\n";
      } else if (dataOne->relDir == "Past to present") {
	fileOut << "source: " << "(" << currentPair[0] + 1 << " / " << dataOne->rowData.size() - 1 <<
	  ")" << "\t" << "target: " << "(" << currentPair[1] - currentPair[0] << " / " <<
	  dataOne->rowData.size() - 1 - currentPair[0] << ")" << "\t" << report << "\n";
      }
    }
    // Now we are finished, and we can close the file.
    fileOut.close();
  }
}

