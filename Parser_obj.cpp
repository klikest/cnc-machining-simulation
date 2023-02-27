#include "Parser_obj.h"
#include<iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <stdio.h>

using namespace std;

const char* name_blank = "Models\\tool.stl";
const char* name_tool = "Models\\cube_2.stl";


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

            string V1 = "0";

            if (ReadLine.size() > 11)
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                V1 = mass[0];
            }
            else
            {
                V1 = "0";
            }

            if (V1 == "vertex")
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

    for (int i = 0; i < vertices.size(); i += 3)
    {
        //cout << i/3 + 1 << endl;
        //cout << vertices[i] << '\t' << vertices[i + 1] << '\t' << vertices[i + 2] << '\n' << endl;
    }

	return vertices;
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

            string V1 = "0";

            if (ReadLine.size() > 11)
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                V1 = mass[0];
            }
            else
            {
                V1 = "0";
            }

            if (V1 == "vertex")
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
