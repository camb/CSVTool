#include <iostream>
#include <string>
#include <vector>
#include "Table.h"


namespace {
const char* HELP =
    "\n"
    "                    CSV TOOL\n"
    "Load and query tables of numerical data from CSV files.\n"
    "The numerical data is zero-indexed, excluding the column headers.\n"
    "Commands are case-insensitive and are executed sequentially.\n"
    "\n"
    "Separate commands by spaces and parameters by the \'-\' symbol.\n"
    "e.g. \"readcsv-data1.csv innerjoin-data2.csv-ID printtable\"\n"
    "\n"
    "These are all available operations:\n"
    "  READCSV         - Read in CSV data from file\n"
    "                    readcsv-[filename.csv]\n"
    "  PRINTTABLE      - Prints the table headers and data.\n"
    "                    printtable\n"
    "  PRINTHEADERS    - Prints the table headers.\n"
    "                    printheaders\n"
    "  PRINTROW        - Prints the row number provided.\n"
    "                    printrow-#\n"
    "  PRINTCOLUMN     - Prints the column number provided.\n"
    "                    printcolumn-#\n"
    "  PRINTCOLUMNS    - Prints the column numbers and ranges provided.\n"
    "                    printcolumns-#-#t#-#\n"
    "  PRINTNUMCOLUMNS - Prints the number of columns of data.\n"
    "                    printnumcolumns\n"
    "  PRINTNUMROWS    - Prints the number of rows of data, excluding headers.\n"
    "                    printnumrows\n"
    "  DELETECOLUMN    - Deletes a column by number.\n"
    "                    deletecolumn-#\n"
    "  DELETEROW       - Deletes a row by number.\n"
    "                    deleterow-#\n"
    "  INNERJOIN       - Left joins a second table of CSV data.\n"
    "                    innerjoin-[filename.csv]-[join column name]\n"
    "  OUTERJOIN       - Full outer joins a second table of CSV data.\n"
    "                    outerjoin-[filename.csv]-[join column name]\n"
    "  AVERAGECOLUMN   - Prints the average of a given column.\n"
    "                    averagecolumn-#\n"
    "  MEDIANCOLUMN    - Prints the median of a given column.\n"
    "                    mediancolumn-#\n"
    "  MINCOLUMN       - Prints the minimum value of a given column.\n"
    "                    mincolumn-#\n"
    "  MAXCOLUMN       - Prints the maximum value of a given column.\n"
    "                    maxcolumn-#\n"
    "  SUMCOLUMNS      - Sum two columns and append the result to table.\n"
    "                    sumcolumns-#-#\n"
    "  SUBTRACTCOLUMNS - Subtract two columns and append the result to table.\n"
    "                    subtractcolumns-#-#\n"
    "  DIVIDECOLUMNS   - Divide two columns and append the result to table.\n"
    "                    dividecolumns-#-#\n"
    "  MULTIPLYCOLUMNS - Multiply two columns and append the result to table.\n"
    "                    multiplycolumns-#-#\n"
    "  QUIT            - Exits the program.\n"
    "                    quit\n"
    "\n\n";
} // namespace


int main (int argc, char* argv[]) {
    Table T; // Default Table object
    if (argc > 1) { // Load the input CSV file if provided.
        if (!T.readCSV(std::string(argv[1]))) return 1;
        std::cout << "Table: " << argv[1] << " loaded.\n";
    }

    if (argc > 2) { // If > 2 arguments, parse them and exit.
        for (int i = 2; i < argc; i++) {
            if (!T.parseArg(std::string(argv[i]))) break;
        }
    } else { // No further arguments provided. Enter interactive mode.
        std::cout << "Interactive terminal...\n";
        std::cout << "Type \"help\" for options or \"quit\" to exit.\n\n";
        std::string input;
        while (true) { // Keep taking input until user quits.
            std::getline(std::cin, input, '\n');
            if (input == "help") std::cout << HELP;
            else if (input == "quit") return 0;
            else { // Parse each argument.
                std::vector<std::string> args = split(input, ' ');
                for (const auto& arg : args) {
                    if (!T.parseArg(arg)) break;
                }
            }
        }
    }
    return 0;
}