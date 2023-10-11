#include <QPainter>
#include <QDir>

#include "IssuesView.h"

namespace Utils {

namespace {
const int ELLIPSIS_GRADIENT_WIDTH = 16;
}

unsigned int Issue::s_nextId = 1;

Issue::Issue() : taskId(0), type(Unknown), line(-1), column(0)
{ }

Issue::Issue(IssueType type_, const QString &description_,
             const QString &file_, int line_, int column_, const QString &category_) :
    taskId(s_nextId), type(type_), description(description_), file(file_),
    line(line_), column(column_), category(category_)
{
    ++s_nextId;
}

bool Issue::isNull() const
{ return taskId == 0; }

bool operator==(const Issue &t1, const Issue &t2)
{
    return t1.taskId == t2.taskId;
}

uint qHash(const Issue &task)
{
    return task.taskId;
}

IssuesModel::IssuesModel() :
    m_maxSizeOfFileName(0),
    m_errorIcon(QLatin1String(":/images/compile_error.png")),
    m_warningIcon(QLatin1String(":/images/compile_warning.png")),
    m_issuesCount(0),
    m_errorIssuesCount(0),
    m_sizeOfLineNumber(0)
{
}

int IssuesModel::issuesCount() const
{ return m_issuesCount; }

int IssuesModel::errorIssuesCount()
{
    return m_errorIssuesCount;
}

bool IssuesModel::hasFile(const QModelIndex &index) const
{
    int row = index.row();
    if (!index.isValid() || row < 0 || row >= m_issues.count())
        return false;
    return !m_issues.at(row).file.isEmpty();
}

QIcon IssuesModel::taskTypeIcon(Issue::IssueType t) const
{
    switch (t) {
    case Issue::Warning:
        return m_warningIcon;
    case Issue::Error:
        return m_errorIcon;
    case Issue::Unknown:
        break;
    }
    return QIcon();
}

void IssuesModel::addCategory(const QString &categoryId, const QString &categoryName)
{
    Q_ASSERT(!categoryId.isEmpty());
    m_categories.insert(categoryId, categoryName);
}

QList<Issue> IssuesModel::tasks(const QString &categoryId) const
{
    if (categoryId.isEmpty()) {
        return m_issues;
    } else {
        return m_issuesInCategory.value(categoryId);
    }
}

void IssuesModel::addIssue(const Issue &issue)
{
    Q_ASSERT(m_categories.keys().contains(issue.category));

    if (m_issuesInCategory.contains(issue.category)) {
        m_issuesInCategory[issue.category].append(issue);
    } else {
        QList<Issue> temp;
        temp.append(issue);
        m_issuesInCategory.insert(issue.category, temp);
    }

    beginInsertRows(QModelIndex(), m_issues.size(), m_issues.size());
    m_issues.append(issue);
    endInsertRows();

    m_maxSizeOfFileName = 0;
    ++m_issuesCount;
    if (issue.type == Issue::Error)
        ++m_errorIssuesCount;
}

void IssuesModel::removeTask(const Issue &task)
{
    if (m_issues.contains(task)) {
        int index = m_issues.indexOf(task);
        beginRemoveRows(QModelIndex(), index, index);
        m_issues.removeAt(index);
        --m_issuesCount;
        if (task.type == Issue::Error)
            --m_errorIssuesCount;
        endRemoveRows();
    }
}

void IssuesModel::clearIssues(const QString &categoryId)
{
    if (categoryId.isEmpty()) {
        if (m_issues.size() == 0)
            return;
        beginRemoveRows(QModelIndex(), 0, m_issues.size() -1);
        m_issues.clear();
        m_issuesInCategory.clear();
        m_issuesCount = 0;
        m_errorIssuesCount = 0;
        endRemoveRows();
        m_maxSizeOfFileName = 0;
    } else {
        int index = 0;
        int start = 0;
        int subErrorTaskCount = 0;
        while (index < m_issues.size()) {
            while (index < m_issues.size() && m_issues.at(index).category != categoryId) {
                ++start;
                ++index;
            }
            if (index == m_issues.size())
                break;
            while (index < m_issues.size() && m_issues.at(index).category == categoryId) {
                if (m_issues.at(index).type == Issue::Error)
                    ++subErrorTaskCount;
                ++index;
            }
            // Index is now on the first non category
            beginRemoveRows(QModelIndex(), start, index - 1);

            for (int i = start; i < index; ++i) {
                m_issuesInCategory[categoryId].removeOne(m_issues.at(i));
            }

            m_issues.erase(m_issues.begin() + start, m_issues.begin() + index);

            m_issuesCount -= index - start;
            m_errorIssuesCount -= subErrorTaskCount;

            endRemoveRows();
            index = start;
        }
        // what to do with m_maxSizeOfFileName ?
    }
}


QModelIndex IssuesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex IssuesModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int IssuesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_issues.count();
}

