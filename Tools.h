#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h> // strspn
using namespace std;
namespace Tools{
//Премахване на водещи и крайни "празни" символи
inline string trim(string s){
    const char t[8] = " \t\n\r\f\v"; //Интервал, табулатор, нов ред, надолу, нова страница, вертикален табулатор
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}
//Превръщане на номер на стая от тип string в тип int
inline int strToNum(string num){
    int number;
    char * s = new char[num.length() + 1];
    strcpy(s, num.c_str());
    sscanf(s, "%d", & number); // %d - integer
    delete[] s;
    return number;
}
//Връща последната дума на низ
inline string getLastWord(string s){
    int i = s.length() - 1;
    while (i != 0 && !isspace(s[i])) 
    {
        --i;
    }
    string last = s.substr(i + 1);
    return last;
}
//Проверка дали дума представлява число 
inline bool isNumber(const string &s){
    return (strspn(s.c_str(), "0123456789") == s.size());
}
};
#endif
