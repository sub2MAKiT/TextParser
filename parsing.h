#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

struct variables {
    char name;
    double value;
};

struct subequation {
    std::string sEq;
    int siEq;
    std::vector<variables> vEq;
    char cEq[1];
};

struct numberParsed {
    double number;
    int final;
};

class parsing {
public:
    bool moduloFordoubles(double a, double b);

    subequation bracketParser(subequation &eq);

    numberParsed numberParser(subequation &eq, int startingPoint);

    subequation calculatorParser(subequation &eq, int stage);

    double textParserEquation(subequation &eq);

    int sizeOfNumber(double a);

    subequation resultImporting(subequation &eq, int startingPoint, int finalPoint, double number);

    subequation numberToChar(subequation &eq, double number);

    subequation textFixer(subequation &eq);

    subequation variableParser(subequation &eq);

    subequation variableAdder(subequation &eq, char symbol, double value);

    char subEQS = '_';
    char addEQS = '+';
    char mulEQS = '*';
    char divEQS = '/';
    char powEQS = '^';
    char negEQS = '-';
    char fraEQS = '.';
};