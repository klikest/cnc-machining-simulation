#pragma once
#include<iostream>
#include<vector>

class Mesh
{
	float x = 0;
	float y = 0;
	float z = 0;

	std::vector<float> vertices_and_normals;
	std::vector<float> vertices;
	std::vector<int> indices;

public:
	Mesh();

	void add_Data_vertices(std::vector<float> vertices_and_normals_enter);
	std::vector<float> get_Data_vertices();
	void moving_x(float x_p);
	void moving_y(float y_p);
	void moving_z(float z_p);


	std::vector<float> Get_vertives_to_OpenGl(std::vector<float> vert);
	


};

