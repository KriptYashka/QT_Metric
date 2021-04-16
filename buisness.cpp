#include "buisness.h"
using namespace std;

bool is_normal_metric(string text){
    bool flag = true;
    if (text == "")
        flag = false;;
    for (int i = 0; i < text.length(); ++i){
        if (!isdigit(text[i]) && (text[i] != '-' && text[i] != '.')){
            flag = false;
        }
    }
    return flag;
}

void calculate(vector<double> arr, double& min, double& max, double& med){
    sort(arr.begin(), arr.end());
    min = 0;
    max = 0;
    med = 0;
    if (arr.size() != 0){
        min = arr[0];
        max = arr[arr.size() - 1];
        if (arr.size() % 2){
            med = arr[arr.size() / 2];
        } else {
            med = (arr[arr.size() / 2] + arr[arr.size() / 2 - 1]) / 2.0;
        }
    }
}

vector<string> split_line(string line){
    vector<string> result;
    string word = "";
    for (int i = 0; i < line.length(); ++i){
        char symbol = line[i];
        if ((symbol == ',') || symbol == '\n'){
            result.push_back(word);
            word = "";
            continue;
        }
        word += symbol;
    }
    result.push_back(word);
    return result;
}

vector<vector<string>> read_csv_file(string path){
    /* Считывает данные из файла и записывает в модель
    Возвращает код ошибки*/

    vector<vector<string>> result;
    string line;
    ifstream myFile(path);

    while(getline(myFile, line)){
        vector<string> line_model = split_line(line);
        result.push_back(line_model);
    }
    return result;
}
