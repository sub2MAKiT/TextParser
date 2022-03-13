#include "parsing.h"
#include <iostream>
#include <cstring>

subequation parsing::numberToChar(subequation &eq,double number)
{
    subequation ntc = {};
    ntc.sEq = std::to_string(number);
    eq.sEq = ntc.sEq;
    if(eq.sEq.size() != 0 )
    {
        eq.siEq = eq.sEq.size();
        eq.cEq[eq.siEq];
        eq.sEq.copy(eq.cEq, eq.sEq.size(),0);
    }
    return eq;
}

int parsing::sizeOfNumber(double a)
{
    double fractpart, intpart;
    int b = 1;
    fractpart = modf(a, &intpart);
    if(fractpart != floor(fractpart))
        b++;
    for(int i = 1; fractpart * i != floor(fractpart * i); i *= 10)
    {
        b++;
    }
    for(int i = 1; i < intpart; i *= 10)
    {
        b++;
    }
    if(a < 0)
        b++;
    return b;
}

subequation parsing::resultImporting(subequation &eq, int startingPoint, int finalPoint, double number)
{
    if(finalPoint - startingPoint + 1 < sizeOfNumber(number))
    {
        for(int i = eq.siEq - 1; i > finalPoint; i--)
        {
            eq.cEq[i + 1] = eq.cEq[i + finalPoint - startingPoint + 1];
        }
    }
    subequation temp = {};
    numberToChar(temp, number);
    for(int i = startingPoint; i <= finalPoint; i++)
    {
        eq.cEq[i] = ' ';
    }
    for(int i = 0; i < temp.siEq; i++)
    {
        eq.cEq[i + startingPoint] = temp.cEq[i];
    }
    return eq;
}

subequation parsing::bracketParser(subequation &eq)
{
    int openB = 0;
    int startB = 0;
    int endB = 0;
    bool first = true;
    for(int i = 0; i < eq.siEq; i++)
    {
        if(eq.cEq[i] == '(')
        {
            if(first)
            {
                startB = i;
                first = false;
            }
            openB++;
        }
        if(eq.cEq[i] == ')')
        {
            endB = i;
            openB--;
        }
    }

    if(endB != startB)
    {
        eq.cEq[startB] = ' ';
        eq.cEq[endB] = ' ';
    }
    subequation subEq = {};
    if(!first)
    {
        subEq.siEq = endB - startB - 1;
        
        subEq.cEq[subEq.siEq];
        for(int i = 0; i < subEq.siEq;i++)
        {
            subEq.cEq[i] = eq.cEq[startB + i + 1];
        }
        resultImporting(eq,startB,endB,textParserEquation(subEq));
    }


    return eq;
}

numberParsed parsing::numberParser(subequation &eq, int startingPoint) {
    bool negative = false;
    bool decimal = true;
    double tempDivider = 1;
    double tempNumber = 0;
    int iaiai = 0;
    for(int i = startingPoint; i < eq.siEq; i++)
    {
        if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == fraEQS || eq.cEq[i] == negEQS)
        {
            if(eq.cEq[i] == fraEQS)
            {
                decimal = false;
            } else if(eq.cEq[i] == negEQS) {
                negative = true;
            }else if(eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9){
                if(decimal)
                    tempNumber *= 10;
                else
                    tempDivider *= 0.1;
                tempNumber += (eq.cEq[i] - '0') * tempDivider;
            }
        } else {
            break;
        }
        iaiai = i;
    }
    if(negative)
        tempNumber *= -1;
    numberParsed returnValue = {};
    returnValue.number = tempNumber;
    returnValue.final = iaiai;
    return returnValue;
}

