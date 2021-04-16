#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <fstream>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LoadData_button_clicked();
    void on_inputPath_clicked();
    void on_Calculate_metrics_clicked();
    void closeApp();

private:
    Ui::MainWindow *ui;
    QShortcut *keyEnter;
    QShortcut *keyEsc;

};
#endif // MAINWINDOW_H
