#ifndef READBACKWIDGET_H
#define READBACKWIDGET_H

#include "TabWidgetBase.h"
#include "../../Rules/ReadbackRule.h"
#include "../../Public/AppTypes.h"
#include "../../Setting/ReadbackSetting.h"
#include "../../Setting/PlatformSetting.h"
#include "ScatterObserver.h"
#include <map>

namespace Ui
{
class ReadBackWidget;
}
class MainWindow;
class ReadBackAddressDialog;
class PlatformSetting;
class ReadBackWidget : public TabWidgetBase, public APCore::IPlatformOb, public IScatterObj
{
    Q_OBJECT
public:
    enum Column{
        ColumnEnable = 0,
        ColumnName,
        ColumnReadFlag,
        ColumnAddr,
        ColumnLength,
        ColumnRegion,
        ColumnFile
    };

    explicit ReadBackWidget(QTabWidget *parent, MainWindow *window);
    ~ReadBackWidget();

    DECLARE_TABWIDGET_VFUNCS()

    virtual void onPlatformChanged();
    virtual void OnScatterChanged(bool showRegion);

    void SetReadbackListItem(
        QSharedPointer<APCore::ReadbackSetting> &readback_setting);
    void ShowReadFlagColumn(bool show);

private:
    Ui::ReadBackWidget *ui_;
    MainWindow *main_window_;
    ReadBackAddressDialog *addr_dialog_;

    std::map<QString, NUTL_ReadFlag_E> read_flag_map_;

    void AppendRBItem(int row);
    void DeleteRBItem(int row);
    ReadbackItem GetRBItem(int row);

    NUTL_ReadFlag_E ParseReadFlag(const QString &flag_str);
//    U32 ParseRegionID(const QString &region_str);
    QString GetReadFlagStr(NUTL_ReadFlag_E flag);

    bool ValidateBeforeReadback();
    bool ValidateAddrAlignment(U64 addr, U64 len);
    void UpdateReadBackItemsByScatter();

private:
   HW_StorageType_E storage_;
   bool platform_changed_;

signals:
   void signal_platform_changed(HW_StorageType_E &storage);

public slots:

private slots:
    void on_toolButton_readBack_clicked();
    void on_toolButton_stop_clicked();
    void on_toolButton_add_clicked();

    void on_toolButton_remove_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);
};

#endif // READBACKWIDGET_H
