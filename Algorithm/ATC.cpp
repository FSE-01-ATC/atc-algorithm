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
*   lecture and tutorial will have consecutive timeslots
*   prof of a course teaches both lecture and tutorial
*   professor can only choose a preferred timeslot not a day
*   lecture will always be at either 1st or 2nd timeslots
*	lectures are attended by all groups
*	number of courses for a group is <= 5
*	works for only one class(year)
*	fields of CELL class should be changed to pointers
*/
const int N = 505;

vector<PROFESSOR> profs;
vector<COURSE> courses;
vector<ROOM> rooms;
vector<CLASS> classes;
vector<GROUP> groups;
vector<PREFERENCES> prefs;
vector<TA> TAs;
vector<int> class_groups[N];
vector<int> course_TAs[N];
int day[N];
int timeslot[N];
	

int row, col;
CELL grid[5][7][5][6]; //[days][timeslot][year][group];

void fill_lectures_and_tutorials()
{
	int cnt = 0;
	memset(timeslot, 0, sizeof timeslot);
    memset(day, 0, sizeof day);
    int prefers[profs.size()][7];
    memset(prefers, 0, sizeof prefers);
    for(int i=0; i<prefs.size(); i++) prefers[prefs[i].prof->id][prefs[i].time] = 1;
    for(int i=0; i<classes.size(); i++)
    {
    	if(classes[i].type == 1)
    	{
    		for(int j=0; j<6; j++)
	    	{
	    		int pref = 1;
	    		if(prefers[classes[i].course->prof->id][0]) pref = 0;
	    		CELL &ref = grid[cnt][pref][groups[class_groups[i][0]].year-1][j];
	    		ref.type = 1;
	    		ref.course_name=classes[i].course->name;
	    		ref.instructor_name=classes[i].course->prof->name;
	    		day[classes[i].course->id] = cnt;
	    		timeslot[classes[i].course->id] = pref;
//	    		if(!j)
//	    		{
//	    			cout << ref.course_name << " " << ref.type << " " << ref.instructor_name << endl;
//	    			cout << cnt << " " << pref << endl;
//	    		}
			}
	    	cnt++;
    	}
	}
	for(int i=0; i<classes.size(); i++)
	{
		if(classes[i].type == 2)
		{
			for(int j=0; j<6; j++)
			{
				int idx = classes[i].course->id;
				CELL &ref = grid[day[idx]][timeslot[idx]+1][groups[class_groups[i][0]].year-1][j];
				ref.type = 2;
				ref.course_name=classes[i].course->name;
				ref.instructor_name=classes[i].course->prof->name;
//				if(!j) 
//					cout << ref.course_name << " " << ref.type << " " << ref.instructor_name << endl;
	    	
			}
		}	
	}
}

void fill_labs()
{
	
	for(int i=0; i<classes.size(); i++)
	{
		if(classes[i].type != 3) continue;
		int idx = classes[i].course->id;
		int G = class_groups[i].size();
		int C = G/course_TAs[i].size();
		int year = groups[class_groups[i][0]].year-1;
		for(int j=0; j<G; j++)
		{
			CELL &ref = grid[day[idx]][timeslot[idx]+2+(j%C)][year][j];
			ref.type = 3;
			ref.course_name=classes[i].course->name;
			ref.instructor_name=TAs[course_TAs[i][j/C]].name;
		}
	}
}

int main()
{
 	//taking input
    int n;
    // professors
    cin >> n;
    profs.resize(n);
    while(n--)
    {
        PROFESSOR p;
        cin >> p.id >> p.name >> p.email;
        p.id--;
        profs[p.id]=p;
    }
	//TAs
	cin >> n;
	TAs.resize(n);
	while(n--)
	{
		TA t;
		cin >> t.id >> t.name >> t.email;
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
        cin >> c.id >> c.code >> c.name >> pid;
        pid--;
        c.id--;
        c.prof = &profs[pid];
        courses[c.id] = c;
    }

    //classes
    cin >> n;
    classes.resize(n);
    while(n--)
    {
        CLASS c;
        int cid;
        cin >> c.id >> c.type >> cid;
        c.id--;
        cid--;
        c.course = &courses[cid];
        classes[c.id] = c;
    }
    //groups
    cin >> n;
    groups.resize(n);
    while(n--)
    {
        GROUP g;
        cin >> g.id >> g.degree >> g.number >> g.year;
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
        cin >> p.id >> p.time >> pid;
        pid--;
        p.id--;
        p.prof = &profs[pid];
    	prefs[p.id] = p;
	}
    //CLASS_GROUP
    cin >> n;
    while(n--)
    {
    	int cid, gid;
    	cin >> cid >> gid;
    	cid--, gid--;
    	class_groups[cid].push_back(gid);
    }
    //TA_COURSE
    cin >> n;
    while(n--)
    {
    	int tid, cid;
    	cin >> tid >> cid;
		tid--, cid--;
		course_TAs[cid].push_back(tid);
	} 
    fill_lectures_and_tutorials();
	fill_labs();
}
