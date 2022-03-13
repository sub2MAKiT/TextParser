#include <iostream>
#include "parsing.h"

int main(int argc, char* argv[])
{

    parsing parsingClass;

    variables X = {};
    X.name = 'x';
    X.value = 10;

    std::vector<variables> VV = {X};

    subequation equation = {};
    equation.vEq = VV;
    std::string s;
    std::cin >> s;
    equation.sEq = s;
    double final = parsingClass.textParserEquation(equation);

    return 0;
}