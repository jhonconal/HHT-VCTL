#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#ifdef Q_OS_WIN32
#include <windows.h>
#include <winbase.h>
#include <winnls.h>
#include <dbt.h>
#endif
#include "JlCompress.h"
#include "getusbdiskwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool CompareList(QStringList src,QStringList dest);
    quint64 dirFileSize(const QString &path);

    void quazip_test();
protected:
#if 0
//#ifdef Q_OS_WIN32
    char FirstDriveFromMask(ULONG unitmask);
    bool nativeEvent(const QByteArray & eventType, void * message, long*result);
#endif
public slots:
     void  select_disk_slot(QString select);
private:
    Ui::MainWindow *ui;
    QStringList mExternalDeviceList;
    QString       mOptationDisk;//选择操作external disk
    GetUsbDiskWidget *mGetUsbDiskWidget;
};

#endif // MAINWINDOW_H
