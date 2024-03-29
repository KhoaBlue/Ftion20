#include "Function.h"


bool isValid(const SchoolYear& schoolYear, const Course& newCourse) { // Method check valid course to add
    if (newCourse.ID == "") {
        cout << "Error: Course ID cannot be empty" << endl;
        return false;
    }
    if (newCourse.maxStudent <= 0) {
        cout << "Error: Max number of students must be greater than 0" << endl;
        return false;
    }
    if (newCourse.NumOfCredits <= 0) {
        cout << "Error: Number of credits must be greater than 0" << endl;
        return false;
    }
    if (newCourse.Name == "") {
        cout << "Error: Course name cannot be empty" << endl;
        return false;
    }
    if (newCourse.ClassName == "") {
        cout << "Error: Class name cannot be empty" << endl;
        return false;
    }
    if (newCourse.Lecturer == "") {
        cout << "Error: Lecturer name cannot be empty" << endl;
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        const Semester& semester = schoolYear.SemestersList[i];
        for (int j = 0; j < semester.numOfCourses; ++j) {
            if (semester.CoursesList[j].ID == newCourse.ID) {
                cout << "Error: Course ID already exists" << endl;
                return false;
            }
        }
    }

    return true;
}


void addCourse(SchoolYear& schoolYear) {
    string startYear;
    cout << "Enter startYear: ";
    cin >> startYear;
    string endYear;
    cout << "Enter endYear: ";
    cin >> endYear;
    string semesterName;
    cout << "Enter semester: ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, semesterName);

    Semester se;
    // Find se with info
    for (int i = 0; i < 3; i++) {
        if (schoolYear.SemestersList[i].start_date == startYear && schoolYear.SemestersList[i].end_date == endYear) {
            se = schoolYear.SemestersList[i];
            break;
        }
    }

    if (se.CoursesList == nullptr) {
        se.CoursesList = new Course[1];
    }
    else {
        Course* newCoursesList = new Course[se.numOfCourses + 1];
        for (int i = 0; i < se.numOfCourses; ++i) {
            newCoursesList[i] = se.CoursesList[i];
        }
        delete[] se.CoursesList;
        se.CoursesList = newCoursesList;
    }

    Course newCourse;
    //Course ID,Course Name,Class,Lecturer,Number of credits,Max number of students,Number of Students
    cout << "Enter Course ID: ";
    getline(cin, newCourse.ID);
    cout << "Enter Course Name: ";
    getline(cin, newCourse.Name);
    cout << "Enter Class: ";
    getline(cin, newCourse.ClassName);
    cout << "Enter Lecturer: ";
    getline(cin, newCourse.Lecturer);
    cout << "Enter Number of credits: ";
    cin >> newCourse.NumOfCredits;
    cout << "Enter Max number of students: ";
    cin >> newCourse.maxStudent;
    newCourse.numOfStudent = 0;

    if (!isValid(schoolYear, newCourse)) {
        return;
    }

    se.CoursesList[se.numOfCourses++] = newCourse;

    string fileName = "../data/SchoolYears/" + startYear + "-" + endYear + "/" + semesterName + "/CourseList.csv";
    cout << "fileName: " << fileName << endl;
    ofstream file(fileName, ios::app);
    if (!file.is_open()) {
        cout << "Error: Cannot open file CourseList.csv" << endl;
        return;
    }
    file << newCourse.ID << "," << newCourse.Name << "," << newCourse.ClassName << ","
        << newCourse.Lecturer << "," << newCourse.NumOfCredits << "," << newCourse.maxStudent
        << "," << newCourse.numOfStudent << endl;
    file.close();
}

