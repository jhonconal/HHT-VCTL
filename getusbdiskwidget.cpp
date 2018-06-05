#include "getusbdiskwidget.h"
#include "ui_getusbdiskwidget.h"

GetUsbDiskWidget::GetUsbDiskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetUsbDiskWidget)
{
    ui->setupUi(this);
    this->Init();
    //子窗口置顶父窗口技巧
    this->hide();
    setWindowFlags((Qt::WindowStaysOnTopHint|this->windowFlags())
                   &~Qt::WindowMinMaxButtonsHint);//窗口置顶 隐藏最大最小按钮
    this->show();
}

GetUsbDiskWidget::~GetUsbDiskWidget()
{
    delete ui;
}

void GetUsbDiskWidget::on_checkButton_clicked()
{
    //发送选择信号
#if 0
    QString  selectDisk = ui->comboBox_USB->currentText();
    qDebug()<<">>>"<<selectDisk;
    emit select_disk_signals(selectDisk);
#else
    QString  selectDisk = ui->comboBox_USB->currentText();
    qDebug()<<">>>"<<selectDisk.mid(0,2);  //例如：I: ( 7.24365 GB/6.8998 GB )  --> I:
    emit select_disk_signals(selectDisk.mid(0,2));
#endif
}

//=================================================================================
//系统初始化
//=================================================================================
void GetUsbDiskWidget::Init()
{
    ui->comboBox_USB->clear();
    this->usb_disk_info.clear();
    this->ScanHD();
}
//=================================================================================
//系统启动扫描已经连接的U盘 程序只会在系统执行时执行一次
//=================================================================================
void GetUsbDiskWidget::ScanHD()
{
#ifdef Q_OS_WIN32
    qDebug()<<"开机U盘扫描.";
    char diskPath[5] = { 0 };
    DWORD allDisk = GetLogicalDrives(); //返回一个32位整数，将他转换成二进制后，表示磁盘,最低位为A盘
    if (allDisk!=0)
    {
        for (int i=0;i<13;i++)     //假定最多有12个磁盘从A开始计数
        {
            if ((allDisk & 1)==1)
            {
                sprintf( diskPath, "%c", 'A'+i );
                strcat( diskPath, ":" );
                LPCWSTR RootPathName =(LPCWSTR)QString(diskPath).utf16();

                if (GetDriveType(RootPathName) ==  DRIVE_REMOVABLE)
                {
                    if( GetVolumeInformation(RootPathName,0,0,0,0,0,0,0) ) //判断驱动是否准备就绪
                    {
                        qDebug()<<"U盘准备就绪！"<<diskPath;
                        QString  path(diskPath);
                        this->AddDisk(path);
                    }
                }
            }
            allDisk = allDisk>>1;
        }
    }
#endif
}
//=================================================================================
//增加移动设备
//=================================================================================
void GetUsbDiskWidget::AddDisk(QString &DiskName)
{
    int iter = 0;
    if(SearchDisk(DiskName,iter))
    {
        qDebug()<<"SearchDisk Null.";
        return ;
    }
    USB_DISK_INFO usbtemp;
    usbtemp.DirPath = DiskName;
    GetMemorySize(usbtemp);//获取设备内存信息
    this->usb_disk_info.push_back(usbtemp);
#if SHOW_LESS_INFO
    this->AddComboBoxItem(DiskName);
#else
    QString totalMemory =QString("%1 GB").arg(usbtemp.TotalMemory);
    QString freeMemory =QString("%1 GB").arg(usbtemp.FreeMemory);
    QString &info = (QString &)(DiskName+" ( "+totalMemory+"/"+freeMemory+" ) ");
    this->AddComboBoxItem(info);
#endif
}
//=================================================================================
//删除移动设备
//=================================================================================
void GetUsbDiskWidget::DeleteDisk(QString &DiskName)
{
    int iter = 0;
    if(SearchDisk(DiskName,iter))
    {
        this->usb_disk_info.remove(iter);
#if SHOW_LESS_INFO
        this->DeleteComboBoxItem(DiskName);
#else
        for(int i=0;i<ui->comboBox_USB->count();i++)
        {
            if(ui->comboBox_USB->itemText(i).contains(DiskName))
            {
                QString item =ui->comboBox_USB->itemText(i);
                QString &itemInfo = (QString&)item;
                this->DeleteComboBoxItem(itemInfo);
                break;
            }
        }
#endif
    }
}
//=================================================================================
//搜索移动设备
//=================================================================================
bool GetUsbDiskWidget::SearchDisk(QString &DiskName, int &iter)
{
    for(int i = 0;i < this->usb_disk_info.size();i++)
    {
        if(usb_disk_info.at(i).DirPath.compare(DiskName) == 0)
        {
            iter = i;
            return true;
        }
    }
    return false;
}
//=================================================================================
//获取设备的内存信息
//=================================================================================
void GetUsbDiskWidget::GetMemorySize(USB_DISK_INFO &disk)
{
#ifdef Q_OS_WIN32
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    LPCWSTR lpcwstrDriver=(LPCWSTR)disk.DirPath.utf16();
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
        return ;
    }
    disk.FreeMemory  = (float)liTotalFreeBytes.QuadPart/1024/1024/1024; //剩余内存
    disk.TotalMemory = (float)liTotalBytes.QuadPart/1024/1024/1024;     //总内存
#endif
}

//=================================================================================
//U盘盘符计算
//=================================================================================
char GetUsbDiskWidget::FirstDriveFromMask(ULONG unitmask)
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
//=================================================================================
//增加下拉列表选项
//=================================================================================
void GetUsbDiskWidget::AddComboBoxItem(QString &item)
{
    QStringList ComboboxList = QStringList()<<item;
    //ComboboxList<<item;
    ui->comboBox_USB->addItems(ComboboxList);
}
//=================================================================================
//删除下拉列表选项
//=================================================================================
void GetUsbDiskWidget::DeleteComboBoxItem(QString &item)
{
    int i = ui->comboBox_USB->findText(item);
    ui->comboBox_USB->removeItem(i);
}
//=================================================================================
//下拉列表选项发生改变
//=================================================================================
void GetUsbDiskWidget::on_comboBox_USB_currentIndexChanged(QString e)
{
    qDebug()<<" this is combox changed ="<<e;
    int iter;
    if(this->SearchDisk(e,iter))
    {
        this->CurrentUseDisk = this->usb_disk_info.at(iter);
    }
}
//=================================================================================
//获取下拉列表当前选定的U盘
//=================================================================================
USB_DISK_INFO GetUsbDiskWidget::GetCheckedDisk()
{
    return this->CurrentUseDisk;
}
#ifdef Q_OS_WIN32
bool GetUsbDiskWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    QString DirPath;
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
                    //插入U盘，获取盘符
                    DirPath = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    DirPath += ":";
                    this->AddDisk(DirPath);
                    qDebug()<<"this->USBDisk Path ="<<DirPath;
                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0)
                {
                    qDebug() << "USB_Removed";

                    DirPath = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    DirPath += ":";
                    this->DeleteDisk(DirPath);
                    qDebug()<<"this->Delete Path ="<<DirPath;
                }
            }
            break;
        }
    }
    return false;
}
#endif
