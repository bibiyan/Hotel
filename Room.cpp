#include <iostream>
#include <fstream>
#include "Room.h"
#include "Dates.h"
using namespace std;
//Проверка дали стая е достъпна на определена дата от тип Date
    bool Room::isAvOnDateWithDate(const Date &d, const vector<Reservation>& reservations){
        for (Reservation res: reservations){
            if (numRoom == res.getRoom()){
                if (d <= res.getDateTo() && d >= res.getDateFrom()) return false;
            }
        }
        return true;
    }
    //Проверка дали стая е достъпна на определена дата от string
    bool  Room::isAvailableOnDate(const string &d, const vector<Reservation>& reservations){
        Date onDate;
        if (d != "") onDate=d;
        return isAvOnDateWithDate(onDate,reservations);
    }
    //Извежда информация за стая
    void  Room::printRoom(){
        cout << "Room: " << getNumRoom() << endl;
        cout << "Beds: " << getBeds() << endl;
        if (getNote() != "") cout << "Note: " << getNote() << endl;
    }
    //Прочита данни от файл (файлът със стаите)
    void  Room::getData(istream & in ){
        int r, i;
        string s;
        in >> r;
        setNumRoom(r); 
        in >> i;
        setBeds(i);
        getline(in,s);
        setNote(s);
    }
    //Предефиниран поток "<<"
     ostream &operator << (ostream & out,const Room & other){
        out << other.numRoom << endl
            << other.bedsR << endl
            << other.noteR << endl;
        return out;
    }