int IssuesModel::columnCount(const QModelIndex &parent) const
{
        return parent.isValid() ? 0 : 1;
}

QVariant IssuesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_issues.size() || index.column() != 0)
        return QVariant();

    if (role == IssuesModel::File) {
        return m_issues.at(index.row()).file;
    } else if (role == IssuesModel::Line) {
        if (m_issues.at(index.row()).line <= 0)
            return QVariant();
        else
            return m_issues.at(index.row()).line;
    } else if (role == IssuesModel::Description) {
        return m_issues.at(index.row()).description;
    } else if (role == IssuesModel::FileNotFound) {
        return m_fileNotFound.value(m_issues.at(index.row()).file);
    } else if (role == IssuesModel::Type) {
        return (int)m_issues.at(index.row()).type;
    } else if (role == IssuesModel::Category) {
        return m_issues.at(index.row()).category;
    } else if (role == IssuesModel::Icon) {
        return taskTypeIcon(m_issues.at(index.row()).type);
    } else if (role == IssuesModel::Task_t) {
        return QVariant::fromValue(task(index));
    }
    return QVariant();
}

Issue IssuesModel::task(const QModelIndex &index) const
{
    if (!index.isValid())
        return Issue();
    return m_issues.at(index.row());
}

QStringList IssuesModel::categoryIds() const
{
    return m_categories.keys();
}

QString IssuesModel::categoryDisplayName(const QString &categoryId) const
{
    return m_categories.value(categoryId);
}

int IssuesModel::sizeOfFile(const QFont &font)
{
    QString fontKey = font.key();
    if (m_maxSizeOfFileName > 0 && fontKey == m_fileMeasurementFont)
        return m_maxSizeOfFileName;

    QFontMetrics fm(font);
    m_fileMeasurementFont = fontKey;

    foreach (const Issue & t, m_issues) {
        QString filename = t.file;
        const int pos = filename.lastIndexOf(QLatin1Char('/'));
        if (pos != -1)
            filename = filename.mid(pos +1);

        m_maxSizeOfFileName = qMax(m_maxSizeOfFileName, fm.width(filename));
    }
    return m_maxSizeOfFileName;
}

int IssuesModel::sizeOfLineNumber(const QFont &font)
{
    QString fontKey = font.key();
    if (m_sizeOfLineNumber == 0 || fontKey != m_lineMeasurementFont) {
        QFontMetrics fm(font);
        m_lineMeasurementFont = fontKey;
        m_sizeOfLineNumber = fm.width("8888");
    }
    return m_sizeOfLineNumber;
}

void IssuesModel::setFileNotFound(const QModelIndex &idx, bool b)
{
    if (idx.isValid() && idx.row() < m_issues.size()) {
        m_fileNotFound.insert(m_issues[idx.row()].file, b);
        emit dataChanged(idx, idx);
    }
}

IssuesFilterModel::IssuesFilterModel(IssuesModel *sourceModel, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setSourceModel(sourceModel);
    setDynamicSortFilter(true);
    m_includeUnknowns = m_includeWarnings = m_includeErrors = true;
}

IssuesModel *IssuesFilterModel::issuesModel() const
{
    return static_cast<IssuesModel*>(sourceModel());
}

bool IssuesFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    bool accept = true;

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    Issue::IssueType type = Issue::IssueType(index.data(IssuesModel::Type).toInt());
    switch (type) {
    case Issue::Unknown:
        accept = m_includeUnknowns;
        break;
    case Issue::Warning:
        accept = m_includeWarnings;
        break;
    case Issue::Error:
        accept = m_includeErrors;
        break;
    }

    const QString &categoryId = index.data(IssuesModel::Category).toString();
    if (m_categoryIds.contains(categoryId))
        accept = false;

    return accept;
}

