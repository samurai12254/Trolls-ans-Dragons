#include "mainwindow.h"
#include <bits/stdc++.h>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QRandomGenerator>
#include <QSpinBox>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    GameWindow window;
    window.setWindowTitle("Hero Adventure Game");
    window.show();

    return a.exec();
}
