#ifndef EDITABLESQLMODEL_H
#define EDITABLESQLMODEL_H

#include <QtSql>

class EditableSqlModel : public QSqlQueryModel
 {
     Q_OBJECT

 public:
    EditableSqlModel(QObject *parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int /* role */);
    void refresh();
    bool setL(int Id, const QString &str, int);
 };


#endif // EDITABLESQLMODEL_H
