#include <QColorDialog>
#include <QFontDatabase>
#include <QItemDelegate>

#include "highlighterformatsconfigurewidget.h"
#include "ui_highlighterformatsconfigurewidget.h"

class HighlighterFormatDelegate: public QItemDelegate
{
public:
    HighlighterFormatDelegate(QObject *parent = 0)
        : QItemDelegate(parent)
    { }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        if ( option.state & QStyle::State_Selected ) {
            QStyleOptionViewItem newOption(option);
            newOption.state &= ~QStyle::State_Selected;
            QItemDelegate::paint(painter, newOption, index);
        } else {
            QItemDelegate::paint(painter, option, index);
        }
    }
};

HighlighterFormatModel::HighlighterFormatModel(HighlighterFormatData *formatData, const QFont &font, const QColor &defaultBackgroundColor, QObject *parent)
    : QAbstractListModel(parent)
    , m_formatData(formatData)
    , m_formats(formatData->cloneFormats())
    , m_font(font)
    , m_defaultBackgroundColor(defaultBackgroundColor)
{ }

HighlighterFormatModel::~HighlighterFormatModel()
{
    qDeleteAll(m_formats);
}

int HighlighterFormatModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_formats.size();
}

QVariant HighlighterFormatModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.column() != 0 || index.row() < 0 || index.row() >= m_formats.size() )
        return QVariant();

    switch ( role ) {
        case Qt::DisplayRole:       return m_formats[index.row()]->name;
        case Qt::ForegroundRole:    return m_formats[index.row()]->foregroundColor;
        case Qt::BackgroundRole:    return m_formats[index.row()]->backgroundColor.isValid() ?
                                           m_formats[index.row()]->backgroundColor :
                                           m_defaultBackgroundColor;
        case Qt::FontRole: {
            QFont font = m_font;
            font.setBold(m_formats[index.row()]->bold);
            font.setItalic(m_formats[index.row()]->italic);
            return font;
        }
    }

    return QVariant();
}

void HighlighterFormatModel::setForegroundColor(const QColor &color, const QModelIndex &index)
{
    m_formats[index.row()]->foregroundColor = color;
    emit dataChanged(index, index);
}

void HighlighterFormatModel::setBackgroundColor(const QColor &color, const QModelIndex &index)
{
    m_formats[index.row()]->backgroundColor = color;
    emit dataChanged(index, index);
}

void HighlighterFormatModel::setBold(bool bold, const QModelIndex &index)
{
    m_formats[index.row()]->bold = bold;
    emit dataChanged(index, index);
}

void HighlighterFormatModel::setItalic(bool italic, const QModelIndex &index)
{
    m_formats[index.row()]->italic = italic;
    emit dataChanged(index, index);
}

QColor HighlighterFormatModel::foregroundColor(const QModelIndex &index)
{
    return m_formats[index.row()]->foregroundColor;
}

QColor HighlighterFormatModel::backgroundColor(const QModelIndex &index)
{
    return m_formats[index.row()]->backgroundColor;
}

bool HighlighterFormatModel::isBold(const QModelIndex &index)
{
    return m_formats[index.row()]->bold;
}

bool HighlighterFormatModel::isItalic(const QModelIndex &index)
{
    return m_formats[index.row()]->italic;
}

HighlighterFormat *HighlighterFormatModel::highlighterFormat(int row)
{
    return m_formats[row];
}

void HighlighterFormatModel::save()
{
    m_formatData->reinit(m_formats);
}

void HighlighterFormatModel::onFontChanged(const QFont &font)
{
    beginResetModel();
    m_font = font;
    endResetModel();
}

void HighlighterFormatModel::onDefaultBackgroundColorChanged(const QColor &color)
{
    beginResetModel();
    m_defaultBackgroundColor = color;
    endResetModel();
}

