#include <iostream>
#include "parsing.h"

int main(int argc, char* argv[])
{

    parsing parsingClass;

    subequation equation = {};
    parsingClass.variableAdder(equation,'x',10);

    std::string s;
    std::cin >> s;
    equation.sEq = s;

    double final = parsingClass.textParserEquation(equation);

    return 0;
}