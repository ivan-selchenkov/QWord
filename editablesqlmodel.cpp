#include "editablesqlmodel.h"

EditableSqlModel::EditableSqlModel(QObject *parent = 0): QSqlQueryModel(parent)
{
}
Qt::ItemFlags EditableSqlModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    flags |= Qt::ItemIsEditable;
    return flags;
}
bool EditableSqlModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() < 1 || index.column() > 2) // changing l1 & l2 only
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt();

    clear();

    bool ok;
    if (index.column() == 1) {
        ok = setL(id, value.toString(), 1);
    } else {
        ok = setL(id, value.toString(), 2);
    }
    refresh();
    return ok;
 }
void EditableSqlModel::refresh()
 {
    setQuery("SELECT * FROM dict");
    setHeaderData(1, Qt::Horizontal, tr("First phrase"));
    setHeaderData(2, Qt::Horizontal, tr("Second phrase"));
 }

 bool EditableSqlModel::setL(int Id, const QString &str, int l)
 {
    QSqlQuery query;
    if(l == 1)
        query.prepare("update dict set  l1 = ? where id = ?");
    else
        query.prepare("update dict set  l2 = ? where id = ?");
    query.addBindValue(str);
    query.addBindValue(Id);
    return query.exec();
 }
