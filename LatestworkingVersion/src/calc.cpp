#include <iostream>
#include <list.h>
#include <calc.h>
#include <math.h>
#include <easylogging++.h>

double calc(const char *expression, long long len, int *ERROR_CODE) {
    int CALC_ERROR_CODE{0};
    long long buffSize{0}, i{0}, newExpEnd{0};
    char *newExp{NULL}, *endPtr{NULL};
    double number{0};
    double result{0}, temp{0};
    Node *node{NULL};
    char *tmpPtr{NULL};

    newExp = (char*)malloc(len * sizeof(char));
    if (!newExp) {
        *ERROR_CODE = 2;
        free(newExp);
        return 0;
    }
    // if (!bracketsStatus) {
    //     //Копирование const выражения в новую переменную
    //     tmpPtr = expression;
    //     // Функция подсчета выражений в скобках, вызывается рекурсивно
    //     newExp = bracketsConvert(&tmpPtr, newExp, len - 1, newExpEnd, ERROR_CODE);
    // }

    newExp = convertToPolishForm(expression, newExp, &len, &newExpEnd, &CALC_ERROR_CODE);
    if (CALC_ERROR_CODE) {
        *ERROR_CODE = 3;
        free(newExp);
        return 0;
    }

    if(VLOG_IS_ON(2)) LOG(TRACE) << "going to calculations";
    for (i = 0; i < newExpEnd; i++) {
        switch(newExp[i]) {
            case '.': {
                continue;
            }
            case '*': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op * BEGIN | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * BEGIN | buffSize: " << buffSize;
                temp = popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                temp *= popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                insertInd(&node, buffSize++, temp, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                i++;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op * END | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * END | buffSize: " << buffSize;
                continue;
            }
            case '/': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op / BEGIN | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * BEGIN | buffSize: " << buffSize;
                temp = popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                temp /= popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                insertInd(&node, buffSize++, temp, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                i++;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op / END | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * END | buffSize: " << buffSize;
                continue;
            }
            case '+': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op + BEGIN | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * BEGIN | buffSize: " << buffSize;
                temp = popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                temp += popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                insertInd(&node, buffSize++, temp, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                i++;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * END | buffSize: " << buffSize;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op + END | len: " << listLen(node);
                continue;
            }
            case '-': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op - BEGIN | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * BEGIN | buffSize: " << buffSize;
                temp = popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                temp -= popInd(&node, --buffSize, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                insertInd(&node, buffSize++, temp, &CALC_ERROR_CODE);
                if (CALC_ERROR_CODE) {
                    *ERROR_CODE = CALC_ERROR_CODE;
                    free(newExp);
                    listTrash(&node);
                    return 0;
                }
                i++;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "calc op - END | len: " << listLen(node);
                if(VLOG_IS_ON(2)) LOG(TRACE) << "buffSize * END | buffSize: " << buffSize;
                continue;
            }

            case ' ': {
                i++;
                continue;
            }
        }
        if(VLOG_IS_ON(2)) LOG(TRACE) << "NOT AN OPERATION ... BEGIN | len: " << listLen(node);
        number = strtod(newExp + i, &endPtr);
        if(VLOG_IS_ON(2)) LOG(TRACE) << "NOT AN OPERATION ... END | number: " << number << " | newExp: \"" << newExp << "\" | newExp + i: \"" << newExp + i << "\" | i: " << i << " | len: " << listLen(node);
        insertInd(&node, buffSize++, number, &CALC_ERROR_CODE);
        if (CALC_ERROR_CODE) {
            *ERROR_CODE = CALC_ERROR_CODE;
            free(newExp);
            listTrash(&node);
            return 0;
        }
        i = endPtr - newExp;
    }
    if(VLOG_IS_ON(2)) LOG(TRACE) << "AFTER CALCULATIONS | len: " << listLen(node);
    result = popInd(&node, 0, &CALC_ERROR_CODE);
    listTrash(&node);
    Node *p = node;
    if (p) {
        while (p->next) {
            if(VLOG_IS_ON(2)) LOG(TRACE) << printf("memoryFreeing: p->next input\n");
            free(p);
            p = p->next;
        }
        free(p);
    }
    free(newExp);
    return result;
}

