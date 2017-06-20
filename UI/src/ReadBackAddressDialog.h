#ifndef READBACKADDRESSDIALOG_H
#define READBACKADDRESSDIALOG_H

#include <QDialog>
#include "../../Public/AppTypes.h"
#include "../../Rules/ReadbackRule.h"

namespace Ui {
    class ReadBackAddressDialog;
}

class ReadBackAddressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReadBackAddressDialog(QWidget *parent = 0);

    ~ReadBackAddressDialog();

    void SetCurrentState(NUTL_ReadFlag_E read_flag,
                         const QString &addr,
                         const QString &len,
                         const U32 part_id);

    NUTL_ReadFlag_E read_flag();
    U64 addr();
    U64 length();
    U32 region_id();
    const QString region();
    void SetStorageType(HW_StorageType_E storage_type);
    void UpdateUI();
    void OnScatterChanged(bool showRegion);

signals:

public slots:
    void onToggleHex(bool);
    void onToggleDec(bool);


private slots:
    void on_radioButton_readPageSpare_clicked();
    void slot_platform_changed(HW_StorageType_E &storage);

private:
    Ui::ReadBackAddressDialog *ui;
    HW_StorageType_E storage_type_;
    void onHexToDec();
    void onDecToHex();
    QStringList mEmmcRegionList;
    QStringList mUfsRegionList;

};

#endif // READBACKADDRESSDIALOG_H
