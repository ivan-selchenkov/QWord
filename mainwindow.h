#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QQueue>
#include <QSystemTrayIcon>
#include <QFileInfo>

struct DictItem
{
    QString l1;
    QString l2;
    bool isl2Origin; // if true, combination will not be use in learn mode
    int id;
    int random;
    bool operator== ( const DictItem & other ) const {
        if(l1 == other.l1 && l2 == other.l2)
        {
            return true;
        }
        else
            return false;
    }
};

namespace Ui
{
    class MainWindowClass;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString database;
    QList<DictItem> list;
    QQueue<DictItem> queue;
    QQueue<DictItem> main_queue;
    QFileInfo fileInfo;

    //******* Inquiry
    int inquiryDelay;
    int inquiryNumWords;
private:
    Ui::MainWindowClass *ui;

    int countErrors;
    int countTotal;

    void createActions();
    void LoadDict();
    void createTrayIcon();

    void setUiEnabled(bool value);
    bool getItem(DictItem&);

    DictItem current;
    bool error;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *quitAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;

    // for hidden mode
    int inquiryCount;
    void startInquiry();
    void checkAnswer();
    bool isHidden;
    QTimer* timer;
private slots:
    void on_actionHidden_triggered();
    void on_actionNormal_triggered();
    void on_actionNew_triggered();
    void on_actionShow_Dict_triggered();
    void on_txtAnswer_returnPressed();
    void on_btnStart_clicked();
    void on_pushButton_clicked();
    void on_actionAdd_words_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_newWord(DictItem);
    void on_delete_item(int);
    void on_next_question();

signals:
    void signal_updateQueue();
};

#endif // MAINWINDOW_H
