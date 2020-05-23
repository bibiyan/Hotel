#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>  // setfill, setw

using namespace std;

/*
 void Hcheckin
    Hreport
    Hfind
    HfindUrgent
    Hunavailable
*/

 /*long long formDate (char date[]) {
        long long fdate=0;
        // YYYY-MM-DD
        for (int i=0; i<4;i++) fdate=10*fdate+date[i]-'0'; // годината
        for (int i=5;i<7;i++) fdate=10*fdate+date[i]-'0'; // месеца
        for (int i=8;i<10;i++) fdate=10*fdate+date[i]-'0'; // деня
        return fdate;
    }
*/
//ГЛОБАЛИ
 int mNo=-1;//Номер на изпълняваната подпрограма
 string Param;//Подадени параметри
 //string room; // текуща стая

 vector<Room> rooms;
 string currentFile;
 vector <Reservation> reservations; // datefrom, dateto, note, beds, guests


    //Проверка за високосна година
bool isLeap(int y)
{if (!(y%400)) return true;
 if (!(y%100)) return false;
 return !(y%4);//!(y&3); 
}
//Брой дни в месеца
int days(int m,int y)
{switch(m)
 {case 2: return isLeap(y)?29:28;
  case 4:case 6:case 9:case 11:return 30;
 }
 return 31;
}
//Брой високосни години преди дадена година
int count_leapyears(int y)
{return (y>>2)-(y/100)+y/400; 
}
//Превръщане на дата във формат time_t
//Вход: година, месец, ден

 //char today[11];
//char *getToday(char *r){
   /* time_t now=time(0);
    tm *ltm=localtime(&now);
    // YYYY-MM-DD
    strftime(r,11,"%Y-%m-%d",ltm);
    puts(r);
    return r;
    }*/

int strToNum(string num)
{
    int number;
    char *s=new char[num.length()+1];
    strcpy(s,num.c_str());
    sscanf(s,"%d",&number);
    delete[]s;
    return number;
}    
// Превръщане на дата във формат time_t
time_t toTime_t(string date)
{
int y, m, d;
string yy,mm,dd,helper;
yy=date.substr(0,date.find_first_of("-")); 
helper=date.substr(date.find_first_of("-")+1);
mm=helper.substr(0,helper.find_first_of("-"));
helper=helper.substr(helper.find_first_of("-")+1);
dd=helper.substr(0,2);
//cout<<yy<<" "<<mm<<" "<<dd<<endl;
//Превръщане на string в int:

//за годината
/*char *hy=new char[5];
strcpy(hy,yy.c_str());
sscanf(hy,"%d",&y);
//cout<<"hy="<<hy<<endl;
//cout<<"y="<<y<<endl;
delete[]hy;*/
y=strToNum(yy);

//за месеца
/*char *hm=new char[3];
strcpy(hm,mm.c_str());
sscanf(hm,"%d",&m);
//cout<<"m="<<m<<endl;
delete[]hm;*/
m=strToNum(mm);

//за деня
/*char *hd=new char[3];
strcpy(hd,dd.c_str());
sscanf(hd,"%d",&d);
//cout<<"d="<<d<<endl;
delete[]hd;*/
d=strToNum(dd);


unsigned int leapD=count_leapyears(y-1)-count_leapyears(1970);
 time_t r=(y-1970)*365+leapD;
 for (int i=1;i<m;i++) r+=days(i,y);
 r+=d-1;
 r=r*86400+12*3600;
 return r;
}

time_t getToday(){
    time_t t=time(0);
 tm *strT=localtime(&t);
 //cout<<1900+strT->tm_year<<'-'<<setfill('0')<<setw(2)<<1+strT->tm_mon<<'-'<<setw(2)<<strT->tm_mday<<endl;
 return t;
}

