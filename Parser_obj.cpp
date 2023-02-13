#include "Parser_obj.h"
#include<iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <stdio.h>

using namespace std;

const char* name_blank = "H:\\Projects\\cnc machining simulation\\Models\\blank.stl";
const char* name_tool = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\tool.obj";


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
    vector<float> normals;

    vector<float> vert ;

    //объявляем переменную которая будет ссылкой на файл 
    std::fstream FileObj;
    /*связываем переменную с файлом
    name - имя файла
    In - открываем файл на чтение
    */
    FileObj.open(name_blank, std::ios::in);
    //проверяем открылся ли файл 
    if (FileObj.is_open())
    {
        //обьявляем переменную для чтения строк текста из файла
        std::string ReadLine;
        //Читаем файл пока он не закончился
        while (!FileObj.eof())
        {
            //получаем 1 строку из файла
            std::getline(FileObj, ReadLine, '\n');

            string V1 = "0";
            string V2 = "0";

            if (ReadLine.size() > 11)
            {
            vector<string> mass = split(ReadLine.c_str(), " ");
            V1 = mass[0];
            V2 = mass[1];
            }
            else
            {
                V1 = "0";
                V2 = "0";
            }

            if (V1 == "vertex")
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                vertices.push_back(stof(mass[1]));
                vertices.push_back(stof(mass[2]));
                vertices.push_back(stof(mass[3]));
            }

            else if (V1 == "facet" && V2 == "normal")
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                normals.push_back(stof(mass[2]));
                normals.push_back(stof(mass[3]));
                normals.push_back(stof(mass[4]));

            }
   
        }

        float x1, y1, z1, x2, y2, z2, x3, y3, z3, nx, ny, nz;

        int count = 0;

        for (int i = 0; i < vertices.size(); i += 9)
        {
            
            x1 = vertices[i];
            y1 = vertices[i + 1];
            z1 = vertices[i + 2];

            x2 = vertices[i + 3]; 
            y2 = vertices[i + 4];
            z2 = vertices[i + 5];

            x3 = vertices[i + 6];
            y3 = vertices[i + 7];
            z3 = vertices[i + 8];

            nx = normals[count];
            ny = normals[count + 1];
            nz = normals[count + 2];



            vert.push_back(x1);
            vert.push_back(y1);
            vert.push_back(z1);
            vert.push_back(nx);
            vert.push_back(ny);
            vert.push_back(nz);

            vert.push_back(x2);
            vert.push_back(y2);
            vert.push_back(z2);
            vert.push_back(nx);
            vert.push_back(ny);
            vert.push_back(nz);

            vert.push_back(x3);
            vert.push_back(y3);
            vert.push_back(z3);
            vert.push_back(nx);
            vert.push_back(ny);
            vert.push_back(nz);

            count += 1;

        }





    }

    else
    {
        std::cout << "Cant open file blank" << std::endl;
    }

	return vert;
}


vector<float> Parse_normals_blank()
{
    vector<float> normals;
    std::fstream FileObj;
    FileObj.open(name_blank, std::ios::in);
    if (FileObj.is_open())
    {
        std::string ReadLine;
        while (!FileObj.eof() )
        {
            std::getline(FileObj, ReadLine, '\n');

            string V1 = "0";
            string V2 = "0";

            if (ReadLine.size() > 11)
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                V1 = mass[0];
                V2 = mass[1];
            }
            else
            {
                V1 = "0";
            }

            if (V1 == "facet" && V2 == "normal")
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                normals.push_back(stof(mass[2]));
                normals.push_back(stof(mass[3]));
                normals.push_back(stof(mass[4]));

            }

        }

    }

    else
    {
        std::cout << "Cant open file blank" << std::endl;
    }

    return normals;
}



vector<float> Parse_vertices_tool()
{


    vector<float> vertices;
    vector<float> normals;

    vector<float> vert;

    //объявляем переменную которая будет ссылкой на файл 
    std::fstream FileObj;
    /*связываем переменную с файлом
    name - имя файла
    In - открываем файл на чтение
    */
    FileObj.open(name_tool, std::ios::in);
    //проверяем открылся ли файл 
    if (FileObj.is_open())
    {
        //обьявляем переменную для чтения строк текста из файла
        std::string ReadLine;
        //Читаем файл пока он не закончился
        while (!FileObj.eof())
        {
            //получаем 1 строку из файла
            std::getline(FileObj, ReadLine, '\n');
            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];
            //получаем 1 символ из строки и по нему определяем что будет читаться в данный момент

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


        float x, y, z, nx, ny, nz;


        for (int i = 0; i < vertices.size(); i += 3)
        {

            x = vertices[i];
            y = vertices[i + 1];
            z = vertices[i + 2];


            vert.push_back(x);
            vert.push_back(y);
            vert.push_back(z);
            vert.push_back(normals[i]);
            vert.push_back(normals[i + 1]);
            vert.push_back(normals[i + 2]);


        }





    }

    else
    {
        std::cout << "Cant open file tool" << std::endl;
    }

    return vert;
}


vector<int> Parse_indices_tool()
{

    vector<int> indices;


    //объявляем переменную которая будет ссылкой на файл 
    std::fstream FileObj;
    /*связываем переменную с файлом
    name - имя файла
    In - открываем файл на чтение
    */
    FileObj.open(name_tool, std::ios::in);
    //проверяем открылся ли файл 
    if (FileObj.is_open())
    {

        //обьявляем переменную для чтения строк текста из файла
        std::string ReadLine;
        //Читаем файл пока он не закончился
        while (!FileObj.eof())
        {
            //получаем 1 строку из файла
            std::getline(FileObj, ReadLine, '\n');
            char V1 = ReadLine.c_str()[0],
                V2 = ReadLine.c_str()[1];
            //получаем 1 символ из строки и по нему определяем что будет читаться в данный момент

            if (V1 == 'f' && V2 == ' ')
            {
                vector<string> mass = split(ReadLine.c_str(), " ");
                indices.push_back(stoi(split(mass[1], "//")[0]) - 1);
                indices.push_back(stoi(split(mass[2], "//")[0]) - 1);
                indices.push_back(stoi(split(mass[3], "//")[0]) - 1);
            }

        }

    }

    return indices;
}