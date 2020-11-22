#include <bits/stdc++.h>
#include "structures.h"
#define f first
#define s second


/*
*   Restrictions/Assumptions
* 	--------------------------
*   no two rooms can be used at the same time.
*   no two TAs can have labs at the same time.
*   no two profs can have classes at the same time.
*   no overlapping between groups who have the same TA.
*   for every group only one lecture at a day
*   one professor one lecture at a day
*	each professor teaches at most 2 lectures in total
*   professor can only choose a preferred timeslot not a day
*   lecture will always be at either 1st or 2nd timeslots
*   lecture and tutorial will have consecutive timeslots
*   prof of a course teaches both lecture and tutorial
*	courses are attended by all groups
*	number of courses for a group is <= 5
*
*	No rooms are assigned // UNDONE
*	works for only one class(year) // UNDONE
*
*	Optimizations
*	------------------
*	Merge all input in one loop
*	fields of CELL class should be changed to pointers //WITHDRAWN
*	
*
*/

bool check(int prof_id, int year, int cur, int pref)
{
	for(int g=0; g<G; g++)
	{
		for(int y=0; y<Y; y++)
		{
			if(grid[cur][pref][y][g].type == 1 && 
			grid[cur][pref][y][g].instructor_id == prof_id) return 1;
			if(grid[cur][pref+1][y][g].type == 1 && 
			grid[cur][pref+1][y][g].instructor_id == prof_id) return 1;
		}
		if(grid[cur][pref][year][g].type != -1) return 1;
		if(grid[cur][pref+1][year][g].type != -1) return 1;
	}
	return 0;
}

int get_day(int prof_id, int year, int cur, int pref)
{
	while(check(prof_id, year, cur, pref)) cur += 2, cur %= 5;
	return cur;	
}

void AssignLectureAndTutorial(int id, int day, int slot, int year, int group)
{
	CELL &lec = grid[day][slot][year][group];
	lec.course_name = courses[id].name;
	lec.type = 1;
	lec.instructor_id = courses[id].prof->id;
	lec.instructor_name = courses[id].prof->name;
	lec.room_id = rooms[0][cnt[day][slot][0]].id;
	
	CELL &tut = grid[day][slot+1][year][group];
	tut.course_name = courses[id].name;
	tut.type = 2;
	tut.instructor_id = courses[id].prof->id; 
	tut.instructor_name = courses[id].prof->name;
	tut.room_id = rooms[1][cnt[day][slot+1][1]].id;
	if(group == G-1) cnt[day][1+slot][1]++, cnt[day][slot][0]++;
}

void AssignLab(int id, int day, int slot, int year, int group)
{
	int sz = courses[id].TAs.size();
	int C = (G + sz - 1)/sz;
	CELL &lab = grid[day][slot+2+(group%C)][year][group];
	lab.type = 3;
	lab.course_name = courses[id].name;
	lab.instructor_id = TAs[courses[id].TAs[group/C]].id;
	lab.instructor_name = TAs[courses[id].TAs[group/C]].name;
	lab.room_id = rooms[2][cnt[day][slot+2+(group%C)][2]++].id;
}


void AssignCourse(int id, int pref, int cur)
{
	int year = courses[id].year-1;
	for(int j=0; j<G; j++)
	{
		AssignLectureAndTutorial(id, cur, pref, year, j);
		AssignLab(id, cur, pref, year, j);
	}
}




void GenerateTimetable()
{
	int cnt = 0;
	int cur[6] = {};
	int year_day[5][5] = {{}};
	int prof_day[profs.size()][5] = {{}};
    int prefers[profs.size()][7];
    memset(prefers, 0, sizeof prefers);
    for(int i=0; i<prefs.size(); i++) prefers[prefs[i].prof->id][prefs[i].time] = 1;
    for(int i=0; i<courses.size(); i++)
    {
		int pref = 1;
		int prof_id = courses[i].prof->id;
		if(prefers[prof_id][0]) pref = 0;
		int year = courses[i].year-1;
		cur[year] = get_day(prof_id, year, cur[year], pref);
    	AssignCourse(courses[i].id, pref, cur[year]);
		cur[year]+=2, cur[year] %= 5;
	}
}


void FillSchedClasses()
{
	int cnt = 1;
	for(int d=0; d<D; d++)
	{
		for(int t=0; t<T; t++)
		{
			for(int y=0; y<Y; y++)
			{
				for(int g=0; g<G; g++)
				{
					CELL &cell = grid[d][t][y][g];
					if(cell.type == -1) continue;
					SCHED_CLASS c;
					c.room_id = cell.room_id+1;
					c.id = cnt++;
					c.timeslot = t+1;
					c.day = d+1;
					c.group_id = g+1;
					c.instructor_id = cell.instructor_id+1;
					c.type = cell.type;
					output.push_back(c);
				}
			}
		}
	}
}

void PrintTimetable()
{
	string title(((15*G+1)*Y-1)/2, '*');
	title = title + " TESTING PHASE " + title;
	cout << title << endl << endl;
	string tmp(13+(15*G+1)*Y, '-');
	for(int d=0; d<D; d++)
	{
		cout << "Day: " << days[d] << endl;
		for(int t=0; t<T; t++)
		{
			cout << slot[t] << " | ";
			for(int i=0; i<Y; i++)
			{
				for(int j=0; j<G; j++)
				{
					CELL &ref = grid[d][t][i][j];
					if(ref.type != -1)
						cout << ref.course_name << " " << types[ref.type-1] 
						<< " " << R[ref.room_id].number << " |"; 
					else cout << "            " << " |";
					if(j != 2) cout << " ";
				}
				cout << "| ";
			}
			cout << endl;
		}
		cout << tmp << endl;
	}
}

void PrintSchedClasses()
{
	cout << output.size() << endl;
	for(int i=0; i<(int)output.size(); i++)
	{
		cout << output[i];
	}
}
int main()
{
	freopen("input.txt", "r", stdin);
	freopen("timetable.txt", "w", stdout);
 	//taking input
    int n;
    // professors
    cin >> n;
    profs.resize(n);
    while(n--)
    {
        PROFESSOR p;
        cin >> p;
        p.id--;
        profs[p.id]=p;
    }
	//TAs
	cin >> n;
	TAs.resize(n);
	while(n--)
	{
		TA t;
		cin >> t;
		t.id--;
		TAs[t.id]=t;
	}
    //courses
    cin >> n;
    courses.resize(n);
    while(n--)
    {
        COURSE c;
        int pid;
        cin >> c >> pid;
        pid--;
        c.id--;
        c.prof = &profs[pid];
        courses[c.id] = c;
    }
	//rooms
	cin >> n;
	R.resize(n);
	while(n--)
	{
		ROOM r;
		cin >> r;
		r.id--;
		rooms[r.type-1].push_back(r);
		R[r.id] = r;
	}	
    //groups
    cin >> n;
    groups.resize(n);
    while(n--)
    {
        GROUP g;
        cin >> g;
        g.id--;
        groups[g.id] = g;
    }
    //preferences
    cin >> n;
    prefs.resize(n);
    while(n--)
    {
        PREFERENCES p;
        int pid;
        cin >> p >> pid;
        pid--;
        p.id--;
        p.prof = &profs[pid];
    	prefs[p.id] = p;
	}
    //TA_COURSE
    cin >> n;
    while(n--)
    {
    	int tid, cid;
    	cin >> tid >> cid;
		tid--, cid--;
		courses[cid].TAs.push_back(tid);
	}
    GenerateTimetable();
	FillSchedClasses();
//	PrintSchedClasses();
	PrintTimetable();
}