HighlighterFormatsConfigureWidget::HighlighterFormatsConfigureWidget(const QList<HighlighterFormatData *> &languageFormats, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HighlighterFormatsConfigureWidget)
    , m_languageFormats(languageFormats)
{
    ui->setupUi(this);

//    const QMap<QString, QVariant> &valuesMap = SettingsManager::instance()->valuesMap("000_TextEditor");
//    QFont font = qvariant_cast<QFont>(valuesMap["Font"]);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QColor defaultColor = m_languageFormats[0]->m_formats[0]->backgroundColor;

    foreach (HighlighterFormatData *formatData, m_languageFormats) {
        ui->languagesComboBox->addItem(formatData->languageId());
        QListView *listView = new QListView();
        HighlighterFormatDelegate *delegate = new HighlighterFormatDelegate(this);
        listView->setItemDelegate(delegate);
        HighlighterFormatModel *formatModel = new HighlighterFormatModel(formatData, font, defaultColor);
        m_models.append(formatModel);
        listView->setModel(formatModel);
        ui->languagesStackedWidget->addWidget(listView);
        connect(this, SIGNAL(fontChanged(QFont)), formatModel, SLOT(onFontChanged(QFont)));
        m_formatsListViews.append(listView);
        connect(listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onFormatIndexChanged(QModelIndex)));
    }

    connect(ui->languagesComboBox, SIGNAL(currentIndexChanged(int)), ui->languagesStackedWidget, SLOT(setCurrentIndex(int)));
    ui->languagesComboBox->setCurrentIndex(0);
    connect(ui->languagesStackedWidget, SIGNAL(currentChanged(int)), this, SLOT(onLanguageChanged(int)));

    updateControls(0);
}

HighlighterFormatsConfigureWidget::~HighlighterFormatsConfigureWidget()
{
    delete ui;
}

void HighlighterFormatsConfigureWidget::save()
{
    foreach (HighlighterFormatModel *model, m_models)
        model->save();
    emit formatsChanged();
}

void HighlighterFormatsConfigureWidget::onLanguageChanged(int index)
{
    onFormatIndexChanged(index == -1 ? QModelIndex() : m_formatsListViews[index]->currentIndex());
}

void HighlighterFormatsConfigureWidget::onFormatIndexChanged(const QModelIndex &current)
{
    updateControls(current.isValid() ? m_models[ui->languagesStackedWidget->currentIndex()]->highlighterFormat(current.row()) : 0);
}

void HighlighterFormatsConfigureWidget::onDefaultBackgroundColorChanged(const QColor &color)
{
    foreach (HighlighterFormatModel *model, m_models)
        model->onDefaultBackgroundColorChanged(color);
}

static inline QString colorButtonStyleSheet(const QColor &bgColor)
{
    if (bgColor.isValid()) {
        QString rc = QLatin1String("border: 2px solid black; border-radius: 2px; background:");
        rc += bgColor.name();
        return rc;
    }
    return QLatin1String("border: 2px dotted black; border-radius: 2px;");
}

void HighlighterFormatsConfigureWidget::updateControls(HighlighterFormat *format)
{
    ui->backgroundToolButton->setEnabled(format);
    ui->foregroundToolButton->setEnabled(format);
    ui->eraseBackgroundToolButton->setEnabled(format);
    ui->eraseForegroundToolButton->setEnabled(format);
    ui->boldCheckBox->setEnabled(format);
    ui->italicCheckBox->setEnabled(format);

    ui->backgroundToolButton->setStyleSheet(colorButtonStyleSheet(format ? format->backgroundColor : QColor()));
    ui->foregroundToolButton->setStyleSheet(colorButtonStyleSheet(format ? format->foregroundColor : QColor()));
    if ( format ) {
        if ( !format->backgroundColor.isValid() )
            ui->eraseBackgroundToolButton->setEnabled(false);
        if ( !format->foregroundColor.isValid() )
            ui->eraseForegroundToolButton->setEnabled(false);
        const bool boldBlocked = ui->boldCheckBox->blockSignals(true);
        ui->boldCheckBox->setChecked(format->bold);
        ui->boldCheckBox->blockSignals(boldBlocked);
        const bool italicBlocked = ui->italicCheckBox->blockSignals(true);
        ui->italicCheckBox->setChecked(format->italic);
        ui->italicCheckBox->blockSignals(italicBlocked);
    }
}

