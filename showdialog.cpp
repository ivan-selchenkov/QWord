#include "showdialog.h"
#include "ui_showdialog.h"

ShowDialog::ShowDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ShowDialog)
{
    m_ui->setupUi(this);    
    MainWindow* _mw = (MainWindow*) parent;

    setWindowTitle(QString("QWord - %1").arg(_mw->fileInfo.fileName()));

    m_ui->lstWords->clear();
    foreach(DictItem di, _mw->main_queue)
        m_ui->lstWords->addItem(QString("%1| %2 - %3").arg(di.id).arg(di.l1).arg(di.l2));
}

ShowDialog::~ShowDialog()
{
    delete m_ui;
}
void ShowDialog::on_queue_update()
{
    m_ui->lstWords->clear();
    foreach(DictItem di, ((MainWindow*) this->parent())->main_queue)
        m_ui->lstWords->addItem(QString("%1| %2 - %3").arg(di.id).arg(di.l1).arg(di.l2));
}
void ShowDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ShowDialog::on_btnClose_clicked()
{
    close();
}

void ShowDialog::on_btnDelete_clicked()
{
    QList<QListWidgetItem* > selected_list = m_ui->lstWords->selectedItems();
    QString str;
    QStringList sl;

    foreach(QListWidgetItem* si, selected_list)
    {
        str = si->text();
        sl = str.split('|');
        if(sl.size() > 0)
        {
            emit signalDeleteItem(sl.at(0).toInt());
        }
    }
}
