#ifndef _ROOM_H_
#define _ROOM_H_
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Reservation.h"
#include "Dates.h"
using namespace std;
class Room {
    int numRoom; //номер на стая
    int bedsR; // брой легла
    string noteR; // бележка

    public:
    Room(): numRoom(0), bedsR(0) {} // Конструктор по подразбиране
    Room(Room const& other){ //Копи-конструктор
        numRoom = other.numRoom;
        bedsR = other.bedsR;
        noteR = other.noteR;
    }
    Room & operator = (Room const & other){ //Предефиниран оператор „=“. 
        if (this != & other)
        {
            numRoom = other.numRoom;
            bedsR = other.bedsR;
            noteR = other.noteR;
        }
        return *this;
    }
    // Set-функции
    void setNumRoom(int _numRoom){
        numRoom = _numRoom;
    }
    void setNote(string _note){
        noteR = _note;
    }
    void setBeds(int _beds){
        bedsR = _beds;
    }
    // Get-функции
    int getNumRoom() const{
        return numRoom;
    }
    string getNote() const{
        return noteR;
    }
    int getBeds() const{
        return bedsR;
    }
    //Проверка дали стая е достъпна на определена дата от тип Date
    bool isAvOnDateWithDate(const Date &d, const vector<Reservation>& reservations);
    //Проверка дали стая е достъпна на определена дата от string
    bool isAvailableOnDate(const string &d, const vector<Reservation>& reservations);
    //Извежда информация за стая
    void printRoom();
    //Прочита данни от файл (файлът със стаите)
    void getData(istream & in );
    //Предефиниран поток "<<"
    friend ostream & operator << (ostream & out,const Room & other);
};

#endif