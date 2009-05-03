#ifndef SHOWDIALOG_H
#define SHOWDIALOG_H

#include <QtGui/QDialog>
#include "mainwindow.h"

namespace Ui {
    class ShowDialog;
}

class ShowDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ShowDialog)
public:
    explicit ShowDialog(QWidget *parent = 0);
    virtual ~ShowDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ShowDialog *m_ui;

private slots:
    void on_btnDelete_clicked();
    void on_btnClose_clicked();
public slots:
    void on_queue_update();
signals:
    void signalDeleteItem(int);
};

#endif // SHOWDIALOG_H
