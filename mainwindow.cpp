 #include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTime>

#include "buttonController.h"
#include "settingStruct.h"
#include "fileUtils.h"

#include <QTimeEdit>
#include <QTimer>

/*тестовую программу, которая модифицирует входные файлы бинарной операцией с заданным значением.
Порядок работы программы:

После запуска настраивается пользователем:
а) маска входных файлов, например .txt, testFile.bin
b) настройка необходимости удалять входные файлы или нет
c) путь для сохранения результирующих файлов
d) действия при повторении имени выходного файла: перезапись или
модификация, например, счетчик к имени файла
e) работа по таймеру или разовый запуск
f) периодичность опроса наличия входного файла (таймер)
g) значение 8 байт для бинарной операции модификации файла
Функциональность: модифицирует входные файлы, например операция XOR с 8-байтной переменной, введенной с формы (Пункт 1,g)
Защита от «дурака»: если входной файл не закрыт - не трогать его.*/

Setting settingStruct;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenFile_clicked()
{
    //openFile();
    QString filtred="*.bin *.txt";
    settingStruct.inputStr = QFileDialog::getOpenFileName(nullptr, "выбрать файл", "", filtred);
}

void MainWindow::on_SavePath_clicked()
{
    //savePath();
    QString selectedPath = QFileDialog::getExistingDirectory(nullptr, "выбрать путь", "");

    settingStruct.savePath=selectedPath;
}

void MainWindow::on_XOR_clicked()
{
    settingStruct.logicalOperator="XOR";
}


void MainWindow::on_AND_clicked()
{
    settingStruct.logicalOperator="AND";
}

void MainWindow::on_OR_clicked()
{
    settingStruct.logicalOperator="OR";
}

void MainWindow::on_MaskValue_cursorPositionChanged(int arg1, int arg2)
{
    QByteArray newMask = QByteArray::fromHex(ui->MaskValue->text().toUtf8());
    if (newMask != settingStruct.Mask) {
        if (!newMask.isEmpty()) {
            settingStruct.Mask = newMask;
        } else {
            QMessageBox::critical(this, "Error", "Invalid mask value. Please enter a valid hexadecimal mask.");
            settingStruct.Mask = QByteArray::fromHex("00000001");
        }
    }
}


void MainWindow::on_DeleteFile_checkStateChanged(const Qt::CheckState &checkState)
{
    settingStruct.checkState=checkState;
}


void MainWindow::on_Modificate_clicked()
{
    FileUtils::ModifiedText text;
    FileUtils::readModifcateFile(settingStruct,text);
    // Заполнение полей.
    ui->Reverse_2->setText(text.reverseText);
    ui->Result_2->setText(text.text);
}

//0x00000001
//  0xFFFFFFFE


void MainWindow::on_timeEdit_userTimeChanged(const QTime &time)
{
    QTime initialTime = time; // локальная копия времени
    QTime internalTime = time; // локальная копия времени
    QTimer *timer = new QTimer(this);
    timer->start(1000); // Запуск таймера с интервалом 1000 мс (1 сек)

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        internalTime = internalTime.addSecs(-1); // Уменьшаем время на 1 секунду

        // Обновляем отображение времени
        ui->timeEdit->setTime(internalTime);

        // Проверяем, если время достигло 00:00:00
        if (internalTime == QTime(0, 0, 0))
        {
            timer->stop(); // Останавливаем таймер
            FileUtils::ModifiedText text;
            FileUtils::readModifcateFile(settingStruct,text);
            ui->Reverse_2->setText(text.reverseText);
            ui->Result_2->setText(text.text);

            // Перезапускаем таймер с исходным временем
            timer->start(initialTime.msecsTo(QTime::currentTime()));
        }
    });
}

void MainWindow::on_SaveRule_checkStateChanged(const Qt::CheckState &indexCheck)
{
    if (indexCheck) {
        settingStruct.saveRule = false;
    } else {
        settingStruct.saveRule = true;
    }
}


void MainWindow::on_MaskValue_textEdited(const QString &maskValue)
{
    QByteArray mask = ui->MaskValue->text().toUtf8();
    if (!mask.isEmpty()) {
        settingStruct.Mask = mask;
    } else {
        QMessageBox::critical(this, "Error", "Invalid mask value. Please enter a valid hexadecimal mask.");
        settingStruct.Mask = QByteArray::fromHex("00000001");
    }
}
