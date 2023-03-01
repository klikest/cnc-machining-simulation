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


#define my_assert(cond)                             \
    if (!(cond)) {                                  \
        fprintf(stderr, "MCUT error: %s\n", #cond); \
        std::exit(1);                               \
    }



class Mesh {
	float x;
	float y;
	float z;

	std::vector<float> vertices_and_normals;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<int> indices;
	std::vector<uint32_t> face_sizes;
	std::vector<uint32_t> indices_u;

public:
	Mesh(std::vector<float> vertices_input)
	{
		x = 0;
		y = 0;
		z = 0;


		vertices = vertices_input;

		for (int i = 0; i < vertices.size()/3; ++i)
		{
			indices.push_back(i);
			indices_u.push_back((uint32_t)i);
		}

		for (int i = 0; i < indices.size() / 3; ++i)
		{
			face_sizes.push_back((uint32_t)3);
		}

		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "Vertices count = " << get_vert_count() << std::endl;
		std::cout << "Indices count = " << get_ind_count() << std::endl;
		std::cout << "Face size count = " << get_face_sizes().size() << std::endl;
		std::cout << "----------------------------------------------" << std::endl;

	}

	void set_vertices(std::vector<float> vetrtices_)
	{
		vertices.clear();
		vertices = vetrtices_;
	}

	void set_indices(std::vector<uint32_t> indices_)
	{
		indices.clear();
		
		for (int i = 0; i < indices_.size(); ++i)
		{
			int j = (int) indices_[i];
			indices.push_back(j);
		}
	}


	std::vector<float> get_vertices()
	{
		return vertices;
	}

	std::uint32_t get_vert_count()
	{
		return (uint32_t)(vertices.size()/3);
	}

	std::vector<float> get_vertices_normals()
	{
		vertices_and_normals.clear();
		vertices_and_normals = calc_vert_n();
		return vertices_and_normals;
	}

	std::vector<float> calc_vert_n()
	{
		vertices_and_normals.clear();

		calc_normals();

		int count = 0;

		for (int i = 0; i < vertices.size(); i += 9)
		{
			vertices_and_normals.push_back(vertices[i]);
			vertices_and_normals.push_back(vertices[i+1]);
			vertices_and_normals.push_back(vertices[i+2]);

			vertices_and_normals.push_back(normals[count]);
			vertices_and_normals.push_back(normals[count +1]);
			vertices_and_normals.push_back(normals[count +2]);


			vertices_and_normals.push_back(vertices[i + 3]);
			vertices_and_normals.push_back(vertices[i + 4]);
			vertices_and_normals.push_back(vertices[i + 5]);

			vertices_and_normals.push_back(normals[count]);
			vertices_and_normals.push_back(normals[count + 1]);
			vertices_and_normals.push_back(normals[count + 2]);


			vertices_and_normals.push_back(vertices[i + 6]);
			vertices_and_normals.push_back(vertices[i + 7]);
			vertices_and_normals.push_back(vertices[i + 8]);

			vertices_and_normals.push_back(normals[count]);
			vertices_and_normals.push_back(normals[count + 1]);
			vertices_and_normals.push_back(normals[count + 2]);

			count += 3;
		}
		return vertices_and_normals;
	}


	std::vector<int> get_indices()
	{

		return indices;
	}

	std::vector<uint32_t> get_indices_u()
	{
		return indices_u;
	}

	std::uint32_t get_ind_count()
	{
		return (uint32_t)(indices.size() / 3);
	}

	std::vector<uint32_t> get_face_sizes()
	{
		return face_sizes;
	}


	std::vector<float> calc_normals()
	{
		normals.clear();

		float x1, y1, z1, x2, y2, z2, x3, y3, z3;

		std::vector<float> P1, P2, N;

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

				P1.push_back(x1 - x2);
				P1.push_back(y1 - y2);
				P1.push_back(z1 - z2);

				P2.push_back(x3 - x2);
				P2.push_back(y3 - y2);
				P2.push_back(z3 - z2);

				N.push_back(P1[2] * P2[1] - P1[1] * P2[2]);
				N.push_back(P1[0] * P2[2] - P1[2] * P2[0]);
				N.push_back(P1[1] * P2[0] - P1[0] * P2[1]);

				normals.push_back(N[0]);
				normals.push_back(N[1]);
				normals.push_back(N[2]);

				P1.clear();
				P2.clear();
				N.clear();

		}

		return normals;
	}


	void move_x_vert(float dx, VBO VBO)
	{
		x += dx;
		for (int i = 0; i < vertices_and_normals.size(); i += 6)
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
			vertices_and_normals[i + 1] += dy;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices_and_normals.size() * sizeof(float), vertices_and_normals.data(), GL_DYNAMIC_DRAW);

	}

	void move_z_vert(float dz, VBO VBO)
	{
		z += dz;
		for (int i = 0; i < vertices_and_normals.size(); i += 6)
		{
			vertices_and_normals[i + 2] += dz;
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


	std::vector<float> cubeVertices = {
	-5, -5, 5,  // 0
	5, -5, 5,   // 1
	5, 5, 5,    //2
	-5, 5, 5,   //3
	-5, -5, -5, //4
	5, -5, -5,  //5
	5, 5, -5,   //6
	-5, 5, -5   //7
	};
	std::vector<uint32_t> cubeFaces = {
		0, 1, 2, 3, //0
		7, 6, 5, 4, //1
		1, 5, 6, 2, //2
		0, 3, 7, 4, //3
		3, 2, 6, 7, //4
		4, 5, 1, 0  //5
	};
	uint32_t cubeFaceSizes[] = {
		4, 4, 4, 4, 4, 4 };
	uint32_t numCubeVertices = 8;
	uint32_t numCubeFaces = 6;

	// the cut mesh
	// ---------
	float cutMeshVertices[] = {
		-20, -4, 0, //0
		0, 20, 20,  //1
		20, -4, 0,  //2
		0, 20, -20  //3
	};
	uint32_t cutMeshFaces[] = {
		0, 1, 2, //0
		0, 2, 3  //1
	};
	//uint32_t cutMeshFaceSizes[] = {
	//    3, 3};
	uint32_t numCutMeshVertices = 4;
	uint32_t numCutMeshFaces = 2;

	// 2. create a context
	// -------------------
	McContext context = MC_NULL_HANDLE;
	McResult err = mcCreateContext(&context, MC_NULL_HANDLE);

	if (err != MC_NO_ERROR)
	{
		fprintf(stderr, "could not create context (err=%d)\n", (int)err);
		exit(1);
	}

	// 3. do the magic!
	// ----------------
	err = mcDispatch(
		context,
		MC_DISPATCH_VERTEX_ARRAY_FLOAT,
		cubeVertices.data(),
		cubeFaces.data(),
		cubeFaceSizes,
		numCubeVertices,
		numCubeFaces,
		cutMeshVertices,
		cutMeshFaces,
		nullptr, // cutMeshFaceSizes, // no need to give 'faceSizes' parameter since cut-mesh is a triangle mesh
		numCutMeshVertices,
		numCutMeshFaces);

	if (err != MC_NO_ERROR)
	{
		fprintf(stderr, "dispatch call failed (err=%d)\n", (int)err);
		exit(1);
	}

	// 4. query the number of available connected component (all types)
	// -------------------------------------------------------------

	uint32_t numConnComps;
	std::vector<McConnectedComponent> connComps;
	mcGetConnectedComponents(context, MC_CONNECTED_COMPONENT_TYPE_ALL, 0, NULL, &numConnComps);
	connComps.resize(numConnComps);
	mcGetConnectedComponents(context, MC_CONNECTED_COMPONENT_TYPE_ALL, (uint32_t)connComps.size(), connComps.data(), NULL);

	std::cout << numConnComps << std::endl;

	McConnectedComponent connComp = connComps[2];
	uint64_t numBytes = 0;


	mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_FLOAT, 0, NULL, &numBytes);
	std::vector<float> vertices_m;
	vertices_m.resize(numBytes / sizeof(float)); //... or --> numberOfVertices * 3
	mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_FLOAT, numBytes, (void*)vertices_m.data(), NULL);

	mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE, 0, NULL, &numBytes);
	std::vector<uint32_t> faceIndices;
	faceIndices.resize(numBytes / sizeof(uint32_t));
	mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE, numBytes, faceIndices.data(), NULL);



	mcReleaseConnectedComponents(context, 0, NULL);
	mcReleaseContext(context);

	//blank.set_vertices(vertices_m);
	//blank.set_indices(faceIndices);
	//blank.calc_vert_n();







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
	glm::vec3 toolPos = glm::vec3(0.0f, 0.0f, 0.0f);
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
		}*/
		

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

		//glBindBuffer(GL_ARRAY_BUFFER, VBO_blank.ID);
		//glBufferData(GL_ARRAY_BUFFER, tool_vert.size() * sizeof(float), tool_vert.data(), GL_DYNAMIC_DRAW);
		
		//glBindVertexArray(VAO_blank.ID);
		// Draw primitives, number of indices, datatype of indices, index of indices
		//glDrawElements(GL_TRIANGLES, blank.get_indices().size(), GL_UNSIGNED_INT, 0);


		
		blank_shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(blank_shaderProgram, "camMatrix");

		VAO_blank.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, blank.get_indices().size(), GL_UNSIGNED_INT, 0);
		tool_shaderProgram.Activate();
		camera.Matrix(tool_shaderProgram, "camMatrix");

		//VAO_tool.Bind();
		//glDrawElements(GL_TRIANGLES, tool.get_indices().size(), GL_UNSIGNED_INT, 0);
		
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