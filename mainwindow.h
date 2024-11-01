#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingStruct.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Setting settingStruct;

private slots:

    void on_XOR_clicked();

    void on_AND_clicked();

    void on_OR_clicked();

    void on_OpenFile_clicked();

    void on_SavePath_clicked();

    void on_DeleteFile_checkStateChanged(const Qt::CheckState &arg1);

    void on_Modificate_clicked();

    void on_timeEdit_userTimeChanged(const QTime &time);

    void on_SaveRule_checkStateChanged(const Qt::CheckState &indexCheck);

    void on_MaskValue_editingFinished(const QString &maskValue);

    void on_MaskValue_editingFinished();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
