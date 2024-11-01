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

MainWindow::MainWindow(QWidget *parent)// конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() // декструтор
{
    delete ui;
}

void MainWindow::on_OpenFile_clicked()// входной файл, например .txt, testFile.bin
{
    //openFile();
    QString filtred="*.bin *.txt"; // создание объекта класса QString
    settingStruct.inputStr = QFileDialog::getOpenFileName(nullptr, "выбрать файл", "", filtred); //создание объект класса QString, результат работы getOpenFileName равен новой переменной

}

void MainWindow::on_SavePath_clicked()//путь для сохранения результирующих файлов
{
    //savePath();
    QString selectedPath = QFileDialog::getExistingDirectory(nullptr, "выбрать путь", "");//создание объект класса QString, результат работы  равен новой переменной, что за метод

    settingStruct.savePath=selectedPath;
}
// Эти функции обрабатывают нажатие кнопок для выбора логического оператора (XOR, AND, OR). Выбранный оператор сохраняется в структуре settingStruct, в поле logicalOperator.

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

//та функция вызывается при изменении состояния флажка (чекбокса). Если флажок установлен (состояние Checked), вызывается метод QFile::remove(), который удаляет файл, путь к которому хранится в поле DeleteFile структуры settingStruct.

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


void MainWindow::on_MaskValue_editingFinished()
{
    QString maskValue = ui->MaskValue->text();  // Получение значения из UI

    // Проверка, что строка содержит только шестнадцатеричные символы и имеет четную длину
    static const QRegularExpression hexRegex("^[0-9A-Fa-f]{2,}$");
    if (hexRegex.match(maskValue).hasMatch() && (maskValue.size() % 2 == 0)) {
        QByteArray mask = QByteArray::fromHex(maskValue.toLatin1());
        if (!mask.isEmpty()) {
            settingStruct.Mask = mask;
            return;
        }
    }
    QMessageBox::critical(this, "Error", "Invalid mask value. Please enter a valid hexadecimal mask.");
}




