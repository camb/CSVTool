#include "Table.h"
#include <cmath>
#include <fstream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <unordered_map>


// Split input string into vector of strings on char delimiter.
// Needed for parsing args and parameters.
std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> output;
    std::stringstream ss(str);
    std::string arg;
    while (std::getline(ss, arg, delim)) {
        if (!arg.empty())
            *(std::back_inserter(output)++) = arg;
    }
    return output;
}

namespace { // Anonymous namespace for helper functions.
// Quick Sort Implementation for getColMedian().
unsigned int getPivot(std::vector<double>& values, unsigned int start, 
    unsigned int end, unsigned int pos) {
    std::swap(values[start], values[pos]);

    unsigned int left = start + 1;
    unsigned int right = end;
    while (left < right) {
        if (values[left] < values[start]) left++;
        else if (values[--right] < values[start])
            std::swap(values[left], values[right]);
    }

    std::swap(values[start], values[--left]);
    return left;
}

void quickSort(std::vector<double>& values,
               unsigned int left, unsigned int right) {
    if (left >= right) return;
    unsigned int mid = (left + right)/2;
    unsigned int pivot = getPivot(values, left, right, mid);

    if (left < pivot) quickSort(values, left, pivot);
    if (pivot < right) quickSort(values, pivot + 1, right);
}

void quickSort(std::vector<double>& values) {
    if (values.empty()) return;
    quickSort(values, 0, values.size());
}

// Enum and hash map for quick command lookups in parseArg().
enum CommandsEnum {
    READCSV,
    PRINTTABLE,
    PRINTHEADERS,
    PRINTROW,
    PRINTCOLUMN,
    PRINTCOLUMNS,
    PRINTNUMCOLUMNS,
    PRINTNUMROWS,
    DELETECOLUMN,
    DELETEROW,
    INNERJOIN,
    OUTERJOIN,
    AVERAGECOLUMN,
    MEDIANCOLUMN,
    MINCOLUMN,
    MAXCOLUMN,
    SUMCOLUMNS,
    SUBTRACTCOLUMNS,
    DIVIDECOLUMNS,
    MULTIPLYCOLUMNS,
    QUIT,
};
static std::unordered_map<std::string, CommandsEnum> command_to_int{
    {"READCSV",         READCSV},
    {"PRINTTABLE",      PRINTTABLE},
    {"PRINTHEADERS",    PRINTHEADERS},
    {"PRINTROW",        PRINTROW},
    {"PRINTCOLUMN",     PRINTCOLUMN},
    {"PRINTCOLUMNS",    PRINTCOLUMNS},
    {"PRINTNUMCOLUMNS", PRINTNUMCOLUMNS},
    {"PRINTNUMROWS",    PRINTNUMROWS},
    {"DELETECOLUMN",    DELETECOLUMN},
    {"DELETEROW",       DELETEROW},
    {"INNERJOIN",       INNERJOIN},
    {"OUTERJOIN",       OUTERJOIN},
    {"AVERAGECOLUMN",   AVERAGECOLUMN},
    {"MEDIANCOLUMN",    MEDIANCOLUMN},
    {"MINCOLUMN",       MINCOLUMN},
    {"MAXCOLUMN",       MAXCOLUMN},
    {"SUMCOLUMNS",      SUMCOLUMNS},
    {"SUBTRACTCOLUMNS", SUBTRACTCOLUMNS},
    {"DIVIDECOLUMNS",   DIVIDECOLUMNS},
    {"MULTIPLYCOLUMNS", MULTIPLYCOLUMNS},
    {"QUIT",            QUIT},
};
} // namespace


