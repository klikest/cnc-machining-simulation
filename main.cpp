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



// Vertices coordinates
GLfloat vertices[] =
{ // COORDINATES / NORMALS //
-0.5f, 0.0f, 0.5f,     0.0f, -1.0f, 0.0f, // Bottom side
-0.5f, 0.0f, -0.5f,    0.0f, -1.0f, 0.0f, // Bottom side
0.5f, 0.0f, -0.5f,     0.0f, -1.0f, 0.0f, // Bottom side
0.5f, 0.0f, 0.5f,      0.0f, -1.0f, 0.0f, // Bottom side

-0.5f, 0.0f, 0.5f,    -0.8f, 0.5f, 0.0f, // Left Side
-0.5f, 0.0f, -0.5f,   -0.8f, 0.5f, 0.0f, // Left Side
0.0f, 0.8f, 0.0f,     -0.8f, 0.5f, 0.0f, // Left Side

-0.5f, 0.0f, -0.5f,    0.0f, 0.5f, -0.8f, // Non-facing side
0.5f, 0.0f, -0.5f,     0.0f, 0.5f, -0.8f, // Non-facing side
0.0f, 0.8f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

0.5f, 0.0f, -0.5f,     0.8f, 0.5f, 0.0f, // Right side
0.5f, 0.0f, 0.5f,      0.8f, 0.5f, 0.0f, // Right side
0.0f, 0.8f, 0.0f,      0.8f, 0.5f, 0.0f, // Right side

0.5f, 0.0f, 0.5f,      0.0f, 0.5f, 0.8f, // Facing side
-0.5f, 0.0f, 0.5f,     0.0f, 0.5f, 0.8f, // Facing side
0.0f, 0.8f, 0.0f,      0.0f, 0.5f, 0.8f // Facing side
};




GLfloat vertices2[] =
{ // COORDINATES     /       NORMALS        //
-0.5f, 0.0f, 0.2f,       0.0f, -1.0f, 0.0f, // Bottom side
-0.5f, 0.0f, -0.5f,      0.0f, -1.0f, 0.0f, // Bottom side
 0.5f, 0.0f, -0.5f,      0.0f, -1.0f, 0.0f, // Bottom side
 0.5f, 0.0f, 0.2f,       0.0f, -1.0f, 0.0f, // Bottom side

-0.5f, 0.0f, 0.2f,      -0.8f, 0.5f, 0.0f, // Left Side
-0.5f, 0.0f, -0.5f,     -0.8f, 0.5f, 0.0f, // Left Side
 0.0f, 0.5f, 0.0f,      -0.8f, 0.5f, 0.0f, // Left Side

-0.5f, 0.0f, -0.5f,      0.0f, 0.5f, -0.8f, // Non-facing side
 0.5f, 0.0f, -0.5f,      0.0f, 0.5f, -0.8f, // Non-facing side
 0.0f, 0.5f, 0.0f,       0.0f, 0.5f, -0.8f, // Non-facing side

 0.5f, 0.0f, -0.5f,      0.8f, 0.5f, 0.0f, // Right side
 0.5f, 0.0f, 0.2f,       0.8f, 0.5f, 0.0f, // Right side
 0.0f, 0.5f, 0.0f,       0.8f, 0.5f, 0.0f, // Right side

 0.5f, 0.0f, 0.2f,       0.0f, 0.5f, 0.8f, // Facing side
-0.5f, 0.0f, 0.2f,       0.0f, 0.5f, 0.8f, // Facing side
 0.0f, 0.5f, 0.0f,       0.0f, 0.5f, 0.8f // Facing side
};


// Indices for vertices order
GLuint indices[] =
{
0, 1, 2, // Bottom side
0, 2, 3, // Bottom side
4, 6, 5, // Left side
7, 9, 8, // Non-facing side
10, 12, 11, // Right side
13, 15, 14 // Facing side
};

GLfloat lightVertices[] =
{ // COORDINATES //
-0.1f, -0.1f, 0.1f,
-0.1f, -0.1f, -0.1f,
0.1f, -0.1f, -0.1f,
0.1f, -0.1f, 0.1f,
-0.1f, 0.1f, 0.1f,
-0.1f, 0.1f, -0.1f,
0.1f, 0.1f, -0.1f,
0.1f, 0.1f, 0.1f
};

GLuint lightIndices[] =
{
0, 1, 2,
0, 2, 3,
0, 4, 7,
0, 7, 3,
3, 7, 6,
3, 6, 2,
2, 6, 5,
2, 5, 1,
1, 5, 4,
1, 4, 0,
4, 5, 6,
4, 6, 7
};


int main()
{


	char** lines = Parse_vertices();
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


	/*
	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices2, sizeof(vertices2));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();*/

	Shader shaderProgram("default.vert", "default.frag");

	GLuint VAO_1, VBO_1, EBO_1;

	// Generate the VAO, VBO, and EBO with only 1 object each
	glGenVertexArrays(1, &VAO_1);
	glGenBuffers(1, &VBO_1);
	glGenBuffers(1, &EBO_1);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO_1);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_1);
	// Introduce the indices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Bind the EBO to 0 so that we don't accidentally modify it
	// MAKE SURE TO UNBIND IT AFTER UNBINDING THE VAO, as the EBO is linked in the VAO
	// This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);













	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 1.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);






	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	int frame = 0;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		// Tells OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		// Binds texture so that is appears in rendering
		//brickTex.Bind();
		// Bind the VAO so OpenGL knows to use it

		
		
		if (frame % 2 == 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		}

		
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO_1);
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

		frame += 1;
	}



	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &VBO_1);
	glDeleteBuffers(1, &EBO_1);
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}