#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "buisness.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(7);
    csvModel->setHorizontalHeaderLabels(QStringList() << "Year" << "Region" << "Natural growth" << "Birth rate" << "Death rate"
                                        << "General dem. weight" << "Urbanization");
}

MainWindow::~MainWindow(){
    delete ui;
}
/* Глобальные переменные */
QStandardItemModel *general_model = new QStandardItemModel;
QVector<QString> col_names;

QList<QStandardItem *> get_row(QStandardItemModel* model, int row){
    QList<QStandardItem *> res;
    for (int i = 0; i < model->columnCount(); ++i){
        res.append(model->item(row, i));
    }
    return res;
}

void model_cpy(QStandardItemModel* from, QStandardItemModel* to){
   to->clear();
   for (int row = 0 ; row < from->rowCount() ; row++){
       QList<QStandardItem *> res;
       for (int i = 0; i < from->columnCount(); ++i){
           res.append(new QStandardItem(from->item(row, i)->text()));
       }
      to->appendRow(res);
   }
}

int read_csv_file(QString path, QStandardItemModel* model){
    /* Считывает данные из файла и записывает в модель
    Возвращает код ошибки*/
    std::ifstream file_csv(path.toStdString());
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return 0;
    }

    model->clear();
    model->setColumnCount(7);
    model->setHorizontalHeaderLabels(QStringList() << "Year" << "Region" << "Natural growth" << "Birth rate" << "Death rate"
                                        << "General dem. weight" << "Urbanization");

    QTextStream in(&file);
    if (!in.atEnd()){
        QString line = in.readLine();
    }
    while (!in.atEnd()){
        // Считывание из файла и перенос в таблицу
        QString line = in.readLine();
        QList<QStandardItem *> standardItemsList;
        for (QString item : line.split(",")) {
            standardItemsList.append(new QStandardItem(item));
        }
        model->insertRow(model->rowCount(), standardItemsList);
    }
    file.close();
    return 1;
}

void MainWindow::on_btn_loadfile_clicked(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"));
    int cod = read_csv_file(filePath, csvModel);
    model_cpy(csvModel, general_model);
    if (cod == 0){
        ui->label_title->setText("Нет таблицы");
        ui->label_result->setText("Не возможно открыть файл");
        ui->line_region->setText("");
        ui->line_col->setText("");
    } else {
        ui->label_title->setText(filePath);
    }
}


void MainWindow::on_btn_load_clicked(){
    ui->table_metric->setModel(csvModel);
}

int check_column(QString col){
    bool flag = true;
    int res = col.toInt(&flag);
    if (flag)
        return res;
    return -1;
}

bool is_normal_metric(QString text){
    if (text == "")
        return false;
    for (int i = 0; i < text.length(); ++i){
        if (!text[i].isDigit() && (text[i] != '-' && text[i] != '.')){
            return false;
        }
    }
    return true;
}

void MainWindow::on_btn_metric_clicked(){
    QString region = ui->line_region->text();
    QString column = ui->line_col->text();
    int col_metric = check_column(column);
    if (col_metric == -1 || col_metric > csvModel->rowCount() || col_metric < 1){
        ui->label_result->setText("Переданы некорректные значения");
        return;
    }
    col_metric++;

    double minimum = 0;
    double maximum = 0;
    double average = 0;

    general_model->clear();
    general_model->setColumnCount(7);
    general_model->setHorizontalHeaderLabels(QStringList() << "Year" << "Region" << "Natural growth" << "Birth rate" << "Death rate"
                                        << "General dem. weight" << "Urbanization");


    // Выборка нужных записей
    for (int row = 0; row < csvModel->rowCount(); ++row){
        if ((csvModel->item(row, 1)->text() == region || region == "")){
            QList<QStandardItem *> res;
            for (int i = 0; i < csvModel->columnCount(); ++i){
                res.append(new QStandardItem(csvModel->item(row, i)->text()));
            }
            general_model->insertRow(general_model->rowCount(), res);
        }
    }

    ui->table_metric->setModel(general_model);

    std::vector<double> arr;
    // Просчет метрик
    for (int row = 0; row < general_model->rowCount(); ++row){
        QString str = general_model->item(row, col_metric)->text();
        if (is_normal_metric(str)){
            arr.push_back(general_model->item(row, col_metric)->text().toDouble());
        }
    }

    calc_metric(arr, col_metric, &minimum, &maximum, &average);
    QString result_text = "Минимум: "+ QString::number(minimum) +"\nМаксимум: "+ QString::number(maximum)
            +"\nМедиана: "+ QString::number(average);
    ui->label_result->setText(result_text);
}
