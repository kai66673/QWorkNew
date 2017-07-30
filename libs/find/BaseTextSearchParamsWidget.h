#ifndef QCOMMONSEARCHPARAMSWIDGET_H
#define QCOMMONSEARCHPARAMSWIDGET_H

#include <QWidget>
#include <QCheckBox>

class BaseTextSearchParamsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTextSearchParamsWidget(QWidget *parent = 0);

    inline bool caseSensetive() const { return m_caseSensetiveCheckBox->isChecked(); }
    inline bool wholeWordsOnly() const { return m_wholeWordsOnlyCheckBox->isChecked(); }
    inline bool useRegularExpressions() const { return m_useRegExpCheckBox->isChecked(); }

signals:

public slots:

private:
    QCheckBox *m_caseSensetiveCheckBox;
    QCheckBox *m_wholeWordsOnlyCheckBox;
    QCheckBox *m_useRegExpCheckBox;

};

#endif // QCOMMONSEARCHPARAMSWIDGET_H