IssueItemDelegate::IssueItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

IssueItemDelegate::~IssueItemDelegate()
{
}

QSize IssueItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QFontMetrics fm(option.font);
    int fontHeight = fm.height();
    int fontLeading = fm.leading();

    const QAbstractItemView * view = qobject_cast<const QAbstractItemView *>(opt.widget);
    IssuesModel *model = static_cast<IssuesFilterModel *>(view->model())->issuesModel();
//    TaskModel *model = static_cast<TaskModel *>(view->model());
    Positions positions(option, model);

    QSize s;
    s.setWidth(option.rect.width());
    if (view->selectionModel()->currentIndex() == index) {
        QString description = index.data(IssuesModel::Description).toString();
        // Layout the description
        int leading = fontLeading;
        int height = 0;
        description.replace('\n', QChar::LineSeparator);
        QTextLayout tl(description);
        tl.beginLayout();
        while (true) {
            QTextLine line = tl.createLine();
            if (!line.isValid())
                break;
            line.setLineWidth(positions.textAreaWidth());
            height += leading;
            line.setPosition(QPoint(0, height));
            height += static_cast<int>(line.height());
        }
        tl.endLayout();

        s.setHeight(height + leading + fontHeight + 3);
    } else {
        s.setHeight(fontHeight + 3);
    }
    if (s.height() < positions.minimumHeight())
        s.setHeight(positions.minimumHeight());
    return s;
}

void IssueItemDelegate::emitSizeHintChanged(const QModelIndex &index)
{
    emit sizeHintChanged(index);
}

void IssueItemDelegate::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit sizeHintChanged(current);
    emit sizeHintChanged(previous);
}

void IssueItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);
    painter->save();

    QFontMetrics fm(opt.font);
    QColor backgroundColor;
    QColor textColor;

    const QAbstractItemView * view = qobject_cast<const QAbstractItemView *>(opt.widget);
    bool selected = view->selectionModel()->currentIndex() == index;

    if (selected) {
        painter->setBrush(opt.palette.highlight().color());
        backgroundColor = opt.palette.highlight().color();
    } else {
        painter->setBrush(opt.palette.background().color());
        backgroundColor = opt.palette.background().color();
    }
    painter->setPen(Qt::NoPen);
    painter->drawRect(opt.rect);

    // Set Text Color
    if (selected)
        textColor = opt.palette.highlightedText().color();
    else
        textColor = opt.palette.text().color();

    painter->setPen(textColor);

    IssuesModel *model = static_cast<IssuesFilterModel *>(view->model())->issuesModel();
