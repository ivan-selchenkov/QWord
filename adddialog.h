#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include "mainwindow.h"
#include <QtGui/QDialog>

namespace Ui {
    class AddDialog;
}

class AddDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AddDialog)
public:
    explicit AddDialog(QWidget *parent = 0);
    virtual ~AddDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AddDialog *m_ui;
    void saveValues();
private slots:
    void on_rbEs_toggled(bool checked);
    void on_txtRu_returnPressed();
    void on_txtEs_returnPressed();
    void on_btnNext_clicked();
    void on_lineEdit_2_returnPressed();
    void on_lineEdit_returnPressed();
    void on_btnClose_clicked();
signals:
    void signalNewWord(DictItem);
};

#endif // ADDDIALOG_H
