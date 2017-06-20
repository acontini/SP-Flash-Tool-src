#ifndef PROCESSINGDIALOG_H
#define PROCESSINGDIALOG_H

#include <QDialog>

namespace Ui
{
class ProcessingDialog;
}
class ProcessingAnimation;

class ProcessingDialog : public QDialog
{
    Q_OBJECT

public:
    ProcessingDialog(QWidget *parent = 0);
    ~ProcessingDialog();

    void showDialog(const QString& message);
    void hideDialog();
    void showCancelButton(bool show);

    Ui::ProcessingDialog *ui;
private:
    ProcessingAnimation * animation;
    QPoint drag_position;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void user_cancel_processing();

private slots:
    void on_pushButton_cancel_clicked();
};

#endif // PROCESSINGDIALOG_H
