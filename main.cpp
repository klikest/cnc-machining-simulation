#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "mcut/mcut.h"


#include <Eigen/Core>
#include <igl/barycentric_coordinates.h>
#include <igl/barycentric_interpolation.h>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOBJ.h>

#include <stdio.h> 
#include <time.h> 


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
	std::vector<std::vector<double>> V, TC, N;
	std::vector<std::vector<int>> F, FTC, FN;
	std::vector<std::tuple<std::string, unsigned, unsigned>> FM;

	// variables for mesh data in a format suited for MCUT
	std::string fpath; // path to mesh file
	std::vector<uint32_t> faceSizesArray; // vertices per face
	std::vector<uint32_t> faceIndicesArray; // face indices
	std::vector<double> vertexCoordsArray; // vertex coords
};





class Mesh {
	float x;
	float y;
	float z;

	std::vector<float> vertices_and_normals;
	std::vector<float> vertices;
	//std::vector<float> normals;
	std::vector<int> indices;
	std::vector<uint32_t> face_sizes;
	std::vector<uint32_t> indices_u;
	std::vector<int> draw_ind;

public:
	Mesh()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	void set_vertices(std::vector<float> vetrtices_)
	{
		vertices.clear();
		vertices = vetrtices_;
	}

	void set_indices_u(std::vector<uint32_t> indices_)
	{
		indices.clear();
		indices_u.clear();
		indices_u = indices_;
		for (int i = 0; i < indices_u.size(); ++i)
		{
			indices.push_back(i);
		}	
	}

	void set_indices(std::vector<int> indices_)
	{
		indices.clear();
		indices_u.clear();
		indices = indices_;
		for (int i = 0; i < indices_u.size(); ++i)
		{
			indices_u.push_back(i);
		}
	}


	std::vector<float> get_vertices()
	{
		return vertices;
	}

	std::vector<int> get_indices()
	{

		return indices;
	}

	std::vector<uint32_t> get_indices_u()
	{
		return indices_u;
	}


	std::vector<float> calc_norm(std::vector<uint32_t> indices, std::vector<float> vertices)
	{
		std::vector<float> ind_and_vert;
		std::vector<std::vector<float>> vert_by_num;
		std::vector<float> P1, P2, N;
		int V1, V2, V3;
		float x1, y1, z1, x2, y2, z2, x3, y3, z3;
		for (int i = 0; i < vertices.size(); i += 3)
		{
			vert_by_num.push_back({ vertices[i], vertices[i + 1], vertices[i + 2] });
		}
		for (int i = 0; i < indices.size(); i += 3)
		{
			//
			V1 = indices[i];
			V2 = indices[i + 1];
			V3 = indices[i + 2];
			x1 = vert_by_num[V1][0];
			y1 = vert_by_num[V1][1];
			z1 = vert_by_num[V1][2];
			x2 = vert_by_num[V2][0];
			y2 = vert_by_num[V2][1];
			z2 = vert_by_num[V2][2];
			x3 = vert_by_num[V3][0];
			y3 = vert_by_num[V3][1];
			z3 = vert_by_num[V3][2];
			P1.push_back(x1 - x2);
			P1.push_back(y1 - y2);
			P1.push_back(z1 - z2);
			P2.push_back(x3 - x2);
			P2.push_back(y3 - y2);
			P2.push_back(z3 - z2);
			N.push_back(P1[2] * P2[1] - P1[1] * P2[2]);
			N.push_back(P1[0] * P2[2] - P1[2] * P2[0]);
			N.push_back(P1[1] * P2[0] - P1[0] * P2[1]);
			ind_and_vert.push_back(x1);
			ind_and_vert.push_back(y1);
			ind_and_vert.push_back(z1);
			ind_and_vert.push_back(N[0]);
			ind_and_vert.push_back(N[1]);
			ind_and_vert.push_back(N[2]);
			ind_and_vert.push_back(x2);
			ind_and_vert.push_back(y2);
			ind_and_vert.push_back(z2);
			ind_and_vert.push_back(N[0]);
			ind_and_vert.push_back(N[1]);
			ind_and_vert.push_back(N[2]);
			ind_and_vert.push_back(x3);
			ind_and_vert.push_back(y3);
			ind_and_vert.push_back(z3);
			ind_and_vert.push_back(N[0]);
			ind_and_vert.push_back(N[1]);
			ind_and_vert.push_back(N[2]);

			P1.clear();
			P2.clear();
			N.clear();
		}

		return ind_and_vert;
	}
	
	std::vector<float> get_draw_vert()
	{
		return calc_norm(get_indices_u(), get_vertices());
	}
		
