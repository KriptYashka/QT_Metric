#include "buisness.h"
using namespace std;

bool is_digit(char c){
    if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0')
        return true;
    return false;
}

bool is_normal_metric(string text){
    /* Проверяет метрику на числовой формат */
    if (text == "")
        return false;
    for (int i = 0; i < text.length(); ++i){
        if (!is_digit(text[i]) && (text[i] != '-' && text[i] != '.')){
            return false;
        }
    }
    return true;
}

/*std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<int>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    int val;

    // Read the column names
    if(myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<int> {}});
        }
    }

    // Read data, line by line
    while(std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;

        // Extract each integer
        while(ss >> val){

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();

            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    myFile.close();

    return result;
}*/

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
    if (!myFile.is_open()) throw runtime_error("Could not open file");

    //getline(myFile, line);

    while(getline(myFile, line)){
        vector<string> line_model = split_line(line);
        result.push_back(line_model);
    }
    return result;
}