struct Reservation
{
    int room;
    time_t dateFrom;
    time_t dateTo;
   // int daysOfStay=formDate(dateTo)-formDate(dateFrom);
    string note;
    int guests;
     bool operator ==(const Reservation &r) const
  {return room==r.room &&
          dateFrom==r.dateFrom &&
          dateTo==r.dateTo &&
          note==r.note &&
          guests==r.guests;}
    void printRes()
    {
        time_t t;
        t=dateFrom;
        tm* d=localtime(&t);

        cout<<"Room: "<<room<<endl;
        cout<<"Date from: "<<d->tm_year+1900<<"-"<<setfill('0')<<setw(2)<<d->tm_mon+1<<"-"<<setw(2)<<d->tm_mday<<endl;
        t=dateTo;
        d=localtime(&t);
        cout<<"Date to: "<<d->tm_year+1900<<"-"<<setfill('0')<<setw(2)<<d->tm_mon+1<<"-"<<setw(2)<<d->tm_mday<<endl;
        cout<<"Note: "<<note<<endl;
        cout<<"Number of guests: "<<guests<<endl;
    }
};
ostream& operator<<(ostream& os,const Reservation& r){
            tm *fr=localtime(&r.dateFrom);
             os<<r.room<<endl;
                //<<r.dateFrom<<endl
               os<<1900+fr->tm_year<<'-'<<setfill('0')<<setw(2)<<1+fr->tm_mon<<'-'<<setw(2)<<fr->tm_mday<<endl;
                tm *to=localtime(&r.dateTo);
                //<<r.dateFrom<<endl
               os<<1900+to->tm_year<<'-'<<setfill('0')<<setw(2)<<1+to->tm_mon<<'-'<<setw(2)<<to->tm_mday<<endl;
                os<<r.note<<endl<<r.guests<<endl;
               /*  os<<r.room<<endl
                <<r.dateFrom<<endl
                <<r.dateTo<<endl
                <<r.note<<endl
                <<r.guests<<endl;*/
        return os;
    }
istream& operator>>(istream& in, Reservation& r){
       string s;
        in>>r.room;
        in>>s;
        r.dateFrom=toTime_t(s);
        in>>s;
        r.dateTo=toTime_t(s);
        in>>r.note>>r.guests;
        return in;
    }

class Room{
  //  vector <Reservation> reservations; // datefrom, dateto, note, beds, guests
    int numRoom;
  //  bool isAvailable;
    int bedsR;
    string noteR;
   // time_t dateFromR;
   // time_t dateToR;
    //time_t dateR;
   
    void copy(Room const& other){
        numRoom=other.numRoom;
       // isAvailable=other.isAvailable;
        bedsR=other.bedsR;
        noteR=other.noteR;

}
public:
    Room():numRoom(0),bedsR(0){}
    Room(Room const &other){
        copy(other);
    }
    Room &operator = (Room const& other)
    {
        if (this!=&other){
            copy(other);
        }
        return *this;
    }
    void setRoom(int _numRoom, int _beds){
        setNumRoom(_numRoom);
        setNote(nullptr);
       // setIsAvailable(true);
       // setDate();
       // isAvailableOnDate=true;
        setBeds(_beds);
    }
    
    void setNumRoom(int _numRoom){numRoom=_numRoom;}
    void setNote(string _note){noteR=_note;}
   // void setIsAvailable(bool _isAvailable){isAvailable=_isAvailable;}
   // void setDate(){}
    void setBeds(int _beds){bedsR=_beds;}
   
    int getNumRoom(){return numRoom;}
    string getNote(){return noteR;}
    //bool getIsAvailable(){return isAvailable;}
   /* bool getIfIsAvailable(){
        for(int i=0;i<reservations.size();i++)
        {
            if (numRoom==reservations[i].room)
        }
    }*/


     bool isAvOnDateWithDate(time_t d){
        for (int i=0;i<reservations.size();i++)
        {
            if (numRoom==reservations[i].room)
            {
                if(d<=reservations[i].dateTo && d>=reservations[i].dateFrom) return false;
            }
        }
        return true;
    }


    bool isAvailableOnDate(string d){
        time_t onDate;
        if (d=="") onDate=getToday();
        else{
            onDate=toTime_t(d);
        }
        for (int i=0;i<reservations.size();i++)
        {
            if (numRoom==reservations[i].room)
            {
                //long long df=formDate(reservations[i].dateFrom);
              //  long long dt=formDate(reservations[i].dateTo);
               // long long ond=formDate(onDate);
                if(onDate<=reservations[i].dateTo && onDate>=reservations[i].dateFrom) return false;
            }
        }
        return true;
    }

    int getBeds(){return bedsR;}

