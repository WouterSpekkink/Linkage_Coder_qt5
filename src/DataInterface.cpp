/*
 Copyright 2017 Wouter Spekkink 
 Authors : Wouter Spekkink <wouterspekkink@gmail.com> 
 Website : http://www.wouterspekkink.org

 This file is part of the Linkage Code Tool.

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
  This class handles all interaction of the program with 'external data'. These data
  typically take the form csv files selected by the user, or save files created by the 
  user while interacting with the program. The DataInterface class can be used to read,
  store, and write data. 
*/

#include <QCoreApplication>
#include <QDir>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <QtWidgets/QMessageBox>
#include <QPointer>
#include "../include/DataInterface.h"
#include "../include/ProgressBar.h"

const std::string DataInterface::IMPORTED_BEGIN = "<imported_data>";
const std::string DataInterface::IMPORTED_END = "</imported_data>";
const std::string DataInterface::ROW_PART = "<row rowid=";
const std::string DataInterface::ROW_END =  "</row>";
const std::string DataInterface::SOURCE_BEGIN = "<source_row_index>";
const std::string DataInterface::SOURCE_END = "</source_row_index>";
const std::string DataInterface::TARGET_BEGIN = "<target_row_index>";
const std::string DataInterface::TARGET_END = "</target_row_index>";
const std::string DataInterface::COLUMN_BEGIN = "<event_column_index>";
const std::string DataInterface::COLUMN_END = "</event_column_index>";
const std::string DataInterface::RELDIR_BEGIN = "<relationship_direction>";
const std::string DataInterface::RELDIR_END = "</relationship_direction>";
const std::string DataInterface::RELDESC_BEGIN = "<relationship_description>";
const std::string DataInterface::RELDESC_END = "</relationship_description>";
const std::string DataInterface::SEP_BEGIN = "<sep>";
const std::string DataInterface::SEP_END = "</sep>";
const std::string DataInterface::CODINGTYPE_BEGIN = "<coding_type>";
const std::string DataInterface::CODINGTYPE_END = "</coding_type>";
const std::string DataInterface::CODED_BEGIN = "<coded_linkages>";
const std::string DataInterface::CODED_END = "</coded_linkages>"; // LAST ONE SO NO NEWLINE
const std::string DataInterface::ROW_PART_2 = "<row_";
const std::string DataInterface::MEMOS_BEGIN = "<memos>";
const std::string DataInterface::MEMOS_END = "</memos>";
const std::string DataInterface::FLAGS_BEGIN = "<flags>";
const std::string DataInterface::FLAGS_END = "</flags>";

// Constructor for this class.
DataInterface::DataInterface()  
{
}

// This method is used to read data from a newly imported csv file. 
void DataInterface::readFile(const QString &fileName, const QString &sep) 
{
  // Let's first make sure that our current DataInterface is clean.
  header.clear();
  rowData.clear();
  linkages.clear();
  memos.clear();
  flagIndex.clear();
  relDir = "";
  relDesc = "";
  
  // Translating the method arguments to something that this class can use.
  // We sometimes need the separator as a char, and sometimes we need it as a string.
  const std::string inputFile = fileName.toStdString();
  std::string sepString = sep.toStdString();
  std::istringstream convert(sepString.c_str());
  char sepChar;
  convert >> sepChar;
    
  // Let's first make a vector of vectors of strings. Currently, this will make a matrix of strings.
  std::vector <std::vector <std::string> > dataVector;

  // Set up an file instream for the input file.
  std::ifstream myFile (inputFile.c_str());
  // Then we read all the lines of data in the input file.
  while(myFile) {
    // The buffer will hold one line of raw data temporarily, and we will do some processing on it.
    std::string buffer;

    if (!getline(myFile, buffer)) break;
    /* I had some problems reading files with embedded line breaks. 
       It should be easy to spot these linebreaks by checking for unmatched quotes.  
       However, I am not sure whether unmatched quotes are necessarily caused by embedded linebreaks.
       Therefore, the program shows an error box, cancels the import process, and leaves it to the 
       user to deal with the problem outside the program. 
    */
    bool quoteFound = false;
      
    for (std::string::size_type i = 0; i != buffer.length(); i++) {
      if (quoteFound == false && buffer[i] == '"') {
	quoteFound = true;
      } else if (quoteFound == true && buffer[i] == '"') {
	  quoteFound = false;
      }
    }
    if (quoteFound == true) {
      QPointer<QMessageBox> errorBox = new QMessageBox;
      errorBox->setText(tr("<b>ERROR: Import cancelled</b>"));
      errorBox->setInformativeText("Unmatched quotes (\") were found in one of the lines of the file.");
      errorBox->exec();
      return;
    }
    // This boolean will indicate whether or not we find ourselves in a text field. These may hold
    // delimiter symbols that should be ignored. The code below is customized to do just that.
    bool inTextField = false;
    std::vector <std::string> currentLineProcessed;
    std::string::size_type stringLength = 0;
    std::string::size_type previousPos = 0;
    for (std::string::size_type i = 0; i != buffer.length(); i++) {
      if (inTextField == false && buffer[i] == '"') {
	inTextField = true;
	previousPos++;
	stringLength--;
      } else if (inTextField == true && buffer[i] == '"' && (buffer[i + 1] == sepChar || i == buffer.length() - 1)) {
	inTextField = false;
	stringLength--;
      }
      if (inTextField == false && buffer[i] == sepChar) {
	while (buffer[previousPos] == ' ') {
	  previousPos++;
	  stringLength--;
	}
	std::string tempString = buffer.substr(previousPos, stringLength);
	currentLineProcessed.push_back(tempString);
	previousPos = i + 1;
	stringLength = 0;
      } else {
	stringLength++;
      }
    }
    while (buffer[previousPos] == ' ') {
      previousPos++;
      stringLength--;
    }
    std::string tempString = buffer.substr(previousPos, stringLength);
    currentLineProcessed.push_back(tempString);
    // And then we push this line of data in the larger data vector.
    dataVector.push_back(currentLineProcessed);
  }

  // This will disect the data into the header row and the data rows. 
  std::vector<std::vector <std::string> >::iterator it;
  for(it = dataVector.begin(); it != dataVector.end(); it++) {
    // The first line is always the header
    if(it == dataVector.begin()) {
      std::vector<std::string> tempVector = *it;
      std::vector<std::string>::iterator it2;
      for(it2 = tempVector.begin(); it2 != tempVector.end(); it2++)
	header.push_back(*it2);
    } else {
      // Then we handle the other 'rows' in the data vector.
      rowData.push_back(*it);
    }
  }
  if (header.empty()) {
    QPointer<QMessageBox> errorBox =  new QMessageBox;
    errorBox->setText(tr("<b>ERROR: Import cancelled</b>"));
    errorBox->setInformativeText("Something strange happened during the import. Did you set the delimiters correctly?");
    errorBox->exec();
    return;
  }
  // This signal is sent to the main dialog to let it know we have finished importing the file.

  /* 
     The actual linkages that the user codes are stored in a matrix the 
     dimensions of which are based on the number of rows in the dataset that we import. .
  */
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != rowData.size(); i++) {
    std::vector <bool> newLinkagesRow;
    for (std::vector <std::vector <std::string> >::size_type j = 0; j != rowData.size(); j++) {
      newLinkagesRow.push_back(false);
    }    
    linkages.push_back(newLinkagesRow);
  }

  // And we set all entries of the flag indexes to false.
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != rowData.size(); i++) {
    flagIndex.push_back(false);
  }
  
  emit importFinished();
  return;
}