	std::vector<int> get_draw_ind()
	{
		draw_ind.clear();

		for (int i = 0; i < vertices.size() / 6; ++i)
		{
			draw_ind.push_back(i);
		}
		return draw_ind;
	}



	void move_x_vert(float dx, VBO VBO)
	{
		x += dx;
		for (int i = 0; i < vertices.size(); i += 6)
		{
			vertices[i] += dx;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	}

	void move_y_vert(float dy, VBO VBO)
	{
		y += dy;
		for (int i = 0; i < vertices.size(); i += 6)
		{
			vertices[i + 1] += dy;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	}

	void move_z_vert(float dz, VBO VBO)
	{
		z += dz;
		for (int i = 0; i < vertices.size(); i += 6)
		{
			vertices[i + 2] += dz;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

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

void draw_model(std::vector<float> vertices, std::vector<int> indices, VBO VBO_, VAO VAO_, EBO EBO_)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_.ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(VAO_.ID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}








int main()
{

	// load meshes.
	// -----------------
	InputMesh srcMesh;

	std::vector<float> blank_vert;
	std::vector<uint32_t> blank_ind;
	srcMesh.fpath = "Models\\cube.obj";
	bool srcMeshLoaded = igl::read_triangle_mesh(srcMesh.fpath, srcMesh.V, srcMesh.F);

	if (!srcMeshLoaded) {
		std::fprintf(stderr, "error: could not load source mesh --> %s\n", srcMesh.fpath.c_str());
		std::exit(1);
	}

	// copy vertices
	for (int i = 0; i < (int)srcMesh.V.size(); ++i) {
		const std::vector<double>& v = srcMesh.V[i];
		my_assert(v.size() == 3);
		srcMesh.vertexCoordsArray.push_back(v[0]);
		srcMesh.vertexCoordsArray.push_back(v[1]);
		srcMesh.vertexCoordsArray.push_back(v[2]);

		blank_vert.push_back(v[0]);
		blank_vert.push_back(v[1]);
		blank_vert.push_back(v[2]);
	}

	// copy faces
	for (int i = 0; i < (int)srcMesh.F.size(); ++i) {
		const std::vector<int>& f = srcMesh.F[i];
		for (int j = 0; j < (int)f.size(); ++j) {
			srcMesh.faceIndicesArray.push_back(f[j]);
			blank_ind.push_back(f[j]);
		}

		srcMesh.faceSizesArray.push_back((uint32_t)f.size());
	}

	Mesh blank;
	blank.set_vertices(blank_vert);
	blank.set_indices_u(blank_ind);



	std::vector<float> tool_vert;
	std::vector<uint32_t> tool_ind;

	InputMesh cutMesh;
	cutMesh.fpath = "Models\\cyl.obj";
	bool cutMeshLoaded = igl::read_triangle_mesh(cutMesh.fpath, cutMesh.V, cutMesh.F);

	if (!cutMeshLoaded) {
		std::fprintf(stderr, "error: could not load source mesh --> %s\n", cutMesh.fpath.c_str());
		std::exit(1);
	}

	// copy vertices
	for (int i = 0; i < (int)cutMesh.V.size(); ++i) {
		const std::vector<double>& v = cutMesh.V[i];
		my_assert(v.size() == 3);
		cutMesh.vertexCoordsArray.push_back(v[0]);
		cutMesh.vertexCoordsArray.push_back(v[1]);
		cutMesh.vertexCoordsArray.push_back(v[2]);

		tool_vert.push_back(v[0]);
		tool_vert.push_back(v[1]);
		tool_vert.push_back(v[2]);
	}

	// copy faces
	for (int i = 0; i < (int)cutMesh.F.size(); ++i) {
		const std::vector<int>& f = cutMesh.F[i];
		for (int j = 0; j < (int)f.size(); ++j) {
			cutMesh.faceIndicesArray.push_back(f[j]);
			tool_ind.push_back(f[j]);
		}

		cutMesh.faceSizesArray.push_back((uint32_t)f.size());
	}


	Mesh tool;
	tool.set_vertices(tool_vert);
	tool.set_indices_u(tool_ind);

	McContext context = MC_NULL_HANDLE;
	McResult err = mcCreateContext(&context, MC_DEBUG);
	my_assert(err == MC_NO_ERROR);

	//  do the cutting (boolean ops)
	// -----------------------------


	err = mcDispatch(
		context,
		MC_DISPATCH_VERTEX_ARRAY_DOUBLE | // vertices are in array of doubles
		MC_DISPATCH_ENFORCE_GENERAL_POSITION | // perturb if necessary
		MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE, // filter flags which specify the type of output we want
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

	my_assert(err == MC_NO_ERROR);

	// query the number of available connected component
	// --------------------------------------------------
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

	my_assert(err == MC_NO_ERROR);

	// query the data of each connected component from MCUT
	// -------------------------------------------------------

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

	std::vector<float> vert_res;
	for (int i = 0; i < ccVertices.size(); ++i)
	{
		vert_res.push_back((float)ccVertices[i]);
	}


	// query the faces
	// -------------------
	numBytes = 0;


	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, 0, NULL, &numBytes);
	my_assert(err == MC_NO_ERROR);
	std::vector<uint32_t> ccFaceIndices(numBytes / sizeof(uint32_t), 0);
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, numBytes, ccFaceIndices.data(), NULL);
	my_assert(err == MC_NO_ERROR);

	Mesh res;
	res.set_vertices(vert_res);
	res.set_indices_u(ccFaceIndices);


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
	VBO VBO_blank(blank.get_draw_vert().data(), blank.get_draw_vert().size() * sizeof(float));
	EBO EBO_blank(blank.get_draw_ind().data(), blank.get_draw_ind().size() * sizeof(int));
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
	VAO_tool.LinkAttrib(VBO_tool, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_tool.LinkAttrib(VBO_tool, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_tool.Unbind();
	VBO_tool.Unbind();
	EBO_tool.Unbind();

	Shader res_shaderProgram("test.vert", "test.frag");
	VAO VAO_res;
	VAO_res.Bind();
	VBO VBO_res(res.get_vertices().data(), res.get_vertices().size() * sizeof(float));
	EBO EBO_res(res.get_indices().data(), res.get_indices().size() * sizeof(int));
	VAO_res.LinkAttrib(VBO_res, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_res.LinkAttrib(VBO_res, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_res.Unbind();
	VBO_res.Unbind();
	EBO_res.Unbind();


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(500.0f, 500.0f, 500.0f);

	glm::vec3 toolPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 toolModel = glm::mat4(1.0f);
	toolModel = glm::translate(toolModel, toolPos);

	glm::vec3 blankPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 blankModel = glm::mat4(1.0f);
	blankModel = glm::translate(blankModel, blankPos);

	glm::vec3 resPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 resModel = glm::mat4(1.0f);
	resModel = glm::translate(resModel, resPos);


	tool_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(tool_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(toolModel));
	glUniform4f(glGetUniformLocation(tool_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(tool_shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	blank_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(blank_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(blankModel));
	glUniform4f(glGetUniformLocation(blank_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	res_shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(res_shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(resModel));
	glUniform4f(glGetUniformLocation(res_shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(res_shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	int frame = 0;

	double prevTime = 0.0f;
	double crntTime = 0.0f;
	double timeDiff = 0.0f;
	unsigned int counter = 0;


	bool draw_cut_mesh = false;
	bool draw_src_mesh = false;
	bool draw_res = false;




	blank.set_vertices(blank.get_draw_vert());
	blank.set_indices(blank.get_draw_ind());

	tool.set_vertices(tool.get_draw_vert());
	tool.set_indices(tool.get_draw_ind());

	res.set_vertices(res.get_draw_vert());
	res.set_indices(res.get_draw_ind());




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

		glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(blank_shaderProgram, "camMatrix");


		glUniform3f(glGetUniformLocation(tool_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(tool_shaderProgram, "camMatrix");

		glUniform3f(glGetUniformLocation(res_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(res_shaderProgram, "camMatrix");
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (draw_cut_mesh)
		{
			//blank.move_x_vert(0.1, VBO_blank);
			blank_shaderProgram.Activate();
			VAO_blank.Bind();
			draw_model(blank.get_vertices(), blank.get_indices(), VBO_blank, VAO_blank, EBO_blank);
		}

		if (draw_src_mesh)
		{
			//tool.move_y_vert(0.1, VBO_tool);
			tool_shaderProgram.Activate();
			VAO_tool.Bind();
			draw_model(tool.get_vertices(), tool.get_indices(), VBO_tool, VAO_tool, EBO_tool);
		}
		
		if (draw_res)
		{
			res_shaderProgram.Activate();
			VAO_res.Bind();
			draw_model(res.get_vertices(), tool.get_indices(), VBO_res, VAO_res, EBO_res);
		}
		
		
		std::string x_cord = "X = " + std::to_string(blank.get_x());
		std::string y_cord = "Y = " + std::to_string(blank.get_y());
		std::string z_cord = "Z = " + std::to_string(blank.get_z());



		ImGui::Begin("Tool info");
		ImGui::Checkbox("Draw blank mesh", &draw_cut_mesh);
		ImGui::Checkbox("Draw tool mesh", &draw_src_mesh);
		ImGui::Checkbox("Draw result mesh", &draw_res);
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
	VAO_res.Delete();
	VBO_res.Delete();
	EBO_res.Delete();
	res_shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}