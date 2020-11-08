#include<string>
#include<vector>
using namespace std;

struct PROFESSOR {
	int id;
	string name, email;
};

struct COURSE {
	int id;
	string code, name;	
	PROFESSOR* prof;
};

struct TA {
	int id;
	string name, email;
};

struct CLASS {
	int id, day, type, timeslot;
	COURSE* course;
};

struct ROOM {
	int id, number, type;	
};

struct GROUP {
	int id, number, year;	
	string degree;
};

struct PREFERENCES {
	int id, time;
	PROFESSOR* prof;
};

struct CELL {
	int type, room;
	string instructor_name, course_name;
};
