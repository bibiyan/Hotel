#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>
#include "Room.h"
#include "Reservation.h"
#include "Tools.h"
#include "Dates.h"
using namespace std;
//Прототипи
void availability(); void checkin(); void checkout(); void close();
void exit(); void find(); void findNot(); void help(); void open();
void report(); void save(); void saveas(); void unavailable();
//Масив от функции
void(* menuNo[13])(void) = { availability, checkin, checkout, close, exit, find, findNot, //Масив от функции
help, open, report, save, saveas, unavailable};
//Команда
struct Command {
    string menuItem;
    //Предефиниране на оператор "<"
    bool operator < (const Command & c) const {
        return menuItem < c.menuItem;
    }
    //Предефиниране на оператор "=="
    bool operator == (const Command & c) const {
        return menuItem == c.menuItem;
    }
};
//Предефиниране на оператор "<<"
ostream & operator << (ostream & out,
    const Command & c) {
    return out << c.menuItem;
}
//Команди, подредени по азбучен ред
Command menu[13]={"availability", "checkin", "checkout", "close","exit", "find", "find!", 
                "help", "open", "report", "save", "saveas", "unavailable"};

//ГЛОБАЛИ
int mNo = -1; //Номер на изпълняваната подпрограма
string Param; //Подадени параметри
vector <Room> rooms; // Вектор от стаи
vector <Reservation> reservations; // Вектор от резервации 
string currentFile;
int getCmdNo(string s){//Търсене номер на функция от менюто с команди (двоично търсене)
    int l = 0, r = 12, m;
    Command t;
    t.menuItem = s;
    do {
        m = (l + r)/2;
        if (t == menu[m]) break;
        if (t < menu[m]) r = r-1;
        else l = l + 1;
    } while (l <= r);
    if (l <= r) return m; //Номерът в менюто е m
    return -1; //Командата не е намерена
}
//Намира номера на командата и параметрите в глобалата Param
int getCmdAndParam(string s){
    int number, p, p1;
    string s1;
    number = getCmdNo(s);  //Ако целият входен низ е команда 
    if (number >= 0) {
        Param = "";
        return number;
    }
    p = s.find_first_of(" \t"); //Иначе търсим интервал или табулатор в s
    if (p == string::npos){
        Param = "";
        return -1; //Ако не намерим - лоша команда
    } 
    //Търсим втори разделител в s
    s1 = Tools::trim(s.substr(p + 1));
    p1 = s1.find_first_of(" \t");
    if (p1 == string::npos){ //Ако няма, s1 да е параметър а думата преди него - команда
        Param = s1;
        return getCmdNo(s.substr(0, p)); 
    }
    //Тук има втори разделител. 
    number = getCmdNo(s.substr(0, p) + " " + s1.substr(0, p1));
    if (number >= 0) {  //Първите две думи са команда и всичко след тях параметър
        Param = Tools::trim(s1.substr(p1 + 1));
        return number;
    }
    //Първата дума е команда и всичко след нея - параметри
    Param = s1;
    return getCmdNo(s.substr(0, p));
}
//Превръща номер на стая от тип string в тип int
int getRoomNum(string num){
    char * rr = new char[num.length() + 1];
    strcpy(rr, num.c_str());
    int numroom;
    sscanf(rr, "%d", & numroom);
    delete[] rr;

    for (Room r: rooms) {
        if (r.getNumRoom() == numroom) return numroom;
    }
    return -1;
}
// При влизане във всяка от долните функции, глобалите съдържат:
//  mNo - Номера на изпълняваната подпрограма
//  Param -Подадени параметри: низът, който се намира след разпознатата команда
void availability(){ //Извежда списък на свободните стаи на дата <date>, ако не е зададена, се използва текущата дата.
    Date onDate;
    if(Param!=""){
        string d = Param.substr(0, Param.find_first_of(" \t")); //Отделяне на датата от Param
        onDate = d;
    }
    int k = 0; // Брояч, който се увеличава, ако стаята е заета на конкретната дата
    for (Room room: rooms) {
        for (Reservation res: reservations) {
            if (room.getNumRoom() == res.getRoom()) {
                if (onDate <= res.getDateTo() && onDate >= res.getDateFrom()) {
                    k++;
                }
            }
        }
        if (!k) room.printRoom();
        k = 0;
    }
}
void checkin(){//Създаване на резервация
    string helper, helper2, islast, roomNum, dateFrom, dateTo, note;
    int roomNumber, numberOfGuests;
    Date dateFr;
    Date dateT;
    roomNum = Param.substr(0, Param.find_first_of(" \t")); // първа дума на Param (номер на стая)
    roomNumber = getRoomNum(roomNum);
    if (roomNumber == -1){ // няма такава стая 
        cout << "This room does not exist!" << endl;
        return;
    }
    helper = Param.substr(Param.find_first_of(" \t") + 1); // от втората дума на Param
    dateFrom = helper.substr(0, helper.find_first_of(" \t")); // втора дума на Param (дата на настаняване)
    dateFr = dateFrom;
    helper2 = helper.substr(helper.find_first_of(" \t") + 1); // от третата дума на Param
    dateTo = helper2.substr(0, helper2.find_first_of(" \t")); // трета дума на Param (дата на напускане)
    dateT = dateTo;
    if (dateT <= dateFr) { 
        cout << "Invalid dates." << endl;
        return;
    }
    Date check(dateFr);
    for (Reservation res: reservations) { //Проверка дали стаята е свободна в този период (dateFrom-dateTo)
        while (check <= dateT) {
            if (rooms[roomNumber - 1].getNumRoom() == res.getRoom() &&
                rooms[roomNumber - 1].isAvOnDateWithDate(check,reservations) == false) {
                cout << "The room is not available." << endl;
                return;
            }
            check.incDay(); 
        }
        check = dateFr;
    }
    note = helper2.substr(helper2.find_first_of(" \t") + 1); // от четвъртата дума
    int numg; // Брой гости
    string lastWord =Tools:: getLastWord(Param); 
    if (Tools::isNumber(lastWord) == false){ // Ако последната дума на низа е число, то не е въведен брой на гости
        numberOfGuests = rooms[roomNumber - 1].getBeds();
    }
    else{
        numg = Tools::strToNum(lastWord); //Отделяне на броя гости от стринга
        int newSize = note.length() - 1;
        while (newSize != 0 && !isspace(note[newSize])) {
            --newSize;
        }
        note.resize(newSize);
        if (numg > rooms[roomNumber - 1].getBeds()){
            cout << "The number of guests is bigger than the number of beds in the room." << endl;
            return;
        }
        if (numg < 1) numberOfGuests = rooms[roomNumber - 1].getBeds();
        else numberOfGuests = numg;
    }
    
    Reservation reserv;
    reserv.setRoom(roomNumber);
    reserv.setDateFrom(dateFr);
    reserv.setDateTo(dateT);
    reserv.setNote(note);
    reserv.setGuests(numberOfGuests);
    reservations.push_back(reserv);
}
void checkout(){ //Освобождаване на заета стая с номер <room>.
    int checkRoom;
    for (unsigned int j = 0; j < reservations.size(); j++){
        checkRoom = getRoomNum(Param);
        if (reservations[j].getRoom() == checkRoom)
            reservations.erase(reservations.begin() + j);
    }
}
void close(){ //Затваря текущо отворения документ, изчиства текущо заредената информация. 
    rooms.clear();
    reservations.clear();
}
void exit(){ //Излизане от програмата.
    if (Param != "") {cout << "Invalid parameter\n";mNo = -1;} //Командата няма параметри
}
void find(){//Намира подходяща свободна стая
    string beds, from, to, helper;
    Date dFrom;
    Date dTo;
    int numberOfBeds;
    beds = Param.substr(0, Param.find_first_of(" \t")); // първа дума на Param (легла)
    if (!Tools::isNumber(beds)){
        cout << "Invalid." << endl;
        return;
    }
    numberOfBeds = Tools::strToNum(beds);
    helper = Param.substr(Param.find_first_of(" \t") + 1); // от втора дума до края
    from = helper.substr(0, helper.find_first_of(" \t")); // втора дума (дата от)
    helper = helper.substr(helper.find_first_of(" \t") + 1); // от трета дума до края
    to = helper.substr(0, helper.find_first_of(" \t\n")); // (дата до)
    dFrom = from;
    dTo = to;
    if (dFrom >= dTo){
        cout << "Invalid dates." << endl;
        return;
    }
    Room bestRoom;
    Date day(dFrom);
    int count = 0, minbeds = 0; //minbeds-ще приема стойността на леглата в най-подходящата стая
    for (Room r: rooms){
        if (minbeds < r.getBeds()) minbeds = r.getBeds(); // minbeds приема стойността най-много легла в хотела
    }
    minbeds++;
    for (Room ro: rooms){
        while (day <= dTo){
            if (ro.isAvOnDateWithDate(day,reservations) == false)
                count++; // count се увеличава, ако стаята е заета
            if (count) break;
            day.incDay();
        }
        // count=0, ако стаята room[i] е свободна
        if (count == 0 && numberOfBeds <= ro.getBeds() && minbeds > ro.getBeds()) {
            bestRoom = ro;
            minbeds = ro.getBeds();
        }
        count = 0;
        day = dFrom;
    }
    if (!bestRoom.getBeds()) {
        cout << "Invalid." << endl;
        return;
    }
    bestRoom.printRoom();
}
void findNot(){}
void help(){ //Извежда кратка информация за поддържаните от програмата команди.
    cout << "The following commands are supported:" << endl;
    cout << "open <file>	     opens <file>" << endl <<
        "close             closes opened files" << endl <<
        "save              saves the currently open file" << endl <<
        "saveas <file>     saves the currently open file in <file>" << endl <<
        "help              prints this information" << endl <<
        "exit              exit the program" << endl <<
        "checkin           books a room (checkin <room> <from> <to> <note> [<guests>])" << endl <<
        "availability      shows if a room is available on date (availability [<date>])" << endl <<
        "checkout          check out a room (checkout <room>)" << endl <<
        "report            shows rooms which are booked in a period (report <from> <to>)" << endl <<
        "find              find the best room from... to... with at least <beds> beds (find <beds> <from> <ro>)" << endl <<
        "find!             find a room for a vip (find! <beds> <from> <to>" << endl <<
        "unavailable       room is unavailable from... to... (unavailable <room> <from> <to> <note>)" << endl;
    cout << endl;
}
void open(){ //Зарежда съдържанието на даден файл. Ако такъв не съществува, се създава нов с празно съдържание.
    if (Param.substr(Param.size() - 4) != ".txt")
        Param.insert(Param.size(), ".txt"); // Ако се въведе име на файл без текстово разширение, се добавя такова
    if (Param == "rooms.txt") { // Ако отваря файла със стаи
        ifstream f(Param);
        if (!f.good()) {
            ofstream of (Param); of .close();
            f.close();
            return;
        }
        Room r;
        r.getData(f);
        while (f.good()){ // Вмъкване на информацията от файла със стаи във вектора от стаи
            rooms.push_back(r);
            r.getData(f);
        }
        f.close();
        return; 
    } 
    //При отваряне на различен файл от този със стаите
    currentFile = Param;
    ifstream f(Param);
    if (!f.good()){
        ofstream of (Param);
        of .close();
        return;
    }
    Reservation rr;
    f >> rr;
    while (f.good()){
        reservations.push_back(rr);
        f >> rr;
    }
    f.close();
}
void report(){ //Извежда справка за използването на стаи в опеделен период и броя на дните, в които са били заети.
    string helper, from, to;
    Date dateFrom;
    Date dateTo;
    from = Param.substr(0, Param.find_first_of(" \t")); // първа дума на Param (дата от)
    dateFrom = from;
    helper = Param.substr(Param.find_first_of(" \t") + 1); // от втората дума на Param
    to = helper.substr(0, helper.find_first_of(" \t")); // втора дума на Param (дата до)
    dateTo = to;
    for (Room r: rooms){
        Date helperDate(dateFrom); // Обхожда дните на зададения период
        long daysOfUsage = 0; // Брои общо дните, в които дадена стая е заета
        for (Reservation res: reservations){
            long days = 0;
            if (r.getNumRoom() == res.getRoom()){
                while (helperDate <= res.getDateTo()){
                    if (r.isAvOnDateWithDate(helperDate,reservations) == false){
                        daysOfUsage++;
                        days++;
                    }
                    helperDate.incDay();
                }
            }
            if (days){
                res.printRes();
            }
        }
        if (daysOfUsage){
            cout << "Room " << r.getNumRoom() << " is used " << daysOfUsage << " days in this period." << endl;
        }
    }
}
void save(){ //Записва направените промени в текущия файл
    ofstream f(currentFile);
    if (!f) {
        f.close();
        return;
    }
    for (Reservation res: reservations){
        f << res;
    }
    f.close();
}