// Table Class Implementations
bool Table::parseArg(const std::string& arg) {
    std::vector<std::string> params;
    params = split(arg, '-');
    // First parameter in arg is a command, ie. READCSV, PRINTROW, etc.
    // Following parameters are filename, row #, etc.

    // Uppercase the command to ignore case sensitivity.
    std::string command(params[0]);
    for (auto& ch : command) ch = std::toupper(ch);
    // Check valid command.
    if (!command_to_int.count(command)) {
        std::cout << "Unable to parse command: " << params[0] << "\n\n";
        return false;
    }

    // Lambda to confirm number of params and output message.
    static auto checkParams = [](int s, int p) {
        if (s != p) std::cout << "Bad parameters. Check help.\n\n";
        return (s == p);
    };

    // Run the appropriate Table method based on input command.
    switch (command_to_int[command]) {
        case(READCSV):
            if (checkParams(params.size(), 2))
                return readCSV(params[1]);
            return false;
        case(PRINTTABLE):
            return printTable();
        case(PRINTHEADERS):
            return printHeaders();
        case(PRINTROW):
            if (checkParams(params.size(), 2))
                return printRow(stoi(params[1]));
            return false;
        case(PRINTCOLUMN):
            if (checkParams(params.size(), 2))
                return printColumn(stoi(params[1]));
            return false;
        case(PRINTCOLUMNS):
            return printColumns(params);
        case(PRINTNUMCOLUMNS):
            return printNumColumns();
        case(PRINTNUMROWS):
            return printNumRows();
        case(DELETECOLUMN):
            if (checkParams(params.size(), 2))
                return deleteColumn(stoi(params[1]));
            return false;
        case(DELETEROW):
            if (checkParams(params.size(), 2))
                return deleteRow(stoi(params[1]));
            return false;
        case(INNERJOIN):
            if (checkParams(params.size(), 3)) {
                Table other(params[1]);
                return innerJoin(other, params[2]);
            }
            return false;
        case(OUTERJOIN):
            if (checkParams(params.size(), 3)) {
                Table other(params[1]);
                return outerJoin(other, params[2]);
            }
            return false;
        case(AVERAGECOLUMN):
            if (checkParams(params.size(), 2)) 
                return printColumnAverage(stoi(params[1]));
            return false;
        case(MEDIANCOLUMN):
            if (checkParams(params.size(), 2)) 
                return printColumnMedian(stoi(params[1]));
            return false;
        case(MINCOLUMN):
            if (checkParams(params.size(), 2)) 
                return printColumnMin(stoi(params[1]));
            return false;
        case(MAXCOLUMN):
            if (checkParams(params.size(), 2)) 
                return printColumnMax(stoi(params[1]));
            return false;
        case(SUMCOLUMNS):
            if (checkParams(params.size(), 3)) 
                return sumColumns(stoi(params[1]), stoi(params[2]));
            return false;
        case(SUBTRACTCOLUMNS):
            if (checkParams(params.size(), 3)) 
                return subtractColumns(stoi(params[1]), stoi(params[2]));
            return false;
        case(DIVIDECOLUMNS):
            if (checkParams(params.size(), 3)) 
                return divideColumns(stoi(params[1]), stoi(params[2]));
            return false;
        case(MULTIPLYCOLUMNS):
            if (checkParams(params.size(), 3)) 
                return multiplyColumns(stoi(params[1]), stoi(params[2]));
            return false;
        case(QUIT):
            std::exit(0);
    }
}


bool Table::readCSV(const std::string& filename) {
    // Ensure data/headers are empty before reading in data_.
    if (!headers_.empty() || !data_.empty()) {
        std::cout << "Table already loaded: " << name_ << "\n\n";
        return false;
    }

    // This would need to be more robust to error handling in production.
    std::ifstream csv_file;
    csv_file.open("./" + filename);
    if (!csv_file) {
        std::cout << "Unable to open CSV file: " << filename << "\n\n";
        return false;
    }
    name_ = filename;

    std::string line;
    // Read in column headers.
    std::getline(csv_file, line);
    std::stringstream ss(line);
    while (ss) {
        std::string cel;
        if (!std::getline(ss, cel, ',')) break;
        headers_.push_back(cel);
    }

    // Read in the numeric data_.
    while (std::getline(csv_file, line)) {
        std::stringstream ss(line);
        std::vector<double> row;

        while (ss) {
            std::string cell;
            if (!std::getline(ss, cell, ',')) break;
            // Would ideally want to handle bad/varied input...
            if (cell.empty()) row.push_back(NAN); // If cell empty, NaN.
            else row.push_back(stod(cell));
        }
        data_.push_back(row);
    }
    csv_file.close();
    return true;
}