subequation parsing::calculatorParser(subequation &eq, int stage)
{
    double buffera;
    double bufferb;
    int equationDone = 0;
    int startEquation;
    if(stage == 0)
    {
        for(int i = 0; i < eq.siEq; i++)
        {
            if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == fraEQS || eq.cEq[i] == negEQS)
            {
                if(equationDone == 1)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    double originalNumber = buffera;
                    for(int a = 1; a < bufferb;a++)
                    {
                        buffera *= originalNumber;
                    }
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = NP.final;
                    equationDone = 0;
                }
                else
                {
                    startEquation = i;
                    numberParsed tempNP = numberParser(eq,i);
                    buffera = tempNP.number;
                    i = tempNP.final;
                }
            }
            if(eq.cEq[i] == powEQS)
            {
                equationDone = 1;
            }
        }
    }
    if(stage == 1)
    {
        for(int i = 0; i < eq.siEq; i++)
        {
            if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == fraEQS || eq.cEq[i] == negEQS)
            {
                if(equationDone == 1)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera / bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = NP.final;
                    equationDone = 0;
                } else if (equationDone == 2)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera * bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = NP.final;
                    equationDone = 0;
                }
                else
                {
                    startEquation = i;
                    numberParsed tempNP = numberParser(eq,i);
                    buffera = tempNP.number;
                    i = tempNP.final;
                }
            }
            if(eq.cEq[i] == divEQS)
            {
                equationDone = 1;
            }            
            if(eq.cEq[i] == mulEQS)
            {
                equationDone = 2;
            }
        }
    }
    if(stage == 2)
    {
        for(int i = 0; i < eq.siEq; i++)
        {
            if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == fraEQS || eq.cEq[i] == negEQS)
            {
                if(equationDone == 1)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera - bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = NP.final;
                    equationDone = 0;
                } else if (equationDone == 2)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera + bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = NP.final;
                    equationDone = 0;
                }
                else
                {
                    startEquation = i;
                    numberParsed tempNP = numberParser(eq,i);
                    buffera = tempNP.number;
                    i = tempNP.final;
                }
            }
            if(eq.cEq[i] == subEQS)
            {
                equationDone = 1;
            }            
            if(eq.cEq[i] == addEQS)
            {
                equationDone = 2;
            }
        }
    }
    return eq;
}

subequation parsing::variableParser(subequation &eq)
{
    for(int i = 0; i < eq.siEq; i++)
    {
        for(int a = 0; a < eq.vEq.size();a++)
        {
            if(eq.cEq[i] == eq.vEq[a].name)
            {
                eq.siEq += sizeOfNumber(eq.vEq[a].value);
                eq.cEq[eq.siEq];
                for(int b = 0; b < eq.siEq - i; b++)
                {
                    eq.cEq[eq.siEq - b + sizeOfNumber(eq.vEq[a].value)] = eq.cEq[eq.siEq - b];
                }
                resultImporting(eq,i,i,eq.vEq[a].value);
            }
        }
    }
    return eq;
}

subequation parsing::textFixer(subequation &eq)
{
    int subtract = 0;
    for(int i = 0; i < eq.siEq; i++)
    {
        if(eq.cEq[i] == ' ')
        {
            subtract++;
            eq.cEq[i] = eq.cEq[i+1];
            eq.cEq[i+1] = ' ';
        }
    }
    return eq;
}

subequation parsing::variableAdder(subequation &eq, char symbol, double value)
{
    eq.vEq.push_back({ symbol, value });
}

double parsing::textParserEquation(subequation &eq)
{
    if(eq.sEq.size() != 0 )
    {
        eq.siEq = eq.sEq.size();
        eq.cEq[eq.siEq];
        eq.sEq.copy(eq.cEq, eq.sEq.size(),0);
    }
    eq = variableParser(eq);
    eq = bracketParser(eq);
    for(int i = 0; i < 3; i++)
    {
        eq = calculatorParser(eq, i);
    }
    numberParsed FINALNUMBER = {};
    FINALNUMBER = numberParser(eq, 0);
    printf("FINAL FINAL: %f", FINALNUMBER.number);
    return FINALNUMBER.number;
}