void saveas(){ //Записва направените промени в нов файл с име, избрано от потребителя
    if (Param.substr(Param.size() - 4) != ".txt")
        Param.insert(Param.size(), ".txt"); // Ако се въведе име на файл без текстово разширение, се добавя такова
    ofstream f(Param);
    if (!f) {
        f.close();
        return;
    }
    for (Reservation res: reservations) {
        f << res;
    }
    f.close();
}

void unavailable(){ // Обявява стая за временно недостъпна
    string helper, helper2, islast, roomNum, dateFrom, dateTo, note;
    int roomNumber, numberOfGuests;
    Date dateFr;
    Date dateT;
    roomNum = Param.substr(0, Param.find_first_of(" \t")); // първа дума на Param
    roomNumber = getRoomNum(roomNum);
    if (roomNumber == -1){
        cout << "This room does not exist!" << endl;
        return;
    }
    helper = Param.substr(Param.find_first_of(" \t") + 1); // от втората дума на Param
    dateFrom = helper.substr(0, helper.find_first_of(" \t")); // втора дума на Param 
    dateFr = dateFrom;
    helper2 = helper.substr(helper.find_first_of(" \t") + 1); // от третата дума на Param
    dateTo = helper2.substr(0, helper2.find_first_of(" \t")); // трета дума на Param
    dateT = dateTo;
    if (dateT < dateFr) cout << "Wrong dates." << endl;
    Date check(dateFr);
    for (Reservation res: reservations){ //Проверка дали има настанени гости в стаята
        if (rooms[roomNumber - 1].getNumRoom() == res.getRoom() &&
            rooms[roomNumber - 1].isAvOnDateWithDate(check,reservations) == false) {
            cout << "There are guests in this room." << endl;
            return;
        }
        check.incDay();
    }
    note = helper2.substr(helper2.find_first_of(" \t") + 1); // от четвъртата дума
    numberOfGuests = 0;
    Reservation reserv;
    reserv.setRoom(roomNumber);
    reserv.setDateFrom(dateFr);
    reserv.setDateTo(dateT);
    reserv.setNote(note);
    reserv.setGuests(0);
    reservations.push_back(reserv);
}
int main(){
    //Основен цикъл
    string cmd;
    do{
        getline(cin, cmd); //Четем входния ред
        cmd = Tools::trim(cmd);//Премахваме водещи и крайни "празни" символи
        mNo = getCmdAndParam(cmd); //Намираме номера на командата
        if (mNo >= 0){ //Ако е двйствителна ...
            menuNo[mNo](); //... изпълняваме я...
            cout << "Done\n"; //... и отразяваме, че е изпълнена.
        }
        else
            cout << "Unknown command. Type \"help\" to show all commands.\n";
        if (mNo == 4) break; //exit
    }while (true);
    return 0;
}