void DataInterface::writeSave(const QString &fileName,
			      const QString &type,
			      std::vector<std::vector <std::string> >::size_type sourceRowIndex,
			      std::vector<std::vector <std::string> >::size_type targetRowIndex,
			      std::vector<std::string>::size_type columnIndex,
			      const QString &relationshipDirection,
			      const QString &relationshipDescription,
			      const QString &sep) {
  // Let's first create versions of the arguments that we can work with.
  std::stringstream ss;
  ss << sourceRowIndex;
  const std::string sourceIndex = ss.str();
  ss.str(std::string());
  ss << targetRowIndex;
  const std::string targetIndex = ss.str();
  ss.str(std::string());
  ss << columnIndex;
  const std::string colIndex = ss.str();
  ss.str(std::string());
  const std::string saveFile = fileName.toStdString();
  const std::string relDirection = relationshipDirection.toStdString();
  const std::string relDescription = relationshipDescription.toStdString();
  const std::string separator = sep.toStdString();
  const std::string codingType = type.toStdString();

  // Let's then set up our save file.
  std::ofstream fileOut(saveFile.c_str());

  if (!fileOut.is_open()) {
    QPointer<QMessageBox> errorBox =  new QMessageBox;
    errorBox->setText(tr("<b>ERROR: Cannot write file</b>"));
    errorBox->setInformativeText("It appears impossible to open the file to write data.");
    errorBox->exec();
    return;
  }

  // We first write the header of the xml-file.
  fileOut << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

  // Next we write the data from the imported file.
  fileOut << IMPORTED_BEGIN << "\n";
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != rowData.size(); i++) {
    std::vector <std::string> currentRow = rowData[i];
    ss << i;
    std::string rowLabel = "row_" + ss.str();
    ss.str(std::string());
    
    fileOut << ROW_PART << "\"" << rowLabel << "\">\n";
    for (std::vector <std::string>::size_type j = 0; j != header.size(); j++) {
      std::string currentHeader = header[j];
      std::string currentCell = currentRow[j];
      /*
	When writing and reading save files, we would potentially get in trouble if the user's data
	file has lines that start with "<", so we change these now in case there are any.
      */
      if (currentCell[0] == '<') {
	currentCell[0] = '_';
      }
      fileOut << "<" << currentHeader << ">\n" << currentCell << "\n" << "</" << currentHeader << ">\n";
    }
    fileOut << ROW_END << "\n";
  }
  fileOut << IMPORTED_END << "\n";

  // Next, we will write the indexes that the user is currently at. 
 
  fileOut << SOURCE_BEGIN << "\n" << sourceRowIndex << "\n" << SOURCE_END << "\n";
  fileOut << TARGET_BEGIN << "\n" << targetRowIndex << "\n" << TARGET_END << "\n";
  fileOut << COLUMN_BEGIN << "\n" << colIndex << "\n" << COLUMN_END << "\n";
  // Now some other stuff relevant to restoring previous sessions from the save file
  fileOut << RELDIR_BEGIN << "\n" << relDirection << "\n" << RELDIR_END << "\n";
  fileOut << RELDESC_BEGIN << "\n" << relDescription << "\n" << RELDESC_END << "\n";
  fileOut << SEP_BEGIN << "\n" << separator << "\n" << SEP_END << "\n";
  fileOut << CODINGTYPE_BEGIN << "\n" << codingType << "\n" << CODINGTYPE_END << "\n";
							       
  // Next, we will write the linkages that the user has coded so far.
  fileOut << CODED_BEGIN << "\n";

  std::vector <std::vector <bool> >::iterator linIt;
  int counter = 0;
  for (linIt = linkages.begin(); linIt != linkages.end(); linIt++) {
    std::vector <bool> currentRow = *linIt;
    ss << counter;
    counter++;
    std::string rowLabel = ROW_PART_2 + ss.str();
    ss.str(std::string());
    fileOut << rowLabel << ">\n";
    std::vector <bool>::iterator linkagesColIterator;
    for (linkagesColIterator = currentRow.begin(); linkagesColIterator != currentRow.end(); linkagesColIterator++) {
      bool currentCol = *linkagesColIterator;
      if (linkagesColIterator == currentRow.end() - 1) {
	fileOut << currentCol << "\n" << ROW_END << "\n";
      } else {
	fileOut << currentCol << ";";
      }
    }
  }
  fileOut << CODED_END << "\n"; 

  fileOut << MEMOS_BEGIN << "\n";
  std::vector <std::vector <std::string> >::iterator memIt;
  std::vector <std::string>::iterator memIt2;
  for (memIt = memos.begin(); memIt != memos.end(); memIt++) {
    if (memIt != memos.end() - 1) {
      for (memIt2 = memIt->begin(); memIt2 != memIt->end(); memIt2++) {
	if (memIt2 != memIt->end() - 1) {
	  fileOut << *memIt2 << "|";
	} else {
	  fileOut << *memIt2 << ";";
	}
      }
    } else {
      for (memIt2 = memIt->begin(); memIt2 != memIt->end(); memIt2++) {
	if (memIt2 != memIt->end() - 1) {
	  fileOut << *memIt2 << "|";
	} else {
	  fileOut << *memIt2 << "\n";
	}
      }
    }
  }
  
  fileOut << MEMOS_END << "\n"; 
  
  fileOut << FLAGS_BEGIN << "\n";
  std::vector<bool>::iterator flagIt;
  for (flagIt = flagIndex.begin(); flagIt != flagIndex.end(); flagIt++) {
    if (flagIt != flagIndex.end() - 1) {
      if (*flagIt == true) {
	fileOut << "1;"; 
      } else if (*flagIt == false) {
	fileOut << "0;";
      }
    } else {
      if (*flagIt == true) {
	fileOut << "1\n"; 
      } else if (*flagIt == false) {
	fileOut << "0\n";
      }
    }
  }
  
  fileOut << FLAGS_END; // No newline needed, because we are at the end of the file.
}