    void printRoom(){
        cout<<"Room: "<<getNumRoom()<<endl;
        cout<<"Beds: "<<getBeds()<<endl;
        if (getNote()!="") cout<<"Note: "<<getNote()<<endl;
    }

void getData(istream &in){
        int r, i;
        string s;
       // cout<<endl<<"Room number: ";
        in>>r;
       // cout<<r<<endl;
        setNumRoom(r);
      //  cout<<endl<<"Number of beds: ";
        in>>i;
        setBeds(i);
        getline(in,s);
        setNote(s);
        }

friend ostream& operator<<(ostream& out, const Room& other){
             out<<other.numRoom<<endl
                <<other.bedsR<<endl
                <<other.noteR<<endl;
        return out;
        }

};
// Прототипи
void availability(); void checkin(); void checkout(); void close();
void exit(); void find(); void findNot(); void help(); void open();
void report(); void save(); void saveas(); void unavailable();

//Масив от функции
void (*menuNo[13])(void)={availability, checkin, checkout, close,
exit, find, findNot, help, open, report, save, saveas, unavailable};

//Команда
struct Command
{ string menuItem;
  bool operator <(const Command &c) const
  {return menuItem<c.menuItem;}
  bool operator ==(const Command &c) const
  {return menuItem==c.menuItem;}
};
ostream &operator <<(ostream &os,const Command &c)
{return os<<c.menuItem;}
//Команди, подредени по азбучен ред
Command menu[13]=
{"availability", "checkin", "checkout", "close",
"exit", "find", "find!", "help", "open", "report",
 "save", "saveas", "unavailable"};


// Инструменти




//Премахване на водещи и крайни "празни" символи
string trim(string s)
{const char t[8] = " \t\n\r\f\v";
 s.erase(0, s.find_first_not_of(t));
 s.erase(s.find_last_not_of(t) + 1);
 return s;
}

