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

		//std::cout << "----------------------------------------------" << std::endl;
		//std::cout << "Vertices count = " << get_vert_count() << std::endl;
		//std::cout << "Indices count = " << get_ind_count() << std::endl;
		//std::cout << "Face size count = " << get_face_sizes().size() << std::endl;
		//std::cout << "----------------------------------------------" << std::endl;

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
		//normals.clear();

		std::vector<float> normals;

		float x1, y1, z1, x2, y2, z2, x3, y3, z3;

		std::vector<float> P1, P2, N;

		int V1, V2, V3;

		std::vector<std::vector<float>> vert;

		

		std::vector<float> coords;
		
		std::vector<uint32_t> ind;


		for (int i = 0; i < vertices.size(); i += 3)
		{
			coords.push_back(vertices[i]);
			coords.push_back(vertices[i+1]);
			coords.push_back(vertices[i+2]);
			vert.push_back(coords);
			coords.clear();
		}



		for (int i = 0; i < indices.size(); i += 3)
		{
			V1 = indices[i];
			V2 = indices[i + 1];
			V3 = indices[i + 2];
			x1 = vert[V1][0];
			y1 = vert[V1][1];
			z1 = vert[V1][2];
			x2 = vert[V2][0];
			y2 = vert[V2][1];
			z2 = vert[V2][2];
			x3 = vert[V3][0];
			y3 = vert[V3][1];
			z3 = vert[V3][2];

			P1.push_back(x1 - x2);
			P1.push_back(y1 - y2);
			P1.push_back(z1 - z2);

			P2.push_back(x3 - x2);
			P2.push_back(y3 - y2);
			P2.push_back(z3 - z2);

			N.push_back(P1[2] * P2[1] - P1[1] * P2[2]);
			N.push_back(P1[0] * P2[2] - P1[2] * P2[0]);
			N.push_back(P1[1] * P2[0] - P1[0] * P2[1]);

			normals.push_back(x1);
			normals.push_back(y1);
			normals.push_back(z1);
			normals.push_back(N[0]);
			normals.push_back(N[1]);
			normals.push_back(N[2]);
			normals.push_back(x2);
			normals.push_back(y2);
			normals.push_back(z2);
			normals.push_back(N[0]);
			normals.push_back(N[1]);
			normals.push_back(N[2]);
			normals.push_back(x3);
			normals.push_back(y3);
			normals.push_back(z3);
			normals.push_back(N[0]);
			normals.push_back(N[1]);
			normals.push_back(N[2]);

			P1.clear();
			P2.clear();
			N.clear();


		}

		for (uint32_t i = 0; i < normals.size() / 6; ++i)
		{
			ind.push_back(i);
		}

		set_indices(ind);

		return normals;

		normals.clear();
		vert.clear();
		ind.clear();
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

