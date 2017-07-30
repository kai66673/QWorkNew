#ifndef ISSUESVIEW_H
#define ISSUESVIEW_H

#include <QListView>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QStyledItemDelegate>
#include <QTextLayout>
#include <QSortFilterProxyModel>

#include "utils_global.h"

namespace Utils {

class QTCREATOR_UTILS_EXPORT Issue
{
public:
    enum IssueType {
        Unknown,
        Error,
        Warning
    };

    Issue();
    Issue(IssueType type_, const QString &description_,
         const QString &file_, int line_, int column_, const QString &category_);

    bool isNull() const;

    unsigned int taskId;
    IssueType type;
    QString description;
    QString file;
    int line;
    int column;
    QString category;
    // Having a QList<QTextLayout::FormatRange> in Task isn't that great
    // It would be cleaner to split up the text into
    // the logical hunks and then assemble them again
    // (That is different consumers of tasks could show them in
    // different ways!)
    // But then again, the wording of the text most likely
    // doesn't work if you split it up, nor are our parsers
    // anywhere near being that good
    QList<QTextLayout::FormatRange> formats;
private:
    static unsigned int s_nextId;
};

bool operator==(const Issue &t1, const Issue &t2);
uint qHash(const Issue &task);

class IssuesModel : public QAbstractItemModel
{
public:
    // Model stuff
    IssuesModel();
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Issue task(const QModelIndex &index) const;

    QStringList categoryIds() const;
    QString categoryDisplayName(const QString &categoryId) const;
    void addCategory(const QString &categoryId, const QString &categoryName);

    QList<Issue> tasks(const QString &categoryId = QString()) const;
    void addIssue(const Issue &issue);
    void removeTask(const Issue &task);
    void clearIssues(const QString &categoryId = QString());

    int sizeOfFile(const QFont &font);
    int sizeOfLineNumber(const QFont &font);
    void setFileNotFound(const QModelIndex &index, bool b);

    enum Roles { File = Qt::UserRole, Line, Description, FileNotFound, Type, Category, Icon, Task_t };

    QIcon taskTypeIcon(Issue::IssueType t) const;

    int issuesCount() const;
    int errorIssuesCount();

    bool hasFile(const QModelIndex &index) const;

private:
    QHash<QString,QString> m_categories; // category id -> display name
    QList<Issue> m_issues;   // all tasks (in order of insertion)
    QMap<QString,QList<Issue> > m_issuesInCategory; // categoryId->tasks

    QHash<QString,bool> m_fileNotFound;
    int m_maxSizeOfFileName;
    QString m_fileMeasurementFont;
    const QIcon m_errorIcon;
    const QIcon m_warningIcon;
    int m_issuesCount;
    int m_errorIssuesCount;
    int m_sizeOfLineNumber;
    QString m_lineMeasurementFont;
};

class IssuesFilterModel : public QSortFilterProxyModel
{
public:
    IssuesFilterModel(IssuesModel *sourceModel, QObject *parent = 0);

    IssuesModel *issuesModel() const;

    bool filterIncludesUnknowns() const { return m_includeUnknowns; }
    void setFilterIncludesUnknowns(bool b) { m_includeUnknowns = b; invalidateFilter(); }

    bool filterIncludesWarnings() const { return m_includeWarnings; }
    void setFilterIncludesWarnings(bool b) { m_includeWarnings = b; invalidateFilter(); }

    bool filterIncludesErrors() const { return m_includeErrors; }
    void setFilterIncludesErrors(bool b) { m_includeErrors = b; invalidateFilter(); }

    QStringList filteredCategories() const { return m_categoryIds; }
    void setFilteredCategories(const QStringList &categoryIds) { m_categoryIds = categoryIds; invalidateFilter(); }

    Issue issue(const QModelIndex &index) const
    { return static_cast<IssuesModel *>(sourceModel())->task(mapToSource(index)); }

    bool hasFile(const QModelIndex &index) const
    { return static_cast<IssuesModel *>(sourceModel())->hasFile(mapToSource(index)); }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    bool m_includeUnknowns;
    bool m_includeWarnings;
    bool m_includeErrors;
    QStringList m_categoryIds;
};

class IssueItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    IssueItemDelegate(QObject * parent = 0);
    ~IssueItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // TaskView uses this method if the size of the taskview changes
    void emitSizeHintChanged(const QModelIndex &index);

public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void generateGradientPixmap(int width, int height, QColor color, bool selected) const;

