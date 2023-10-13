#ifndef HIGHLIGHTERFORMATSCONFIGUREWIDGET_H
#define HIGHLIGHTERFORMATSCONFIGUREWIDGET_H

#include <QWidget>
#include <QAbstractListModel>
#include <QListView>

#include "highlighterformatsmanager.h"
#include "corelib_global.h"

class HighlighterFormatModel: public QAbstractListModel
{
    Q_OBJECT

public:
    HighlighterFormatModel(HighlighterFormatData *formatData, const QFont &font, const QColor &defaultBackgroundColor, QObject *parent = 0);
    virtual ~HighlighterFormatModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setForegroundColor(const QColor &color, const QModelIndex &index);
    void setBackgroundColor(const QColor &color, const QModelIndex &index);
    void setBold(bool bold, const QModelIndex &index);
    void setItalic(bool italic, const QModelIndex &index);

    QColor foregroundColor(const QModelIndex &index);
    QColor backgroundColor(const QModelIndex &index);
    bool isBold(const QModelIndex &index);
    bool isItalic(const QModelIndex &index);

    HighlighterFormat *highlighterFormat(int row);

    void save();

public slots:
    void onFontChanged(const QFont &font);
    void onDefaultBackgroundColorChanged(const QColor &color);

protected:
    HighlighterFormatData *m_formatData;
    QList<HighlighterFormat *> m_formats;
    QFont m_font;
    QColor m_defaultBackgroundColor;
};

namespace Ui {
class HighlighterFormatsConfigureWidget;
}

class CORELIB_EXPORT HighlighterFormatsConfigureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HighlighterFormatsConfigureWidget(const QList<HighlighterFormatData *> &languageFormats, QWidget *parent = 0);
    ~HighlighterFormatsConfigureWidget();

    void save();

private slots:
    void onLanguageChanged(int index);
    void onFormatIndexChanged(const QModelIndex &current);
    void onDefaultBackgroundColorChanged(const QColor &color);

    void on_foregroundToolButton_clicked();
    void on_backgroundToolButton_clicked();
    void on_eraseForegroundToolButton_clicked();
    void on_eraseBackgroundToolButton_clicked();
    void on_boldCheckBox_toggled(bool checked);
    void on_italicCheckBox_toggled(bool checked);

signals:
    void formatsChanged();
    void fontChanged(const QFont &font);

private:
    void updateControls(HighlighterFormat *format);

    Ui::HighlighterFormatsConfigureWidget *ui;

    QList<HighlighterFormatData *> m_languageFormats;
    QList<QListView *> m_formatsListViews;
    QList<HighlighterFormatModel *>m_models;
};

#endif // HIGHLIGHTERFORMATSCONFIGUREWIDGET_H