//    TaskModel *model = static_cast<TaskModel *>(view->model());
    Positions positions(opt, model);

    // Paint TaskIconArea:
    QIcon icon = index.data(IssuesModel::Icon).value<QIcon>();
    painter->drawPixmap(positions.left(), positions.top(),
                        icon.pixmap(positions.taskIconWidth(), positions.taskIconHeight()));

    // Paint TextArea:
    if (!selected) {
        // in small mode we lay out differently
        QString bottom = index.data(IssuesModel::Description).toString().split('\n').first();
        painter->setClipRect(positions.textArea());
        painter->drawText(positions.textAreaLeft(), positions.top() + fm.ascent(), bottom);
        if (fm.width(bottom) > positions.textAreaWidth()) {
            // draw a gradient to mask the text
            int gradientStart = positions.textAreaRight() - ELLIPSIS_GRADIENT_WIDTH + 1;
            QLinearGradient lg(gradientStart, 0, gradientStart + ELLIPSIS_GRADIENT_WIDTH, 0);
            lg.setColorAt(0, Qt::transparent);
            lg.setColorAt(1, backgroundColor);
            painter->fillRect(gradientStart, positions.top(), ELLIPSIS_GRADIENT_WIDTH, positions.firstLineHeight(), lg);
        }
    } else {
        // Description
        QString description = index.data(IssuesModel::Description).toString();
        // Layout the description
        int leading = fm.leading();
        int height = 0;
        description.replace('\n', QChar::LineSeparator);
        QTextLayout tl(description);
        tl.setAdditionalFormats(index.data(IssuesModel::Task_t).value<Issue>().formats);
        tl.beginLayout();
        while (true) {
            QTextLine line = tl.createLine();
            if (!line.isValid())
                break;
            line.setLineWidth(positions.textAreaWidth());
            height += leading;
            line.setPosition(QPoint(0, height));
            height += static_cast<int>(line.height());
        }
        tl.endLayout();
        tl.draw(painter, QPoint(positions.textAreaLeft(), positions.top()));

        QColor mix;
        mix.setRgb( static_cast<int>(0.7 * textColor.red()   + 0.3 * backgroundColor.red()),
                static_cast<int>(0.7 * textColor.green() + 0.3 * backgroundColor.green()),
                static_cast<int>(0.7 * textColor.blue()  + 0.3 * backgroundColor.blue()));
        painter->setPen(mix);

        const QString directory = QDir::toNativeSeparators(index.data(IssuesModel::File).toString());
        int secondBaseLine = positions.top() + fm.ascent() + height + leading;
        if (index.data(IssuesModel::FileNotFound).toBool()) {
            QString fileNotFound = tr("File not found: %1").arg(directory);
            painter->setPen(Qt::red);
            painter->drawText(positions.textAreaLeft(), secondBaseLine, fileNotFound);
        } else {
            painter->drawText(positions.textAreaLeft(), secondBaseLine, directory);
        }
    }
    painter->setPen(textColor);

    // Paint FileArea
    QString file = index.data(IssuesModel::File).toString();
    const int pos = file.lastIndexOf(QLatin1Char('/'));
    if (pos != -1)
        file = file.mid(pos +1);
    const int realFileWidth = fm.width(file);
    painter->setClipRect(positions.fileArea());
    painter->drawText(qMin(positions.fileAreaLeft(), positions.fileAreaRight() - realFileWidth),
                      positions.top() + fm.ascent(), file);
    if (realFileWidth > positions.fileAreaWidth()) {
        // draw a gradient to mask the text
        int gradientStart = positions.fileAreaLeft() - 1;
        QLinearGradient lg(gradientStart + ELLIPSIS_GRADIENT_WIDTH, 0, gradientStart, 0);
        lg.setColorAt(0, Qt::transparent);
        lg.setColorAt(1, backgroundColor);
        painter->fillRect(gradientStart, positions.top(), ELLIPSIS_GRADIENT_WIDTH, positions.firstLineHeight(), lg);
    }

    // Paint LineArea
    QString lineText = index.data(IssuesModel::Line).toString();
    painter->setClipRect(positions.lineArea());
    const int realLineWidth = fm.width(lineText);
    painter->drawText(positions.lineAreaRight() - realLineWidth, positions.top() + fm.ascent(), lineText);
    painter->setClipRect(opt.rect);

    // Separator lines
    painter->setPen(QColor::fromRgb(150,150,150));
    painter->drawLine(0, opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());
    painter->restore();
}

IssuesView::IssuesView( QWidget *parent )
    : QListView(parent)
{
    m_model = new IssuesModel();
    m_filterModel = new IssuesFilterModel(m_model);
    setModel(m_filterModel);
    IssueItemDelegate *tld = new IssueItemDelegate(this);
    setItemDelegate(tld);

    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            tld, SLOT(currentChanged(QModelIndex,QModelIndex)));
}

IssuesView::~IssuesView()
{ }

void IssuesView::clearIssues( const QString &categoryId )
{
    m_model->clearIssues(categoryId);
}

void IssuesView::addIssue( const Issue &issue )
{
    m_model->addIssue(issue);
}

Issue IssuesView::issue( const QModelIndex &index ) const
{ return m_filterModel->issue(index); }

void IssuesView::setFileNotFound( const QModelIndex &index, bool b )
{ m_model->setFileNotFound(index, b); }

int IssuesView::issuesCount() const
{ return m_model->issuesCount(); }

void IssuesView::resizeEvent( QResizeEvent *e )
{
    Q_UNUSED(e)
    static_cast<IssueItemDelegate *>(itemDelegate())->emitSizeHintChanged(selectionModel()->currentIndex());
}

void IssuesView::keyPressEvent( QKeyEvent *e )
{
    if ( !e->modifiers() && e->key() == Qt::Key_Return ) {
        emit activated(currentIndex());
        e->accept();
        return;
    }
    QListView::keyPressEvent(e);
}

void IssuesView::setFilterIncludesWarnings( bool onoff )
{
    m_filterModel->setFilterIncludesWarnings(onoff);
}

}   // namespace Utils
