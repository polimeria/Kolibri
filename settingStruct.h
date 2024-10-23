#pragma once
#include <QString>


#ifndef SETTINGSTRUCT_H
#define SETTINGSTRUCT_H

#endif // SETINGSTRUCT_H


struct Setting
{
    // полное имя входного файла.
    QString inputStr;
    // Удалять ли входящий файл.
    bool checkState = false;
    // логический оператор and or xor.
    QString logicalOperator;
    // Маска входящая.
    QByteArray Mask;
    // Путь сохранения файла.
    QString savePath;
    // Обрезанное имя файла.
    QString cutFileName;
    // Правила сохранения переменной.
    bool saveRule = true;
};

