#include <QMenu>
#include <QTreeView>
#include <QSplitter>

#include "SnippetsDockWidget.h"
#include "snippets/snippetscollection.h"
#include "snippets/snippeteditor.h"
#include "snippets/SnippetsTableModel.h"
#include "Core.h"

using namespace TextEditor;
using namespace TextEditor::Internal;

// SnippetsSettingsPagePrivate
class SnippetsSettingsPagePrivate : public QSplitter
{
    Q_OBJECT
public:
    SnippetsSettingsPagePrivate(QComboBox *groupCombo, QAction *actAdd, QAction *actDelete, QWidget *parent = 0);
    ~SnippetsSettingsPagePrivate() { delete m_model; }

    const QString &displayName() const { return m_displayName; }
    void configureUi();

    void apply();
    void finish();

    QPointer<QWidget> m_widget;

private slots:
    void loadSnippetGroup(int index);
    void markSnippetsCollection();
    void addSnippet();
    void removeSnippet();
    void revertBuiltInSnippet();
    void restoreRemovedBuiltInSnippets();
    void selectSnippet(const QModelIndex &parent, int row);
    void selectMovedSnippet(const QModelIndex &, int, int, const QModelIndex &, int row);
    void setSnippetContent();
    void updateCurrentSnippetDependent(const QModelIndex &modelIndex = QModelIndex());

private:
    SnippetEditorWidget *currentEditor() const;
    SnippetEditorWidget *editorAt(int i) const;

    const QString m_displayName;
    const QString m_settingsPrefix;
    SnippetsTableModel *m_model;
    bool m_snippetsCollectionChanged;

    QStackedWidget *m_snippetsEditorStack;
    QTreeView *m_snippetsTable;

    QComboBox *m_groupCombo;
    QAction *m_actAdd;
    QAction *m_actDelete;
};

SnippetsSettingsPagePrivate::SnippetsSettingsPagePrivate(QComboBox *groupCombo, QAction *actAdd, QAction *actDelete, QWidget *parent)
    : QSplitter(parent)
    ,  m_displayName(tr("Snippets"))
    , m_settingsPrefix(QLatin1String("Text"))
    , m_model(new SnippetsTableModel(0))
    , m_snippetsCollectionChanged(false)
    , m_groupCombo(groupCombo)
    , m_actAdd(actAdd)
    , m_actDelete(actDelete)
{
    configureUi();
}

SnippetEditorWidget *SnippetsSettingsPagePrivate::currentEditor() const
{
    return editorAt(m_snippetsEditorStack->currentIndex());
}

SnippetEditorWidget *SnippetsSettingsPagePrivate::editorAt(int i) const
{
    return static_cast<SnippetEditorWidget *>(m_snippetsEditorStack->widget(i));
}

void SnippetsSettingsPagePrivate::configureUi()
{
    setOrientation(Qt::Vertical);
    m_snippetsTable = new QTreeView(this);
    m_snippetsTable->setFrameShape(QFrame::NoFrame);
    m_snippetsTable->setLineWidth(0);
    m_snippetsEditorStack = new QStackedWidget(this);
    m_snippetsEditorStack->setFrameShape(QFrame::NoFrame);
    m_snippetsEditorStack->setLineWidth(0);
    addWidget(m_snippetsTable);
    addWidget(m_snippetsEditorStack);

    setStyleSheet("QSplitter::handle:horizontal { width: 1px; color: black; }\n"
                  "QSplitter::handle:vertical { height: 1px; color: black; }");

    TextEditor::Internal::SnippetsCollection *snippetsCollection = TextEditor::Internal::SnippetsCollection::instance();

    snippetsCollection->identifyGroups();

    foreach (ISnippetProvider *provider, snippetsCollection->providers()) {
        m_groupCombo->addItem(provider->displayName(), provider->groupId());
        SnippetEditorWidget *snippetEditor = new SnippetEditorWidget(this);
        provider->decorateEditor(snippetEditor);
        m_snippetsEditorStack->insertWidget(m_groupCombo->count() - 1, snippetEditor);
        connect(snippetEditor, SIGNAL(snippetContentChanged()), this, SLOT(setSnippetContent()));
    }

    m_snippetsTable->setModel(m_model);
//    new Utils::HeaderViewStretcher(m_ui.snippetsTable->header(), 1);


    loadSnippetGroup(m_groupCombo->currentIndex());

    connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(selectSnippet(QModelIndex,int)));
    connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(markSnippetsCollection()));
    connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(markSnippetsCollection()));
    connect(m_model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(selectMovedSnippet(QModelIndex,int,int,QModelIndex,int)));
    connect(m_model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(markSnippetsCollection()));
    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(markSnippetsCollection()));
    connect(m_model, SIGNAL(modelReset()), this, SLOT(updateCurrentSnippetDependent()));
    connect(m_model, SIGNAL(modelReset()), this, SLOT(markSnippetsCollection()));

    connect(m_groupCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSnippetGroup(int)));
    connect(m_actAdd, SIGNAL(triggered()), this, SLOT(addSnippet()));
    connect(m_actDelete, SIGNAL(triggered()), this, SLOT(removeSnippet()));
    connect(m_snippetsTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(updateCurrentSnippetDependent(QModelIndex)));
}