void DataInterface::readSave(const QString &fileName) {
  const std::string loadFile = fileName.toStdString();
  
  // Let's first make sure that our current DataInterface is clean.
  header.clear();
  rowData.clear();

  linkages.clear();
  memos.clear();
  
  // Then we make some strings that we need to store data temporarily.
  std::string sourceIndex;
  std::string targetIndex;
  std::string colIndex;
  std::string relDirection;
  std::string relDescription;
  std::string separator;
  std::string type;

  // Set up an file instream for the input file.
  std::ifstream myFile (loadFile.c_str());
  
  // We use this to identify the field that we are in.
  enum FieldStatus {none, imported, source, target, column, reldir, reldesc, sep, codingtype, coded, memosfield, flagsfield};
  FieldStatus field = none;

  /* 
     We use inRow to check whether or not we are in a row of imported data.
     We use inMatrixRow to check whether or not we are in a row of the linkages matrix.
  */
  bool inRow = false;
  bool inMatrixRow = false;
  // We only need to import the headers once. this bool helps with that.
  bool headersImported = false;

  // We beed to declare a vector for holding row data here, for scope reasons.
  std::vector <std::string> currentRow;
 
  // We need to declare a vector for holding a row of linkages here, for scope reasons.
  std::vector <bool> currentLinkages;

  // Then we read all the lines of data in the input file.
  while(myFile) {
    // The buffer will hold one line of raw data temporarily, and we will do some processing on it.
    std::string buffer;
   
    if (!getline(myFile, buffer)) break;

    // First we check whether we are in a field and, if yes, which one.
    if (buffer == IMPORTED_BEGIN) {      
      field = imported;
    } else if (buffer == IMPORTED_END) {
      field = none;
    } else if (buffer == SOURCE_BEGIN) {
      field = source;
    } else if (buffer == SOURCE_END) {
      field = none;
    } else if (buffer == TARGET_BEGIN) {
      field = target;
    } else if (buffer == TARGET_END) {
      field = none;
    } else if (buffer == COLUMN_BEGIN) {
      field = column;
    } else if (buffer == COLUMN_END) {
      field = none;
    } else if (buffer == RELDIR_BEGIN) {
      field = reldir;
    } else if (buffer == RELDIR_END) {
      field = none;
    } else if (buffer == RELDESC_BEGIN) {
      field = reldesc;
    } else if (buffer == RELDESC_END) {
      field = none;
    } else if (buffer == SEP_BEGIN) {
      field = sep;
    } else if (buffer == SEP_END) {
      field = none;
    } else if (buffer == CODINGTYPE_BEGIN) {
      field = codingtype;
    } else if (buffer == CODINGTYPE_END) {
      field = none;
    } else if (buffer == CODED_BEGIN) {
      field = coded;
    } else if (buffer == CODED_END) {
      field = none;
    } else if (buffer == MEMOS_BEGIN) {
      field = memosfield;
    } else if (buffer == MEMOS_END) {
      field = none;
    } else if (buffer == FLAGS_BEGIN) {
      field = flagsfield;
    } else if (buffer == FLAGS_END) {
      field = none;
    }

    // Then we do something, and what we do depends on the field we are in.
    switch(field) {
    case imported:
      {
	// First we check whether we are in a new row.
	if(buffer.substr(0, 11) == ROW_PART) {
	  inRow = true;
	}
	if (inRow && buffer[0] != '<') {
	  currentRow.push_back(buffer);
	}
	if (inRow && !headersImported && buffer[0] == '<' && buffer.substr(0,2) != "</" &&
	    buffer.substr(0, 11) != ROW_PART) {
	  std::string::size_type labelLength = 0;
	  for (std::string::size_type i = 1; i != buffer.length(); i++) {
	    if (buffer[i] == '>') {
	      labelLength = i - 1;
	      break;
	    }
	  }
	  std::string currentHeader = buffer.substr(1, labelLength);
	  header.push_back(currentHeader);
	}
	if (buffer == ROW_END){
	  inRow = false;
	  if (!headersImported) {
	    headersImported =  true; // The headers should have been imported after the first row.
	  }
	  // And we can also push back the data read so far to rowData.
	  rowData.push_back(currentRow);
	  currentRow.clear();
	}
	// We only need to import the headers once.
	break;
      }
    case source:
      {
	if (buffer != SOURCE_BEGIN) sourceIndex = buffer;
	break;
      }
    case target:
      {
	if (buffer != TARGET_BEGIN) targetIndex = buffer;
	break;
      }
    case column:
      {
	if (buffer != COLUMN_BEGIN) colIndex = buffer;
	break;
      }
    case reldir:
      {
	if (buffer != RELDIR_BEGIN) {
	  relDirection = buffer;
	  relDir = buffer;
	}
	break;
      }
    case reldesc:
      {
	if (buffer != RELDESC_BEGIN) {
	  relDescription = buffer;
	  relDesc = buffer;
	}
	break;
      }
    case sep:
      {
	if (buffer != SEP_BEGIN) separator = buffer;
	break;
      }
    case codingtype:
      {
	if (buffer != CODINGTYPE_BEGIN) type = buffer;
      }
    case coded:
      {
	if (buffer[0] == '<' && buffer.substr(0,2) != "</") {
	  inMatrixRow = true;
	}
	if (inMatrixRow && buffer != CODED_BEGIN && buffer != CODED_END &&
	    buffer != ROW_END && buffer.substr(0,5) != ROW_PART_2) {

	  std::string::iterator it;
	  for (it = buffer.begin(); it != buffer.end(); it++) {
	    char currentCell = *it;
	    if (currentCell == '0') {
	      currentLinkages.push_back(false);
	    } else if (currentCell == '1') {
	      currentLinkages.push_back(true);
	    }
	  }
	}
	if (buffer == ROW_END) {
	  inMatrixRow = false;
	  linkages.push_back(currentLinkages);
	  currentLinkages.clear();
	}
	break;
      }
    case memosfield:
      {
	if (buffer != MEMOS_BEGIN && buffer != MEMOS_END) {
	  std::istringstream stringStream(buffer);
	  while (stringStream) {
	    std::vector<std::string> tempMemo;
	    std::string memoString;
	    if (!getline(stringStream, memoString, ';')) break;
	    std::istringstream memostream(memoString);
	    while (memostream) {
	      std::string indivItem;
	      if (!getline(memostream, indivItem, '|')) break;
	      tempMemo.push_back(indivItem);
	    }
	    memos.push_back(tempMemo);
	  }
	}
	break;
      }
    case flagsfield:
      {
	std::istringstream stringStream(buffer);
	std::vector<std::string> currentRow;
	
	while(stringStream) {
	  std::string s;
	  if (!getline(stringStream, s, ';')) break;
	  if (s == "1") {
	    flagIndex.push_back(true);
	  } else if (s == "0") {
	    flagIndex.push_back(false);
	  }
	}
	break;
      }
    case none:
      {
	break;
      }
    }
  }
  // We should check whether data has actually been read.
  if (header.empty() || rowData.empty() || linkages.empty()) {
      QPointer<QMessageBox> errorBox =  new QMessageBox;
    errorBox->setText(tr("<b>ERROR: Could not load data</b>"));
    errorBox->setInformativeText("It appears there was a problem while importing data. Perhaps your save file has been corrupted.");
    errorBox->exec();
    return; 
  } else {
    QString sI = QString::fromStdString(sourceIndex);
    QString tI = QString::fromStdString(targetIndex);
    QString cI = QString::fromStdString(colIndex);
    QString rDi = QString::fromStdString(relDirection);
    QString rDe = QString::fromStdString(relDescription);
    QString qS = QString::fromStdString(separator);
    QString cT = QString::fromStdString(type);
    
    emit loadFinished(sI, tI, cI, rDi, rDe, qS, cT); 
  }
}

