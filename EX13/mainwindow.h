#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloader.h"
#include <QProgressBar>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Downloader* downloader;
    QProgressBar *ppb;
    QLineEdit *txt;
    QPushButton *button;
    void showPic(const QString&);

private slots:
    void slotGo();
    void slotError();
    void slotDownloadProgress(quint64,quint64);
    void slotDone(const QUrl&, const QByteArray&);
public:
    QString getHtml(const QString&);

};
#endif // MAINWINDOW_H
