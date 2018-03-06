# CSV TOOL

Load and query tables of numerical data from CSV files.


## Build Instructions:

    This code was written and compiled on Ubuntu 16.04 with g++ using C++ 11.


## Compile command:

    g++ -std=c++0x CSVTool.cpp Table.cpp -o CSVTool;
    

## Organization of Files:

 - CSVTool.cpp 
     - Contains the main() function.
 - Table.h 
     - The Table class header file.
 - Table.cpp
     - The Table class implementation file.
 - data1.csv and data2.csv
     - Simple CSV files with a shared ID column.


## Instructions:

This tool can be run with a single complete query or run in an interactive mode where repeated queries can be made. A complete query begins with CSV filename and then subsequent commands. If only a CSV filename is provided, the CSV file will be loaded and then interactive mode begins. In interactive mode, CSV files can also be loaded with the READCSV command.

A main Table can only be loaded once per session or complete query. Inner and outer join operations join the second Table data into the main Table object.

Arguments are separated by space and parameters within an argument are separated by the '-' symbol. Commands are case-insensitive and are executed sequentially. Multiple commands can be typed sequentially and entered at once. If a bad command is entered in a series, the tool will stop parsing and print a statement.

Rows and columns are zero-indexed for the Table data. The header row is separately accessed with the PRINTHEADERS command.


## These are all available operations:

    READCSV         - Read in CSV data from file
                    readcsv-[filename.csv]
    PRINTTABLE      - Prints the table headers and data.
                    printtable
    PRINTHEADERS    - Prints the table headers.
                    printheaders
    PRINTROW        - Prints the row number provided.
                    printrow-#
    PRINTCOLUMN     - Prints the column number provided.
                    printcolumn-#
    PRINTCOLUMNS    - Prints the column numbers and ranges provided.
                    printcolumns-#-#t#-#
    PRINTNUMCOLUMNS - Prints the number of columns of data.
                    printnumcolumns
    PRINTNUMROWS    - Prints the number of rows of data, excluding headers.
                    printnumrows
    DELETECOLUMN    - Deletes a column by number.
                    deletecolumn-#
    DELETEROW       - Deletes a row by number.
                    deleterow-#
    INNERJOIN       - Left joins a second table of CSV data.
                    innerjoin-[filename.csv]-[join column name]
    OUTERJOIN       - Full outer joins a second table of CSV data.
                    outerjoin-[filename.csv]-[join column name]
    AVERAGECOLUMN   - Prints the average of a given column.
                    averagecolumn-#
    MEDIANCOLUMN    - Prints the median of a given column.
                    mediancolumn-#
    MINCOLUMN       - Prints the minimum value of a given column.
                    mincolumn-#
    MAXCOLUMN       - Prints the maximum value of a given column.
                    maxcolumn-#
    SUMCOLUMNS      - Sum two columns and append the result to table.
                    sumcolumns-#-#
    SUBTRACTCOLUMNS - Subtract two columns and append the result to table.
                    subtractcolumns-#-#
    DIVIDECOLUMNS   - Divide two columns and append the result to table.
                    dividecolumns-#-#
    MULTIPLYCOLUMNS - Multiply two columns and append the result to table.
                    multiplycolumns-#-#
    QUIT            - Exits the program.
                    quit


## Example Complete Queries:

    ./CSVTool data1.csv printrow-2
    ./CSVTool data1.csv innerjoin-data2.csv-ID printtable
    ./CSVTool data1.csv multiplycolumns-2-3 printtable mincolumn-5
    ./CSVTool data1.csv subtractcolumns-1-2 dividecolumns-2-3 printtable printnumcolumns


## Example Interactive Mode Queries (run ./CSVTool to begin):

    readcsv-data1.csv
    printtable
    sumcolumns-1-2 printtable
    innerjoin-data2.csv-ID printtable
    printtable deleterow-0 deletecolumn-1 printtable
    multiplycolumns-0-1 multiplycolumns-1-2 printtable