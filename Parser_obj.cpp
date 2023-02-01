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






int Parse_vertices()
{


    vector<float> vertices;

    const char* name = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\blank.txt";


    //объявляем переменную которая будет ссылкой на файл 
    std::fstream FileObj;
    /*связываем переменную с файлом
    name - имя файла
    In - открываем файл на чтение
    */
    FileObj.open(name, std::ios::in);
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
                std::cout << "Point \t" << ReadLine.c_str() << endl;;
                vector<string> mass = split(ReadLine.c_str(), " ");
                cout << mass[1] << "\t" << mass[2] << "\t" << mass[3] << endl;
                vertices.push_back(stof(mass[1]));
                vertices.push_back(stof(mass[2]));
                vertices.push_back(stof(mass[3]));
            }

            else if (V1 == 'v' && V2 == 'n')
            {
                std::cout << "\n Normal \t" << ReadLine.c_str();
            }

            
        }




    }


	return 0;
}
