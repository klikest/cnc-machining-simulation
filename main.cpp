#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
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


#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS 1

#ifdef _WIN32
#pragma warning(disable : 26812) // Unscoped enums from mcut.h
#endif // _WIN32
#endif

#define my_assert(cond)                             \
    if (!(cond)) {                                  \
        fprintf(stderr, "MCUT error: %s\n", #cond); \
        std::exit(1);                               \
    }



struct InputMesh {
	// variables for reading .obj file data with libigl
	std::vector<std::vector<double>> V;
	std::vector<std::vector<int>> F;

	// variables for mesh data in a format suited for MCUT
	std::string fpath; // path to mesh file
	std::vector<uint32_t> faceSizesArray; // vertices per face
	std::vector<uint32_t> faceIndicesArray; // face indices
	std::vector<double> vertexCoordsArray; // vertex coords
};

const unsigned int width = 800;
const unsigned int height = 800;

const	std::string tool = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\tool.obj";
const	std::string blank = "C:\\Users\\User\\source\\repos\\cnc-machining-simulation\\Models\\blank.obj";


int main()
{

	float vertices_blank[10000];
	int indices_blank[10000];
	float vertices_tool[10000];
	int indices_tool[10000];

	std::vector<float> vert_blank = Parse_vertices_blank();
	for (int i = 0; i < vert_blank.size(); ++i)
	{
		vertices_blank[i] = vert_blank[i];
	}
	for (int i = 0; i < vert_blank.size()/2; ++i)
	{
		indices_blank[i] = i;
	}


	std::vector<float> vert_tool = Parse_vertices_tool();
	for (int i = 0; i < vert_tool.size(); ++i)
	{
		vertices_tool[i] = vert_tool[i];
	}
	for (int i = 0; i < vert_tool.size() / 2; ++i)
	{
		indices_tool[i] = i;
	}







	/*


	McContext context = MC_NULL_HANDLE;
	McResult err = mcCreateContext(&context, MC_DEBUG);
	my_assert(err == MC_NO_ERROR);


	const std::map<std::string, McFlags> booleanOps = {
	{ "A_NOT_B", MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE },
	{ "B_NOT_A", MC_DISPATCH_FILTER_FRAGMENT_SEALING_OUTSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW },
	{ "UNION", MC_DISPATCH_FILTER_FRAGMENT_SEALING_OUTSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE },
	{ "INTERSECTION", MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW }
	};


	const auto boolOpFlags = MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW;

	err = mcDispatch(
		context,
		MC_DISPATCH_VERTEX_ARRAY_DOUBLE | // vertices are in array of doubles
			MC_DISPATCH_ENFORCE_GENERAL_POSITION | // perturb if necessary
			boolOpFlags, // filter flags which specify the type of output we want
		// source mesh
		reinterpret_cast<const void*>(srcMesh.vertexCoordsArray.data()),
		reinterpret_cast<const uint32_t*>(srcMesh.faceIndicesArray.data()),
		srcMesh.faceSizesArray.data(),
		static_cast<uint32_t>(srcMesh.vertexCoordsArray.size() / 3),
		static_cast<uint32_t>(srcMesh.faceSizesArray.size()),
		// cut mesh
		reinterpret_cast<const void*>(cutMesh.vertexCoordsArray.data()),
		cutMesh.faceIndicesArray.data(),
		cutMesh.faceSizesArray.data(),
		static_cast<uint32_t>(cutMesh.vertexCoordsArray.size() / 3),
		static_cast<uint32_t>(cutMesh.faceSizesArray.size()));


	uint32_t numConnComps;
	err = mcGetConnectedComponents(context, MC_CONNECTED_COMPONENT_TYPE_FRAGMENT, 0, NULL, &numConnComps);
	my_assert(err == MC_NO_ERROR);

	printf("connected components: %d\n", (int)numConnComps);

	if (numConnComps == 0) {
		fprintf(stdout, "no connected components found\n");
		exit(0);
	}

	std::vector<McConnectedComponent> connectedComponents(numConnComps, MC_NULL_HANDLE);
	connectedComponents.resize(numConnComps);
	err = mcGetConnectedComponents(context, MC_CONNECTED_COMPONENT_TYPE_FRAGMENT, (uint32_t)connectedComponents.size(), connectedComponents.data(), NULL);



	McConnectedComponent connComp = connectedComponents[0];

	// query the vertices
	// ----------------------

	uint64_t numBytes = 0;
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_DOUBLE, 0, NULL, &numBytes);
	my_assert(err == MC_NO_ERROR);
	uint32_t ccVertexCount = (uint32_t)(numBytes / (sizeof(double) * 3));
	std::vector<double> ccVertices((uint64_t)ccVertexCount * 3u, 0);
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_DOUBLE, numBytes, (void*)ccVertices.data(), NULL);
	my_assert(err == MC_NO_ERROR);

	// query the faces
	// -------------------
	numBytes = 0;


	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, 0, NULL, &numBytes);
	my_assert(err == MC_NO_ERROR);
	std::vector<uint32_t> ccFaceIndices(numBytes / sizeof(uint32_t), 0);
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, numBytes, ccFaceIndices.data(), NULL);
	my_assert(err == MC_NO_ERROR);

	std::vector<uint32_t> faceSizes(ccFaceIndices.size() / 3, 3);
	*/



	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
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
	VBO VBO_blank(vertices_blank, sizeof(vertices_blank));
	EBO EBO_blank(indices_blank, sizeof(indices_blank));
	VAO_blank.LinkAttrib(VBO_blank, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_blank.LinkAttrib(VBO_blank, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_blank.Unbind();
	VBO_blank.Unbind();
	EBO_blank.Unbind();
	

	Shader tool_shaderProgram("light.vert", "light.frag");
	VAO VAO_tool;
	VAO_tool.Bind();
	VBO VBO_tool(vertices_tool, sizeof(vertices_tool));
	EBO EBO_tool(indices_tool, sizeof(indices_tool));
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
		tool_shaderProgram.Activate();
		camera.Matrix(tool_shaderProgram, "camMatrix");
		VAO_tool.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices_tool) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		ImGui::Begin("Header");
		ImGui::Text("Text");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		//frame += 1;
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