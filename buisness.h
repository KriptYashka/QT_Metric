#ifndef BUISNESS_H
#define BUISNESS_H
#include "mainwindow.h"
using namespace std;

void calculate(std::vector<double> arr, double& min, double& max, double& avg);
bool is_normal_metric(string text);
bool is_digit(char c);
bool is_csv_file(string path);
vector<vector<string>> read_csv_file(string path);

#endif // BUISNESS_H
