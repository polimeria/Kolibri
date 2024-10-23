#include <QFileDialog>
#include <QFile>
#include "buttonController.h"


// открытие файла хранящего маску
void openFile()
{
    QString filtred="*.bin *.txt"; // стринг переменная
    QString inputstr = QFileDialog::getOpenFileName(nullptr, "выбрать файл", "" , filtred); //перемнная приарвнивается к полному имени файла
}


//сохранения пути
void savePath() //путь для сохранения результирующих файлов
{
    QString selectedPath = QFileDialog::getExistingDirectory(nullptr, "выбрать путь", "");
}

