#ifndef REPORTINPUTPARAMETERS_H
#define REPORTINPUTPARAMETERS_H

#include <QTableView>
#include <QItemDelegate>
#include <QAbstractTableModel>

namespace RPML {

struct ReportInputParameter
{
    enum ParameterType {
        String = 0,
        Numeric,
        Date,
        Time,
        DateTime
    };

    enum ParameterMandatory {
        Required = 0,
        Optional
    };

    ReportInputParameter( int id_,
                          QString description_,
                          int type_ = String,
                          int mandatory_ = Required,
                          QVariant defaultValue_ = QVariant() )
        : id(id_)
        , description(description_)
        , type(type_)
        , mandatory(mandatory_)
        , defaultValue(defaultValue_)
    {}

    int id;
    QString description;
    int type;
    int mandatory;
    QVariant defaultValue;
};

struct InputParameterValue
{
    InputParameterValue( bool disabled_ = true , const QVariant &value_ = QVariant() )
        : disabled(disabled_)
        , value(value_)
    {}

    bool disabled;
    QVariant value;
};

class ReportInputParameterTableView : public QTableView
{
    Q_OBJECT

public:
    explicit ReportInputParameterTableView( QWidget *parent = 0 );
    
signals:
    void insertInputParameter( int index );

protected:
    virtual void mouseDoubleClickEvent( QMouseEvent *event );
};

class ReportInputParameterDelegate : public QItemDelegate
{
public:
    ReportInputParameterDelegate( QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index ) const;

    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index ) const;
};

class ReportInputParametersModel: public QAbstractTableModel
{
public:
    ReportInputParametersModel( QList<ReportInputParameter *> &parameters, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data( const QModelIndex &index, int role  = Qt::DisplayRole ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role );

    void startResetModel();
    void finishResetModel();

    void addInputParameter();
    void delInputParameter( int row );

private:
    QString typeDescription( int t ) const;
    QString mandatoryDescription( int m ) const;
    bool checkDefaultValue( int type, const QString &valString ) const;
    void clearDefaultValue( int row );
    QList<ReportInputParameter *> &m_parameters;
};

}   // namespace RPML

#endif // REPORTINPUTPARAMETERS_H