bool Table::printTable() const {
    printHeaders();
    // printRow takes int argument.
    for (unsigned int row = 0; row < data_.size(); row++) printRow(row);
    std::cout << std::endl;
    return true;
}


bool Table::printHeaders() const {
    for (const auto& h : headers_) std::cout << h << ","; // Trailing comma.
    std::cout << std::endl;
    return true;
}


bool Table::printColumn(const unsigned int col, const bool header_on) const {
    // Ideally justify to data_ width.
    if (!checkValidColumn(col)) return false;
    if (header_on) std::cout << headers_[col] << std::endl;
    for (const auto& row : data_) std::cout << row[col] << std::endl;
    return true;
}


bool Table::printColumns(const std::vector<std::string>& params,
                      const bool header_on) const {
    // Ideally justify to data_ width.
    std::vector<unsigned int> cols;
    // Parse individual col numbers as well as ranges formatted "9t11".
    // Inclusively handle ranges, ie. 9t11 prints 9, 10, 11.
    for (int i = 1; i < params.size(); i++) { // Ignore params[0], the command.
        std::vector<std::string> range = split(params[i], 't');
        if (range.size() == 2) {
            unsigned int start = stoi(range[0]);
            unsigned int end = stoi(range[1]);
            checkValidColumn(start);
            checkValidColumn(end);
            for (unsigned int i = start; i <= end; i++)
                cols.push_back(i);
        } else if (range.size() > 2) {
            std::cout << "Incorrect range format: " << params[i] << std::endl;
            return false;
        } else {
            unsigned int col = stoi(params[i]);
            if (!checkValidColumn(col)) return false;
            cols.push_back(col);
        }
    }
    // Print selected column headers.
    if (header_on) {
        for (const auto& col : cols) std::cout << headers_[col] << ",";
        std::cout << std::endl;
    }
    // Print selected column data_.
    for (const auto& row : data_) {
        for (const auto& col : cols) std::cout << row[col] << ",";
        std::cout << std::endl;
    }
    return true;
}


bool Table::printRow(const unsigned int row, const bool header_on) const {
    if (!checkValidRow(row)) return false;
    if (header_on) {
        for (const auto& h : headers_) std:: cout << h << ",";
        std::cout << std::endl;
    }
    for (const auto& cel : data_[row]) std::cout << cel << ",";
    std::cout << std::endl;
    return true;
}


bool Table::checkValidRow(const unsigned int row) const {
    if (row >= data_.size()) {
        std::cout << "Row out of range: " << row << std::endl;
        return false;
    }
    return true;
}


bool Table::checkValidColumn(const unsigned int col) const {
    if (col >= headers_.size()) {
        std::cout << "Col out of range: " << col << std::endl;
        return false;
    }
    return true;
}


void Table::appendColumn(const std::string& col_name,
                      const std::vector<double>& col_vals) {
    headers_.push_back(col_name);
    for (unsigned int row = 0; row < data_.size(); row++)
        data_[row].push_back(col_vals[row]);
}


bool Table::deleteColumn(const unsigned int col) {
    if (!checkValidColumn(col)) return false;
    headers_.erase(headers_.begin() + col);
    for (auto& row : data_)
        row.erase(row.begin() + col);
    return true;
}


bool Table::deleteRow(const unsigned int row) {
    if (!checkValidRow(row)) return false;
    data_.erase(data_.begin() + row);
    return true;
}


bool Table::findMatchingColumn(const Table& other, const std::string& join_col_name,
                            int& this_col, int& other_col) const {
    for (unsigned int i = 0; i < this->headers_.size(); i++) {
        if (this->headers_[i] == join_col_name) {
            this_col = i;
            break;
        }
    }
    for (unsigned int i = 0; i < other.headers_.size(); i++) {
        if (other.headers_[i] == join_col_name) {
            other_col = i;
            break;
        }
    }
    // If either Table is missing the join column, we can't join.
    if (this_col == -1 || other_col == -1) {
        std::cout << "Column not found: " << join_col_name << std::endl;
        return false;
    }
    return true;
}