void draw_model(std::vector<float> vertices, std::vector<int> indices, VBO VBO_, VAO VAO_, EBO EBO_)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_.ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(VAO_.ID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
		vert_by_num.push_back( {vertices[i], vertices[i+1], vertices[i+2]} );
	}



	for (int i = 0; i < indices.size(); i += 3)
	{
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




int main()
{

	Mesh blank(Parse_vertices_blank(), Parse_indices_blank());
	Mesh tool(Parse_vertices_tool(), Parse_indices_tool());






	// load meshes.
	// -----------------
	InputMesh srcMesh;

	std::vector<float> s_vert;
	std::vector<uint32_t> s_ind;
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

		s_vert.push_back(v[0]);
		s_vert.push_back(v[1]);
		s_vert.push_back(v[2]);
	}

	// copy faces
	for (int i = 0; i < (int)srcMesh.F.size(); ++i) {
		const std::vector<int>& f = srcMesh.F[i];
		for (int j = 0; j < (int)f.size(); ++j) {
			srcMesh.faceIndicesArray.push_back(f[j]);
			s_ind.push_back(f[j]);
		}

		srcMesh.faceSizesArray.push_back((uint32_t)f.size());
	}

	printf("source mesh:\n\tvertices=%d\n\tfaces=%d\n", (int)srcMesh.V.size(), (int)srcMesh.F.size());

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
		cutMesh.vertexCoordsArray.push_back(v[0]+0.5);
		cutMesh.vertexCoordsArray.push_back(v[1]+0.5);
		cutMesh.vertexCoordsArray.push_back(v[2]+0.5);
	}

	// copy faces
	for (int i = 0; i < (int)cutMesh.F.size(); ++i) {
		const std::vector<int>& f = cutMesh.F[i];
		for (int j = 0; j < (int)f.size(); ++j) {
			cutMesh.faceIndicesArray.push_back(f[j]);
		}

		cutMesh.faceSizesArray.push_back((uint32_t)f.size());
	}

	printf("cut mesh:\n\tvertices=%d\n\tfaces=%d\n", (int)cutMesh.V.size(), (int)cutMesh.F.size());

	McContext context = MC_NULL_HANDLE;
	McResult err = mcCreateContext(&context, MC_DEBUG);
	my_assert(err == MC_NO_ERROR);

	//  do the cutting (boolean ops)
	// -----------------------------



	time_t start, end;

	time(&start);




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

	// my_assert(numConnComps == 1); // exactly 1 result (for this example)

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
		vert_res.push_back((float)ccVertices[i]*100);
		//std::cout << ccVertices[i] * 100 << std::endl;
	}

	blank.set_vertices(s_vert);
	//tool.set_vertices(vert_res);
	tool.set_vertices(vert_res);



	// query the faces
	// -------------------
	numBytes = 0;


	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, 0, NULL, &numBytes);
	my_assert(err == MC_NO_ERROR);
	std::vector<uint32_t> ccFaceIndices(numBytes / sizeof(uint32_t), 0);
	err = mcGetConnectedComponentData(context, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, numBytes, ccFaceIndices.data(), NULL);
	my_assert(err == MC_NO_ERROR);

	blank.set_indices(s_ind);
	//tool.set_indices(ccFaceIndices);
	tool.set_indices(ccFaceIndices);
	/*
	for (int i = 0; i < tool.get_indices().size(); i += 3)
	{
		std::cout << tool.get_indices()[i] << '\t' << tool.get_indices()[i + 1] << '\t' << tool.get_indices()[i + 2] << std::endl; 
	}
	std::cout << tool.get_vertices().size() << std::endl;
	*/


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




	std::vector<float> ind_vert = calc_norm(ccFaceIndices, vert_res);

	std::vector<int> indices_;
	for (int i = 0; i < ind_vert.size() / 6; ++i)
	{
		indices_.push_back(i);
	}





	Shader blank_shaderProgram("default.vert", "default.frag");
	VAO VAO_blank;
	VAO_blank.Bind();
	VBO VBO_blank(blank.get_vertices().data(), blank.get_vertices().size() * sizeof(float));
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
	VAO_tool.LinkAttrib(VBO_tool, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_tool.LinkAttrib(VBO_tool, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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


	bool draw_cut_mesh = true;
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

		glUniform3f(glGetUniformLocation(blank_shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(blank_shaderProgram, "camMatrix");

		camera.Matrix(tool_shaderProgram, "camMatrix");
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (draw_cut_mesh)
		{
			
			blank_shaderProgram.Activate();
			VAO_blank.Bind();
			draw_model(ind_vert, indices_, VBO_blank, VAO_blank, EBO_blank);
		}

		if (draw_src_mesh)
		{
			tool_shaderProgram.Activate();
			VAO_tool.Bind();
			draw_model(tool.get_vertices(), tool.get_indices(), VBO_tool, VAO_tool, EBO_tool);
		}

		
		std::string x_cord = "X = " + std::to_string(blank.get_x());
		std::string y_cord = "Y = " + std::to_string(blank.get_y());
		std::string z_cord = "Z = " + std::to_string(blank.get_z());



		ImGui::Begin("Tool info");
		ImGui::Checkbox("Draw blank mesh", &draw_cut_mesh);
		ImGui::Checkbox("Draw tool mesh", &draw_src_mesh);
		//ImGui::Checkbox("Draw result mesh", &draw_result);
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