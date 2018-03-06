#pragma once
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> split(const std::string &str, char delim);

class Table {
public:
    Table() = default;
    Table(const std::string& filename) { readCSV(filename); }
    ~Table() = default;

    // File input and parsing
    bool parseArg(const std::string& arg);
    bool readCSV(const std::string& filename);

private:
    std::string name_;
    std::vector<std::string> headers_;
    std::vector<std::vector<double>> data_;

    // Display methods
    bool printTable() const;
    bool printHeaders() const;
    bool printColumn(const unsigned int col, const bool header_on=true) const;
    bool printColumns(const std::vector<std::string>& params,
                      const bool header_on=true) const;
    bool printRow(const unsigned int row, const bool header_on=false) const;

    // Table operations
    bool printNumColumns() const { std::cout << headers_.size() << std::endl; 
                                   return true; }
    bool printNumRows() const { std::cout << data_.size() << std::endl;
                                return true; }

    bool checkValidRow(const unsigned int row) const;
    bool checkValidColumn(const unsigned int col) const;

    void appendColumn(const std::string& col_name,
                      const std::vector<double>& col_vals);
    bool deleteColumn(const unsigned int col);
    bool deleteRow(const unsigned int row);

    bool findMatchingColumn(const Table& other,
                            const std::string& join_col_name,
                            int& this_col, int& other_col) const;
    void joinMissingColumns(const Table& other, int& this_col, int& other_col);
    bool innerJoin(const Table& other, const std::string& join_col_name);
    bool outerJoin(const Table& other, const std::string& join_col_name);

    // Mathematical methods
    std::vector<double> getColumnValues(const unsigned int col) const;
    bool printColumnMin(const unsigned int col) const;
    bool printColumnMax(const unsigned int col) const;
    bool printColumnAverage(const unsigned int col) const;
    bool printColumnMedian(const unsigned int col) const;

    bool sumColumns(const unsigned int col1, const unsigned int col2);
    bool subtractColumns(const unsigned int col1, const unsigned int col2);
    bool divideColumns(const unsigned int col1, const unsigned int col2);
    bool multiplyColumns(const unsigned int col1, const unsigned int col2);
};
