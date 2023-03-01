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
	Mesh(std::vector<float> vertices_input, std::vector<int> indices_input)
	{
		x = 0;
		y = 0;
		z = 0;


		vertices = vertices_input;
		indices = indices_input;



		for (int i = 0; i < indices.size(); i++)
		{
			indices_u.push_back((uint32_t)indices[i]);
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





void draw_model_u(std::vector<float> vertices, std::vector<uint32_t> indices, VBO VBO_, EBO EBO_)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_.ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_.ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.ID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void draw_model(std::vector<float> vertices, std::vector<int> indices, VBO VBO_, EBO EBO_)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_.ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_.ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.ID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}




int main()
{

	Mesh blank(Parse_vertices_blank(), Parse_indices_blank());
	Mesh tool(Parse_vertices_tool(), Parse_indices_tool());

	std::cout << "Vertices:" << std::endl;
	for (int i = 0; i < blank.get_vertices().size(); i += 3)
	{
		std::cout << blank.get_vertices()[i] << '\t' << blank.get_vertices()[i + 1] << '\t' << blank.get_vertices()[i + 2] << std::endl;
	}
	std::cout << "---------------------------------------" << std::endl;

	std::cout << "Indices:" << std::endl;
	for (int i = 0; i < blank.get_indices().size(); i += 3)
	{
		std::cout << blank.get_indices()[i] << '\t' << blank.get_indices()[i + 1] << '\t' << blank.get_indices()[i + 2] << std::endl;
	}
	std::cout << "---------------------------------------" << std::endl;


	std::cout << "Face sizes:" << std::endl;
	for (int i = 0; i < blank.get_face_sizes().size(); i++)
	{
		std::cout << blank.get_face_sizes()[i] << std::endl;
	}
	std::cout << "---------------------------------------" << std::endl;

	

	



	/*
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
	std::vector<uint32_t> cubeFaceSizes = {
		4, 4, 4, 4, 4, 4 };
	uint32_t numCubeVertices = 8;
	uint32_t numCubeFaces = 6;

	// the cut mesh
	// ---------
	std::vector<float> cutMeshVertices = {
		0,    -40,   -10, //0
		0,    0, 200,  //1
		0,    40, -10 //2
	};
	std::vector<uint32_t> cutMeshFaces = {
		0, 1, 2 //1
	};
	//uint32_t cutMeshFaceSizes[] = {
	//    3, 3};
	uint32_t numCutMeshVertices = 3;
	uint32_t numCutMeshFaces = 1;

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
		cubeFaceSizes.data(),
		cubeVertices.size()/3,
		cubeFaces.size()/4,
		cutMeshVertices.data(),
		cutMeshFaces.data(),
		nullptr, // cutMeshFaceSizes, // no need to give 'faceSizes' parameter since cut-mesh is a triangle mesh
		cutMeshVertices.size()/3,
		cutMeshFaces.size()/3 );
*/



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
	std::vector<uint32_t> cubeFaceSizes = {
		4, 4, 4, 4, 4, 4 };
	uint32_t numCubeVertices = 8;
	uint32_t numCubeFaces = 6;

	// the cut mesh
	// ---------
	std::vector<float> cutMeshVertices = {
		0,    -400,   -100, //0
		0,    0, 200,  //1
		0,    400, -100 //2
	};
	std::vector<uint32_t> cutMeshFaces = {
		0, 1, 2 //1
	};
	//uint32_t cutMeshFaceSizes[] = {
	//    3, 3};
	uint32_t numCutMeshVertices = 3;
	uint32_t numCutMeshFaces = 1;

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
		MC_DISPATCH_VERTEX_ARRAY_FLOAT | // vertices are in array of doubles
		MC_DISPATCH_ENFORCE_GENERAL_POSITION,
		reinterpret_cast<const void*>(tool.get_vertices().data()),
		reinterpret_cast<const uint32_t*>(tool.get_indices_u().data()),
		tool.get_face_sizes().data(),
		static_cast<uint32_t>(tool.get_vertices().size() / 3),
		static_cast<uint32_t>(tool.get_face_sizes().size()),
		cutMeshVertices.data(),
		cutMeshFaces.data(),
		nullptr, // cutMeshFaceSizes, // no need to give 'faceSizes' parameter since cut-mesh is a triangle mesh
		cutMeshVertices.size() / 3,
		cutMeshFaces.size() / 3);







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

	McConnectedComponent connComp = connComps[0];
	uint64_t numBytes = 0;
	// query the vertices
	// ----------------------

	numBytes = 0;
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_FLOAT, 0, NULL, &numBytes);

	if (err != MC_NO_ERROR)
	{
		fprintf(stderr, "1:mcGetConnectedComponentData(MC_CONNECTED_COMPONENT_DATA_VERTEX_FLOAT) failed (err=%d)\n", (int)err);
		exit(1);
	}

	uint32_t numberOfVertices = (uint32_t)(numBytes / (sizeof(float) * 3));

	std::vector<float> vertices(numberOfVertices * 3u);

	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_FLOAT, numBytes, (void*)vertices.data(), NULL);

	if (err != MC_NO_ERROR)
	{
		fprintf(stderr, "2:mcGetConnectedComponentData(MC_CONNECTED_COMPONENT_DATA_VERTEX_FLOAT) failed (err=%d)\n", (int)err);
		exit(1);
	}

	// query the faces
	// -------------------

	numBytes = 0;
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, 0, NULL, &numBytes);

	if (err != MC_NO_ERROR)
	{
		fprintf(stderr, "1:mcGetConnectedComponentData(MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION) failed (err=%d)\n", (int)err);
		exit(1);
	}

	std::vector<uint32_t> triangulationIndices;
	triangulationIndices.resize(numBytes / sizeof(uint32_t));

	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, numBytes, triangulationIndices.data(), NULL);

	if (err != MC_NO_ERROR)
	{
		fprintf(stderr, "2:mcGetConnectedComponentData(MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION) failed (err=%d)\n", (int)err);
		exit(1);
	}

	std::vector<uint32_t> faceSizes(triangulationIndices.size() / 3, 3);



	mcReleaseConnectedComponents(context, 0, NULL);
	mcReleaseContext(context);


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
	
	Shader tool_shaderProgram("tool.vert", "tool.frag");
	VAO VAO_tool;
	VAO_tool.Bind();
	VBO VBO_tool(tool.get_vertices().data(), tool.get_vertices().size() * sizeof(float));
	EBO EBO_tool(tool.get_indices().data(), tool.get_indices().size() * sizeof(int));
	VAO_blank.LinkAttrib(VBO_tool, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	//VAO_blank.LinkAttrib(VBO_tool, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	int frame = 0;

	double prevTime = 0.0f;
	double crntTime = 0.0f;
	double timeDiff = 0.0f;
	unsigned int counter = 0;


	bool draw_cut_mesh = false;
	bool draw_src_mesh = false;
	bool draw_result = false;


	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window))
	{


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
		//glDrawElements(GL_TRIANGLES, blank.get_indices().size(), GL_UNSIGNED_INT, 0);



		tool_shaderProgram.Activate();
		VAO_tool.Bind();
		//glDrawElements(GL_TRIANGLES, tool.get_indices().size(), GL_UNSIGNED_INT, 0);

		camera.Matrix(tool_shaderProgram, "camMatrix");

		
		std::string x_cord = "X = " + std::to_string(blank.get_x());
		std::string y_cord = "Y = " + std::to_string(blank.get_y());
		std::string z_cord = "Z = " + std::to_string(blank.get_z());



		if (draw_cut_mesh)
		{
			draw_model_u(cutMeshVertices, cutMeshFaces, VBO_tool, EBO_tool);
		}

		else if (draw_src_mesh)
		{
			draw_model_u(blank.get_vertices(), blank.get_indices_u(), VBO_tool, EBO_tool);
		}
		else if (draw_result)
		{
			draw_model_u(vertices, triangulationIndices, VBO_tool, EBO_tool);
		}



		ImGui::Begin("Tool info");
		ImGui::Checkbox("Draw cut mesh", &draw_cut_mesh);
		ImGui::Checkbox("Draw src mesh", &draw_src_mesh);
		ImGui::Checkbox("Draw result mesh", &draw_result);
		//ImGui::Text(x_cord.c_str());
		//ImGui::Text(y_cord.c_str());
		//ImGui::Text(z_cord.c_str());
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