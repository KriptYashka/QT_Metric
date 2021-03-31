#ifndef BUISNESS_H
#define BUISNESS_H
#include "mainwindow.h"
using namespace std;

void calc_metric(std::vector<double> arr, int col_metric, double* min, double* max, double* avg);
bool is_normal_metric(string text);
bool is_digit(char c);
bool is_csv_file(string path);
vector<vector<string>> read_csv_file(string path);

#endif // BUISNESS_H
