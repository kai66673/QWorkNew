#ifndef QSTATUSWIDGET_H
#define QSTATUSWIDGET_H

#include <QWidget>

#include "QWorkBase.h"

namespace Ui {
    class TextPositionStatusBarWidget;
}

class TextPositionStatusBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextPositionStatusBarWidget(QWidget *parent = 0);
    ~TextPositionStatusBarWidget();

public slots:
    void onCurrentDocumentChanged( IDocument *document );

private slots:
    void onGotoLineNumber();

private:
    Ui::TextPositionStatusBarWidget *ui;
};

#endif // QSTATUSWIDGET_H