// void lltoa(long long num, char **expr, long long *buff, long long *len, int *CALC_ERROR_CODE) {
//     long long i, j, tmp;
//     i = *buff;
//     while (num) {
//         tmp = num % 10;
//         if (*buff + 1 > *len) {
//             *expr = (char*)realloc(*expr, (*buff + 10) * sizeof(char));
//             *len += 10;
//         }
//         (*expr)[*buff] = tmp + '0';
//         *buff += 1;
//         num = num / 10;
//     }
//     j = *buff - 1;
//     while (i < j) {
//         tmp = (*expr)[i];
//         (*expr)[i] = (*expr)[j];
//         (*expr)[j] = tmp;
//         ++i;
//         --j;
//     }
//     if(VLOG_IS_ON(2)) LOG(TRACE) << "--lltoa-- long long END\n";
// }


void lltoa(double num, char **expr, long long *buff, long long *len, int *ERROR_CODE) {
    if(VLOG_IS_ON(2)) LOG(TRACE) << "input in lltoa, buff: " << (*buff) << ", len: " << (*len) <<", num: " << num << "\n";
    long long i, j, tmp;
    long long nAfterDot = 4; //цифры после точки с запятой

    long long whole = floor(num);
    long long fractional = ((num - floor(num)) * pow(10, nAfterDot));

    long long rateWhole = ((!whole) ? 1 : (floor(log10(whole)) + 1));
    long long rateFractional = ((!fractional) ? 1 : (floor(log10(fractional)) + 1));
    if(VLOG_IS_ON(3)) LOG(TRACE) << "rateWhole: " << rateWhole << "\nrateFract: " << rateFractional << "\n";
    if ((!expr) || (!buff) || (!len)) {
        *ERROR_CODE = 2;
        return;
    }
    while(rateWhole) {
        tmp = whole / pow(10, --rateWhole);
        if (*buff + 2 > *len) {
            *expr = (char*)realloc(*expr, (*buff + 10) * sizeof(char));
            *len += 10;
        }
        (*expr)[*buff] = tmp + '0';
        *buff += 1;
        whole = whole % ((long long)(pow(10, rateWhole)));
    }

    (*expr)[*buff] = '.';
    *buff += 1;

    while(rateFractional) {
        tmp = whole / pow(10, --rateFractional);
        if (*buff + 2 > *len) {
            *expr = (char*)realloc(*expr, (*buff + 10) * sizeof(char));
            *len += 10;
        }
        (*expr)[*buff] = tmp + '0';
        *buff += 1;
        whole = whole % ((long long)(pow(10, rateFractional)));
    }

    (*expr)[*buff] = ' ';
    *buff += 1;

    if(VLOG_IS_ON(2)) LOG(TRACE) << "--lltoa-- double END\n";
}

// char * bracketsConvert(char *tmpPtr, char *newExp, long long *right, long long *newExpEnd, int *ERROR_CODE) {
//     int bracketsCount{0}, i{0}, j{0}, flag{0}, bracketsMax{0};
//     for (; i < right; i++) {
//         if (tmpPtr[i] == '(') {
//             if (!flag) flag = i;
//             bracketsCount++;
//             if (bracketsCount > bracketsMax) {
//                 bracketsMax = bracketsCount;
//             }
//         } else if (tmpPtr[i] == ')') {
//             if (!--bracketsCount) {
//                 if (!VLOG_IS_ON(2)) printf("[bracketsConvert]: endPtr: %.*s", i - flag - 1, tmpPtr);
//                 flag = 0;
//                 *right = i - 1; //Присваиваем правой границе индекс последней закрывающей скобки
//                 if (bracketsMax == 1) { // Если существует 1 открывающая и 1 закрывающая, то необходимо посчитать выражение
//                     const char *tmpExpression = tmpPtr;
//                     // tmpPtr = calc(tmpExpression, *right, ERROR_CODE); //convertToPolishForm
//                     return tmpPtr;
//                 } else {
//                     tmpPtr += flag;
//                     return bracketsConvert(expression, tmpPtr, newExp, right, newExpEnd, ERROR_CODE);
//                 }
//             }
//         }
//     }
//     return 0;
// }

