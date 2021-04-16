#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "buisness.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

/* Глобальные переменные */
QStandardItemModel *view_model = new QStandardItemModel;
QStandardItemModel *csv_model = new QStandardItemModel;
QStringList headers;

void MainWindow::closeApp(){
    QApplication::exit();
}

void model_cpy(QStandardItemModel* from, QStandardItemModel* to){
    /* Копирует данные из одной модели в другую */
    to->clear();
    for (int row = 0 ; row < from->rowCount() ; row++){
        QList<QStandardItem *> res;
        for (int i = 0; i < from->columnCount(); ++i){
            res.append(new QStandardItem(from->item(row, i)->text()));
        }
       to->appendRow(res);
    }
}

void MainWindow::on_inputPath_clicked(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"));
    vector<vector<string>> all_lines = read_csv_file(filePath.toStdString());

    if (all_lines.size() == 0){
        ui->Result_metrics->setText("Невозможно открыть файл");
        ui->Input_Region->setText("");
        ui->Input_number->setText("");
    } else {
        // Устанавливаем CSV модель

        csv_model->clear();
        csv_model->setColumnCount(all_lines.at(0).size());
        // Устанавливаем заголовки
        headers.clear();
        for (int i = 0; i < all_lines.at(0).size(); i++) {
            headers.push_back(QString::fromStdString(all_lines.at(0).at(i)));
        }
        csv_model->setHorizontalHeaderLabels(headers);
        for (int i = 1; i < all_lines.size(); i++) {
            vector<string> item_list = all_lines.at(i);
            QList<QStandardItem *> standardItemsList;
            for (string item_str : item_list){ // перебираем все элементы из массива
                QString q_item_str = QString::fromStdString(item_str);
                standardItemsList.append(new QStandardItem(q_item_str));
            }
            csv_model->insertRow(csv_model->rowCount(), standardItemsList);
        }
        model_cpy(csv_model, view_model);
    }
}


void MainWindow::on_LoadData_button_clicked(){
    ui->tableInformation->setModel(csv_model);
}


void MainWindow::on_Calculate_metrics_clicked(){
    /* Поиск нужный метрик и их вычисление */
    QString region = ui->Input_Region->text();
    QString column = ui->Input_number->text();
    bool flag;
    int res = column.toInt(&flag);
    int col_metric = -1;
    if (flag)
        col_metric = column.toInt();

    if (col_metric == -1 || col_metric > csv_model->columnCount() || col_metric < 1){
        ui->Result_metrics->setText("Переданы некорректные значение колонки.\nКолонка должна быть в диапозоне от 1 до 7.");
        return;
    }
    col_metric--;

    double minimum = 0;
    double maximum = 0;
    double average = 0;

    view_model->clear();
    view_model->setColumnCount(7);
    view_model->setHorizontalHeaderLabels(headers);

    // Выборка нужных записей
    for (int row = 0; row < csv_model->rowCount(); ++row){
        if ((csv_model->item(row, 1)->text() == region || region == "")){
            QList<QStandardItem *> res;
            for (int i = 0; i < csv_model->columnCount(); ++i){
                res.append(new QStandardItem(csv_model->item(row, i)->text()));
            }
            view_model->insertRow(view_model->rowCount(), res);
        }
    }

    ui->tableInformation->setModel(view_model);

    vector<double> metric_column;
    // Просчет метрик
    for (int row = 0; row < view_model->rowCount(); ++row){
        QString str = view_model->item(row, col_metric)->text();
        if (is_normal_metric(str.toStdString())){
            metric_column.push_back(view_model->item(row, col_metric)->text().toDouble());
        }
    }

    calculate(metric_column, minimum, maximum, average);
    QString result_text = "Минимум: "+ QString::number(minimum) +"\nМаксимум: "+ QString::number(maximum)
            +"\nМедиана: "+ QString::number(average);
    if (metric_column.size() == 0){
        result_text = "Нет результатов. Проверьте название региона или выбранную колонку.";
    }
    ui->Result_metrics->setText(result_text);
}
