#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QObject>
#include <QNetworkReply>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    downloader = new Downloader(this);
    ppb = new QProgressBar(this);
    txt = new QLineEdit(this);
    button = new QPushButton("Start",this);

    connect(button, SIGNAL(clicked()),SLOT(slotGo()));
    connect(downloader, SIGNAL(downloadProgress(quint64,quint64)),SLOT(slotDownloadProgress(quint64,quint64)));
    connect(downloader, SIGNAL(done(const QUrl&,const QByteArray&)),SLOT(slotDone(const QUrl&,const QByteArray&)));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(txt,0,0);
    layout->addWidget(button,0,1);
    layout->addWidget(ppb,1,0,1,2);

    QWidget *wgt = new QWidget(this);
    setCentralWidget(wgt);
    wgt->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getHtml(const QString &url)
{
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
    QEventLoop event;
    QObject::connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    response->deleteLater();
    return response->readAll();
}



void MainWindow::showPic(const QString &path)
{
    QPixmap pix(path);
    pix = pix.scaled(pix.size() / 3);
    QLabel* label = new QLabel();
    label->setPixmap(pix);
    label->setFixedSize(pix.size());
    label->show();


}

void MainWindow::slotGo()
{
    downloader->download(QUrl("https://yandex.ru/images/search?text=" +txt->text()));
}

void MainWindow::slotError()
{
    QMessageBox::critical(this,"Error","Error!");
}

void MainWindow::slotDownloadProgress(quint64 received, quint64 total)
{
    if(total <= 0)
    {
        slotError();
        return;
    }
    ppb->setValue(100*received / total);
}

void MainWindow::slotDone(const QUrl &url, const QByteArray &ba)
{
    QStringList list;
    QString html = getHtml(url.toString());
    QRegExp regex("(src=\")(.*)(\")");
    int lastPos = 0;
    int count = 0;
    while((lastPos =regex.indexIn(html,lastPos)) != -1 && count < 3)
    {
        lastPos+=regex.matchedLength();
        list.append(regex.cap(2));

    }
    for(int i = 0; i < list.length(); i++)
    {
    QFile file(list[i]);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(ba);
        file.close();
    }
        showPic(file.fileName());
    }

}


