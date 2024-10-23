#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QFile>
#include <QByteArray>
#include <QFile>
#include "settingStruct.h"

#endif // FILEUTILS_H



//класс, содержащий методы для работы с файлами и содержимым.

class FileUtils
{
public:
    // Структура для просмотр итогового результата и его реверса.
    struct ModifiedText
    {
        QString text;
        QString reverseText;
    };
    //чтение файла
    static QByteArray fileReaders (QString& fileName);
    //модификация файла
    static  QString textModify (QByteArray& fileReaders, QByteArray& value, QString& logicalOperator);
    // Обратная модификация текста
    static QString textModifyReverse(QByteArray &fileReaders, QByteArray& value, QString& logicalOperator);
    //сохранение
    static bool saveFile(const QString& savePath, const QString& newText, const QString& oldFileName, bool& checkState, bool overwrite = true);
    //вызов функций
    static void readModifcateFile (Setting& settingStruct, ModifiedText& text);
    // Сообщение с предупреждением.
    static bool warningMessange(Setting& settingStruct);

};
