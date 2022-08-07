/*
* file    :  calendar.cpp
*
* title   :  shamsi calendar beta
*
* C abbas makhzoomi 
* copyrights reserved
*/

#include <iostream>
#include <conio.h>		//getch()

#include <math.h>		// math header for rounding in this case
#include <ctime>		//get the current timestamp

#include <iomanip>      //setfill,setw
#include <string>		
#include <windows.h>	// WinApi header for highlight

using namespace std;

int GetFirstDayOfYear(int);						//gets the number of year like 1398 and may returns 0 (= shanbe) up to 6 (= jome)
void output(int,int,int,int,int,int,int,bool);	//printing out the frame of the calendar
void centerput(char,string,int);				//gets width (int) and prints the string given at the center and fills out the empty space with given char
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case 1: {
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
int main(){ 

	// 6813000  = 1  farvardin 1349 00:00
	// time(0)  = 11 dey 	  1348 03:30
	// 31556926 = 365.24 Day = 1 year
	// 2629743	= 30.44  Day = 1 month
	// 86400	= 1 Day
	
	// (time(0)-6813000+86400)%31556926/86400 = days passed from last 29 or 30 esfand till now
	
	bool warn;
	int day,FirstDayOfMonth,PassedDays,CurrentMonth,CurrentYear,FirstDayOfYear,ConstYear,ConstMonth,ConstDay;
	char chr;
	long long int CurrentTimestamp = time(0) ; //timestamp -- https://www.unixtimestamp.com/

	bool flag = 1; //recognise whether its first time of the loop or not
	while(true){
	
		//Current Timestamp is the seconds passed from 1348/10/11 03:30 till now , 
		//so if we take 6813000 from it , we will get the seconds passed from 1349/1/1 00:00 
		
		//CurrentTimestamp % 31556926 will give us passed seconds from the beginning of the year and divide it by "one daye" will convert it to days
		PassedDays = ( CurrentTimestamp - 6812000 ) % 31556926 / 86400 ; // + 86400
		
		//CurrentMonth = ( CurrentTimestamp - 6813000 ) / 2629743 % 12 + 1 ;     ignore this.just a big headache
		CurrentYear = ( CurrentTimestamp - 6813000 ) / 31556926 + 1349 ;
	
		if(PassedDays <= 6 * 31)
			CurrentMonth = PassedDays/31 + 1;
		else if(PassedDays > 365 - 29)
			CurrentMonth = 12;	
		else 
			CurrentMonth = (PassedDays - 6 * 31) /30 + 7;
		

		FirstDayOfYear = GetFirstDayOfYear(CurrentYear);
	
		
		if (0<CurrentMonth && CurrentMonth<7){

			day = PassedDays % 31 + 1 ;
			FirstDayOfMonth = ( ( CurrentMonth - 1 ) * 31 + FirstDayOfYear ) % 7;
	
		} else if (CurrentMonth == 12){
			
			FirstDayOfMonth = (6 * 31) + (5 * 30);
			day = PassedDays - FirstDayOfMonth + 1 ;
			FirstDayOfMonth = ( FirstDayOfMonth + FirstDayOfYear ) % 7;
	
		} else {

			FirstDayOfMonth = (CurrentMonth - 7) * 30 + (6 * 31);
			day = PassedDays - FirstDayOfMonth + 1 ;
			FirstDayOfMonth = ( FirstDayOfMonth + FirstDayOfYear ) % 7;
		}  

		if(flag){ //that will execute just the first time that the while loop runs 
				  //and will set the current day to make it blue later
			ConstYear = CurrentYear;
			ConstMonth = CurrentMonth;
			ConstDay = day;
			flag = 0;
		}

		output(CurrentYear,CurrentMonth,FirstDayOfMonth,day,ConstYear,ConstMonth,ConstDay,warn);
		
		warn = 0;
	
		chr = getch();
	
		switch(chr){
			case'q':
			case'Q':
				CurrentTimestamp -= 86400;//one day
				break;
				
			case'e':
			case'E':
				CurrentTimestamp += 86400;//one day
				break;
								
			case'a':
			case'A':
				CurrentTimestamp -= 2629743;//one month(30.44 days)
				break;
											
			case'd':
			case'D':
				CurrentTimestamp += 2629743;//one month(30.44 days)
				break;
											
			case'z':
			case'Z':
				CurrentTimestamp -= 31556926;//one year(365.24 days)
				break;	
				
			case'c':
			case'C':
				CurrentTimestamp += 31556926;//one year(365.24 days)
				break;
			case 32:
				CurrentTimestamp = time(0);//now!
				break;
			default:
				warn = 1;
		}
		
	}

	return 0;  

}
void output(int year,int MonthNum,int FirstDayOfMonth,int day,int ConstYear,int ConstMonth,int ConstDay,bool warn){
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");
	
	string  MonthNames[12] = {"farvardin","ordibhsht","khordad","tir","mordad","shahrivar","mehr","aban","azar","dey","bahman","esfand"};
	int DaysOfMonth[12] ={31,31,31,31,31,31,30,30,30,30,30,29};
	
	if (year%33 == 1 || year%33 == 5 || year%33 == 9 || year%33 == 13 || year%33 == 17 || year%33 == 22 || year%33 == 26 || year%33 == 30)
		DaysOfMonth[11] = 30;  //Leap year
	
	
	////////////////////////////////////////////////////// size of the calendar  /////////////////////////////////////////////////////
	//int width = 7 * 5;
	//int width = 7 * 10;
	int width = 7 * 4; //4 Represents the space that every day of the week will have
	
	
	
	cout<<' ';
	for(int i = 0;i<width;i++)
		cout<<'_'; //a row of _ s !
	cout<<' ';
	
	cout<<endl;
	
	cout<<'|';
	centerput('-',to_string(year),width);//change int(year) to string
	cout<<'|';
	
	cout<<endl;
	
	cout<<'|';
	centerput('=',MonthNames[MonthNum-1],width);//print the month name in the center of '='s !
	cout<<'|';
	
	cout<<endl;
	
	cout<<'|';
	cout<<setfill(' ');                                               // is that correct ?
	cout<<setw(width/7)<<"SH"<<setw(width/7)<<setw(width/7)<<1<<setw(width/7)<<2<<setw(width/7)<<3<<setw(width/7)<<4<<setw(width/7)<<5<<setw(width/7)<<'J';
	cout<<'|';

	cout<<endl;
	
	cout<<'|';
	for(int i = 0;i<width;i++)
		cout<<'=';
	cout<<'|';

	cout<<endl;
	

	
	
	
	////////////body of the calendar
	
	bool flag = 0;
	int DayNum = 1;
	
	while(DayNum <= DaysOfMonth[MonthNum - 1]){ //while there is days left to print
	
	cout<<'|';
	for(int i = 0;i<7;i++){ //each day of the week
		
		if(FirstDayOfMonth == i)//for instance if FirstDayOfMonth = 3 (3shanbe) that mean first 3 days should be empty 
								// so it will loop and fill the space up with a ' ' ! and finally when i equals FirstDayOfMonth
								// it will start counting days till it reaches the month days limit 
			flag = 1;
		
		if(DayNum > DaysOfMonth[MonthNum - 1])
			flag = 0;
		
		if(flag){ //way better than V1 !!
			
		
			
			if(DayNum == day) //Red has Higher Priority than blue
				SetConsoleTextAttribute(hConsole, 79);//red
			else if(DayNum == ConstDay and MonthNum == ConstMonth and year == ConstYear)
				SetConsoleTextAttribute(hConsole, 31);//blue

			cout<<setw(width/7)<<DayNum;

			SetConsoleTextAttribute(hConsole, 7);

			DayNum++;

		} else { //put an empty space if there is no days to be assigned 
			cout<<setw(width/7)<<' ';
		}
	}
	
	cout<<'|';

	cout<<endl;	

	}
	
	//bottom
	cout<<'|';
	for(int i = 0;i<width;i++)
		cout<<'_';
	cout<<'|';
	
	cout<<endl;
	
	
	if(warn){
		SetConsoleTextAttribute(hConsole, 79); //red
		cout<<"Only the following keys are allowed (you may need to change the language of the keyboard)\n";
		SetConsoleTextAttribute(hConsole, 7); //default
	}
	cout<<"Q : previous Day	E : Next Day\n";
	cout<<"A : previous Month	D : Next Month\n";
	cout<<"Z : previous Year	C : Next Year\n";
	cout<<"space : Jump to current Day\n";
}
void centerput(char fill, string str, int width){

    int len = str.length();
    cout<<setfill(fill)<<setw((width/2)+len/2)<<str<<setw((width/2)-len/2)<<fill;
    if(width%2==1)
		cout<<fill;
}
int GetFirstDayOfYear(int year){
	//according to this link 
	//http://www.aftabir.com/encyclopedia/urgent/calendar/chronometry/leapyear_new_week.php#year
	//this give us the first day of the year
	year += 2345;
	year = ceil(year * 365.24219879); //Rounds upward
	year -= 4;
	return year % 7;
	//0 Represents "shanbe" and 6 Represents "jome"
}
