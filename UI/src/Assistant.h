#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QObject>
#include <QString>
#include <QProcess>

class MainWindow;

class Assistant : public QObject
{
    Q_OBJECT
public:
    explicit Assistant(QObject *parent = 0);
    ~Assistant();
    bool StartAssistant();
    void ShowDocumentation(const QString& page, bool showContent = false, bool showIndex = false);

private:
    void UpdateCacheDir(void);
    bool ClearUpEnv();

private:
    QProcess* process_;
    QString cache_dir_;
    MainWindow *main_window_;
signals:

public slots:

};

#endif // ASSISTANT_H