void SnippetsSettingsPagePrivate::apply()
{
    if (currentEditor()->document()->isModified())
        setSnippetContent();

    if (m_snippetsCollectionChanged) {
        QString errorString;
        if (SnippetsCollection::instance()->synchronize(&errorString))
            m_snippetsCollectionChanged = false;
        else
            Core::Storage::warning(tr("Error While Saving Snippet Collection:\n%1").arg(errorString));
    }
}

void SnippetsSettingsPagePrivate::finish()
{
    if (m_snippetsCollectionChanged) {
        SnippetsCollection::instance()->reload();
        m_snippetsCollectionChanged = false;
    }
}

void SnippetsSettingsPagePrivate::loadSnippetGroup(int index)
{
    if (index == -1)
        return;

    m_snippetsEditorStack->setCurrentIndex(index);
    currentEditor()->clear();
    m_model->load(m_groupCombo->itemData(index).toString());
}

void SnippetsSettingsPagePrivate::markSnippetsCollection()
{
    if (!m_snippetsCollectionChanged)
        m_snippetsCollectionChanged = true;
}

void SnippetsSettingsPagePrivate::addSnippet()
{
    const QModelIndex &modelIndex = m_model->createSnippet();
    selectSnippet(QModelIndex(), modelIndex.row());
    m_snippetsTable->edit(modelIndex);
}

void SnippetsSettingsPagePrivate::removeSnippet()
{
    const QModelIndex &modelIndex = m_snippetsTable->selectionModel()->currentIndex();
    if (!modelIndex.isValid()) {
        QMessageBox::critical(0, tr("Error"), tr("No snippet selected."));
        return;
    }
    m_model->removeSnippet(modelIndex);
}

void SnippetsSettingsPagePrivate::restoreRemovedBuiltInSnippets()
{
    m_model->restoreRemovedBuiltInSnippets();
}

void SnippetsSettingsPagePrivate::revertBuiltInSnippet()
{
    m_model->revertBuitInSnippet(m_snippetsTable->selectionModel()->currentIndex());
}

void SnippetsSettingsPagePrivate::selectSnippet(const QModelIndex &parent, int row)
{
    QModelIndex topLeft = m_model->index(row, 0, parent);
    QModelIndex bottomRight = m_model->index(row, 1, parent);
    QItemSelection selection(topLeft, bottomRight);
    m_snippetsTable->selectionModel()->select(selection, QItemSelectionModel::SelectCurrent);
    m_snippetsTable->setCurrentIndex(topLeft);
    m_snippetsTable->scrollTo(topLeft);
}

void SnippetsSettingsPagePrivate::selectMovedSnippet(const QModelIndex &,
                                                     int sourceRow,
                                                     int,
                                                     const QModelIndex &destinationParent,
                                                     int destinationRow)
{
    QModelIndex modelIndex;
    if (sourceRow < destinationRow)
        modelIndex = m_model->index(destinationRow - 1, 0, destinationParent);
    else
        modelIndex = m_model->index(destinationRow, 0, destinationParent);
    m_snippetsTable->scrollTo(modelIndex);
    currentEditor()->setPlainText(m_model->snippetAt(modelIndex).content());
}

void SnippetsSettingsPagePrivate::updateCurrentSnippetDependent(const QModelIndex &modelIndex)
{
    if (modelIndex.isValid()) {
        const Snippet &snippet = m_model->snippetAt(modelIndex);
        currentEditor()->setPlainText(snippet.content());
    } else {
        currentEditor()->clear();
    }
}

void SnippetsSettingsPagePrivate::setSnippetContent()
{
    const QModelIndex &modelIndex = m_snippetsTable->selectionModel()->currentIndex();
    if (modelIndex.isValid()) {
        m_model->setSnippetContent(modelIndex, currentEditor()->toPlainText());
        markSnippetsCollection();
    }
}

SnippetsDockWidget::SnippetsDockWidget(QMainWindow *mainWindow, QComboBox *groupsComboBox,
                                       QAction *actAdd, QAction *actDelete)
    : QwCustomDockWidget(mainWindow, tr("Snippets"), "SnippetsDockWidget",
                         QKeySequence("CTRL+D,A"), QIcon(":/images/aliases_dock.png"),
                         Qt::BottomDockWidgetArea, QwCustomDockWidget::Second,
                         QList<QAction *>() << actAdd << actDelete, groupsComboBox)
    , m_groupsComboBox(groupsComboBox)
    , m_actAdd(actAdd)
    , m_actDelete(actDelete)
{
    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_actAdd);
    contextMenu->addAction(m_actDelete);
    setContextMenu(contextMenu);

    m_data = new SnippetsSettingsPagePrivate(m_groupsComboBox, m_actAdd, m_actDelete);

    setWidget(m_data);
}

void SnippetsDockWidget::synchronize()
{
    m_data->apply();
}

#include "SnippetsDockWidget.moc"
