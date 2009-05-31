#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adddialog.h"
#include <QFileDialog>
#include <QtSql>
#include <QMessageBox>
#include "showdialog.h"
#include <QInputDialog>
#include "inquiryinitial.h"
#include "dictionaryedit.h"

bool lessThan(const DictItem &s1, const DictItem &s2);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    createActions();
    createTrayIcon();

    trayIcon->setIcon(this->windowIcon());
    trayIcon->show();

    isHidden = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_next_question()));
}

MainWindow::~MainWindow()
{
    QSettings settings;
    int x = this->x();
    int y = this->y();
    settings.setValue("X", x);
    settings.setValue("Y", y);

    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString str = QFileDialog::getOpenFileName(this, tr("Select dictionary..."), "", "*.db");
    if(str == "") return;
    if(!QFileInfo(str).exists()) {
        QMessageBox msgBox;
        msgBox.setText(tr("File not found."));
        msgBox.exec();
        return;
    }
    database = str;
    LoadDict();
}

void MainWindow::LoadDict()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    fileInfo.setFile(database);
    db.setDatabaseName(database);
    if(!db.open())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database error"));
        msgBox.exec();
        return;
    }
/*
  CREATE TABLE dict(id INTEGER PRIMARY KEY AUTOINCREMENT, l1 TEXT, l2 TEXT, origin INTEGER(1) NOT NULL DEFAULT 0);
  */
    QSqlQuery query(db);
    query.exec("SELECT * FROM dict");

    QSqlRecord rec = query.record();

    if(rec.indexOf("origin") == -1) { // old type of database
        QSqlQuery _query(db);
        if(!_query.exec("ALTER TABLE dict ADD origin INTEGER(1) NOT NULL DEFAULT 0"))
            qDebug() << "UPDATING STRUCTURE ERROR: " << _query.lastError();
            query.exec("SELECT * FROM dict");
            rec = query.record();
    }


    DictItem di;
    queue.clear();
    main_queue.clear();
    while(query.next())
    {
        di.l1 = query.value(rec.indexOf("l1")).toString().trimmed();
        di.l2 = query.value(rec.indexOf("l2")).toString().trimmed();
        di.isl2Origin = (query.value(rec.indexOf("origin")).toString() == "1") ? true : false;
        di.id = query.value(rec.indexOf("id")).toInt();
        qsrand(di.l1.size() + di.l2.size() + QTime::currentTime().msec());
        di.random = qrand();
        queue.enqueue(di);
    }

    DictItem __i;
    foreach(__i, queue)
        main_queue.append(__i);

    this->setWindowTitle(QString("QWord - %1").arg(fileInfo.fileName()));
    ui->actionAdd_words->setEnabled(true);
    ui->actionShow_Dict->setEnabled(true);
    ui->btnStart->setEnabled(true);
    //ui->menuStart->setEnabled(true);
    //ui->btnStartHidden->setEnabled(true);
    db.close();
}
bool lessThan(const DictItem &s1, const DictItem &s2)
{
    return (qrand() % 2 == 0)?true:false; //s1.random < s2.random;
}
void MainWindow::on_actionAdd_words_triggered()
{
    AddDialog dialog(this);
    connect(&dialog, SIGNAL(signalNewWord(DictItem)), this, SLOT(on_newWord(DictItem)));
    dialog.exec();
}
void MainWindow::on_newWord(DictItem _i)
{
    qsrand(_i.l1.size() + _i.l2.size() + QTime::currentTime().msec());
    _i.random = qrand();


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database);
    if(!db.open())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database error"));
        msgBox.exec();
        return;
    }
    DictItem di;
    foreach(di, main_queue)
    {
        if(di == _i)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Record exists..."));
            msgBox.exec();
            return;
        }
    }
