#include<string>
#include<vector>
using namespace std;

struct PROFESSOR {
	int id;
	string name, email;
};

istream& operator >> (istream& is, PROFESSOR& p)
{
	is >> p.id >> p.name >> p.email;
	return is;
}

struct COURSE {
	int id, year;
	string code, name, degree;	
	PROFESSOR* prof;
	vector<int> TAs;
};

istream& operator >> (istream& is, COURSE& c)
{
	int id;
	is >> c.id >> c.code >> c.name >> c.degree >> c.year;
	return is;
}

struct TA {
	int id;
	string name, email;
};

istream& operator >> (istream& is, TA& t)
{
	is >> t.id >> t.name >> t.email;
	return is;
}

struct CLASS {
	int id, type;
	COURSE* course;
};

istream& operator >> (istream& is, CLASS& c)
{
	is >> c.id >> c.type;
	return is;
}

struct ROOM {
	int id, number, type;	
};

istream& operator >> (istream& is, ROOM& r)
{
	is >> r.id >> r.number >> r.type;
	return is;
}

struct GROUP {
	int id, number, year;	
	string degree;
};

istream& operator >> (istream& is, GROUP& g)
{
	cin >> g.id >> g.degree >> g.number >> g.year;
	return is;
}

struct PREFERENCES {
	int id, time;
	PROFESSOR* prof;
};

istream& operator >> (istream& is, PREFERENCES& p)
{
	is >> p.id >> p.time;
	return is;
}

struct CELL {
	int type=-1, room_id;
	int instructor_id;
	string instructor_name, course_name;
};

struct SCHED_CLASS {
	int id;
	int day, timeslot;
	int type=-1;
	int instructor_id, room_id, group_id;
};

ostream& operator << (ostream& os, SCHED_CLASS& c)
{
	os << c.id << " " << c.day << " " << c.timeslot << " " << c.type
	<< " " << c.instructor_id << " "  << c.group_id << endl;
	return os;
}

const int N = 505;
const int G = 4, Y = 3, D = 5, T = 7;
vector<PROFESSOR> profs;
vector<COURSE> courses;
vector<ROOM> rooms[3];
vector<ROOM> R;
vector<CLASS> classes;
vector<GROUP> groups;
vector<PREFERENCES> prefs;
vector<SCHED_CLASS> output;
vector<TA> TAs;
string days[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};
string slot[] = {
	"09:00-10:30", "10:40-12:10", "12:40-14:10", "14:20-15:50", "16:00-17:30", "17:40-19:10", "19:20-20:50"
};
string types[] = {"Lec  ", "Tut  ", "Lab  "};

int cnt[D][T][3];
CELL grid[D][T][Y][G]; //[days][timeslot][year][group];

