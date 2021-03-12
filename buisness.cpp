#include "buisness.h"
#include "mainwindow.h"

void calc_metric(std::vector<double> arr, int col_metric, double* min, double* max, double* avg){
    std::sort(arr.begin(), arr.end());
    *min = 0;
    *max = 0;
    *avg = 0;
    if (arr.size() != 0){
        *min = arr[0];
        *max = arr[arr.size() - 1];
        *avg = arr[arr.size() / 2];
    }
}
