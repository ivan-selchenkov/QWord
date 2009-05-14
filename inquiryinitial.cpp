#include "inquiryinitial.h"
#include "ui_inquiryinitial.h"
#include "mainwindow.h"

InquiryInitial::InquiryInitial(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::InquiryInitial)
{
    m_ui->setupUi(this);
}

InquiryInitial::~InquiryInitial()
{
    delete m_ui;
}

void InquiryInitial::changeEvent(QEvent *e)
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

void InquiryInitial::on_buttonBox_accepted()
{
    ((MainWindow*) this->parent())->inquiryDelay = m_ui->spinMinutes->value();
    ((MainWindow*) this->parent())->inquiryNumWords = m_ui->spinNumbers->value();
}