void Table::joinMissingColumns(const Table& other,
                               int& this_col, int& other_col) {
    // Find columns to join from other Table that are not in this Table.
    // Performance could be improved here.
    std::vector<unsigned int> other_cols_to_join;
    for (unsigned int i = 0; i < other.headers_.size(); i++) {
        bool found_col = false;
        for (unsigned int j = 0; j < this->headers_.size(); j++) {
            if (other.headers_[i] == this->headers_[j]) {
                found_col = true;
                break;
            }
        }
        if (!found_col) other_cols_to_join.push_back(i);
    }
    // Check that there are columns in other to append.
    if (other_cols_to_join.empty()) return;

    // Map matching rows between Tables in a vector.
    std::vector<int> this_to_other_row(this->data_.size(), -1);
    for (unsigned int i = 0; i < other.data_.size(); i++) {
        for (unsigned int j = 0; j < this->data_.size(); j++) {
            if (this->data_[j][this_col] == other.data_[i][other_col]) {
                this_to_other_row[j] = i;
            }
        }
    }
    // Append the new columns to all existing rows.
    for (auto col : other_cols_to_join) {
        std::vector<double> other_col_vals(this->data_.size(), NAN);
        for (unsigned int i = 0; i < this->data_.size(); i++) {
            if (this_to_other_row[i] == -1) continue; // Default NaN.
            other_col_vals[i] = other.data_[this_to_other_row[i]][col];
        }
        this->appendColumn(other.headers_[col], other_col_vals);
    }
}


bool Table::innerJoin(const Table& other, const std::string& join_col_name) {
    if (headers_.empty() || data_.empty()) {
        std::cout << "No main table data loaded.\n";
        return false;
    }

    // Find matching column index between Tables.
    int this_col = -1, other_col = -1;
    if(!findMatchingColumn(other, join_col_name, this_col, other_col))
        return false; // No match found.
    // Join Tables based on matching column ID
    joinMissingColumns(other, this_col, other_col);
    return true;
}


bool Table::outerJoin(const Table& other, const std::string& join_col_name) {
    if (headers_.empty() || data_.empty()) {
        std::cout << "No main table loaded.\n";
        return false;
    }

    // Find matching column index between Tables.
    int this_col = -1, other_col = -1;
    if(!findMatchingColumn(other, join_col_name, this_col, other_col))
        return false; // No match found.
    // Perform innerJoin.
    joinMissingColumns(other, this_col, other_col);


    // Collect rows from other Table that are not in this Table.
    // Sorting on the join column would speed this up.
    std::vector<unsigned int> missing_other_rows;
    for (unsigned int i = 0; i < other.data_.size(); i++) {
        bool found_row = false;
        for (unsigned int j = 0; j < this->data_.size(); j++) {
            if (other.data_[i][other_col] == this->data_[j][this_col]) {
                found_row = true;
                break;
            }
        }
        if (!found_row) missing_other_rows.push_back(i);
    }
    // Check that there are rows in other to append.
    if (missing_other_rows.empty()) return true;

    // Map matching column between Tables in a vector.
    std::vector<int> this_to_other_col(this->headers_.size(), -1);
    for (unsigned int i = 0; i < other.headers_.size(); i++) {
        for (unsigned int j = 0; j < this->headers_.size(); j++) {
            if (other.headers_[i] == this->headers_[j]) {
                this_to_other_col[j] = i;
            }
        }
    }

    // Append the new rows to existing Table.
    for (auto row : missing_other_rows) {
        std::vector<double> new_row(this->headers_.size(), NAN);
        for (unsigned int col = 0; col < this->headers_.size(); col++) {
            if (this_to_other_col[col] == -1) continue; // Default NaN.
            new_row[col] = other.data_[row][this_to_other_col[col]];
        }
        this->data_.push_back(new_row);
    }
    return true;
}


