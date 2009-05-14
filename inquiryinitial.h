#ifndef INQUIRYINITIAL_H
#define INQUIRYINITIAL_H

#include <QtGui/QDialog>

namespace Ui {
    class InquiryInitial;
}

class InquiryInitial : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(InquiryInitial)
public:
    explicit InquiryInitial(QWidget *parent = 0);
    virtual ~InquiryInitial();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::InquiryInitial *m_ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // INQUIRYINITIAL_H
