#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

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
#include <string.h>
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

	char *vertex_shader_source = read_shader_from_file("shaders/vertexshader.glsl");
	char *fragment_shader_source = read_shader_from_file("shaders/fragshader.glsl");
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	free(vertex_shader_source);
	free(fragment_shader_source);
	glCompileShader(vertex_shader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "Error compiling vertex shader\n" << infoLog << std::endl;
	}
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "Error compiling fragment shader\n" << infoLog << std::endl;
	}
	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "Error linking shaders\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

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
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
	glfwTerminate();
    return 0;
}