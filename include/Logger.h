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
  See dataInterface.cpp for more details on what this class and its methods do.  

 */


#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <vector>
#include <string>

class Logger : public QObject {
  friend class MainDialog;
  Q_OBJECT

public:
  Logger(); //constructor

  void addToLog(const QString &entry);
  void writeLog(const QString &fileName);

  ~Logger() {};

private:
  std::vector <std::string> logs;
  
};

#endif