/*
  CREATE TABLE dict(id INTEGER PRIMARY KEY AUTOINCREMENT, l1 TEXT, l2 TEXT, origin INTEGER(1) NOT NULL DEFAULT 0);
  */
    QSqlQuery query(db);

    query.prepare("INSERT INTO dict(l1, l2, origin) VALUES(:l1, :l2, :origin);");
    query.bindValue(":l1", _i.l1);
    query.bindValue(":l2", _i.l2);
    query.bindValue(":origin", (_i.isl2Origin == true) ? 1 : 0);

    if(!query.exec())
        qDebug() << "INSERTING ERROR: " << query.lastError();

    int id = query.lastInsertId().toInt();
    _i.id = id;
    main_queue.append(_i);


    db.close();
}
void MainWindow::on_delete_item(int id)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database);
    if(!db.open())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database error"));
        msgBox.exec();
        return;
    }
    for(int i=0; i< main_queue.size(); i++)
    {
        if(main_queue.at(i).id == id)
        {
            main_queue.removeAt(i);
            break;
        }
    }

    QSqlQuery query(db);

    query.prepare("DELETE FROM dict WHERE id = :id;");
    query.bindValue(":id", id);

    if(!query.exec())
        qDebug() << "DELETING ERROR: " << query.lastError();

    db.close();
    emit signal_updateQueue();
}
void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_btnStart_clicked()
{
    if(ui->chbTray->checkState() == Qt::Checked)
    {
        on_btnStartHidden_clicked();
        return;
    }
    queue.clear();

    DictItem __i;
    foreach(__i, main_queue)
        queue.append(__i);

    if(queue.size() <=0) return;
    qSort(queue.begin(), queue.end(), lessThan);
    setUiEnabled(false);

    if(!getItem(current)) return; // getting current item, else return

    if(ui->chbLearn->checkState() == Qt::Checked)
        ui->lblQuestion->setText(QString("%1 - %2").arg(current.l1).arg(current.l2));
    else
        ui->lblQuestion->setText(current.l1);

    countErrors = 0;
    countTotal = 1;

    error = false;
}
void MainWindow::startInquiry()
{
    queue.clear();

    DictItem __i;
    foreach(__i, main_queue)
        queue.append(__i);

    if(queue.size() <=0) return;
    qSort(queue.begin(), queue.end(), lessThan);
    setUiEnabled(false);

    if(!getItem(current)) return; // getting current item, else return

    isHidden = true;

    if(ui->chbLearn->checkState() == Qt::Checked)
        ui->lblQuestion->setText(QString("%1 - %2").arg(current.l1).arg(current.l2));
    else
        ui->lblQuestion->setText(current.l1);

    countErrors = 0;
    countTotal = 1;

    inquiryCount = 1; // first word shown
    error = false;
}
void MainWindow::checkAnswer()
{
    bool goMinimize = true;
    QString answer = ui->txtAnswer->text();
    if(answer == "") return;
    if(answer != current.l2) // Ошибка
    {
        if(error == false) { // Ставим что одна ошибка уже была и даем еще шанс
            error = true;
            ui->lblOK->setText("<font color=\"red\"><b>:(</b></font>");
            return;
        }
        else {
            countErrors++;
            goMinimize = false; // show corrected phrase
            error = false; // Сбрасываем ошибку, добавляем на повтор и идем дальше
            queue.enqueue(current);
            ui->lblOK->setText("<font color=\"red\"><b>:(</b></font>");
        }
    } else
    {
        ui->lblOK->setText("<font color=\"green\"><b>:)</b></font>"); // answer is correct
    }
    ui->txtAnswer->setText("");
    ui->lblQuestion->setText("");
    ui->txtWord->setText(QString("%1 - %2").arg(current.l1).arg(current.l2));

    if(!getItem(current)) { // getting current item, else return
        QMessageBox msgBox;
        msgBox.setText(QString(tr("Finished! <br><br>Total: %1<br>Error: %2")).arg(countTotal).arg(countErrors));
        msgBox.exec();
        return;
    }
    countTotal++;

    if(inquiryCount >= inquiryNumWords) {
        if(goMinimize) emit hide();
        timer->setInterval(60*1000*inquiryDelay);
        timer->start();
    }
    else
    {
        on_next_question();
    }
}

void MainWindow::on_next_question()
{
    if(inquiryCount >= inquiryNumWords) {
        timer->stop();
        emit showNormal();
        inquiryCount = 0;
    }


    error = false;
    inquiryCount++;
    // show answer
    if(ui->chbLearn->checkState() == Qt::Checked)
        ui->lblQuestion->setText(QString("%1 - %2").arg(current.l1).arg(current.l2));
    else
        ui->lblQuestion->setText(current.l1);
}