void addStudentToCourse(SchoolYear& currentSchoolYear)
{

    // Input student ID
    string studentID;
    cout << "Enter student ID: ";
    cin >> studentID;

    // Input course ID
    string courseID;
    cout << "Enter course ID: ";
    cin >> courseID;

    // Find student with studentID
    Student st;
    cout << " currentSchoolYear.NumOfClasses: " << currentSchoolYear.NumOfClasses << "" << endl;

    for (int i = 0; i < currentSchoolYear.NumOfClasses; i++) {
        Node<Student>* current = currentSchoolYear.ClassesList[i].stHead;
        while (current != nullptr) {
            if (current->data.StID == studentID) {
                st = current->data;
                break;
            }
            current = current->next;
        }
    }
    if (st.StID == "") {
        cout << "Error: Student ID not found" << endl;
        return;
    }

    // Find course with courseID
    Course co;
    for (int i = 0; i < currentSchoolYear.numOfSemesters; i++) {
        for (int j = 0; j < currentSchoolYear.SemestersList[i].numOfCourses; j++) {
            cout << "Check courseId = " << currentSchoolYear.SemestersList[i].CoursesList[j].ID << endl;
            if (currentSchoolYear.SemestersList[i].CoursesList[j].ID == courseID) {
                co = currentSchoolYear.SemestersList[i].CoursesList[j];
                break;
            }
        }
    }
    if (co.ID == "") {
        cout << "Error: Course ID not found" << endl;
        return;
    }

    // Check if course is full
    if (co.numOfStudent >= co.maxStudent) {
        cout << "Error: Course is full" << endl;
        return;
    }

    string fileName = "../data/ClassesStudents/" + st.Class + "/CourseAttending/" + st.StID + ".txt";
    ifstream file(fileName);
    if (!file.is_open()) {
        ofstream fileOut(fileName);
    }
    else {
        string currentCourseID = "";
        while (getline(file, currentCourseID)) {
            if (currentCourseID == "") {
                break;
            }
            if (currentCourseID == courseID) {
                cout << "Error: Student is already in course" << endl;
                return;
            }
        }
        file.close();
    }


    // Add student to course
    for (int i = 0; i < 7; i++) {
        if (st.courseID[i] == "") {
            st.courseID[i] = courseID;
            break;
        }
    }

    // Add course to student
    for (int i = 0; i < 7; i++) {
        if (co.stHead == nullptr) {
            co.stHead = new Node<Student>;
            co.stHead->data = st;
            co.stHead->next = nullptr;
            co.numOfStudent++;
            break;
        }
        else {
            Node<Student>* current = co.stHead;
            while (current->next != nullptr) {
                current = current->next;
            }
            Node<Student>* newStudentNode = new Node<Student>;
            newStudentNode->data = st;
            newStudentNode->next = nullptr;
            current->next = newStudentNode;
            co.numOfStudent++;
            break;
        }
    }

    // Update student in class
    for (int i = 0; i < currentSchoolYear.NumOfClasses; i++) {
        Node<Student>* current = currentSchoolYear.ClassesList[i].stHead;
        while (current != nullptr) {
            if (current->data.StID == studentID) {
                current->data = st;
                break;
            }
            current = current->next;
        }
    }

    // Update course in semester
    for (int i = 0; i < currentSchoolYear.numOfSemesters; i++) {
        for (int j = 0; j < currentSchoolYear.SemestersList[i].numOfCourses; j++) {
            if (currentSchoolYear.SemestersList[i].CoursesList[j].ID == courseID) {
                currentSchoolYear.SemestersList[i].CoursesList[j] = co;
                break;
            }
        }
    }

    // Add courseId to file
    ofstream fileOut(fileName, ios::app);
    if (!fileOut.is_open()) {
        cout << "Error: Cannot open file " << fileName << endl;
        return;
    }
    fileOut << courseID << endl;
    fileOut.close();

    cout << "Add student to course successfully" << endl;
}

void addNewClasses(Class*& NewClasses, int& numOfClass)
{
    cout << "Please input the file name: ";
    string filename;
    getline(cin, filename);
    for (int i = 0; i < numOfClass; ++i) {
        cin >> NewClasses[i].Name;
        //readStudentsList(NewClasses[i].stHead, filename);
    }
}

void addClasses(SchoolYear& sy, Class* OldClasses, int NumOldClasses)
{
    int NumNewClasses;
    cout << "Please input the number of new Classes";
    cin >> NumNewClasses;
    sy.NumOfClasses = NumNewClasses + NumOldClasses;
    sy.ClassesList = new Class[sy.NumOfClasses];

    addNewClasses(sy.ClassesList, NumNewClasses);
    for (int i = 0; i < NumOldClasses; ++i) {
        sy.ClassesList[i + NumNewClasses] = OldClasses[i];
    }
}

void initSchoolYear(SchoolYear& sy, Class* OldClasses, int NumOldClasses)
{
    cout << "Input Name of School Year: ";
    getline(cin, sy.Name);
    cout << "Input Start date: ";
    getline(cin, sy.start_date);
    cout << "Input End date: ";
    getline(cin, sy.end_date);
    addClasses(sy, OldClasses, NumOldClasses);
    sy.SemestersList = new Semester[3];
    for (int i = 0; i < 3; ++i) {
        sy.SemestersList[i].CoursesList = new Course[100];
    }
}

void createSchoolYear(SchoolYear*& SyList, int& numSY) {
    SchoolYear newSY;
    initSchoolYear(newSY, SyList[numSY - 1].ClassesList, SyList[numSY - 1].NumOfClasses);
    SchoolYear* newSchoolYearList = new SchoolYear[numSY + 1];
    for (int i = 0; i < numSY; ++i) {
        newSchoolYearList[i] = SyList[i];
    }
    newSchoolYearList[numSY] = newSY;
    delete[] SyList;
    SyList = newSchoolYearList;
    numSY++;
    updateSchoolYearList(SyList, numSY);
}