#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Parser_obj.h"



const unsigned int width = 800;
const unsigned int height = 800;


const	std::string tool = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\tool.obj";
const	std::string blank = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\blank.obj";


int main()
{

	std::vector<float> vert_blank = Parse_vertices_blank();
	float vertices_blank[10000];
	int indices_blank[10000];
	for (int i = 0; i < vert_blank.size(); ++i)
	{
		vertices_blank[i] = vert_blank[i];
	}
	for (int i = 0; i < vert_blank.size()/2; ++i)
	{
		indices_blank[i] = i;
	}


	std::vector<float> vert_tool = Parse_vertices_tool();
	float vertices_tool[10000];
	int indices_tool[10000];
	for (int i = 0; i < vert_tool.size(); ++i)
	{
		vertices_tool[i] = vert_tool[i];
	}
	for (int i = 0; i < vert_tool.size() / 2; ++i)
	{
		indices_tool[i] = i;
	}



	//Parse_vertices();
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	
	// Generates Shader object using shaders default.vert and default.frag
	Shader blank_shaderProgram("default.vert", "default.frag");
	// Generates Vertex Array Object and binds it
	VAO VAO_blank;
	VAO_blank.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO_blank(vertices_blank, sizeof(vertices_blank));
	// Generates Element Buffer Object and links it to indices
	EBO EBO_blank(indices_blank, sizeof(indices_blank));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO_blank.LinkAttrib(VBO_blank, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_blank.LinkAttrib(VBO_blank, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO_blank.Unbind();
	VBO_blank.Unbind();
	EBO_blank.Unbind();

	


	// Shader for light cube
	Shader tool_shaderProgram("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO VAO_tool;
	VAO_tool.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO_tool(vertices_tool, sizeof(vertices_tool));
	// Generates Element Buffer Object and links it to indices
	EBO EBO_tool(indices_tool, sizeof(indices_tool));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO_blank.LinkAttrib(VBO_tool, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_blank.LinkAttrib(VBO_tool, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO_tool.Unbind();
	VBO_tool.Unbind();
	EBO_tool.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(100.5f, 100.5f, 100.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	tool_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(tool_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(tool_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	blank_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(blank_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(blank_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);





	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);


	//GL_SHADER_TYPE()

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	int frame = 0;

	double prevTime = 0.0f;
	double crntTime = 0.0f;
	double timeDiff = 0.0f;
	unsigned int counter = 0;


	glfwSwapInterval(1);



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{



		/*
		for (int i = 0; i < vert.size(); i += 6)
		{
			vert[i] += 0.001;
			vert[i+1] += 0.001;
			vert[i+2] += 0.001;
			
		}

		for (int i = 0; i < vert.size(); ++i)
		{
			vertices0[i] = vert[i];
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO1.ID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_DYNAMIC_DRAW);
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

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 10000.0f);


		// Tells OpenGL which Shader Program we want to use
		blank_shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(blank_shaderProgram, "camMatrix");


	/*
		if (frame % 2 == 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_blank.ID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_blank), vertices_blank, GL_STATIC_DRAW);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO1.ID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		}*/
		


		VAO_blank.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices_blank) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Tells OpenGL which Shader Program we want to use
		tool_shaderProgram.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(tool_shaderProgram, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		VAO_tool.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices_tool) / sizeof(int), GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

		//frame += 1;
	}



	// Delete all the objects we've created
	VAO_blank.Delete();
	VBO_blank.Delete();
	EBO_blank.Delete();
	blank_shaderProgram.Delete();
	VAO_tool.Delete();
	VBO_tool.Delete();
	EBO_tool.Delete();
	tool_shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}