#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "handle_input.h"
#include <math.h>
#include "shader_class.h"
#include <string.h>

#define string std::string

int initialize_glfw_glew(GLFWwindow** window)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    /* Create a windowed mode window and its OpenGL context */
    if (!*window)
    {
        glfwTerminate();
        return -1;
    }
	else
		std::cout << "glfw OK" << std::endl;
    /* Make the window's context current */
    glfwMakeContextCurrent(*window);
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "GLEW error... "<< glewGetErrorString(err) <<std::endl;
	else
		std::cout << "GLEW OK" << std::endl;
}
char *read_shader_from_file(string filename)
{
	string shader;
	string line;
	char *ret;
	std::ifstream shader_file;
	shader_file.open(filename);
	while (getline(shader_file, line))
		shader.append(line + "\n");
	shader_file.close();
	ret = (char *)malloc(sizeof(char) * (shader.size() + 1));
	ret[shader.size()] = '\0';
	strcpy(ret, shader.c_str());
	return (ret);
}



int main(void)
{
    GLFWwindow* window;
	if (initialize_glfw_glew(&window) == -1)
		return(-1);
	
	//set input callbacks
	glfwSetCursorPosCallback(window, on_mouse_moved);
	glfwSetMouseButtonCallback(window, on_mouse_clicked);
	glfwSetKeyCallback(window, on_key_pressed);
	//create data
	float vertices[] = {
		0.5f, 0.5f, 0.0f, //top right
		0.5f,-0.5f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.0f //top left
	};
	unsigned int indices[] = 
	{
		0,1,3,
		1,2,3
	};
//Create shader program
	Shader my_shader = Shader("shaders/vertexshader.glsl", "shaders/fragshader.glsl");


//START buffer stuff
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1,&vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
		glClearColor(0.04f, 0.3f, 0.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
		float t = glfwGetTime();
		float xpos = sin(t);	
		my_shader.use();
		my_shader.setFloat("utest", xpos);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
	glfwTerminate();
    return 0;
}
