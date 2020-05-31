#ifndef _DATES_H_
#define _DATES_H_
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include "Tools.h"
using namespace std;
class Date{
    time_t date;
//Проверка за високосна година
static bool isLeap(int y) {
    if (!(y % 400)) return true;
    if (!(y % 100)) return false;
    return !(y % 4);
}
//Брой дни в месеца
static int days(int m, int y){
    switch (m){
    case 2:
        return isLeap(y) ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    }
    return 31;
}
//Брой високосни години преди дадена година
static int count_leapyears(int y){
    return (y / 4) - (y / 100) + y / 400;
}

// Превръщане на дата от тип string в тип time_t
static time_t toTime_t(const string & date) {
    //YYYY-MM-DD
    int y, m, d;
    string yy, mm, dd, helper;
    yy=date.substr(0, date.find_first_of("-")); // YYYY
    helper = date.substr(date.find_first_of("-")+1);
    mm=helper.substr(0, helper.find_first_of("-")); //MM
    helper = helper.substr(helper.find_first_of("-")+1);
    dd=helper.substr(0,2); //DD
    //за годината
    y=Tools::strToNum(yy);
    //за месеца
    m=Tools::strToNum(mm);
    //за деня
    d=Tools::strToNum(dd);
    unsigned int leapD = count_leapyears(y-1)-count_leapyears(1970);
    time_t r=(y-1970)*365+leapD;
    for (int i=1;i<m;i++)
        r+=days(i, y);
    r+=d-1;
    r=r*86400+12*3600;
    return r;
}
//Връща днешна дата
static time_t getToday(){
    time_t t=time(0);
    tm* strT=localtime( & t);
    return t;
}
    public:
    Date(){//По подразбиране - обект с днешна дата
        date=getToday();
    } 
    Date(const string& d){ // Конструктор с параметър
        date=toTime_t(d);
    }
    Date(const Date& d){ // Копи-конструктор
    
        date=d.date;
    }
    time_t asTime_t() const{
        return date;
    }
    void incDay(){ // Връща следващ ден
        date+=86400;
    }
    //Предефиниране на оператори
    bool operator < (const Date & d) const{
        return date < d.date;
    }
    bool operator <= (const Date & d) const{
        return date <= d.date;
    }
    bool operator >= (const Date & d) const{
        return date >= d.date;
    }
    bool operator == (const Date & d) const{
        return date == d.date;
    }
    Date& operator=(const Date& d){
        date=d.date;
        return *this;
    }
    Date& operator=(const string& s){
        date=toTime_t(s);
        return *this;
    }
    //Предефиниране на потоците "<<" и ">>", ако искаме някъде да изведем дата (пр. cout<<reservation.getDateFrom();)
    friend ostream &operator << (ostream & os, const Date& d){
        os<<d.date;
        return os;
    }
    friend istream &operator >> (istream & in, Date& d){ 
        in >> d.date;
        return in;
    }
};
#endif