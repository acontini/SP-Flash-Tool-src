#ifndef DLDELEGATE_H
#define DLDELEGATE_H

#include <QObject>
#include <QtGui>
#include "TabWidgetBase.h"
#include "../../Public/AppTypes.h"
#include "../../Utility/Utils.h"
#include "ICallback.h"
#include "MainWindow.h"

enum ImageColumn{
    ColumnEnable = 0,
    ColumnName,
    ColumnBeginAddr,
    ColumnEndAddr,
    columnRegion,
    ColumnLocation
};

class MainWindow;

class DLDelegate : public QObject
{
    Q_OBJECT
public:
    explicit DLDelegate(DL_SCATTER_TYPE scatterType,
                        TabWidgetBase *widget, MainWindow *window,
                        QObject *parent);

    void LoadScatterFile(const QString &file_path,
                         ICallback *callback, ICallback *failed_callback);

    void updateTableWidgetList(QTableWidget* widget, std::list<ImageInfo> &imageList);

    void onPlatformChanged();
    bool onLoadEnd();
    void onLoadFailed();
    bool hasEnableRom();

    void startSCIDownload();
    void stopExecute();

    void setTableWidget(QTableWidget* widget){
        table_widget_ = widget;
    }

    bool has_uncheck_item(){
        return has_uncheck_item_;
    }

private:
    bool InitialPlatform();

private:
    DL_SCATTER_TYPE scatterType_;
    TabWidgetBase *widget_;
    MainWindow *main_window_;

    HW_StorageType_E storage_;
    bool has_uncheck_item_;

    QTableWidget *table_widget_;
    
signals:
    
public slots:
    
};

#endif // DLDELEGATE_H