void MainWindow::on_txtAnswer_returnPressed()
{
    if(isHidden) { checkAnswer(); return; }

    QString answer = ui->txtAnswer->text();
    if(answer == "") return;
    if(answer != current.l2) // Ошибка
    {
        if(error == false) { error = true; ui->lblOK->setText("<font color=\"red\"><b>:(</b></font>"); return; } // Ставим что одна ошибка уже была и даем еще шанс
        else { countErrors++; error = false; queue.enqueue(current); ui->lblOK->setText("<font color=\"red\"><b>:(</b></font>"); } // Сбрасываем ошибку и идем дальше
    } else
    {
        ui->lblOK->setText("<font color=\"green\"><b>:)</b></font>");
    }
    ui->txtAnswer->setText("");
    ui->txtWord->setText(QString("%1 - %2").arg(current.l1).arg(current.l2));

    if(!getItem(current)) {
        QMessageBox msgBox;
        msgBox.setText(QString(tr("Finished! <br><br>Total: %1<br>Error: %2")).arg(countTotal).arg(countErrors));
        msgBox.exec();
        return; // getting current item, else return
    }

    countTotal++;

    error = false;
    // show answer
    if(ui->chbLearn->checkState() == Qt::Checked)
        ui->lblQuestion->setText(QString("%1 - %2").arg(current.l1).arg(current.l2));
    else
        ui->lblQuestion->setText(current.l1);

}

void MainWindow::on_actionShow_Dict_triggered()
{
    DictionaryEdit de(this);
    de.exec();
}

void MainWindow::on_actionNew_triggered()
{
    QString str = QFileDialog::getSaveFileName(this, tr("Creating dictionary..."), "", "*.db");
    if(str == "") return;
    if(QFileInfo(str).exists()) {
        QMessageBox msgBox;
        msgBox.setText(tr("File exists."));
        msgBox.exec();
        return;
    }
    if(!str.contains(QRegExp(".+(.db|.DB)$")))
    {
        str.append(".db");
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(str);
    if(!db.open())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database error"));
        msgBox.exec();
        return;
    }
/*
  CREATE TABLE dict(id INTEGER PRIMARY KEY AUTOINCREMENT, l1 TEXT, l2 TEXT, origin INTEGER(1) NOT NULL DEFAULT 0);
  */
    QSqlQuery query(db);
    query.exec("CREATE TABLE dict(id INTEGER PRIMARY KEY AUTOINCREMENT, l1 TEXT, l2 TEXT, origin INTEGER(1) NOT NULL DEFAULT 0);");

    db.close();
    database = str;
    LoadDict();
}

void MainWindow::on_actionNormal_triggered()
{
    on_btnStart_clicked();
}

void MainWindow::on_actionHidden_triggered()
{
    InquiryInitial dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        startInquiry();
    }
}

bool MainWindow::getItem(DictItem& item) {
    if(ui->chbLearn->checkState() == Qt::Checked || ui->chbDirection->checkState() == Qt::Unchecked) // learn mode
    {
        do {
            if(queue.size() <= 0) {
                ui->lblQuestion->setText("");
                setUiEnabled(true);
                return false;
            }
            item = queue.dequeue();
        } while(current.isl2Origin == true);
    }
    else // if normal mode
    {
        if(queue.size() <= 0) {
            ui->lblQuestion->setText("");
            return false;
        }
        item = queue.dequeue();
    }
    return true;
}
void MainWindow::setUiEnabled(bool value)
{
    ui->txtAnswer->setEnabled(!value);
    ui->btnStart->setEnabled(value);
    ui->chbTray->setEnabled(value);
    ui->chbLearn->setEnabled(value);
    ui->chbDirection->setEnabled(value);
}
//********** Maximize, minimize, normal, tray icon **********
void MainWindow::createActions()
 {
     minimizeAction = new QAction(tr("Mi&nimize"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

     //maximizeAction = new QAction(tr("Ma&ximize"), this);
     //connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

     restoreAction = new QAction(tr("&Restore"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

     quitAction = new QAction(tr("&Quit"), this);
     connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
 }

 void MainWindow::createTrayIcon()
 {
     trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(minimizeAction);
     //trayIconMenu->addAction(maximizeAction);
     trayIconMenu->addAction(restoreAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);

     trayIcon = new QSystemTrayIcon(this);
     trayIcon->setContextMenu(trayIconMenu);
 }


void MainWindow::on_btnStartHidden_clicked()
{
    InquiryInitial dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        startInquiry();
    }
}

void MainWindow::on_action1_triggered()
{
    DictionaryEdit de(this);
    de.exec();
}
