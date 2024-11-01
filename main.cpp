#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Создается объект QApplication с передачей ему аргументов командной строки. Этот объект управляет событием и всеми объектами Qt в приложении.
    MainWindow w;
    w.show(); //Метод show() делает главное окно видимым для пользователя.
    return a.exec();// Запускается главный цикл обработки событий приложения
}

