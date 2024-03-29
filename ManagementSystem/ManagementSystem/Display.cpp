#include "Function.h"
#include <string>
#include <sstream>

void outputStaffList(Node<User> *StaffHead)
{
	Node<User> *cur = StaffHead;
	while (cur) {
		cout << cur->data.Fullname << " " << cur->data.DOB << " " << cur->data.Email << " ";
		cout << cur->data.Gender << " " << cur->data.Password << " " << cur->data.Username << endl;
		cur = cur->next;
	}
}

void outputStudentList(Node<Student> *StudentHead)
{
	Node<Student> *cur = StudentHead;
	while (cur) {
		outputStudent(cur->data);
	}
}

void outputStudent(Student x)
{
	cout << x.StID << " " << x.Fullname << " " << x.Gender << " " << x.Class << " ";
	cout << x.DOB << " " << x.SocialID << " " << x.Password << " " << x.numOfCoursesAttending << endl;
}

void viewListCourse(const Student& st, SchoolYear schoolYear) {
    cout << "View list course of student: " << st.FirstName << " " << st.LastName << endl;
    for (int i = 0; i < 3; ++i) {
        const Semester& semester = schoolYear.SemestersList[i];
        for (int j = 0; j < semester.numOfCourses; ++j) {
            const Course& course = semester.CoursesList[j];
            for (int k = 0; k < 7; ++k) {
                if (st.courseID[k] == course.ID) {
                    cout << "Course " << k + 1 << ": " << course.Name << endl;
                }
            }
        }
    }
} // Method view list course of student


void viewScoreboard(SchoolYear currentSchoolYear) {
    string courseId;
    cout << "Enter course ID: ";
    cin >> courseId;
    Semester* semestersList = currentSchoolYear.SemestersList;

    for (int i = 0; i < currentSchoolYear.numOfSemesters; i++) {
        Semester semester = semestersList[i];
        if (semester.numOfCourses <= 0) {
            continue;
        }

        Course* coursesList = semester.CoursesList;
        for (int j = 0; j < semester.numOfCourses; j++) {
            if (coursesList[j].ID == courseId) {
                viewScoreboardInfo(semester, courseId, coursesList[j].ClassName);
            }
        }
    }
}

void viewScoreboardInfo(Semester semester, string courseId, string className) {
    // Read file in path
    string path = "../data/SchoolYears/"
        + semester.start_date
        + "-" + semester.end_date
        + "/" + semester.name
        + "/CoursesStudents/"
        + courseId + "_" + className + ".csv";

    // Read file
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << path << endl;
        return;
    }

    cout << "With class " << className << " and course " << courseId << endl;
    cout << "\t" << left << setw(10) << "No" << setw(20) << "Student ID" << setw(30) << "Student Full Name" << setw(10) << "Midterm" << setw(10) << "Final" << setw(10) << "Other" << setw(10) << "Total" << setw(10) << "GPA" << endl;
    // Read data
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        // Data of line: No,Student ID,Student Full Name,Midterm Mark,Final Mark,Other Mark,Total Mark,GPA
        string token;
        if (firstLine) {
            firstLine = false;
            continue;
        }
        istringstream iss(line);
        getline(iss, token, ',');
        cout << "\t" << left << setw(10) << token;
        cout << setw(20);
        getline(iss, token, ',');
        cout << token;
        cout << setw(30);
        getline(iss, token, ',');
        cout << token;
        cout << setw(10);
        getline(iss, token, ',');
        cout << token;
        cout << setw(10);
        getline(iss, token, ',');
        cout << token;
        cout << setw(10);
        getline(iss, token, ',');
        cout << token;
        cout << setw(10);
        getline(iss, token, ',');
        cout << token;
        cout << setw(10);
        getline(iss, token, ',');
        cout << token << endl;
    }
    file.close();
}

void displayClassesList(SchoolYear SyList, Class* CLassList){
    cout << "List of Class in School Year " << SyList.Name << ":" << endl;
    for (int i = 0; i < SyList.NumOfClasses; ++i) {
        cout << SyList.ClassesList[i].Name << endl;
    }
}

void viewStudentsInClass(SchoolYear Sy, Class* ClassList) {
    cout << "List of CLass In School Year " << Sy.Name << ":" << Sy.NumOfClasses << endl;
    for (int i = 0; i < Sy.NumOfClasses; ++i) {
        cout << i + 1 << ": " << ClassList[i].Name << endl;
    }
    cout << "Enter the number to view the students in the class:";
    int n; cin >> n;
    cout << "The Student in the class " << ClassList[n - 1].Name << ":" << endl;
    Node<Student>* current = ClassList[n - 1].stHead;
    while (current) {
        cout << current->data.Fullname << endl;
        current = current->next;
    }
}

