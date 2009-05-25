#ifndef DICTIONARYEDIT_H
#define DICTIONARYEDIT_H

#include <QtGui/QDialog>
#include <QtSql>
#include "editablesqlmodel.h"

namespace Ui {
    class DictionaryEdit;
}

class DictionaryEdit : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DictionaryEdit)
public:
    explicit DictionaryEdit(QWidget *parent = 0);
    virtual ~DictionaryEdit();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DictionaryEdit *m_ui;
    QSqlDatabase db;
    EditableSqlModel* model;
};
class SimpleDelegate : public QItemDelegate
{
public:
    SimpleDelegate(QObject* pobj = 0) : QItemDelegate(pobj)
    {
    }
    QWidget* createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        return QItemDelegate::createEditor (parent, option, index );
    }

};

#endif // DICTIONARYEDIT_H
