#include "Parser_obj.h"
#include<iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <stdio.h>

using namespace std;

const char* name_blank = "Models\\cube.obj";
const char* name_tool = "Models\\cube_tool.obj";


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






vector<float> Parse_vertices_blank()
{
    vector<float> vertices;

    std::fstream FileObj;
    FileObj.open(name_blank, std::ios::in);
    if (FileObj.is_open())
    {
        std::string ReadLine;
        while (!FileObj.eof())
        {
            std::getline(FileObj, ReadLine, '\n');

            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];

            if (V1 == 'v' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                vertices.push_back(stof(mass[1]));
                vertices.push_back(stof(mass[2]));
                vertices.push_back(stof(mass[3]));
            }
        }
    }


    else
    {
        std::cout << "Cant open file blank" << std::endl;
    }

	return vertices;
}


vector<int> Parse_indices_blank()
{
    vector<int> indices;

    std::fstream FileObj;
    FileObj.open(name_blank, std::ios::in);
    if (FileObj.is_open())
    {
        std::string ReadLine;
        while (!FileObj.eof())
        {
            std::getline(FileObj, ReadLine, '\n');

            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];

            if (V1 == 'f' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                indices.push_back(stof(mass[1]));
                indices.push_back(stof(mass[2]));
                indices.push_back(stof(mass[3]));
            }
        }
    }


    else
    {
        std::cout << "Cant open file blank" << std::endl;
    }

    return indices;
}









vector<float> Parse_vertices_tool()
{
    vector<float> vertices;

    std::fstream FileObj;
    FileObj.open(name_tool, std::ios::in);
    if (FileObj.is_open())
    {
        std::string ReadLine;
        while (!FileObj.eof())
        {
            std::getline(FileObj, ReadLine, '\n');

            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];

            if (V1 == 'v' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                vertices.push_back(stof(mass[1]));
                vertices.push_back(stof(mass[2]));
                vertices.push_back(stof(mass[3]));
            }
        }
    }

    else
    {
        std::cout << "Cant open file blank" << std::endl;
    }

    return vertices;
}



vector<int> Parse_indices_tool()
{
    vector<int> indices;

    std::fstream FileObj;
    FileObj.open(name_tool, std::ios::in);
    if (FileObj.is_open())
    {
        std::string ReadLine;
        while (!FileObj.eof())
        {
            std::getline(FileObj, ReadLine, '\n');

            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];

            if (V1 == 'f' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                indices.push_back(stof(mass[1]));
                indices.push_back(stof(mass[2]));
                indices.push_back(stof(mass[3]));
            }
        }
    }


    else
    {
        std::cout << "Cant open file blank" << std::endl;
    }

    return indices;
}
