/*
 Copyright 2017 Wouter Spekkink
 Authors : Wouter Spekkink <wouter.spekkink@gmail.com>
 Website : http://www.wouterspekkink.org

 This file is part of the Linkage Coder tool.

 DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.

 Copyright 2017 Wouter Spekkink. All rights reserved.

 The Linkage Coder tool is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 The Linkage Coder tool is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with the Linkage Coder tool.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../include/ProgressBar.h"
#include <QtWidgets/QHBoxLayout>

ProgressBar::ProgressBar(QWidget *parent, int min, int max)
    : QDialog(parent), minimum(min), maximum(max)
{
    progress = new QProgressBar;
    progress->setMinimum(minimum);
    progress->setMaximum(maximum);
    QHBoxLayout *barLayout = new QHBoxLayout;
    barLayout->addWidget(progress);
    setLayout(barLayout);
    setWindowTitle(tr("Importing coded linkages"));
    setFixedHeight(sizeHint().height());
    setFixedWidth(300);
}


void ProgressBar::setProgress(int value) 
{
    progress->setValue(value);
}
