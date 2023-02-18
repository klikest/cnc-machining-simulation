#include "Mesh.h"
#include<iostream>



Mesh::Mesh()
{
	x = 0;
	y = 0;
	z = 0;

	std::vector<float> vertices_and_normals;
	std::vector<float> vertices;
	std::vector<int> indices ;
}

void add_Data_vertices(std::vector<float> vertices_and_normals_enter)
{
	for (int i = 0; i < vertices_and_normals_enter.size(); ++i)
	{
		//Mesh::x += 1;
	}
}

void create_Indices(std::vector<float> vertices)
{
	for (int i=0; i<vertices.size(); ++i)
	{
		
	}
}




std::vector<float> get_Data_vertices()
{
	return std::vector<float> {0};
}


void Mesh::moving_x(float delta_x)
{
	for (int i = 0; i < 10; i++)
	{
		vertices_and_normals[i] += delta_x;
	}
}

void Mesh::moving_y(float y_p)
{
}

void Mesh::moving_z(float z_p)
{
}

std::vector<float> Mesh::Get_vertives_to_OpenGl(std::vector<float> vert)
{
	for (int i = 0; i < vert.size(); i += 6)
	{
		vertices.push_back(vert[i]);
		vertices.push_back(vert[i+1]);
		vertices.push_back(vert[i+2]);
	}

	return vertices;
}