void HighlighterFormatsConfigureWidget::on_foregroundToolButton_clicked()
{
    int languageIndex = ui->languagesStackedWidget->currentIndex();
    if ( languageIndex != -1 ) {
        QModelIndex formatIndex = m_formatsListViews[languageIndex]->currentIndex();
        if ( formatIndex.isValid() ) {
            QColor color = m_models[languageIndex]->foregroundColor(formatIndex);
            const QColor newColor = QColorDialog::getColor(color, nullptr);
            if ( newColor.isValid() ) {
                ui->foregroundToolButton->setStyleSheet(colorButtonStyleSheet(newColor));
                ui->eraseForegroundToolButton->setEnabled(true);
                m_models[languageIndex]->setForegroundColor(newColor, formatIndex);
            }
        }
    }
}

void HighlighterFormatsConfigureWidget::on_backgroundToolButton_clicked()
{
    int languageIndex = ui->languagesStackedWidget->currentIndex();
    if ( languageIndex != -1 ) {
        QModelIndex formatIndex = m_formatsListViews[languageIndex]->currentIndex();
        if ( formatIndex.isValid() ) {
            QColor color = m_models[languageIndex]->backgroundColor(formatIndex);
            const QColor newColor = QColorDialog::getColor(color, nullptr);
            if ( newColor.isValid() ) {
                ui->backgroundToolButton->setStyleSheet(colorButtonStyleSheet(newColor));
                ui->eraseBackgroundToolButton->setEnabled(true);
                m_models[languageIndex]->setBackgroundColor(newColor, formatIndex);
                if ( languageIndex == 0 && formatIndex.row() == 0 ) {
                    onDefaultBackgroundColorChanged(newColor);
                    m_formatsListViews[0]->setCurrentIndex(formatIndex);
                }
            }
        }
    }
}

void HighlighterFormatsConfigureWidget::on_eraseForegroundToolButton_clicked()
{
    int languageIndex = ui->languagesStackedWidget->currentIndex();
    if ( languageIndex != -1 ) {
        QModelIndex formatIndex = m_formatsListViews[languageIndex]->currentIndex();
        if ( formatIndex.isValid() ) {
            const QColor newColor;
            ui->foregroundToolButton->setStyleSheet(colorButtonStyleSheet(newColor));
            ui->eraseForegroundToolButton->setEnabled(false);
            m_models[languageIndex]->setForegroundColor(newColor, formatIndex);
        }
    }
}

void HighlighterFormatsConfigureWidget::on_eraseBackgroundToolButton_clicked()
{
    int languageIndex = ui->languagesStackedWidget->currentIndex();
    if ( languageIndex != -1 ) {
        QModelIndex formatIndex = m_formatsListViews[languageIndex]->currentIndex();
        if ( formatIndex.isValid() ) {
            const QColor newColor;
            ui->backgroundToolButton->setStyleSheet(colorButtonStyleSheet(newColor));
            ui->eraseBackgroundToolButton->setEnabled(false);
            m_models[languageIndex]->setBackgroundColor(newColor, formatIndex);
        }
    }
}

void HighlighterFormatsConfigureWidget::on_boldCheckBox_toggled(bool checked)
{
    int languageIndex = ui->languagesStackedWidget->currentIndex();
    if ( languageIndex != -1 ) {
        QModelIndex formatIndex = m_formatsListViews[languageIndex]->currentIndex();
        if ( formatIndex.isValid() )
            m_models[languageIndex]->setBold(checked, formatIndex);
    }
}

void HighlighterFormatsConfigureWidget::on_italicCheckBox_toggled(bool checked)
{
    int languageIndex = ui->languagesStackedWidget->currentIndex();
    if ( languageIndex != -1 ) {
        QModelIndex formatIndex = m_formatsListViews[languageIndex]->currentIndex();
        if ( formatIndex.isValid() )
            m_models[languageIndex]->setItalic(checked, formatIndex);
    }
}
