#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AddDialog)
{
    m_ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete m_ui;
}

void AddDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AddDialog::on_btnClose_clicked()
{
    this->close();
}

void AddDialog::on_lineEdit_returnPressed()
{
    saveValues();
}

void AddDialog::on_lineEdit_2_returnPressed()
{
    saveValues();
}

void AddDialog::on_btnNext_clicked()
{
    saveValues();
}
void AddDialog::saveValues()
{
    if(m_ui->txtEs->text() == "" || m_ui->txtRu->text() == "") return;

    DictItem di;
    di.l1 = m_ui->txtEs->text();
    di.l2 = m_ui->txtRu->text();
    di.isl2Origin = m_ui->rbRu->isChecked();

    emit signalNewWord(di);

    if(m_ui->cboxRevert->isChecked())
    {
        di.l2 = m_ui->txtEs->text();
        di.l1 = m_ui->txtRu->text();
        di.isl2Origin = !m_ui->rbRu->isChecked();
        emit signalNewWord(di);
    }

    m_ui->txtEs->setText("");
    m_ui->txtRu->setText("");
    m_ui->txtEs->setFocus();
}

void AddDialog::on_txtEs_returnPressed()
{
    saveValues();    
}

void AddDialog::on_txtRu_returnPressed()
{
        saveValues();
}

void AddDialog::on_rbEs_toggled(bool checked)
{
    //m_ui->rbRu->setChecked(!checked);
}