// The following function updates codes from an older file.
void DataInterface::importCodes(const QString &fileName, const QString &relDirection, const QString &relDescription) {
  // Let us first convert the parameters to something we will use in this function.
  const std::string loadFile = fileName.toStdString();
  relDir = relDirection.toStdString();
  relDesc = relDescription.toStdString();

  std::vector<bool>::iterator flagIt;
  for (flagIt = flagIndex.begin(); flagIt != flagIndex.end(); flagIt++) {
    *flagIt = false;
  }

  // Let's clear our memo vector.
  memos.clear();
  
  // We need to create some temporary data storage units.
  std::vector<std::string> tempHeader;
  std::vector<std::vector <std::string> > tempRowData;
  std::vector<std::vector <bool> > tempLinkages;

  // Then we make some strings that we need to store data temporarily.
  std::string relationshipDirection;
  std::string relationshipDescription;

  // Set up an file instream for the input file.
  std::ifstream myFile (loadFile.c_str());
  
  // We use this to identify the field that we are in.
  enum FieldStatus {none, imported, reldir, reldesc, coded, memosfield};
  FieldStatus field = none;

  /* 
     We use inRow to check whether or not we are in a row of imported data.
     We use inMatrixRow to check whether or not we are in a row of the linkages matrix.
  */
  bool inRow = false;
  bool inMatrixRow = false;
  // We only need to import the headers once. this bool helps with that.
  bool headersImported = false;

  // We beed to declare a vector for holding row data here, for scope reasons.
  std::vector <std::string> currentRow;
 
  // We need to declare a vector for holding a row of linkages here, for scope reasons.
  std::vector <bool> currentLinkages;

  // These vectors store the names of the columns we need to match.
  QVector<QString> Qmatch;
  std::vector <std::string> matchColumns;

  // A boolean to check if the user wants to mark new rows.
  bool markNew = false;
  
  // Then we read all the lines of data in the input file.
  while(myFile) {
    // The buffer will hold one line of raw data temporarily, and we will do some processing on it.
    std::string buffer;
   
    if (!getline(myFile, buffer)) break;

    // First we check whether we are in a field and, if yes, which one.
    if (buffer == IMPORTED_BEGIN) {      
      field = imported;
    } else if (buffer == IMPORTED_END) {
      field = none;
    } else if (buffer == RELDIR_BEGIN) {
      field = reldir;
    } else if (buffer == RELDIR_END) {
      field = none;
    } else if (buffer == RELDESC_BEGIN) {
      field = reldesc;
    } else if (buffer == RELDESC_END) {
      field = none;
    } else if (buffer == CODED_BEGIN) {
      field = coded;
    } else if (buffer == CODED_END) {
      field = none;
    } else if (buffer == MEMOS_BEGIN) {
      field = memosfield;
    } else if (buffer == MEMOS_END) {
      field = none;
    }

    // Then we do something, and what we do depends on the field we are in.
    switch(field) {
    case imported:
      {
	// First we check whether we are in a new row.
	if(buffer.substr(0, 11) == ROW_PART) {
	  inRow = true;
	}
	if (inRow && buffer[0] != '<') {
	  currentRow.push_back(buffer);
	}
	if (inRow && !headersImported && buffer[0] == '<' && buffer.substr(0,2) != "</" &&
	    buffer.substr(0, 11) != ROW_PART) {
	  std::string::size_type labelLength = 0;
	  for (std::string::size_type i = 1; i != buffer.length(); i++) {
	    if (buffer[i] == '>') {
	      labelLength = i - 1;
	      break;
	    }
	  }
	  std::string currentHeader = buffer.substr(1, labelLength);
	  tempHeader.push_back(currentHeader);
	}
	if (buffer == ROW_END){
	  inRow = false;
	  if (!headersImported) {
	    headersImported =  true; // The headers should have been imported after the first row.
	    std::vector <std::string>::iterator sIt;
	    std::vector <std::string>::iterator sIt2;
	    for (sIt = header.begin(); sIt != header.end(); sIt++) {
	      for (sIt2 = tempHeader.begin(); sIt2 != tempHeader.end(); sIt2++) {
		if (*sIt == *sIt2) {
		  QString temp = QString::fromUtf8((*sIt).c_str());
		  Qmatch.push_back(temp);
		}
	      }
	    }
	    // First we need the user to select which columns should match between the two files.
	    ImportCodesDialog *import = new ImportCodesDialog(0, Qmatch); // A dialog where these columns will be selected.
	    import->deleteLater(); // We need to be able to access this after closure.
	    import->exec(); // Open te dialog.
	
	    Qmatch = import->getColumnNames(); // Recover the column names.
	
	    markNew = import->getMarked(); //SHOULD REACTIVE THIS LATER(?)
	
	    delete import; // And now we can delete the dialog.

	    // Let us convert the column names to strings.
	    QVectorIterator<QString> cI(Qmatch);
	    cI.toFront();
	    while (cI.hasNext()) {
	      QString temp = cI.next();
	      matchColumns.push_back(temp.toStdString());
	    }

	    if (matchColumns.size() == 0) {
	      QPointer <QMessageBox> warningBox = new QMessageBox;
	      warningBox->addButton(QMessageBox::Ok);
	      warningBox->setIcon(QMessageBox::Warning);
	      warningBox->setText("WARNING: No columns were selected!");
	      warningBox->exec();
	      return;
	    }
	  }
	  // And we can also push back the data read so far to rowData.
	  tempRowData.push_back(currentRow);
	  currentRow.clear();
	}
	// We only need to import the headers once.
	break;
      }
    case reldir:
      {
	if (buffer != RELDIR_BEGIN) {
	  relationshipDirection = buffer;
	}
	break;
      }
    case reldesc:
      {
	if (buffer != RELDESC_BEGIN) {
	  relationshipDescription = buffer;
	}
	break;
      }
    case coded:
      {
	if (buffer[0] == '<' && buffer.substr(0,2) != "</") {
	  inMatrixRow = true;
	}
	if (inMatrixRow && buffer != CODED_BEGIN && buffer != CODED_END &&
	    buffer != ROW_END && buffer.substr(0,5) != ROW_PART_2) {
	  std::string::iterator it;
	  for (it = buffer.begin(); it != buffer.end(); it++) {
	    char currentCell = *it;
	    if (currentCell == '0') {
	      currentLinkages.push_back(false);
	    } else if (currentCell == '1') {
	      currentLinkages.push_back(true);
	    }
	  }
	}
	if (buffer == ROW_END) {
	  inMatrixRow = false;
	  tempLinkages.push_back(currentLinkages);
	  currentLinkages.clear();
	}
	break;
      }
    case memosfield:
      {
	if (buffer != MEMOS_BEGIN && buffer != MEMOS_END) {
	  std::istringstream stringStream(buffer);
	  while (stringStream) {
	    std::vector<std::string> tempMemo;
	    std::string memoString;
	    if (!getline(stringStream, memoString, ';')) break;
	    std::istringstream memostream(memoString);
	    while (memostream) {
	      std::string indivItem;
	      if (!getline(memostream, indivItem, '|')) break;
	      tempMemo.push_back(indivItem);
	    }
	    memos.push_back(tempMemo);
	  }
	}
	break;
      }
    case none:
      {
	break;
      }
    }
  } 

  // We first fill Qmatch with common column headers (those that are not common cannot be matched).

  // Let us convert the column names to strings.

  // We start with some basic checks and give the user warnings if necessary.
  if (relationshipDirection != relDir) {
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("WARNING: The direction of the relationship is different for the imported file!");
    warningBox->exec();
  }
  if (relationshipDescription != relDesc) {
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("WARNING: The relationship description differs for the two files!");
    warningBox->exec();
  }
    
  /* 
     Now we do the actual matching.
     First, we should find the indexes of the matching columns.
  */
  
  std::vector <int> indexOriginal;
  std::vector <int> indexLoaded;
  int goal= 0;
  int counterSource = 0;
  int counterTarget = 0;

  // The first step is to identify the indexes of the columns that we need to inspect (for both files).
  for (std::vector<std::string>::size_type i = 0; i != matchColumns.size(); i++) {
    for (std::vector<std::string>::size_type j = 0; j != header.size(); j++) {
      if (header[j] == matchColumns[i]) {
	indexOriginal.push_back(j);
	goal++;
      }
    }
  }
  for (std::vector <std::string>::size_type i = 0; i != matchColumns.size(); i++) {
    for (std::vector <std::string>::size_type j = 0; j != tempHeader.size(); j++) {
      if (tempHeader[j] == matchColumns[i]) {
	indexLoaded.push_back(j);
      }
    }
  }
  
  /*
    Now we are ready to inspect the files. First we will walk through the new file, 
    and the old file (the one that was imported), to find any rows in the new file that
    are not present in the old file. These are new rows.

    If we find a row that is present in both files, then we record their indexes, so tat
    we can translate the codes associated with the old file to the codes associated with
    the new file.
  */

  // Let us check whether all rows are unique
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != rowData.size() - 1; i++) {
    for (std::vector <std::vector <std::string> >::size_type j = i + 1; j != rowData.size(); j++) {
      counterSource = 0;
      for (std::vector<int>::size_type k = 0; k != indexOriginal.size(); k++) {
	if (rowData[i][indexOriginal[k]] == rowData[j][indexOriginal[k]]) {
	  counterSource++;
	}
      }
      if (counterSource == goal) {
	memos.clear();
	QPointer <QMessageBox> warningBox = new QMessageBox;
	warningBox->addButton(QMessageBox::Ok);
	warningBox->setIcon(QMessageBox::Warning);
	warningBox->setText("WARNING: Identical entries detected in current data set with current selection of columns!");
	warningBox->exec();
	return;
      }
    }
  }
    
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != tempRowData.size() - 1; i++) {
    for (std::vector <std::vector <std::string> >::size_type j = i + 1; j != tempRowData.size(); j++) {
      counterTarget = 0;
      for (std::vector<int>::size_type k = 0; k != indexLoaded.size(); k++) {
	if (tempRowData[i][indexLoaded[k]] == tempRowData[j][indexLoaded[k]]) {
	  counterTarget++;
	}
      }
      if (counterTarget == goal) {
	QPointer <QMessageBox> warningBox = new QMessageBox;
	warningBox->addButton(QMessageBox::Ok);
	warningBox->setIcon(QMessageBox::Warning);
	warningBox->setText("WARNING: Identical entries detected in loaded save file with current selection of columns!");
	warningBox->exec();
	memos.clear();
	return;
      }
    }
  }

  // Let's declare some variables that we will use when finding linkages.
  int source = 0;
  int target = 0;
  bool foundSource = false;
  bool foundTarget = false;

  int step = 1;
  int max = tempLinkages.size();
  
  loadProgress = new ProgressBar(0, 1, max);
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();

  
  for (std::vector <std::vector <int> >::size_type i = 0; i != tempLinkages.size(); i++) {
    std::vector <bool> currentRow = tempLinkages[i];
    for (std::vector <int>::size_type j = 0; j != currentRow.size(); j++) {
      if (tempLinkages[i][j] == true) { // If we find a coded linkage in the old file.
	// We walk through the new dataset.
	for (std::vector <std::vector <std::string> >::size_type k = 0; k != rowData.size(); k++) {
	  counterSource = 0;
	  counterTarget = 0;
	  std::vector <std::string> currentData = rowData[k];
	  for (std::vector<int>::size_type l = 0; l != indexOriginal.size(); l++) {
	    if (currentData[indexOriginal[l]] == tempRowData[i][indexLoaded[l]]) {
	      counterSource++;
	    }
	    if (currentData[indexOriginal[l]] == tempRowData[j][indexLoaded[l]]) {
	      counterTarget++;
	    }
	    if (counterSource == goal) { // We found the correct amount of matching cells.
	      source = k;
	      foundSource = true;
	      counterSource = 0; // Reset the counter.
	    }
	    if (counterTarget == goal) {
	      target = k;
	      foundTarget = true;
	      counterTarget = 0; // Reset te counter.
	    }
	  }
	  if (foundSource == true && foundTarget == true) {
	    linkages[source][target] = true;
	    foundSource = false;
	    foundTarget = false;
	  }
	}
      }
    }
    loadProgress->setProgress(step);
    qApp->processEvents();
    step++;
  }
  loadProgress->close();
  delete loadProgress;

  bool rowFound = false;
  std::vector <std::vector <std::string> > eventPairs;
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != rowData.size(); i++) {
    for (std::vector <std::vector <std::string> >::size_type j = 0; j != tempRowData.size(); j++) {
      counterSource = 0;
      for (std::vector<int>::size_type k = 0; k != indexOriginal.size(); k++) {
	if (rowData[i][indexOriginal[k]] == tempRowData[j][indexLoaded[k]]) {
	  counterSource++;
	}
      }
      if (counterSource == goal) {
	rowFound = true;
	std::stringstream ss;
	ss << j;
	std::string currentEvent = ss.str();
	ss.clear();
	ss.str(std::string());
	ss << i;
	std::string newEvent = ss.str();
	std::vector <std::string> tempPair;
	tempPair.push_back(currentEvent);
	tempPair.push_back(newEvent);
	eventPairs.push_back(tempPair);
      }
    }
  }
  rowFound = false;
  std::vector<std::vector <std::vector <std::string> >::size_type> missingOldRows; 
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != tempRowData.size(); i++) {
    for (std::vector <std::vector <std::string> >::size_type j = 0; j != rowData.size(); j++) {
      counterSource = 0;
      for (std::vector<int>::size_type k = 0; k != indexOriginal.size(); k++) {
	if (tempRowData[i][indexOriginal[k]] == rowData[j][indexLoaded[k]]) {
	  counterSource++;
	}
      }
      if (counterSource == goal) {
	rowFound = true;
      }
    }
    if (rowFound == false) {
      missingOldRows.push_back(i);
    } else {
      rowFound = false;
    }
  }

  std::vector<std::vector <std::vector <std::string> >::size_type>::iterator mI;
  for (mI = missingOldRows.begin(); mI != missingOldRows.end(); mI++) {
    std::stringstream ss;
    ss << *mI;
    std::string tempIndex = ss.str();
    for (std::vector <std::vector <std::string> >::size_type f = 0; f != memos.size();) {
      if (memos[f][0] == tempIndex || memos[f][1] == tempIndex) {
	memos.erase(memos.begin() + f);
      } else {
	f++;
      }
    }
  }
    
  for (std::vector <std::vector <std::string> >::size_type i = 0; i != memos.size(); i++) {
    std::vector<std::string> currentMemo = memos[i];
    for (std::vector <std::vector <std::string> >::size_type j = 0; j != eventPairs.size(); j++) {
      std::vector<std::string> currentPair = eventPairs[j];
      if (currentMemo[0] == currentPair[0]) {
	memos[i][0] = currentPair[1];
      }
      if (currentMemo[1] == currentPair[0]) {
	memos[i][1] = currentPair[1];
      }
    }
  }
     
  rowFound = false;
  if (markNew == true) {
    for (std::vector <std::vector <std::string> >::size_type i = 0; i != rowData.size(); i++) {
      for (std::vector <std::vector <std::string> >::size_type j = 0; j != tempRowData.size(); j++) {
	counterSource = 0;
	for (std::vector<int>::size_type k = 0; k != indexOriginal.size(); k++) {
	  if (rowData[i][indexOriginal[k]] == tempRowData[j][indexLoaded[k]]) {
	    counterSource++;
	  }
	}
	if (counterSource == goal) {
	  rowFound = true;
	}
      }
      if (rowFound == false) {
	flagIndex[i] = true;
      } else {
	rowFound = false;
      }
    }
  }
}
  


