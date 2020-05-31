#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include "Reservation.h"
#include "Dates.h"
using namespace std;
//Реализация на методите на клас Reservation
Reservation::Reservation(Reservation const &other){
        room = other.room;
        dateFrom = other.dateFrom;
        dateTo = other.dateTo;
        note = other.note;
        guests = other.guests;
    }
Reservation &Reservation::operator = (Reservation const & other){
        if (this != & other){
            room = other.room;
            dateFrom = other.dateFrom;
            dateTo = other.dateTo;
            note = other.note;
            guests = other.guests;
        }
        return *this;
    }
 bool Reservation:: operator == (const Reservation & r) const{
        return room == r.room &&
            dateFrom == r.dateFrom &&
            dateTo == r.dateTo &&
            note == r.note &&
            guests == r.guests;
    }
void Reservation::printRes(){
        time_t t;
        t = dateFrom.asTime_t();
        tm *d = localtime(&t);
        cout << "Room: " << getRoom() << endl;
        cout << "Date from: " << d->tm_year + 1900 << "-" << setfill('0') << setw(2) << d->tm_mon + 1 << "-" << setw(2) << d->tm_mday << endl; //date format iso 8601
        t = dateTo.asTime_t();
        d = localtime(&t);
        cout << "Date to: " << d->tm_year + 1900 << "-" << setfill('0') << setw(2) << d->tm_mon + 1 << "-" << setw(2) << d->tm_mday << endl; //date format iso 8601
        cout << "Note: " << getNote() << endl;
        cout << "Number of guests: " << getGuests() << endl;
}
 ostream& operator << (ostream& out, const Reservation& r){
    time_t dateFromTime_t=r.dateFrom.asTime_t();
    tm *fr = localtime(&dateFromTime_t);
     out << r.room << endl;
     out << 1900 + fr->tm_year << '-' << setfill('0') << setw(2) << 1 + fr->tm_mon << '-' << setw(2) << fr->tm_mday << endl; //date format iso 8601
    time_t dateToTime_t=r.dateTo.asTime_t();
    tm *to = localtime(&dateToTime_t);
     out << 1900 + to->tm_year << '-' << setfill('0') << setw(2) << 1 + to->tm_mon << '-' << setw(2) << to->tm_mday << endl; //date format iso 8601
     out << r.note << endl
         << r.guests << endl;
    return out;
}
istream& operator >> (istream& in, Reservation& r) {
   string s; 
    in >> r.room;
    in >> s;
    r.dateFrom = s; 
    in >> s;
    r.dateTo = s; 
    in >> r.note >> r.guests;
    return in;
    return in;
}
