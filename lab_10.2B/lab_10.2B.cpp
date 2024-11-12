#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

enum Speciality { ITIS, RE, KN, KI, PZ };
string speciality_str[] = { "�����������", "��������������", "����'����� �����", "����'������ ��������", "��������� ������������" };

struct Students {
    string surname;
    int course;
    Speciality speciality;
    int grade_physics;
    int grade_math;

    union Grade {
        int grade_programming;  // ��� "����'����� �����"
        int grade_numerical_methods;  // ��� "�����������"
        int grade_pedagogy;  // ��� ����� ��������������
    } grade_third;

    int get_third_grade() const {
        if (speciality == KN) return grade_third.grade_programming;
        else if (speciality == ITIS) return grade_third.grade_numerical_methods;
        else return grade_third.grade_pedagogy;
    }
};

void Create(Students* S, const int N);
void Print(const Students* S, const int N, const int* index_array);
void sort_index_array(Students* S, int N, int* index_array);
int binary_search(const Students* S, const int* index_array, int N, const string& surname, Speciality speciality, int third_grade);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int N;
    cout << "������ ������� ��������: ";
    cin >> N;

    Students* S = new Students[N];
    int* index_array = new int[N];

    Create(S, N);

    for (int i = 0; i < N; i++) {
        index_array[i] = i;
    }

    sort_index_array(S, N, index_array);
    Print(S, N, index_array);

    string search_surname;
    Speciality search_speciality;
    int search_third_grade;

    cout << "������ ������� ��� ������: "; cin >> search_surname;
    cout << "������ ������������ (0 - �����������, 1 - ��������������, 2 - ����'����� �����, 3 - ����'������ ��������, 4 - ��������� ������������): ";
    int speciality_input;
    cin >> speciality_input;
    search_speciality = (Speciality)speciality_input;
    cout << "������ ������ � �������� ��������: "; cin >> search_third_grade;

    int index = binary_search(S, index_array, N, search_surname, search_speciality, search_third_grade);
    if (index != -1) {
        cout << "������� ��������� �� �������: " << index + 1 << endl;
    }
    else {
        cout << "������� �� ���������." << endl;
    }

    delete[] S;
    delete[] index_array;
    return 0;
}

void Create(Students* S, const int N) {
    for (int i = 0; i < N; i++) {
        int speciality;
        cout << "����� ��������: " << i + 1 << endl;
        cin.ignore();
        cout << "�������: "; getline(cin, S[i].surname);
        cout << "����: "; cin >> S[i].course;
        cout << "������������ (0 - �����������, 1 - ��������������, 2 - ����'����� �����, 3 - ����'������ ��������, 4 - ��������� ������������): ";
        cin >> speciality;
        S[i].speciality = (Speciality)speciality;
        cout << "������ � ������: "; cin >> S[i].grade_physics;
        cout << "������ � ����������: "; cin >> S[i].grade_math;

        if (S[i].speciality == KN) {
            cout << "������ � �������������: "; cin >> S[i].grade_third.grade_programming;
        }
        else if (S[i].speciality == ITIS) {
            cout << "������ � ��������� ������: "; cin >> S[i].grade_third.grade_numerical_methods;
        }
        else {
            cout << "������ � ���������: "; cin >> S[i].grade_third.grade_pedagogy;
        }
    }
}

void Print(const Students* S, const int N, const int* index_array) {
    cout << "=========================================================================================" << endl;
    cout << "|  �  |   �������   |  ����  | ������������           | Գ���� | ���������� | ����� ������� |" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < N; i++) {
        const Students& student = S[index_array[i]];
        cout << "| " << setw(3) << i + 1
            << " | " << setw(12) << student.surname
            << " | " << setw(5) << student.course
            << " | " << setw(20) << speciality_str[student.speciality]
            << " | " << setw(6) << student.grade_physics
            << " | " << setw(10) << student.grade_math
            << " | " << setw(13) << student.get_third_grade()
            << " |" << endl;
    }

    cout << "=========================================================================================" << endl;
}

void sort_index_array(Students* S, int N, int* index_array) {
    sort(index_array, index_array + N, [S](int a, int b) {
        if (S[a].grade_third.grade_programming != S[b].grade_third.grade_programming)
            return S[a].grade_third.grade_programming > S[b].grade_third.grade_programming;
        if (S[a].speciality != S[b].speciality)
            return S[a].speciality < S[b].speciality;
        return S[a].surname < S[b].surname;
        });
}

int binary_search(const Students* S, const int* index_array, int N, const string& surname, Speciality speciality, int third_grade) {
    int left = 0, right = N - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        const Students& student = S[index_array[mid]];

        if (student.speciality < speciality ||
            (student.speciality == speciality && student.get_third_grade() < third_grade) ||
            (student.speciality == speciality && student.get_third_grade() == third_grade && student.surname < surname)) {
            left = mid + 1;
        }
        else if (student.speciality > speciality ||
            (student.speciality == speciality && student.get_third_grade() > third_grade) ||
            (student.speciality == speciality && student.get_third_grade() == third_grade && student.surname > surname)) {
            right = mid - 1;
        }
        else {
            return index_array[mid];
        }
    }
    return -1;
}