// The following function writes the linkages to a file.
void DataInterface::writeLinkages(const QString &relDescription, QVector<QString> &properties) {
  std::vector<std::string> exportProperties;
  QVectorIterator<QString> pI(properties);
  while (pI.hasNext()) {
    exportProperties.push_back(pI.next().toStdString());
  }
  std::string description = relDescription.toStdString();

  QString path = QCoreApplication::applicationDirPath();
  path.append("/export/");
  if (!QDir(path).exists()) {
    QDir().mkdir(path);
  }
  // We first write the file with Incident Nodes.
  QString fileName = "Linkages_Nodes.csv";
  QString filePath = path;
  filePath.append(fileName);
  std::ofstream fileOut(filePath.toStdString().c_str());

  fileName = "CYPHER.txt";
  filePath = path;
  filePath.append(fileName);
  std::ofstream cypherOut(filePath.toStdString().c_str());

  // Now we write the headers of this output file.
  std::vector<std::vector<std::string>::size_type> index;

  for (std::vector<std::string>::size_type i = 0; i != exportProperties.size(); i++) {
    for (std::vector<std::string>::size_type j = 0; j != header.size(); j++) {
      if (exportProperties[i] == header[j]) {
	index.push_back(j);
      }
    }
  }

  cypherOut << "// Here the incidents are created. Remove this block if they were already included.\n\n";

  std::vector<std::string> cypherTemp; 
  if (index.size() == 0) {
    fileOut << "Id" << "," << "Label" << "," << "Order\n";
  } else {
    fileOut << "Id" << "," << "Label" << "," << "Order" << ",";
    std::vector<std::string>::iterator it;
    for (it = exportProperties.begin(); it != exportProperties.end(); it++) {
      if (it != exportProperties.end() - 1) {
	fileOut << "\"" << *it << "\"" << ",";
      } else {
	fileOut << "\"" << *it << "\"" << "\n";
      }
      cypherTemp.push_back(*it);
    }
  }

  int counter = 1;
  std::vector <std::vector <std::string> >::iterator dIt;
  for (dIt = rowData.begin(); dIt != rowData.end(); dIt++) {
    std::vector<std::string> currentRow = *dIt;
    std::stringstream ss;
    ss << counter;
    std::string curI = "I" + ss.str();
    if (index.size() == 0) {
      fileOut << counter << "," << counter << "," << "," << counter << "\n";
      cypherOut << "CREATE (" << curI << ":Incident {id: " << counter << " , order: "
		<< counter <<"})\n";
    } else {
      fileOut << counter << "," << counter << "," << counter << ",";
      cypherOut << "CREATE (" << curI << ":Incident {id: " << counter  << ", order: "
		<< counter << ", ";
      for (std::vector<std::vector<std::string>::size_type>::size_type i = 0; i != index.size(); i++) {
	if (i != index.size() - 1) {
	  fileOut << "\"" << currentRow[index[i]] << "\"" << ",";
	  cypherOut << cypherTemp[i] << ": " << "\"" << currentRow[index[i]] << "\", ";
	} else {
	  fileOut << "\"" << currentRow[index[i]] << "\"" << "\n";
	  cypherOut << cypherTemp[i] << ": " << "\"" << currentRow[index[i]] << "\"})\n";
	}
      }
    }
    counter++;
  }

  cypherOut << "\n// End of block where incidents are created. Remove the above if incidents were already included.\n\n";
  
  fileOut.close();

  fileName  = "Linkages_Edges.csv";
  filePath = path;
  filePath.append(fileName);
  std::ofstream edgesOut(filePath.toStdString().c_str());

  edgesOut << "Source" << "," << "Target" << "," << "Type" << "," << "Weight" <<
    "," << "Description" << "," << "Memo" <<"\n";

  cypherOut << "// Start of block where linkages are created.\n\n";

  counter = 0;
  for (std::vector <std::vector <bool> >::size_type i = 0; i != linkages.size(); i++) {
    std::vector <bool> currentRow = linkages[i];
    for (std::vector <bool>::size_type j = 0; j != currentRow.size(); j++) {
      bool currentPair = currentRow[j];
      if (currentPair) {
	std::stringstream ss;
	std::stringstream ts;
	ss << i;
	ts << j;
	std::vector <std::vector <std::string> >::iterator memIt;
	std::string memo;
	for (memIt = memos.begin(); memIt != memos.end(); memIt++) {
	  if (*memIt->begin() == ss.str() && *(memIt->begin() + 1) == ts.str()) {
	    memo = *(memIt->begin() + 2);
	  }
	}
	edgesOut << i + 1 << "," << j + 1 << "," << "Directed" << "," << "1" << "," << "Directed"
		 << ",\"" << description << "\",\"" <<  memo << "\"\n";

	std::stringstream ss2;
	ss2 << counter;
	counter++;
	std::string currentR = "r" + ss2.str();
	std::string currentS = "s" + ss2.str();
	std::string currentT = "t" + ss2.str();
	std::string currentLabel = currentS + "---[" + description + "]-->" + currentT;

	cypherOut << "CREATE (" << currentR << ":Incident_Relationship {id: \"" << currentLabel
		  << "\", type: \"" << description << "\", memo: \"" << memo << "\"})\n\n";
	
	cypherOut << "WITH " << currentR << "\n"
		  << "MATCH (" << currentS << ":Incident {id: " << i + 1 << "}), ("
		  << currentT << ":Incident {id: " << j + 1  << "})\n"
		  << "MERGE (" << currentS << ")-[:IS_SOURCE_IN]->(" << currentR << ")\n" 
		  << "MERGE (" << currentT << ")-[:IS_TARGET_IN]->(" << currentR << ")\n\n";
      }
    }
  }

  cypherOut << "\n// End of block where linkages are created.\n\n";
  
  edgesOut.close();
  cypherOut.close();
  QPointer<QMessageBox> errorBox = new QMessageBox;
  errorBox->setText(tr("<b>Files exported</b>"));
  errorBox->setInformativeText("Your files have been exported to the \"../export\" folder in the program's directory.");
  errorBox->exec();
}



