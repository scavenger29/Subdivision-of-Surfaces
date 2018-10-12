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
			e = e->next;
		} while (e != (*f)->edge);
	}
}

void loop_subdivision() {
	loop_generate_edge_points(meshes[itr], meshes[itr - 1]);
	loop_generate_new_vertices(meshes[itr], meshes[itr - 1]);
	loop_connect_new_mesh(meshes[itr], meshes[itr - 1]);
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


	GLuint programID = LoadShaders( "TransformVertexShader1.vertexshader", "TextureFragmentShader.fragmentshader" );

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Load the texture
	GLuint Texture = loadBMP_custom("roughwall_3d.bmp");
	
	 //Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	
	
	Mesh *mesh = new Mesh();
	meshes.push_back(mesh);
	meshes[itr]->load_file("stell.obj");
	
	init_mesh(meshes[itr], draw_vertices);
	
	HalfEdge *edge = meshes[itr]->glvertices[7]->edge;
	meshes[itr]->glvertices[7]->crease = 1;
	do 
	{ 
		HalfEdge *face_edge = edge->face->edge;
		face_edge->start->crease = 1;
		do 
		{
			face_edge = face_edge->next;
			face_edge->start->crease = 1;
		} while (face_edge != edge->face->edge);
		edge = edge->pair->next; 
	} while (edge != meshes[itr]->glvertices[7]->edge);

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
	//cube
	/*static const GLfloat uv_buffer[] = {
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
	};*/

	//oct_ts
	static const GLfloat uv_buffer[] = {
		0.00125126,0.563585,
		0.193304,0.808741,
		0.585009,0.479873,
		0.350291,0.895962,
		0.82284,0.746605,
		0.174108,0.858943,
		0.710501,0.513535,
		0.303995,0.0149846,
		0.0914029,0.364452,
		0.147313,0.165899,
		0.988525,0.445692,
		0.119083,0.00466933,
		0.0089114,0.37788,
		0.531663,0.571184,
		0.601764,0.607166,
		0.166234,0.663045,
		0.450789,0.352123,
		0.0570391,0.607685,
		0.783319,0.802606,
		0.519883,0.30195,
		0.875973,0.726676,
		0.955901,0.925718,
		0.539354,0.142338,
		0.462081,0.235328,
		0.862239,0.209601,
		0.779656,0.843654,
		0.996796,0.999695,
		0.611499,0.392438,
		0.266213,0.297281,
		0.840144,0.0237434,
		0.375866,0.0926237,
		0.677206,0.0562151,
		0.00878933,0.91879,
		0.275887,0.272897,
		0.587909,0.691183,
		0.837611,0.726493,
		0.484939,0.205359,
		0.743736,0.468459,
		0.457961,0.949156,
		0.744438,0.10828,
		0.599048,0.385235,
		0.735008,0.608966,
		0.572405,0.361339,
		0.151555,0.225105,
		0.425153,0.802881,
		0.517106,0.98999,
		0.751549,0.345561,
		0.168981,0.657308,
		0.491897,0.0635395,
		0.699759,0.504807,
		0.147496,0.949583,
		0.141575,0.905118,
		0.692892,0.303049,
		0.426557,0.0703757,
		0.966613,0.683187,
		0.153233,0.877255,
		0.82168,0.582049,
		0.191351,0.177892,
		0.817194,0.475265,
		0.155553,0.503922,
		0.732017,0.405591,
		0.27958,0.568743,
		0.682241,0.755852,
		0.721915,0.475295,
		0.12302,0.367809,
		0.834681,0.0350963,
		0.517014,0.662984,
		0.426222,0.104678,
		0.949339,0.921384,
		0.549547,0.345988,
		0.471725,0.374981,
		0.84698,0.316874,
		0.456099,0.271889,
		0.982971,0.2978,
		0.739189,0.567278,
		0.19599,0.761315,
		0.839442,0.397656,
		0.5009,0.890164,
		0.0274667,0.994629,
		0.572588,0.0505081,
		0.531327,0.194067,
		0.843043,0.626759,
		0.657613,0.197851,
		0.842158,0.123325,
		0.109928,0.743126,
		0.314066,0.941069,
		0.286081,0.336314,
		0.140263,0.733085,
		0.83462,0.707999,
		0.600238,0.747215,
		0.252724,0.144475,
		0.00161748,0.0610065,
		0.806238,0.852626,
		0.210578,0.115604,
		0.553209,0.0142521,
		0.113773,0.454512,
		0.75222,0.686148,
		0.543443,0.0738853,
		0.43672,0.201941,
		0.696219,0.290353,
		0.436689,0.232429,
		0.577868,0.532579,
		0.628681,0.160192,
		0.504135,0.963042,
		0.695761,0.924802,
		0.189947,0.335948,
		0.17835,0.995178,
		0.457442,0.998016,
		0.0975066,0.625172,
		0.0943937,0.437727,
		0.931516,0.0484329,
		0.89462,0.290017,
		0.227302,0.769066,
		0.410718,0.201971,
		0.628071,0.604144,
		0.451613,0.466353,
		0.597827,0.634724,
		0.854793,0.828791,
		0.624775,0.720908,
		0.565752,0.375134,
		0.184271,0.737907,
		0.555132,0.905087,
		0.242866,0.18894,
		0.604724,0.698508,
		0.584613,0.351299,
		0.494461,0.0803858,
		0.740745,0.612049,
		0.62038,0.691122,
		0.804529,0.149113,
		0.576037,0.867733,
		0.911557,0.614704,
		0.727683,0.0432142,
		0.667776,0.976531,
		0.315012,0.569201,
		0.305826,0.173925,
		0.108554,0.869045,
		0.851222,0.744316,
		0.154881,0.326914,
		0.0793481,0.0766015,
		0.64098,0.820002,
		0.545091,0.448256,
		0.408979,0.298746,
		0.46556,0.501205,
		0.152654,0.323038,
		0.737999,0.313883,
		0.826685,0.959075,
		0.873348,0.725028,
		0.300058,0.943999,
		0.127232,0.0657369,
		0.784967,0.524583,
		0.609638,0.956114,
		0.0722678,0.875637,
		0.653859,0.322123,
		0.104801,0.505051,
		0.227088,0.290292,
		0.91998,0.551164,
		0.662801,0.114536,
		0.492538,0.379131,
		0.496811,0.793359,
		0.509262,0.382366,
		0.688162,0.532151,
		0.606281,0.395184,
		0.00589007,0.707877,
		0.10062,0.623066,
		0.863247,0.491501,
		0.747337,0.496902,
		0.380108,0.785363,
		0.552812,0.357097,
		0.955718,0.630848,
		0.17658,0.374248,
		0.131626,0.743278,
		0.95172,0.611988,
		0.0278329,0.329844,
		0.0559099,0.63921,
		0.131626,0.847072,
		0.864315,0.596881,
		0.721641,0.853969,
		0.0146794,0.126469,
		0.707907,0.617145,
		0.217566,0.0659505,
		0.16892,0.624104,
		0.340983,0.319407,
		0.367565,0.661,
		0.802393,0.806879,
		0.526536,0.611103,
		0.798181,0.900601,
		0.14481,0.630177,
		0.402417,0.2537,
		0.13654,0.85519,
		0.0661641,0.427808,
		0.573351,0.302286,
		0.548051,0.225562,
		0.31135,0.11063,
		0.808039,0.134709,
		0.284249,0.78811,
		0.89523,0.789636,
		0.743797,0.615223,
		0.361126,0.856655,
		0.228492,0.863582,
		0.229438,0.24955,
		0.542405,0.984832,
		0.0538041,0.0814234,
		0.524674,0.426801,
		0.0946684,0.258797,
		0.891537,0.232765,
		0.14655,0.125095,
		0.931639,0.0801111,
		0.0470901,0.0587176,
		0.336406,0.914701,
		0.398602,0.432783,
		0.946165,0.837184,
		0.534227,0.842097,
		0.693533,0.397687,
		0.259163,0.00433363,
		0.52559,0.954802,
		0.398694,0.241096
	};

	for (int i = 0; i < draw_vertices.size(); i++)
	{
		//cout << draw_vertices[i].getx() << " " << draw_vertices[i].gety() << " " << draw_vertices[i].getz() << endl;
		//Vector3f temp1(color_buffer[3*i], color_buffer[3 * i + 1], color_buffer[3 * i + 2] );
		Vector3f temp1(uv_buffer[2 * i], uv_buffer[2 * i + 1], 0);
		color_vertices.push_back(temp1);
	}
	
	for (int i = 0; i < meshes[itr]->glvertices.size(); i++)
	{
		meshes[itr]->glvertices[i]->color = color_vertices[i];
	}
	
	int siz = draw_vertices.size();
	vector<Vector3f*> p;
	//vector<Vector3f*> q;
	vector<pair<float,float>*> q;
	vector<int> draw_size;
	vector<vector<Vector3f> > copy_draw_vertices;
	//vector<vector<Vector3f> > copy_color_vertices;
	vector<vector<pair<float, float> > > copy_color_vertices;
	
	//cout << siz << endl;
	vector<Vector3f> temp;
	//<Vector3f > temp1;
	vector<pair<float, float> > temp1;
	for (int i = 0; i < draw_vertices.size(); i++)
	{
		temp.push_back(Vector3f(draw_vertices[i].getx(), draw_vertices[i].gety(), draw_vertices[i].getz()));
		//temp1.push_back(Vector3f(color_vertices[i].getx(), color_vertices[i].gety(), color_vertices[i].getz()));
		temp1.push_back(make_pair(color_vertices[i].getx(), color_vertices[i].gety()));
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
		//vector<Vector3f> temp1;
		vector<pair<float, float> > temp1;
		for (int i = 0; i < draw_vertices.size(); i++)
		{
			temp.push_back(Vector3f(draw_vertices[i].getx(), draw_vertices[i].gety(), draw_vertices[i].getz()));
			//temp1.push_back(Vector3f(color_vertices[i].getx(), color_vertices[i].gety(), color_vertices[i].getz()));
			temp1.push_back(make_pair(color_vertices[i].getx(), color_vertices[i].gety()));
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
			//cout << GL_TEXTURE0+i << endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture);
			// Set our "myTextureSampler" sampler to use Texture Unit 0
			glUniform1i(TextureID, 0);

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
				2,
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
	glDeleteTextures(1, &Texture);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	
	return 0;
}