char * convertToPolishForm(const char *expression, char *newExp,
        long long *len, long long *newExpEnd, int *ERROR_CODE) {
    long long buffSize{0};
    long long newExpLen{0}, i{0};
    double number{0};
    long long counter{0};
    char *endPtr{NULL}, *buff{NULL};
    int brackets{0};
    char tmpP{0};

    if (!expression) {
        *ERROR_CODE = 1;
        return 0;
    }
    // Выделение памяти под массив операций
    buff = (char*)malloc(*len * sizeof(char));
    if (!buff) {
        *ERROR_CODE = 2;
        free(buff);
        return 0;
    }
    newExpLen = *len;

    // //Копирование const выражения в новую переменную
    // tmpPtr = expression;
    // // Функция подсчета выражений в скобках, вызывается рекурсивно
    // newExp = bracketsConvert(&tmpPtr, newExp, len - 1, newExpEnd, ERROR_CODE);

    number = strtod(expression, &endPtr);
    lltoa(number, &newExp, newExpEnd, &newExpLen, ERROR_CODE);
    if (*ERROR_CODE) {
      return NULL;
    }
    if (VLOG_IS_ON(2)) { //was changed
        for (i = 0; i < *newExpEnd - 1; i++) {
            LOG(TRACE) << newExp[i];
        }
        LOG(TRACE) << "\n";
    }

    for (counter = endPtr - expression; counter < *len; counter++) {
        switch(expression[counter]) {
            case '.': {
                continue;
            }
            case '(': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op ( BEGIN";
                if(VLOG_IS_ON(2)) LOG(TRACE) << "ERROR: INPUT IN (";
                // if(VLOG_IS_ON(2)) LOG(TRACE) << "newExp: " << newExp;
                // if(VLOG_IS_ON(2)) LOG(TRACE) << "newExp after converting: " << newExp;

                //-----------------------
                //Recursive function here
                //_______________________
                brackets = 1;
                for (i = counter; expression[i] != ')'; i++) {
                    if (expression[i] == ')') {
                        endPtr += (i - counter + 1);
                        counter = endPtr - expression;
                    }
                }

                //-----------------------
                //Recursive function here
                //_______________________

                // if(VLOG_IS_ON(2)) LOG(TRACE) << "counter: " << counter;
                // if(VLOG_IS_ON(2)) LOG(TRACE) << "endPtr: " << endPtr;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "ERROR: INPUT IN )";
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op ( END";
                continue;
            }
            case ')': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op ) BEGIN";
                if(VLOG_IS_ON(2)) LOG(TRACE) << "newExp: " << newExp;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "ERROR: INPUT IN )";
                while (buffSize) {
                    if (((*newExpEnd) + 2) > newExpLen) {
                        newExp = (char*)realloc(newExp, (newExpLen + 10) * sizeof(char));
                        newExpLen += 10;
                    }
                    newExp[(*newExpEnd)++] = buff[--buffSize];
                    newExp[(*newExpEnd)++] = ' ';
                }
                counter++;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "ERROR: INPUT IN )";
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op ) END";
                return newExp;
            }
            case '*': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op * BEGIN";
                if(VLOG_IS_ON(2)) LOG(TRACE) << "newExp: " << newExp;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "expression: " << expression;
                if(VLOG_IS_ON(2)) LOG(TRACE) << "endPtr: " << endPtr;

                if (endPtr - expression >= *len) {
                    *ERROR_CODE = 3;
                    free(newExp);
                    free(buff);
                    return 0;
                }
                tmpP = *(endPtr + 1);
                if ((tmpP == '(') || (*endPtr == ')')) {
                    number = strtod(endPtr + 2, &endPtr);
                } else {
                    number = strtod(endPtr + 1, &endPtr);
                }

                lltoa(number, &newExp, newExpEnd, &newExpLen, ERROR_CODE);
                if (*ERROR_CODE) {
                  return NULL;
                }
                if (VLOG_IS_ON(2)) { //was changed
                    for (i = 0; i < *newExpEnd - 1; i++) {
                        LOG(TRACE) << newExp[i];
                    }
                    LOG(TRACE) << "\n";
                }
                buff[buffSize++] = '*';
                counter = endPtr - expression - 1;
                if (tmpP == '(') {
                    counter -= 2;
                } else if (tmpP == ')') {
                    counter--;
                }
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op * END";
                break;
            }
            case '/': {
                if (endPtr - expression >= *len) {
                    *ERROR_CODE = 3;
                    free(newExp);
                    free(buff);
                    return 0;
                }
                tmpP = *(endPtr + 1);
                if ((tmpP == '(') || (*endPtr == ')')) {
                    number = strtod(endPtr + 2, &endPtr);
                } else {
                    number = strtod(endPtr + 1, &endPtr);
                }
                lltoa(number, &newExp, newExpEnd, &newExpLen, ERROR_CODE);
                if (*ERROR_CODE) {
                  return NULL;
                }
                if (VLOG_IS_ON(2)) { //was changed
                    for (i = 0; i < *newExpEnd - 1; i++) {
                        LOG(TRACE) << newExp[i];
                    }
                    LOG(TRACE) << "\n";
                }
                buff[buffSize++] = '/';
                counter = endPtr - expression - 1;
                if (tmpP == '(') {
                    counter -= 2;
                } else if (tmpP == ')') {
                    counter--;
                }
                break;
            }
            case '+': {
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op + BEGIN";
                if(VLOG_IS_ON(2)) LOG(TRACE) << "brackets's status: " << brackets;
                if (!brackets) { //added brackets status
                    if (buffSize) {
                        if ((buff[buffSize - 1] == '*') ||
                                (buff[buffSize - 1] == '/') ||
                                (buff[buffSize - 1] == '-')) {
                            while (buffSize) {
                                if (((*newExpEnd) + 2) > newExpLen) {
                                    newExp = (char*)realloc(newExp, (newExpLen + 10) * sizeof(char));
                                    newExpLen += 10;
                                }
                                newExp[(*newExpEnd)++] = buff[--buffSize];
                                newExp[(*newExpEnd)++] = ' ';
                            }
                        }
                    }
                }
                if (endPtr - expression >= *len) {
                    *ERROR_CODE = 3;
                    free(newExp);
                    free(buff);
                    return 0;
                }
                tmpP = *(endPtr + 1);
                if ((tmpP == '(') || (*endPtr == ')')) {
                    number = strtod(endPtr + 2, &endPtr);
                } else {
                    number = strtod(endPtr + 1, &endPtr);
                }
                lltoa(number, &newExp, newExpEnd, &newExpLen, ERROR_CODE);
                if (*ERROR_CODE) {
                  return NULL;
                }
                if (VLOG_IS_ON(2)) { //was changed
                    for (i = 0; i < *newExpEnd - 1; i++) {
                        LOG(TRACE) << newExp[i];
                    }
                    LOG(TRACE) << "\n";
                }
                buff[buffSize++] = '+';
                counter = endPtr - expression - 1;
                if (tmpP == '(') {
                    counter -= 2;
                } else if (tmpP == ')') {
                    counter--;
                }
                if(VLOG_IS_ON(2)) LOG(TRACE) << "op + END";
                break;
            }
            case '-': {
                if (!brackets) { //added brackets status
                    if (buffSize) {
                        if ((buff[buffSize - 1] == '*') ||
                                (buff[buffSize - 1] == '/')) {
                            while (buffSize) {
                                if (((*newExpEnd) + 2) > newExpLen) {
                                    newExp = (char*)realloc(newExp, (newExpLen + 10) * sizeof(char));
                                    newExpLen += 10;
                                }
                                newExp[(*newExpEnd)++] = buff[--buffSize];
                                newExp[(*newExpEnd)++] = ' ';
                            }
                        }
                    }
                }
                if (endPtr - expression >= *len) {
                    *ERROR_CODE = 3;
                    free(newExp);
                    free(buff);
                    return 0;
                }
                tmpP = *(endPtr + 1);
                if ((tmpP == '(') || (*endPtr == ')')) {
                    number = strtod(endPtr + 2, &endPtr);
                } else {
                    number = strtod(endPtr + 1, &endPtr);
                }
                lltoa(number, &newExp, newExpEnd, &newExpLen, ERROR_CODE);
                if (*ERROR_CODE) {
                  return NULL;
                }
                if (VLOG_IS_ON(2)) { //was changed
                    for (i = 0; i < *newExpEnd - 1; i++) {
                        LOG(TRACE) << newExp[i];
                    }
                    LOG(TRACE) << "\n";
                }
                buff[buffSize++] = '-';
                counter = endPtr - expression - 1;
                if (tmpP == '(') {
                    counter -= 2;
                } else if (tmpP == ')') {
                    counter--;
                }
                break;
            }
        }
    }
    while (buffSize) {
        if (((*newExpEnd) + 2) > newExpLen) {
            newExp = (char*)realloc(newExp, (newExpLen + 10) * sizeof(char));
            newExpLen += 10;
        }
        newExp[(*newExpEnd)++] = buff[--buffSize];
        newExp[(*newExpEnd)++] = ' ';
    }
    return newExp;
}
