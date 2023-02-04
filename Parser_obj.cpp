#include "Parser_obj.h"
#include<iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <stdio.h>

using namespace std;




std::vector<std::string> split(const std::string& string,
    const std::string& delims)
{
    std::vector<std::string> result;
    size_t current_idx = 0;
    while (current_idx != std::string::npos) {
        size_t from = string.find_first_not_of(delims, current_idx);
        size_t to = string.find_first_of(delims, from);
        size_t len = std::string::npos;
        if (to != std::string::npos) {
            len = to - from;
        }
        result.push_back(string.substr(from, len));
        current_idx = to;
    }
    return result;
}






vector<float> Parse_vertices()
{


    vector<float> vertices;
    vector<float> normals;

    vector<float> vert ;

    const char* name = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\cube.txt";


    //��������� ���������� ������� ����� ������� �� ���� 
    std::fstream FileObj;
    /*��������� ���������� � ������
    name - ��� �����
    In - ��������� ���� �� ������
    */
    FileObj.open(name, std::ios::in);
    //��������� �������� �� ���� 
    if (FileObj.is_open())
    {
        //��������� ���������� ��� ������ ����� ������ �� �����
        std::string ReadLine;
        //������ ���� ���� �� �� ����������
        while (!FileObj.eof())
        {
            //�������� 1 ������ �� �����
            std::getline(FileObj, ReadLine, '\n');
            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];
            //�������� 1 ������ �� ������ � �� ���� ���������� ��� ����� �������� � ������ ������

            if (V1 == 'v' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                vertices.push_back(stof(mass[1]));
                vertices.push_back(stof(mass[2]));
                vertices.push_back(stof(mass[3]));
            }

            else if (V1 == 'v' && V2 == 'n')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                normals.push_back(stof(mass[1]));
                normals.push_back(stof(mass[2]));
                normals.push_back(stof(mass[3]));
            }



            
        }


        for (int i = 0; i < vertices.size(); i += 3)
        {
            
            vert.push_back(vertices[i]);
            vert.push_back(vertices[i+1]);
            vert.push_back(vertices[i+2]);
            vert.push_back(normals[i]);
            vert.push_back(normals[i+1]);
            vert.push_back(normals[i+2]);
            

        }





    }

	return vert;
}


vector<int> Parse_indices()
{

    vector<int> indices;

    const char* name = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\cube.txt";


    //��������� ���������� ������� ����� ������� �� ���� 
    std::fstream FileObj;
    /*��������� ���������� � ������
    name - ��� �����
    In - ��������� ���� �� ������
    */
    FileObj.open(name, std::ios::in);
    //��������� �������� �� ���� 
    if (FileObj.is_open())
    {
        //��������� ���������� ��� ������ ����� ������ �� �����
        std::string ReadLine;
        //������ ���� ���� �� �� ����������
        while (!FileObj.eof())
        {
            //�������� 1 ������ �� �����
            std::getline(FileObj, ReadLine, '\n');
            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];
            //�������� 1 ������ �� ������ � �� ���� ���������� ��� ����� �������� � ������ ������

            if (V1 == 'f' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                indices.push_back(stoi(split(mass[1], "//")[0]));
                indices.push_back(stoi(split(mass[2], "//")[0]));
                indices.push_back(stoi(split(mass[3], "//")[0]));
            }

        }

    }


    return indices;
}