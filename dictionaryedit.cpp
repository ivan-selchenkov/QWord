#include "dictionaryedit.h"
#include "ui_dictionaryedit.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QItemDelegate>

DictionaryEdit::DictionaryEdit(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DictionaryEdit)
{
    m_ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(((MainWindow*)parent)->database);

    if(!db.open())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database error"));
        msgBox.exec();
        return;
    }

    model = new EditableSqlModel(this);

    model->setQuery("SELECT * FROM dict");
    model->setHeaderData(1, Qt::Horizontal, tr("First phrase"));
    model->setHeaderData(2, Qt::Horizontal, tr("Second phrase"));

    SimpleDelegate *delegate = new SimpleDelegate(this);

    m_ui->viewDict->setItemDelegate(delegate);
    m_ui->viewDict->setEditTriggers(QAbstractItemView::DoubleClicked);    

    m_ui->viewDict->setModel(model);
    m_ui->viewDict->setColumnHidden(0, true);
    m_ui->viewDict->setColumnHidden(3, true);
}

DictionaryEdit::~DictionaryEdit()
{
    delete m_ui;
}

void DictionaryEdit::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
