#include "fileUtils.h"
#include <QFile>
#include <QByteArray>
#include <QFile>
#include <QMessageBox>
#include "settingStruct.h"
#include <QFileInfo>
#include <QDir>


#include <QDebug>

QByteArray FileUtils::fileReaders (QString& fileName)
{
    QByteArray byteFile; // хранение данных, считанных из файла
    QFile file(fileName); //отвечает за взаимодействие с указанным файлом.
    //можно ли открыть файл в режиме "только для чтения". Если файл не удается открыть, то функция немедленно возвращает пустой массив байт byteFile.
    if (!file.open(QIODevice::ReadOnly))
        return byteFile;


    //Если файл успешно открыт, все его содержимое считывается с помощью метода readAll() и сохраняется в переменную byteFile.
    byteFile=file.readAll();
    file.close();

    return byteFile;//Возвращается массив байт, содержащий данные файла.
}

QString FileUtils::textModify (QByteArray &fileReaders, QByteArray& value, QString& logicalOperator)
{
    if (fileReaders.isEmpty() )
        return "";

    for (int i = 0;i<fileReaders.size();++i)
    {
        if (logicalOperator=="XOR")
        {
            fileReaders[i]^=value[i %value.size()];
        }
        else
            if (logicalOperator=="AND")
         {
            fileReaders[i]&=value[i %value.size()];
         }
        else
            if (logicalOperator=="OR")
         {
            fileReaders[i]|=value[i %value.size()];
         }

    }

    QString newText=QString::fromUtf8(fileReaders);
    FileUtils::textModifyReverse(fileReaders,value,logicalOperator);
    return newText;

}

QString FileUtils::textModifyReverse(QByteArray &fileReaders, QByteArray& value, QString& logicalOperator)
{
    QString reverseOriginalText = QString::fromUtf8(fileReaders);
    if (fileReaders.isEmpty() || value.isEmpty())
        return "";

    if (logicalOperator != "XOR" && logicalOperator != "AND" && logicalOperator != "OR")
        return reverseOriginalText;

    for (int i = 0; i < fileReaders.size(); ++i)
    {
        if (logicalOperator == "XOR")
        {
            fileReaders[i] ^= value[i % value.size()];
        }
        else if (logicalOperator == "AND")
        {
            fileReaders[i] &= ~value[i % value.size()];
        }
        else if (logicalOperator == "OR")
        {
            fileReaders[i] |= ~value[i % value.size()];
        }
    }

    QString newText = QString::fromUtf8(fileReaders);
    return newText;
}

bool FileUtils::saveFile(const QString& savePath, const QString& newText, const QString& oldFileName, bool& checkState, bool overwrite)
{
    QString appendName = "_output";
    QString fileExtension = QFileInfo(oldFileName).suffix();
    QString cutFileName = QFileInfo(oldFileName).baseName();
    QString shortName = QDir::cleanPath(savePath + "/" + cutFileName + appendName + "." + fileExtension);

    if (overwrite) {
        // If not overwriting, simply replace the file
        QFile outputFile(shortName);
        if (!outputFile.open(QFile::WriteOnly | QFile::Text)) {
            return false;
        }
        QTextStream out(&outputFile);
        out << newText;
        outputFile.close();
    } else {
        // Если overwrite = false - то  сюда
        QFileInfo fileInfo(shortName);
        if (fileInfo.exists()) {
            int index = 1;
            while (fileInfo.exists(QString("%1/%2%3%4.%5")
                                       .arg(savePath)
                                       .arg(cutFileName)
                                       .arg(appendName)
                                       .arg(index)
                                       .arg(fileExtension))) {
                index++;
            }
            shortName = QDir::cleanPath(QString("%1/%2%3%4.%5")
                                            .arg(savePath)
                                            .arg(cutFileName)
                                            .arg(appendName)
                                            .arg(index)
                                            .arg(fileExtension));
        }
        QFile outputFile(shortName);
        if (!outputFile.open(QFile::WriteOnly | QFile::Text)) {
            return false;
        }
        QTextStream out(&outputFile);
        out << newText;
        outputFile.close();
    }

    if (checkState) {
        QFile::remove(oldFileName);
    }

    return true;
}

void FileUtils::readModifcateFile (Setting& settingStruct, ModifiedText& text)
{
    if (!warningMessange(settingStruct))
        return ;

    // settingStruct.
    QByteArray fileCont= FileUtils::fileReaders(settingStruct.inputStr);
    QString newText=FileUtils::textModify(fileCont,settingStruct.Mask,settingStruct.logicalOperator);
    text.text=newText;
    QByteArray byteArrayText = newText.toUtf8();
    text.reverseText =FileUtils::textModifyReverse(byteArrayText,settingStruct.Mask,settingStruct.logicalOperator);
    FileUtils::saveFile(settingStruct.savePath,newText,settingStruct.inputStr, settingStruct.checkState, settingStruct.saveRule);

}

bool FileUtils::warningMessange(Setting& settingStruct)
{
    for (const QString& field : {
             "исходный файл",
             "путь сохранения",
             "логический оператор",
             "значение маски"
         })
    {
        QString value;
        if (field == "исходный файл")
        {
            value = settingStruct.inputStr;
        }
        else if (field == "путь сохранения")
        {
            value = settingStruct.savePath;
        }
        else if (field == "логический оператор")
        {
            value = settingStruct.logicalOperator;
        }
        else if (field == "значение маски")
        {
            value = QString(settingStruct.Mask);
        }

        if (value.isNull() || value.isEmpty())
        {
            QMessageBox warningBox;
            warningBox.setText("Не указан " + field);
            warningBox.exec();
            return false;
        }
    }

    return true;
}



























































































































