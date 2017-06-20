#ifndef BROMADAPTERWIDGET_H
#define BROMADAPTERWIDGET_H

#include <QWidget>
#include <QtGui>
#include "TabWidgetBase.h"
#include "../../BootRom/flashtool_api.h"

namespace Ui {
    class BromAdapterWidget;
}

typedef enum
{
    BROM_CONNECT,
    BROM_DISCONNECT
}BROM_STATE;

class MainWindow;

class BromAdapterWidget : public TabWidgetBase
{
    Q_OBJECT

public:
    explicit BromAdapterWidget(QTabWidget *parent, MainWindow *window);
    ~BromAdapterWidget();

    DECLARE_TABWIDGET_VFUNCS()

    bool get_is_manual_addr() const;
    bool get_is_jump() const;
    U64  get_start_addr() const;
    U64  get_jump_addr() const;
    U64  get_buf_len() const;
    const unsigned char* get_buf();

    void set_chip_info(const BOOT_RESULT *p_boot_result);

private slots:
    void on_toolButton_download_clicked();

    void on_toolButton_stop_clicked();

    void on_pushButton_data_clicked();

    void lockUI(bool lock);
    void SetupFromIni();


private:
    MainWindow *main_window_;
    Ui::BromAdapterWidget *ui_;

    unsigned char*  download_buf_;
    unsigned int    download_len_;

    BROM_STATE      brom_state_;
    BBCHIP_TYPE     chip_type_;
    EXT_CLOCK       ext_clock_;
    int             sec_header_len_;
};

#endif // BROMADAPTERWIDGET_H