// Used by mathematical methods to gather numbers and exclude NaN values.
std::vector<double> Table::getColumnValues(const unsigned int col) const {
    std::vector<double> vals;
    for (const auto& row : data_) {
        if (std::isnan(row[col])) continue; // Skip NaN cells.
        vals.push_back(row[col]);
    }
    return vals;
}


bool Table::printColumnMin(const unsigned int col) const {
    if (!checkValidColumn(col)) return false;
    std::vector<double> vals = getColumnValues(col);
    if (vals.empty()) return true;

    double min = vals[0];
    for (const auto val : vals) {
        if (val < min) min = val;
    }
    std::cout << min << std::endl;
    return true;
}


bool Table::printColumnMax(const unsigned int col) const {
    if (!checkValidColumn(col)) return false;
    std::vector<double> vals = getColumnValues(col);
    if (vals.empty()) return true;

    double max = vals[0];
    for (const auto val : vals) {
        if (val > max) max = val;
    }
    std::cout << max << std::endl;
    return true;
}


bool Table::printColumnAverage(const unsigned int col) const {
    if (!checkValidColumn(col)) return false;
    double average = 0;
    unsigned int count = 0;
    for (const auto& row : data_) {
        if (std::isnan(row[col])) continue; // Skip NaN cells.
        average+=row[col];
        count++;
    }
    if (count == 0) return true; // No values to average.

    average/=count;
    std::cout << average << std::endl;
    return true;
}


bool Table::printColumnMedian(const unsigned int col) const {
    if (!checkValidColumn(col)) return false;
    std::vector<double> vals = getColumnValues(col);
    if (vals.empty()) return true;

    quickSort(vals);
    double median = vals[vals.size()/2];
    if (vals.size() % 2 == 0)
        median= (median + vals[vals.size()/2 - 1])/2;
    std::cout << median << std::endl;
    return true;
}


bool Table::sumColumns(const unsigned int col1, const unsigned int col2) {
    if (!checkValidColumn(col1)) return false;
    if (!checkValidColumn(col2)) return false;
    std::string new_col_name = headers_[col1] + "_+_" + headers_[col2];
    headers_.push_back(new_col_name);
    for (auto& row : data_) {
        if (std::isnan(row[col1]) || std::isnan(row[col2]))
            row.push_back(NAN); // NaN for NaN operation cells.
        else row.push_back(row[col1] + row[col2]);
    }
    return true;
}


bool Table::subtractColumns(const unsigned int col1, const unsigned int col2) {
    if (!checkValidColumn(col1)) return false;
    if (!checkValidColumn(col2)) return false;
    std::string new_col_name = headers_[col1] + "_-_" + headers_[col2];
    headers_.push_back(new_col_name);
    for (auto& row : data_) {
        if (std::isnan(row[col1]) || std::isnan(row[col2]))
            row.push_back(NAN); // NaN for NaN operation cells.
        else row.push_back(row[col1] - row[col2]);
    }
    return true;
}


bool Table::divideColumns(const unsigned int col1, const unsigned int col2) {
    if (!checkValidColumn(col1)) return false;
    if (!checkValidColumn(col2)) return false;
    std::string new_col_name = headers_[col1] + "_/_" + headers_[col2];
    headers_.push_back(new_col_name);
    for (auto& row : data_) {
        if (std::isnan(row[col1]) || std::isnan(row[col2]))
            row.push_back(NAN); // NaN for NaN operation cells.
        else row.push_back(row[col1] / row[col2]);
    }
    return true;
}


bool Table::multiplyColumns(const unsigned int col1, const unsigned int col2) {
    if (!checkValidColumn(col1)) return false;
    if (!checkValidColumn(col2)) return false;
    std::string new_col_name = headers_[col1] + "_*_" + headers_[col2];
    headers_.push_back(new_col_name);
    for (auto& row : data_) {
        if (std::isnan(row[col1]) || std::isnan(row[col2]))
            row.push_back(NAN); // NaN for NaN operation cells.
        else row.push_back(row[col1] * row[col2]);
    }
    return true;
}
