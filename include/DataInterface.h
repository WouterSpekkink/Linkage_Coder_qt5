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
  See DataInterface.cpp for more details on what this class and its methods do.  

 */
   
#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include "../include/ImportCodesDialog.h"
#include <QObject>
#include <string>
#include <vector>

class DataInterface : public QObject {
  friend class MainDialog;
  friend class CodeDialog;
  friend class ImportCodesDialog;
  friend class ExportDialog;
  Q_OBJECT

public:
  // We first have the constructor.
  DataInterface();

  // Then we have functions for reading, saving and loading files.
  void readFile(const QString &fileName, const QString &sep);
  void writeSave(const QString &fileName,
		 std::vector<std::vector <std::string> >::size_type sourceRowIndex,
		 std::vector<std::vector <std::string> >::size_type targetRowIndex,
		 std::vector<std::string>::size_type columnIndex,
		 const QString &relationshipDirection,
		 const QString &relationshipDescription,
		 const QString &sep);
  void readSave(const QString &fileName);
  void importCodes(const QString &fileName, const QString &relDirection, const QString &relDescription);
  void writeLinkages(const QString &relDescription, QVector<QString> &properties);
  // The deconstructor.
  ~DataInterface() {}
  
signals:
  void importFinished(); // A signal that will be send to the main dialog to indicate that importing data was finished.
  void loadFinished(const QString &sourceIndex,
		    const QString &targetIndex,
		    const QString &colIndex,
		    const QString &relDirection,
		    const QString &relDescription,
		    const QString &separator);
		       
private:
  std::vector<std::string> header; // This will hold the column names of the input data file.
  std::vector<std::vector <std::string> > rowData; // This will hold the data in the file.
  std::vector<std::vector <bool> > linkages; // We use this to store linkages that the user has created.
  std::vector<std::vector <std::string> > memos; // we use this to store memos on the linkages.
  std::vector<bool> flagIndex; // we use this to store user flags for events.
  std::string relDir;
  std::string relDesc;

  // We set some constant static strings that we will use more when saving and loading files.
  const static std::string IMPORTED_BEGIN;
  const static std::string IMPORTED_END;
  const static std::string ROW_PART;
  const static std::string ROW_END;
  const static std::string SOURCE_BEGIN;
  const static std::string SOURCE_END;
  const static std::string TARGET_BEGIN;
  const static std::string TARGET_END;
  const static std::string COLUMN_BEGIN;
  const static std::string COLUMN_END;
  const static std::string RELDIR_BEGIN;
  const static std::string RELDIR_END;
  const static std::string RELDESC_BEGIN;
  const static std::string RELDESC_END;
  const static std::string SEP_BEGIN;
  const static std::string SEP_END;
  const static std::string CODED_BEGIN;
  const static std::string CODED_END;
  const static std::string ROW_PART_2;
  const static std::string MEMOS_BEGIN;
  const static std::string MEMOS_END;
  const static std::string FLAGS_BEGIN;
  const static std::string FLAGS_END;
  
};
#endif
