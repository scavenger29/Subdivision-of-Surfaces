// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<iostream>
#include "halfedge.h"
#include "loop.h"
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>

std::vector<Vector3f> draw_vertices;
std::vector<Vector3f> color_vertices;
std::vector<Mesh*> meshes;
int itr = 0;

void prepare_to_draw(int it) {
	draw_vertices.clear();
	color_vertices.clear();
	//draw_normals.clear();
	if (it < 0 || it > itr) { return; }
	for (std::vector<Face*>::iterator f = meshes[it]->glfaces.begin();
		f != meshes[it]->glfaces.end(); f++) {
		HalfEdge *e = (*f)->edge;
		do {
			draw_vertices.push_back(e->start->pos);
			color_vertices.push_back(e->start->color);
			//draw_normals.push_back(e->start->normal);
			e = e->next;
		} while (e != (*f)->edge);
	}
}

void loop_subdivision() {
	loop_generate_edge_points(meshes[itr], meshes[itr - 1]); // 1
	loop_generate_new_vertices(meshes[itr], meshes[itr - 1]); // 2
	loop_connect_new_mesh(meshes[itr], meshes[itr - 1]); // 3
}

void loop() {
	Mesh *mesh = new Mesh();
	mesh->quad = false;
	meshes.push_back(mesh);
	itr++;
	loop_subdivision();
	
	prepare_to_draw(itr);
	//print_draw();
	//print_indices();
	//setup_vbos();
}

int main( void )
{
	int n;
	cout << "level of subdivision";
	cin >> n;
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow( 1024, 768, "Tutorial 07 - Model Loading", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);
	
		
	glm::mat4 translateMatrix = glm::translate(glm::mat4(), glm::vec3(4.0f, 0.0f, 0.0f));
	glm::mat4 MVP;
	

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Load the texture
	//GLuint Texture = loadDDS("uvmap.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	//GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	
	
	Mesh *mesh = new Mesh();
	meshes.push_back(mesh);
	meshes[itr]->load_file("cube.obj");
	
	init_mesh(meshes[itr], draw_vertices);
	//HalfEdge *edge = meshes[itr]->glfaces[0]->edge;
	//edge->crease = 1;
	//do
	//{
	//	cout << edge->start->pos.getx() << " " << edge->start->pos.gety() << " " << edge->start->pos.getz() << endl;
	//	edge = edge->next;
	//	//edge->crease = 1;
	//} while (edge != meshes[itr]->glfaces[0]->edge);



	static const GLfloat color_buffer[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	for (int i = 0; i < draw_vertices.size(); i++)
	{
		//cout << draw_vertices[i].getx() << " " << draw_vertices[i].gety() << " " << draw_vertices[i].getz() << endl;
		Vector3f temp1(color_buffer[3*i], color_buffer[3 * i + 1], color_buffer[3 * i + 2] );
		color_vertices.push_back(temp1);
	}
	
	for (int i = 0; i < meshes[itr]->glvertices.size(); i++)
	{
		meshes[itr]->glvertices[i]->color = color_vertices[i];
	}
	


	int siz = draw_vertices.size();
	vector<Vector3f*> p;
	vector<Vector3f*> q;
	vector<int> draw_size;
	vector<vector<Vector3f> > copy_draw_vertices;
	vector<vector<Vector3f> > copy_color_vertices;
	//cout << siz << endl;
	vector<Vector3f> temp;
	vector<Vector3f> temp1;
	for (int i = 0; i < draw_vertices.size(); i++)
	{
		temp.push_back(Vector3f(draw_vertices[i].getx(), draw_vertices[i].gety(), draw_vertices[i].getz()));
		temp1.push_back(Vector3f(color_vertices[i].getx(), color_vertices[i].gety(), color_vertices[i].getz()));
	}
	
	copy_draw_vertices.push_back(temp);
	copy_color_vertices.push_back(temp1);
	
	p.push_back(&copy_draw_vertices[itr][0]);
	q.push_back(&copy_color_vertices[itr][0]);
	draw_size.push_back(siz);
	//cout << p[0] << endl;
	
	while (itr < n)
	{
//		cout << itr << endl;
		loop();
		
		siz = draw_vertices.size();
		//cout << siz << endl;
		draw_size.push_back(siz);
		vector<Vector3f> temp;
		vector<Vector3f> temp1;
		for (int i = 0; i < draw_vertices.size(); i++)
		{
			temp.push_back(Vector3f(draw_vertices[i].getx(), draw_vertices[i].gety(), draw_vertices[i].getz()));
			temp1.push_back(Vector3f(color_vertices[i].getx(), color_vertices[i].gety(), color_vertices[i].getz()));
		}
		copy_draw_vertices.push_back(temp);
		copy_color_vertices.push_back(temp1);
		p.push_back(&copy_draw_vertices[itr][0]);
		q.push_back(&copy_color_vertices[itr][0]);
	}

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);	
		//glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		//cout << draw_size[1] << " " << p[1] << endl;
		for (int i = 0; i <= itr; i++)
		{
			//cout << draw_size[i] << " " << p[i] << endl;
			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, draw_size[i] * sizeof(glm::vec3), p[i], GL_STATIC_DRAW);

			GLuint colorbuffer;
			glGenBuffers(1, &colorbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
			glBufferData(GL_ARRAY_BUFFER, draw_size[i] * sizeof(glm::vec3), q[i], GL_STATIC_DRAW);

			/*glm::mat4 Projection = glm::perspective(glm::radians(120.0f), 4.0f / 3.0f, 0.1f, 100.0f);
			glm::mat4 View = glm::lookAt(
				glm::vec3(14, 3, 6),
				glm::vec3(4, 0, 0),
				glm::vec3(0, 1, 0)
			);*/
			computeMatricesFromInputs();
			glm::mat4 ProjectionMatrix = getProjectionMatrix();
			glm::mat4 ViewMatrix = getViewMatrix();
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			ModelMatrix = translateMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			//render(vertexbuffer, colorbuffer, draw_size[i]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);

			glDrawArrays(GL_TRIANGLES, 0, draw_size[i]);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDeleteBuffers(1, &vertexbuffer);
			glDeleteBuffers(1, &colorbuffer);
		}
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	

	glDeleteProgram(programID);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	
	return 0;
}

