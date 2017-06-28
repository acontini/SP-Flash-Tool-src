#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include "TabWidgetBase.h"
#include <QtGui>
#include "../../BootRom/flashtool_api.h"
#include "../../Setting/FormatSetting.h"
#include "../../Setting/PlatformSetting.h"
#include "ScatterObserver.h"

namespace Ui
{
class FormatWidget;
}

class MainWindow;
class PlatformSetting;

class FormatWidget : public TabWidgetBase, public APCore::IPlatformOb, public IScatterObj
{
    Q_OBJECT
public:
    explicit FormatWidget(QTabWidget *parent, MainWindow *window);
    ~FormatWidget();

    DECLARE_TABWIDGET_VFUNCS()

    virtual void onPlatformChanged();
    virtual void OnScatterChanged(bool showRegion);

    bool get_auto_format() const;
    bool get_validation() const;
    U64 get_begin_addr() const;
    U64 get_fmt_length() const;
    U32 get_part_id() const;
    NUTL_EraseFlag_E get_erase_flag() const;
    APCore::FormatSetting::AutoFormatFlag get_auto_format_flag() const;
    bool get_is_com_dl() const;

    void ShowEraseFlag(bool show);
    void ShowMarkAsBadBlockFlag(bool show);

private:
    bool ValidateBeforeFormat();
    void InitDefault();


private:
    MainWindow *main_window_;
    Ui::FormatWidget *ui_;
    HW_StorageType_E storage_;
    bool platform_changed_;
    bool is_combo_dl_;
    QStringList mEmmcRegionList;
    QStringList mUfsRegionList;

signals:
    void signal_start_formating();

public slots:

private slots:
    void on_radioButton_AutoFormat_clicked();
    void on_radioButton_ManualFormat_clicked();
    void on_toolButton_Start_clicked();
    void on_toolButton_stop_clicked();
    void slot_platformChanged();
};

#endif // FORMATWIDGET_H
