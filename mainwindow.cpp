#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    this->quazip_test();
    ui->setupUi(this);
    mGetUsbDiskWidget = new GetUsbDiskWidget();
    connect(mGetUsbDiskWidget,SIGNAL(select_disk_signals(QString)),this,SLOT(select_disk_slot(QString)));
    mGetUsbDiskWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWindow::CompareList
 * @param src
 * @param dest
 * @return
 * 比较两个QStringList是否完全一致
 */
bool MainWindow::CompareList(QStringList src, QStringList dest)
{
    bool result =false;
    int mCount =src.length(),index=0;
    if(src.length()!=dest.length())
        return result;
    for(int i=0;i<mCount;i++)
    {
        for(int j=0;j<dest.length();j++)
        {
            QString filePath = dest.at(i);
            if(filePath.contains(src.at(i)))
            {
                index++;
                break;
            }
        }
    }
    qDebug()<<"+++++"<<mCount<<"########"<<index;
    if(index==mCount)
    {
        result =true;
    }
    else
    {
        result =false;
    }
    return result;
}
/**
 * @brief MainWindow::dirFileSize
 * @param path
 * @return
 * 获取整个目标文件夹大小
 */
quint64 MainWindow::dirFileSize(const QString &path)
{
    QDir dir(path);
    quint64 size = 0;
    foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        size +=fileInfo.size();
    }
    foreach (QString subDir, dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
    {
        size +=dirFileSize(path+QDir::separator()+subDir);
    }
    QString unit ="B";
    quint64 showSize = size;
    if(showSize>1024.0)
    {
        showSize /=1024.0;
        unit  ="K";
        if(showSize>1024.0)
        {
            showSize /=1024.0;
            unit ="M";
            if(showSize>1024.0)
            {
                showSize /=1024.0;
                unit ="G";
            }
        }
    }
    //qDebug()<<"文件夹大小: "<<showSize<<unit;
    return size;
}

void MainWindow::quazip_test()
{
#if 1
    const QString srcPath = "C:/Users/jhonconal/Desktop/HHT-VCTL";
    const QString srcCompare = srcPath+".zip";
    const QString destPath = srcPath+"-extract";
    quint64 size = dirFileSize(srcPath);
    qDebug()<<"+++++"<<size<<" B";
    //压缩 ZIP
    bool result =JlCompress::compressDir(srcCompare,srcPath);
    if(!result)
    {
        qDebug()<<"compress failed.";
    }
    else
    {
        qDebug()<<"compress success.";
    }
    //获取压缩ZIP包含的文件
    QStringList src = JlCompress::getFileList(srcCompare);
    for(int i=0;i<src.length();i++)
    {
        qDebug()<<src.at(i);
    }
    //解压缩ZIP 返回解压缩获取的文件列表
    QStringList dest=JlCompress::extractDir(srcCompare,destPath);
    for(int i=0;i<dest.length();i++)
    {
        qDebug()<<dest.at(i);
    }
    //对比压缩与解压缩包含文件列表,判断解压缩成功
    bool rest= CompareList(src,dest);
    if(rest)
    {
        qDebug()<<"extract zip success.";
    }
#endif
}

#if 0
//#ifdef Q_OS_WIN32
char MainWindow::FirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType==WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags ==0)
                {
                    //插入u盘
                    QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask))+":/";
                    mExternalDeviceList.append(USBDisk);
                    qDebug() << "存储设备插入: "<<USBDisk ;
                    qDebug()<<"###插入设备之后:";
                    for(int i=0;i<mExternalDeviceList.length();i++)
                    {
                        qDebug()<<mExternalDeviceList.at(i);
                    }
                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0)
                {
                    QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask))+":/";
                    for(int i=0;i<mExternalDeviceList.length();i++)
                    {
                        if(mExternalDeviceList.at(i)==USBDisk)
                            mExternalDeviceList.removeAt(i);
                    }
                    qDebug() << "存储设备移除: " <<USBDisk;

                    qDebug()<<"###拔出设备之后:";
                    for(int i=0;i<mExternalDeviceList.length();i++)
                    {
                        qDebug()<<mExternalDeviceList.at(i);
                    }
                }
            }
            break;
        }
    }
    return false;
}
#endif

void MainWindow::select_disk_slot(QString select)
{
    if(!select.isNull())
    {
        mOptationDisk.clear();
        mOptationDisk = select+"/";
        qDebug()<<"(SLOT)选择操作磁盘: "<<mOptationDisk;
        QDesktopServices::openUrl(QUrl(mOptationDisk,QUrl::TolerantMode));
        //TODO
    }
}