    /*
      Collapsed:
      +----------------------------------------------------------------------------------------------------+
      | TASKICONAREA  TEXTAREA                                                           FILEAREA LINEAREA |
      +----------------------------------------------------------------------------------------------------+

      Expanded:
      +----------------------------------------------------------------------------------------------------+
      | TASKICONICON  TEXTAREA                                                           FILEAREA LINEAREA |
      |               more text -------------------------------------------------------------------------> |
      +----------------------------------------------------------------------------------------------------+
     */
    class Positions
    {
    public:
        Positions(const QStyleOptionViewItemV4 &options, IssuesModel *model) :
            m_totalWidth(options.rect.width()),
            m_maxFileLength(model->sizeOfFile(options.font)),
            m_maxLineLength(model->sizeOfLineNumber(options.font)),
            m_realFileLength(m_maxFileLength),
            m_top(options.rect.top()),
            m_bottom(options.rect.bottom())
        {
            int flexibleArea = lineAreaLeft() - textAreaLeft() - ITEM_SPACING;
            if (m_maxFileLength > flexibleArea / 2)
                m_realFileLength = flexibleArea / 2;
            m_fontHeight = QFontMetrics(options.font).height();
        }

        int top() const { return m_top + ITEM_MARGIN; }
        int left() const { return ITEM_MARGIN; }
        int right() const { return m_totalWidth - ITEM_MARGIN; }
        int bottom() const { return m_bottom; }
        int firstLineHeight() const { return m_fontHeight + 1; }
        int minimumHeight() const { return taskIconHeight() + 2 * ITEM_MARGIN; }

        int taskIconLeft() const { return left(); }
        int taskIconWidth() const { return TASK_ICON_SIZE; }
        int taskIconHeight() const { return TASK_ICON_SIZE; }
        int taskIconRight() const { return taskIconLeft() + taskIconWidth(); }
        QRect taskIcon() const { return QRect(taskIconLeft(), top(), taskIconWidth(), taskIconHeight()); }

        int textAreaLeft() const { return taskIconRight() + ITEM_SPACING; }
        int textAreaWidth() const { return textAreaRight() - textAreaLeft(); }
        int textAreaRight() const { return fileAreaLeft() - ITEM_SPACING; }
        QRect textArea() const { return QRect(textAreaLeft(), top(), textAreaWidth(), firstLineHeight()); }

        int fileAreaLeft() const { return fileAreaRight() - fileAreaWidth(); }
        int fileAreaWidth() const { return m_realFileLength; }
        int fileAreaRight() const { return lineAreaLeft() - ITEM_SPACING; }
        QRect fileArea() const { return QRect(fileAreaLeft(), top(), fileAreaWidth(), firstLineHeight()); }

        int lineAreaLeft() const { return lineAreaRight() - lineAreaWidth(); }
        int lineAreaWidth() const { return m_maxLineLength; }
        int lineAreaRight() const { return right(); }
        QRect lineArea() const { return QRect(lineAreaLeft(), top(), lineAreaWidth(), firstLineHeight()); }

    private:
        int m_totalWidth;
        int m_maxFileLength;
        int m_maxLineLength;
        int m_realFileLength;
        int m_top;
        int m_bottom;
        int m_fontHeight;

        static const int TASK_ICON_SIZE = 16;
        static const int ITEM_MARGIN = 2;
        static const int ITEM_SPACING = 2 * ITEM_MARGIN;
    };
};

class QTCREATOR_UTILS_EXPORT IssuesView: public QListView
{
    Q_OBJECT

public:
    explicit IssuesView( QWidget *parent = 0 );
    ~IssuesView();

    void clearIssues( const QString &categoryId = QString() );
    void addIssue( const Issue &issue );
    Issue issue( const QModelIndex &index ) const;
    void setFileNotFound(const QModelIndex &idx, bool b );
    int issuesCount() const;

protected:
    void resizeEvent( QResizeEvent *e );
    void keyPressEvent( QKeyEvent *e );

signals:

public slots:
    void setFilterIncludesWarnings( bool onoff );

private:
    IssuesModel *m_model;
    IssuesFilterModel *m_filterModel;
};

}   // namespace Utils

Q_DECLARE_METATYPE(Utils::Issue)

#endif // ISSUESVIEW_H
