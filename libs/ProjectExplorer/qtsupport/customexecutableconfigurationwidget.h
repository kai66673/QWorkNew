/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef CUSTOMEXECUTABLECONFIGURATIONWIDGET_H
#define CUSTOMEXECUTABLECONFIGURATIONWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLineEdit;
class QComboBox;
class QLabel;
class QAbstractButton;
QT_END_NAMESPACE

namespace Utils {
class DetailsWidget;
class PathChooser;
}

namespace QtSupport {
class CustomExecutableRunConfiguration;

namespace Internal {

class CustomExecutableConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    enum ApplyMode { InstantApply, DelayedApply};
    CustomExecutableConfigurationWidget(CustomExecutableRunConfiguration *rc, ApplyMode mode);
    void apply(); // only used for DelayedApply

    bool isValid() const;
signals:
    void validChanged();

private slots:
    void changed();

    void executableEdited();
    void argumentsEdited(const QString &arguments);
    void workingDirectoryEdited();
    void termToggled(bool);
    void environmentWasChanged();

private:
    bool m_ignoreChange;
    CustomExecutableRunConfiguration *m_runConfiguration;
    Utils::PathChooser *m_executableChooser;
    QLineEdit *m_commandLineArgumentsLineEdit;
    Utils::PathChooser *m_workingDirectory;
    QCheckBox *m_useTerminalCheck;
    Utils::DetailsWidget *m_detailsContainer;
};

} // namespace Internal
} // namespace QtSupport

#endif // CUSTOMEXECUTABLECONFIGURATIONWIDGET_H