void viewListOfCourse(int Option, SchoolYear* SyList,Semester* SemesterList,Course* CourseList, int numSY){
    if (Option == 1) { //View All of Courses
        cout << "List Of All Courses:" << endl;
        for (int i = 0; i < numSY; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < SyList[i].SemestersList[j].numOfCourses; ++k) {
                    cout << SyList[i].SemestersList[j].CoursesList[k].ClassName << ": " << SyList[i].SemestersList[j].CoursesList[k].ID << endl;
                }
               }
        }
    }
    else // Option = 0
    {
        cout << "List Of All Courses in School Year " << SyList[0].Name << ":" << endl;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < SyList[0].SemestersList[i].numOfCourses; ++j) {
                cout << SyList[0].SemestersList[i].CoursesList[j].ClassName << ": " << SyList[0].SemestersList[i].CoursesList[j].ID << endl;
            }
        }
    }
} 

void viewStudentsInCourse(SchoolYear* SyList, Semester* SemesterList, Course* CourseList, int numSY) {
    viewListOfCourse(0, SyList, SemesterList, CourseList, numSY); // 0 - School year nearly
    cout << "Enter the course's ID to view list of students:";
    string tem; cin >> tem;
    bool find = false;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < SyList[0].SemestersList[i].numOfCourses;++j) {
            if (SyList[0].SemestersList[i].CoursesList[j].ID == tem) {
                find = true;
                cout << "List of Student:" << endl;
                Node <Student>* current = SyList[0].SemestersList[i].CoursesList[j].stHead;
                while (current) {
                    cout << current->data.Fullname << endl;
                }
                break;
               }
        }
    }
    if (!find) cout << "Course's ID doesn't exist" << endl;
}

void exportListOfStudentsInCourse(SchoolYear* SyList, Semester* SemesterList, Course* CourseList, int numSY) { // At the end of semester
    viewListOfCourse(0, SyList, SemesterList, CourseList, numSY); 
    cout << "Enter the course's ID to export list of students:";
    string tem; cin >> tem;
    ofstream fout;
    string filename = "../data/Export/" + SyList[0].Name + "/Semester2/" + tem + ".csv";
    fout.open(filename);
    bool find = false;
    if (!fout.is_open()) cout << "Can't export the list";
    else {
        for (int i = 0; i < SyList[0].SemestersList[1].numOfCourses; ++i) {
            if (tem == SyList[0].SemestersList[1].CoursesList[i].ID) {
                find = true;
                Node <Student>* current = SyList[0].SemestersList[1].CoursesList[i].stHead;
                fout << SyList[0].SemestersList[1].CoursesList[i].numOfStudent << endl;
                fout << "ID,Fullname,StudentID" << endl;
                int cnt = 1;
                while (current) {
                    fout << cnt++ << ",";
                    fout << current->data.Fullname << ",";
                    fout << current->data.StID;
                    fout << endl;
                }
                break;
            }
        }
        if (!find) cout << "Course's ID doesn't exist" << endl;
    }
}

bool isStaff(string username) {
	return (!isdigit(username[0]));
}

Node<Student>* findUserStudent(string username, string password, Class* classList, int numOfClasses) {
	Node<Student>* pCur = nullptr;
	for (int i = 0; i < numOfClasses; i++) {
		pCur = classList[i].stHead;
		while (pCur) {
			if (username == pCur->data.StID && password == pCur->data.Password) {
				return pCur;
			}
			pCur = pCur->next;
		}
	}
	return nullptr;
}

Node<User>* findUserStaff(string username, string password, Node<User>* pStaffList) {
	Node<User>* pCur = pStaffList;
	while (pCur) {
		if (username == pCur->data.Username && password == pCur->data.Password) {
			return pCur;
		}
		pCur = pCur->next;
	}
	return nullptr;
}

// return true if user is in data, return false otherwise
bool checkLogin(bool &role, Node<User> *&pUser, Node<Student> *&pStudent, string username, string password, SchoolYear *&currentSchoolYear, Node<User> *StaffHead) {
	if (isStaff(username)) {
		role = true;
		pUser = findUserStaff(username, password, StaffHead);
	}
	else {
		role = false;
		pStudent = findUserStudent(username, password, currentSchoolYear->ClassesList, currentSchoolYear->NumOfClasses);
	}
	return (pStudent != nullptr || pUser != nullptr);
}

