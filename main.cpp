#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "mcut/mcut.h"



#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <map>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Parser_obj.h"
//#include"Mesh.h"


class Mesh {
	float x;
	float y;
	float z;

	std::vector<float> vertices_and_normals;
	std::vector<float> vertices;
	std::vector<int> indices;

public:
	Mesh(std::vector<float> vertices_and_normals_input)
	{	
		x = 0;
		y = 0;
		z = 0;

		vertices_and_normals = vertices_and_normals_input;

		for (int i = 0; i < vertices_and_normals_input.size(); i += 6)
		{
			vertices.push_back(vertices_and_normals_input[i]);
			vertices.push_back(vertices_and_normals_input[i+1]);
			vertices.push_back(vertices_and_normals_input[i+2]);
		}
		for (int i = 0; i < vertices_and_normals_input.size()/2; ++i)
		{
			indices.push_back(i);
		}
	}

	void set_vertices(std::vector<float> vetrtices_)
	{
		vertices.clear();
		vertices = vetrtices_;
	}

	void set_vertices_normals(std::vector<float> vetrtices_)
	{
		vertices_and_normals.clear();
		vertices_and_normals = vetrtices_;
	}

	std::vector<float> get_vertices()
	{
		return vertices;
	}

	std::vector<float> get_vertices_normals()
	{
		return vertices_and_normals;
	}

	std::vector<int> get_indices()
	{
		return indices;
	}


	void move_x_vert(float dx, VBO VBO)
	{
		x += dx;
		for (int i = 0; i < vertices_and_normals.size(); i+=6)
		{
			vertices_and_normals[i] += dx;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices_and_normals.size() * sizeof(float), vertices_and_normals.data(), GL_DYNAMIC_DRAW);

	}

	void move_y_vert(float dy, VBO VBO)
	{
		y += dy;
		for (int i = 0; i < vertices_and_normals.size(); i += 6)
		{
			vertices_and_normals[i+1] += dy;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices_and_normals.size() * sizeof(float), vertices_and_normals.data(), GL_DYNAMIC_DRAW);

	}

	void move_z_vert(float dz, VBO VBO)
	{
		z += dz;
		for (int i = 0; i < vertices_and_normals.size(); i += 6)
		{
			vertices_and_normals[i+2] += dz;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices_and_normals.size() * sizeof(float), vertices_and_normals.data(), GL_DYNAMIC_DRAW);

	}

	float get_x()
	{
		return x;
	}
	float get_y()
	{
		return y;
	}
	float get_z()
	{
		return z;
	}
};






const unsigned int width = 800;
const unsigned int height = 800;

int main()
{

	Mesh blank(Parse_vertices_blank());
	Mesh tool(Parse_vertices_tool());

	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "cnc machining sinulator", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	
	Shader blank_shaderProgram("default.vert", "default.frag");
	VAO VAO_blank;
	VAO_blank.Bind();
	VBO VBO_blank(blank.get_vertices_normals().data(), blank.get_vertices_normals().size() * sizeof(float));
	EBO EBO_blank(blank.get_indices().data(), blank.get_indices().size() * sizeof(int));
	VAO_blank.LinkAttrib(VBO_blank, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_blank.LinkAttrib(VBO_blank, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_blank.Unbind();
	VBO_blank.Unbind();
	EBO_blank.Unbind();
	

	Shader tool_shaderProgram("light.vert", "light.frag");
	VAO VAO_tool;
	VAO_tool.Bind();
	VBO VBO_tool(tool.get_vertices_normals().data(), tool.get_vertices_normals().size() * sizeof(float));
	EBO EBO_tool(tool.get_indices().data(), tool.get_indices().size() * sizeof(int));
	VAO_blank.LinkAttrib(VBO_tool, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_blank.LinkAttrib(VBO_tool, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_tool.Unbind();
	VBO_tool.Unbind();
	EBO_tool.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(500.0f, 500.0f, 500.0f);

	glm::vec3 toolPos = glm::vec3(250.0f, 0.0f, -100.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);

	lightModel = glm::translate(lightModel, toolPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	tool_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(tool_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(tool_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(tool_shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	blank_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(blank_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(blank_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	int frame = 0;

	double prevTime = 0.0f;
	double crntTime = 0.0f;
	double timeDiff = 0.0f;
	unsigned int counter = 0;


	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		/*
		if (frame < 500)
		{
			blank.move_x_vert(0.1, VBO_blank);
			frame += 1;
		}
		else if (frame >= 500 && frame < 1000)
		{
			blank.move_x_vert(-0.1, VBO_blank);
			frame += 1;
		}
		else if (frame >= 1000)
		{
			frame = 0;
		}
		*/

		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string title = "cnc mashining simulator - " + FPS + "FPS";
			glfwSetWindowTitle(window, title.c_str());
			prevTime = crntTime;
			counter = 0;
		}

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 10000.0f);


		

		blank_shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(blank_shaderProgram, "camMatrix");


		VAO_blank.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, blank.get_indices().size(), GL_UNSIGNED_INT, 0);
		tool_shaderProgram.Activate();
		camera.Matrix(tool_shaderProgram, "camMatrix");
		VAO_tool.Bind();
		glDrawElements(GL_TRIANGLES, tool.get_indices().size(), GL_UNSIGNED_INT, 0);
		
		std::string x_cord = "X = " + std::to_string(blank.get_x());
		std::string y_cord = "Y = " + std::to_string(blank.get_y());
		std::string z_cord = "Z = " + std::to_string(blank.get_z());

		ImGui::Begin("Tool info");
		ImGui::Text(x_cord.c_str());
		ImGui::Text(y_cord.c_str());
		ImGui::Text(z_cord.c_str());
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();



	VAO_blank.Delete();
	VBO_blank.Delete();
	EBO_blank.Delete();
	blank_shaderProgram.Delete();
	VAO_tool.Delete();
	VBO_tool.Delete();
	EBO_tool.Delete();
	tool_shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}