int getCmdNo(string s)
{
  int l=0,r=14,m;
  Command t;
  t.menuItem = s;
  do
  { m=(l+r)>>1;
    if (t==menu[m]) break;
    if (t<menu[m]) r=r-1;
    else l=l+1;
  } while (l<=r);
  if (l<=r) return m;
  return -1;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Намира номера на командата и параметрите в глобалата Param
int getCmdAndParam(string s)
{int n,p,p1;
 string s1;
 n=getCmdNo(s);
 if (n>=0){Param="";return n;}
 p=s.find_first_of(" \t");
 if (p==string::npos) {Param="";return -1;}
 s1=trim(s.substr(p+1));
 p1=s1.find_first_of(" \t");
 if (p1==string::npos)
 {
  Param=s1;
  return getCmdNo(s.substr(0,p));
 }
 n=getCmdNo(s.substr(0,p)+" "+s1.substr(0,p1));
 if (n>=0){Param=trim(s1.substr(p1+1));
  return n;
 }
 Param=s1;
 return getCmdNo(s.substr(0,p));
}
 
int getRoomNum(string num){

    char *rr=new char[num.length()+1];
       strcpy(rr,num.c_str());
       int numroom;
       sscanf(rr,"%d",&numroom);
       // return numroom;
        delete[]rr;

     for (int i=0;i<rooms.size();i++)
     {
         if (rooms[i].getNumRoom()==numroom) return numroom;
     }
     return -1;
 }

/*bool isAvailableOnDate2(string d){
        time_t onDate;
        if (d=="") onDate=getToday();
        else{
            onDate=toTime_t(d);
        }
        for(int i=0;i<rooms.size();i++){
            for (int j=0;j<reservations.size();j++)
            {
                if (rooms[i].getNumRoom()==reservations[j].room)
                {
                    //long long df=formDate(reservations[i].dateFrom);
                     //  long long dt=formDate(reservations[i].dateTo);
                    // long long ond=formDate(onDate);
                    if(onDate<=reservations[i].dateTo && onDate>=reservations[i].dateFrom) return false;
                }
            }
        }
        return true;
    }

bool isAvOnDateWithDate2(time_t d){
       for(int i=0;i<rooms.size();i++){
        for (int j=0;j<reservations.size();j++)
        {
            if (rooms[i].getNumRoom()==reservations[j].room)
            {
                if(d<=reservations[j].dateTo && d>=reservations[j].dateFrom) return false;
            }
        }
       }
        return true;
    }
*/

// Превръщане на датата в long long (за пресмятане дни)
/* long long formDate (char date[]) const{
        long long fdate=0;
        // YYYY-MM-DD
        for (int i=0; i<4;i++) fdate=10*fdate+date[i]-'0'; // годината
        for (int i=5;i<7;i++) fdate=10*fdate+date[i]-'0'; // месеца
        for (int i=8;i<10;i++) fdate=10*fdate+date[i]-'0'; // деня
        return fdate;
    }*/
//Реализации на подпрограмите
// При влизане във всяка от долните функции, глобалите съдържат:
//  mNo - Номера на изпълняваната подпрограма
//  Param -Подадени параметри: низът, който се намира след разпознатата команда

void availability(){
    time_t onDate;
    if(Param=="") onDate=getToday();
    else{
        string d=Param.substr(0,Param.find_first_of(" \t"));
        onDate=toTime_t(d);
    }
    int k=0;
    for (int i=0;i<rooms.size();i++){
        for (int j=0;j<reservations.size();j++)
        {
            if (rooms[i].getNumRoom()==reservations[j].room)
            {
               // long long df=formDate(reservations[i].dateFrom);
               // long long dt=formDate(reservations[i].dateTo);
               // long long ond=formDate(onDate);
               // if(ond<=dt&&ond>=df) return false;
              
                  if(onDate<=reservations[j].dateTo && onDate>=reservations[j].dateFrom){
                       k++;}
            }
        }
        if(!k) rooms[i].printRoom();
        cout<<"k="<<k<<endl;
        k=0;
    }
    
}
string getLastWord(string s)
{
    int i = s.length() - 1; 
    while (i != 0 && !isspace(s[i]))
    {
      --i;
    }
    string last = s.substr(i+1);
    return last;
}
bool isNumber(const string& s) // коментар на David Rector в stackoverflow.com
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c/16465826
{
    return(strspn(s.c_str(),"-.0123456789")==s.size());
}
void checkin(){
    // checkin  ||  229 | 2020-03-23 | 2020-03-31 | The Simpsons
    string helper, helper2, islast, roomNum, dateFrom, dateTo, note;
    int roomNumber, numberOfGuests;
    time_t dateFr,dateT;
    roomNum=Param.substr(0,Param.find_first_of(" \t")); // първа дума на Param
    roomNumber=getRoomNum(roomNum);
      if(roomNumber==-1) {cout<<"This room does not exist!"<<endl; return; }
         helper=Param.substr(Param.find_first_of(" \t")+1); // от втората дума на Param
    dateFrom=helper.substr(0,helper.find_first_of(" \t")); // втора дума на Param
    dateFr=toTime_t(dateFrom);
            // helper:   2020-03-23 | 2020-03-31 | The Simpsons
    helper2=helper.substr(helper.find_first_of(" \t")+1); // от третата дума на Param
    // helper2:  2020-03-31 | The Simpsons
    dateTo=helper2.substr(0,helper2.find_first_of(" \t")); // трета дума на Param
    dateT=toTime_t(dateTo);   
    if(dateT<=dateFr) {
        cout<<"Invalid dates."<<endl; 
        return;
    }
    time_t check=dateFr;
    for(int i=0;i<reservations.size();i++)
    {
        if (rooms[roomNumber-1].getNumRoom()==reservations[i].room &&
            rooms[roomNumber-1].isAvOnDateWithDate(check)==false)
            {cout<<"The room is not available."<<endl;
            return;}
            check+=24*3600;
    }

    note=helper2.substr(helper2.find_first_of(" \t")+1); // от четвъртата дума
   
    int numg;
    string lastWord=getLastWord(Param);
    if (isNumber(lastWord)==false)
    {
        numberOfGuests=rooms[roomNumber-1].getBeds();
    }
     else{    
       /* char *gg=new char[lastWord.length()+1];
        strcpy(gg,lastWord.c_str());
        sscanf(gg,"%d",&numg);
        delete[]gg;*/
        numg=strToNum(lastWord);
        int newSize = note.length() - 1; 
        while (newSize != 0 && !isspace(note[newSize]))
         {
          --newSize;
         }
          note.resize(newSize);
          if(numg<1 || numg>rooms[roomNumber-1].getBeds()) numberOfGuests=rooms[roomNumber-1].getBeds();
          else numberOfGuests=numg;
     }

    Reservation r{roomNumber,dateFr,dateT,note,numberOfGuests};
    reservations.push_back(r);

}

void checkout(){
   // for (unsigned int i=0;i<rooms.size();i++){
       int checkRoom;
       for (unsigned int j=0;j<reservations.size();j++){
        //if (rooms[i].getNumRoom()==num) 
        //    rooms[i].setIsAvailable(true);
        cout<<"Param= "<<Param<<endl;
        cout<<"getRoomNum(Param): "<<getRoomNum(Param)<<endl;
        checkRoom=getRoomNum(Param);
        if(reservations[j].room==checkRoom) 
        reservations.erase(reservations.begin()+j);
    }
   // }
}
void close(){
    rooms.clear();
    reservations.clear();
}
void exit(){
 //Командата няма параметри
  if (Param!="") {cout<<"Invalid parameter\n";mNo=-1;}
}
void find(){
// Намиране на подходяща свободна стая с поне <beds> на брой легла
   //  в периода от <from> до <to>. При наличие 
  //  на повече свободни стаи се предпочитат такива с по-малко на брой легла.
string beds, from, to, helper;
time_t dFrom, dTo;
int numberOfBeds;
beds=Param.substr(0,Param.find_first_of(" \t")); // първа дума на Param (легла)
if (!isNumber(beds)){
    cout<<"Invalid."<<endl;
    return;
}
numberOfBeds=strToNum(beds);
helper=Param.substr(Param.find_first_of(" \t")+1); // от втора дума до края
from=helper.substr(0,helper.find_first_of(" \t")); // втора дума (дата от)
helper=helper.substr(helper.find_first_of(" \t")+1); // от трета дума до края
to=helper.substr(0,helper.find_first_of(" \t\n")); // (дата до)
dFrom=toTime_t(from);
dTo=toTime_t(to);
if(dFrom>=dTo) {
    cout<<"Invalid dates."<<endl;
    return;
}
Room bestRoom;
time_t day=dFrom;
int count=0,minbeds=0;
for(int i=0;i<rooms.size();i++)
{
    if(minbeds<rooms[i].getBeds()) minbeds=rooms[i].getBeds();
}
minbeds++;
for(int i=0;i<rooms.size();i++)
    {
        while(day<=dTo){
            if(rooms[i].isAvOnDateWithDate(day)==false) 
                count++;
            if(count) break;
            day+=86400;
        }
        if(count==0 && numberOfBeds<=rooms[i].getBeds() && minbeds>rooms[i].getBeds()){
            cout<<"beds: "<<numberOfBeds<<endl;
            cout<<"room: "<<rooms[i].getNumRoom()<<endl;
            bestRoom=rooms[i];
            minbeds=rooms[i].getBeds();
        }
        count=0;
        day=dFrom;
    }
    if(!bestRoom.getBeds()) {
        cout<<"Invalid."<<endl;
        return;
    }
    bestRoom.printRoom();

}

void findNot(){} 
void help(){
    cout<<"The following commands are supported:"<<endl;
        cout<<"open <file>	     opens <file>"<<endl
            <<"close             closes opened files"<<endl
            <<"save              saves the currently open file"<<endl
            <<"saveas <file>     saves the currently open file in <file>"<<endl
            <<"help              prints this information"<<endl
            <<"login             logs in the system"<<endl
            <<"logout            logs out of the system"<<endl
            <<"exit              exit the program"<<endl
            <<"checkin           books a room (checkin <room> <from> <to> <note> [<guests>])"<<endl
            <<"availability      shows if a room is available on date (availability [<date>])"<<endl
            <<"checkout          check out a room (checkout <room>)"<<endl
            <<"report            shows rooms which are booked in a period (report <from> <to>)"<<endl
            <<"find              find the best room from... to... (find <from> <ro>)"<<endl
            <<"find!             find a room for a vip (find! <beds> <from> <to>"<<endl
            <<"unavailable       room is unavailable from... to... (unavailable <room> <from> <to> <note>)"<<endl;
            cout<<endl;
}
void open(){
    if(Param.substr(Param.size()-4)!=".txt")
        Param.insert(Param.size(),".txt");
        if(Param=="rooms.txt") {
     ifstream f(Param);
        if (!f.good()){
            ofstream of(Param);
            of.close();
            f.close();
            return;
        }
    Room r;
    r.getData(f);
    while(f.good()){ 
        rooms.push_back(r);
        r.getData(f);
    }
    f.close();
    return;}
    
    currentFile=Param;
    ifstream f(Param);
        if (!f.good()){
            ofstream of(Param);
            of.close();
            f.close();
            return;
        }
    Reservation rr;
    f>>rr;
    while(f.good()){ 
        reservations.push_back(rr);
        f>>rr;
    }
    f.close();
}

void report(){

//Извежда справка за използването на стаи в периода 
//от дата <from> до <to>. Извежда се списък, в който за всяка стая,
 //използвана в дадения период, 
//се извежда и броя на дните, в които е била използвана
 string helper, from, to;
 time_t dateFrom, dateTo;
    from=Param.substr(0,Param.find_first_of(" \t")); // първа дума на Param
    dateFrom=toTime_t(from);

         helper=Param.substr(Param.find_first_of(" \t")+1); // от втората дума на Param
    to=helper.substr(0,helper.find_first_of(" \t")); // втора дума на Param
    dateTo=toTime_t(to);
    time_t helperDate=dateFrom;
    for (int i=0;i<rooms.size();i++)
    {
        long days=0;
        long days2=0;
        for (int j=0;j<reservations.size();j++)
        { 
             if(rooms[i].getNumRoom()==reservations[j].room){
                 while(helperDate<=reservations[j].dateTo){
                  if(rooms[i].isAvOnDateWithDate(helperDate)==false)
                    days++;
                    days2++;
                    helperDate+=24*3600;
                    }
             }
             if(days2) reservations[j].printRes();
             days2=0; 
              
        }
        if(days){
            cout<<"Room "<<rooms[i].getNumRoom()<<" is used "<<days<<" days in this period."<<endl;
           
        }
      
         days=0;
        helperDate=dateFrom;
    }


}

/*void save(){
    
    ofstream f("reservations.txt",ios::app);
    if(!f){
            return;
    }
    for (unsigned i=0; i<reservations.size(); i++){
         f<<reservations[i]<<endl;
    }
    f.close();  
}*/
void save(){
 
    ofstream f(currentFile);
    if(!f){
        f.close();
            return;
    }
    for (unsigned i=0; i<reservations.size(); i++){
         f<<reservations[i]<<endl;
    }
    f.close();  
}



void saveas(){
    if(Param.substr(Param.size()-4)!=".txt")
        Param.insert(Param.size(),".txt");
    ofstream f(Param);
    if(!f){
            f.close();
            return;
    }
    for (unsigned i=0; i<reservations.size(); i++){
         f<<reservations[i];
    }
    f.close();
}

void unavailable(){
    // checkin  ||  229 | 2020-03-23 | 2020-03-31 | The Simpsons
    string helper, helper2, islast, roomNum, dateFrom, dateTo, note;
    int roomNumber, numberOfGuests;
    time_t dateFr,dateT;
    roomNum=Param.substr(0,Param.find_first_of(" \t")); // първа дума на Param
    roomNumber=getRoomNum(roomNum);
      if(roomNumber==-1) {cout<<"This room does not exist!"<<endl; return; }
         helper=Param.substr(Param.find_first_of(" \t")+1); // от втората дума на Param
    dateFrom=helper.substr(0,helper.find_first_of(" \t")); // втора дума на Param
    dateFr=toTime_t(dateFrom);
            // helper:   2020-03-23 | 2020-03-31 | The Simpsons
    helper2=helper.substr(helper.find_first_of(" \t")+1); // от третата дума на Param
    // helper2:  2020-03-31 | The Simpsons
    dateTo=helper2.substr(0,helper2.find_first_of(" \t")); // трета дума на Param
    dateT=toTime_t(dateTo);   
    if(dateT<dateFr) cout<<"Wrong dates."<<endl; 
    time_t check=dateFr;
    for(int i=0;i<reservations.size();i++)
    {
        if (rooms[roomNumber-1].getNumRoom()==reservations[i].room &&
            rooms[roomNumber-1].isAvOnDateWithDate(check)==false)
            {cout<<"The room is used."<<endl;
            return;}
            check+=24*3600;
    }
    note=helper2.substr(helper2.find_first_of(" \t")+1); // от четвъртата дума
    numberOfGuests=0;
    Reservation r{roomNumber,dateFr,dateT,note,numberOfGuests};
    reservations.push_back(r);
}

int main()
{

    //Основен цикъл
 string cmd;
  do
  {getline(cin,cmd);
   cmd=trim(cmd);
   mNo=getCmdAndParam(cmd);
   if (mNo>=0)  
   {menuNo[mNo]();
    cout<<"Done\n";
   }
   else cout<<"Unknown command. Type \"help\" to show all commands.\n";
   if (mNo==4) break;
  }while (true);

  return 0;
}
