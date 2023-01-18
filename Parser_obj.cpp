#include "Parser_obj.h"
#include<iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

int CountLinesInFile(string filename)
{
	// 1. �������� ��������� F, ������� ������ � ������ filename.
	// ���� ����������� ��� ������ � ��������� �������.
	ifstream F(filename, ios::in);

	// 2. ��������, ���������� �� ����
	if (!F)
	{
		return -1;
	}

	// 3. ��������� ���������� ����� � �����
	// 3.1. �������� �������������� ����������
	int count = 0;
	char buffer[1000]; // ����� ��� ���������� ����� ������

	// 3.2. ���� ������ �����.
	// ������ ������ ����� �������� �������� getline().
	// �������� ����� ����� �������������� �������� eof().
	while (!F.eof())
	{
		// ��������� ������� �����
		count++;

		// ������� ���� ������ � �����
		F.getline(buffer, 1000);
	}

	// 4. ������� ���� F
	F.close();

	// 5. ������� ���������
	return count;
}


int GetStringsFromFileC(string filename, char*** _lines = nullptr)
{
	// 1. �������������� ����������
	char** lines;
	int n = CountLinesInFile(filename); // �������� ���������� ����� � �����

	// 2. ��������, �������� �� ���������� �����
	if (n == -1) return -1;

	// 3. �������� �������� ����������
	ifstream F(filename); // ������� ���� ��� ������

	// 4. ��������, �������� �� ����
	if (!F) return -1;

	// 5. ������� �������� ������ ��� n �����
	try
	{
		lines = new char* [n];
	}
	catch (bad_alloc e)
	{
		// ���� ���������� �������� ������, �� �����
		cout << e.what() << endl; // ������� ��������� �� ������
		F.close(); // ������� ����
		return -1;
	}

	// 6. ���� ������ ����� � ��������� ������ ��� ���
	int len; // ����� ����� ������
	char buffer[1000]; // ������, ���� ������������ ���� ������ �� �����

	for (int i = 0; i < n; i++)
	{
		// 6.1. ������� ������ �� �����
		F.getline(buffer, 1000);

		// 6.2. ���������� ����� ����������� ������
		for (len = 0; buffer[len] != '\0'; len++);

		// 6.3. �������� ������ ��� len+1 ��������
		lines[i] = new char[len + 1];

		// 6.4. ����������� ������ �� buffer � lines[i]
		for (int j = 0; j < len; j++)
			lines[i][j] = buffer[j];
		lines[i][len] = '\0'; // �������� ������ ����� ������
	}

	// 7. ������� ����
	F.close();

	// 8. �������� ���������
	*_lines = lines;
	return n;
}


char** Parse_vertices()
{
	// ����� ������ ����� ����� �� �����
	// 1. �������� ���������� ����������
	int count; // ���������� �����
	char** lines = nullptr; // ������ ����� ���� char*

	// 2. �������� ������ ����� ���� char**
	count = GetStringsFromFileC("H:\\Projects\\cnc machining simulation\\Models\\blank.txt", &lines);

	// 3. ��������, ������� �� ������
	if (count < 0)
	{
		cout << "������ ��� �������� �����" << endl;
	}

	list<float> vertices;
	char* ptr = 0; 
	char ch2{'v'};

	const char* SEPARATORS = " ";

	int count_vert = 0;
	int count_ind = 0;
	int count_normals = 0;

	//float vertices[5000];
	//float normals[5000];
	//float indices[5000];

	for (int i = 0; i < count; i++)
	{
		if (lines[i][0] == '#')
		{
		};

		if (lines[i][0] == 'v' && lines[i][1] == ' ')
		{
			//&vertices[count_vert] = lines[i];
			count_vert += 1;
		};

		if (lines[i][0] == 'v' && lines[i][1] == 'n')
		{
			count_normals += 1;
		};

		if (lines[i][0] == 'f' && lines[i][1] == ' ')
		{
			count_ind += 1;
		};

	};

	cout << "vertices = " << count_vert << endl;
	cout << "normals = " << count_normals << endl;
	cout << "indices = " << count_ind << endl;






	/*for (int i = 0; i < count; i++)
	{
		ptr = strtok(lines[i], SEPARATORS); //�������� ������ ����� �� ������
		if (ptr == vert)
		{
			cout << lines[i] << endl;
		}
		
	}*/

	//cout << lines[1] << endl;

	// 4. ���������� ������, ���������� ��� ������ ������ lines
	//if (lines != nullptr)
	//	for (int i = 0; i < count; i++)
	//		delete lines[i];

	// 5. ���������� ������, ���������� ��� ������� lines
	//if (lines != nullptr)
	//	delete[] lines;

	return lines;
}
