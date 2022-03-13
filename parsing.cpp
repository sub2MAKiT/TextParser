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
        eq.cEq[eq.siEq]; //#ff0000
        eq.sEq.copy(eq.cEq, eq.sEq.size(),0);//#ff0000
    }
    printf("\n\n\nimporting first: ");
     for(int i = 0; i < eq.siEq; i++)
    {
        printf("%c",eq.cEq[i]);
    }
    printf("\n");
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
    printf("\nimporting result\n");
    if(finalPoint - startingPoint + 1 < sizeOfNumber(number))
    {
        for(int i = eq.siEq - 1; i > finalPoint; i--)
        {
            eq.cEq[i + 1] = eq.cEq[i + finalPoint - startingPoint + 1];
        }
    }
    subequation temp = {};
    printf("test1");
    numberToChar(temp, number);
    for(int i = startingPoint; i <= finalPoint; i++)
    {
        eq.cEq[i] = ' ';
    }
    printf("\n\n\nimporting: ");
    for(int i = 0; i < temp.siEq;i++)
    {
        printf("%c",temp.cEq[i]);
    }
    printf("\n");
    for(int i = 0; i < sizeOfNumber(number); i++)
    {
        eq.cEq[i + startingPoint] = temp.cEq[i];
    }
    return eq;
}

subequation parsing::bracketParser(subequation &eq)
{
    printf("\nparsing brackets\n");
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

    printf("start: %d\n",startB);
    printf("end: %d\n",endB);
    if(endB != startB)
    {
        eq.cEq[startB] = ' ';
        eq.cEq[endB] = ' ';
    }
    //#a0a0fa
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
    //#a0a0fa


    return eq;
}

numberParsed parsing::numberParser(subequation &eq, int startingPoint) {
    printf("\nparsing a number\n");
    bool negative = false;
    bool decimal = true;
    double tempDivider = 1;
    double tempNumber = 0;
    int iaiai = 0;
    for(int i = startingPoint; i < eq.siEq; i++)
    {
        if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == '.' || eq.cEq[i] == '-')
        {
            if(eq.cEq[i] == '.')
            {
                decimal = false;
            } else if(eq.cEq[i] == '-') {
                negative = true;
            }else {
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
    printf("\ncalculating\n");
    double buffera;
    double bufferb;
    int equationDone = 0;
    int startEquation;
    if(stage == 0)
    {
        for(int i = 0; i < eq.siEq; i++)
        {
            if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == '.' || eq.cEq[i] == '-')
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
                    i = startEquation;
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
            if(eq.cEq[i] == '^')
            {
                equationDone = 1;
            }
        }
    }
    if(stage == 1)
    {
        for(int i = 0; i < eq.siEq; i++)
        {
            if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == '.' || eq.cEq[i] == '-')
            {
                if(equationDone == 1)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera / bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = startEquation;
                    equationDone = 0;
                } else if (equationDone == 2)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera * bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = startEquation;
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
            if(eq.cEq[i] == '/')
            {
                equationDone = 1;
            }            
            if(eq.cEq[i] == '*')
            {
                equationDone = 2;
            }
        }
    }
    if(stage == 2)
    {
        for(int i = 0; i < eq.siEq; i++)
        {
            if((eq.cEq[i] - '0' >= 0 && eq.cEq[i] - '0' <= 9) || eq.cEq[i] == '.' || eq.cEq[i] == '-')
            {
                if(equationDone == 1)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    buffera = buffera - bufferb;
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = startEquation;
                    equationDone = 0;
                } else if (equationDone == 2)
                {
                    numberParsed NP = {};
                    NP = numberParser(eq,i);
                    bufferb = NP.number;
                    printf("a%f\n",buffera);
                    printf("b%f\n",bufferb);
                    buffera = buffera + bufferb;
                    printf("a%f\n",buffera);
                    eq = resultImporting(eq, startEquation, NP.final, buffera);
                    i = startEquation;
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
            if(eq.cEq[i] == '_')
            {
                equationDone = 1;
            }            
            if(eq.cEq[i] == '+')
            {
                equationDone = 2;
            }
        }
    }
    return eq;
}

double parsing::textParserEquation(subequation &eq)
{
    if(eq.sEq.size() != 0 )
    {
        eq.siEq = eq.sEq.size();
        eq.cEq[eq.siEq]; //#ff0000
        eq.sEq.copy(eq.cEq, eq.sEq.size(),0);//#ff0000
    }
    printf("\nBEFORE BRACKET PARSER:\n");
    for(int i = 0; i < eq.siEq; i++)
    {
        printf("%c",eq.cEq[i]);
    }
    eq = bracketParser(eq);
    printf("\nAFTER BRACKET PARSER:\n");
    for(int i = 0; i < eq.siEq; i++)
    {
        printf("%c",eq.cEq[i]);
    }
    for(int i = 0; i < 3; i++)
    {
        eq = calculatorParser(eq, i);
    }
    printf("\nFINAL:\n");
    for(int i = 0; i < eq.siEq; i++)
    {
        printf("%c",eq.cEq[i]);
    }
    printf("\n");
    numberParsed FINALNUMBER = {};
    FINALNUMBER = numberParser(eq, 0);
    printf("%f", FINALNUMBER.number);
    return FINALNUMBER.number;
}