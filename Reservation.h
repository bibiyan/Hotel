#ifndef _RESERVATION_H_
#define _RESERVATION_H_
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "Dates.h"
using namespace std;
class Reservation {
    int room; // Номер на стая
    Date dateFrom; // Дата на настаняване
    Date dateTo; // Дата на напускане
    string note; // Бележка
    int guests; // Брой гости
    public:
    Reservation():room(0),guests(0){}; // Конструктор по подразбиране
    Reservation(Reservation const &other); // Конструктор с параметри
    Reservation & operator = (Reservation const & other); //Предефиниран оператор „=“. 
    bool operator == (const Reservation & r) const; // предефиниране на оператор ==
    //Set- функции
    void setRoom(int _room) {room=_room;}
    void setDateFrom(const Date& _dateFrom) {dateFrom=_dateFrom;}
    void setDateTo(const Date& _dateTo) {dateTo=_dateTo;}
    void setNote(string _note) {note=_note;}
    void setGuests(int _guests) {guests=_guests;}
    //Get-функции
    int getRoom()const {return room;}
    Date getDateFrom()const {return dateFrom;}
    Date getDateTo()const {return dateTo;}
    string getNote()const {return note;}
    int getGuests()const {return guests;}
    //Извеждане на информацията за резервация
    void printRes();
    //Предефиниран поток „>>“
    friend ostream& operator << (ostream & out, const Reservation& r);
    //Предефиниран поток „>>“ (за вход)
    friend istream& operator >> (istream & in, Reservation& r);
};
#endif