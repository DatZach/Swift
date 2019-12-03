/*
 *	main.cpp
 *	GLFW Test
 */

#include <iostream>
#include <ctime>
#include <cmath>
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
//#include <Windows.h>
#include <GL/glfw.h>
#include <SOIL.h>

/*const char* vertexSource =
	"#version 150\n"
	"in vec2 position;"
	"in vec3 color;"
	"in vec2 texcoord;"
	"out vec3 Color;"
	"out vec2 Texcoord;"
	"void main() {"
	"	Color = color;"
	"	Texcoord = texcoord;"
	"	gl_Position = vec4( position, 0.0, 1.0 );"
	"}";

const char* fragmentSource =
	"#version 150\n"
	"in vec3 Color;"
	"in vec2 Texcoord;"
	"out vec4 outColor;"
	"uniform sampler2D texKitten;"
	"uniform sampler2D texPuppy;"
	"uniform float alpha;"
	"void main() {"
	"	vec4 colKitten = texture(texKitten, Texcoord);"
	"	vec4 colPuppy = texture(texPuppy, Texcoord);"
	"	outColor = mix(colKitten, colPuppy, alpha) * vec4(Color, 1.0);"
	"}";*/

const char* vertexSource =
	"#version 150\n"
	"in vec2 position;"
	"in vec4 color;"
	"in vec2 texcoord;"
	"out vec4 Color;"
	"out vec2 Texcoord;"
	"void main() {"
	"	Color = color;"
	"	Texcoord = texcoord;"
	"	gl_Position = vec4( position, 0.0, 1.0 );"
	"}";

const char* fragmentSource =
	"#version 150\n"
	"in vec4 Color;"
	"in vec2 Texcoord;"
	"out vec4 outColor;"
	"uniform sampler2D tex;"
	"void main() {"
	"	outColor = texture(tex, Texcoord) * Color;"
	"}";

//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char* argv[])
{
	std::cout << "Initializing GLFW... ";

	if (!glfwInit())
	{
		std::cout << "Failed!" << std::endl;
		return 0;
	}
	
	std::cout << "Done!" << std::endl;

	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
	std::cout << "GLFW v" << glfwMajor << "." << glfwMinor << "." << glfwRevision << std::endl;

	std::cout << "Opening window... ";

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

	std::cout << "Done!" << std::endl;
	
	std::cout << "Centering... ";
	GLFWvidmode desktopMode;
	glfwGetDesktopMode(&desktopMode);
	glfwSetWindowPos(desktopMode.Width / 2 - 800 / 2, desktopMode.Height / 2 - 600 / 2);

	std::cout << "Done!" << std::endl;

	std::cout << "Setting title........ ";
	glfwSetWindowTitle("OpenGL");
	std::cout << "Done!" << std::endl;

	std::cout << "Initializing GLEW... ";

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed!" << std::endl;
		return 0;
	}

	std::cout << "Done!" << std::endl;

	std::cout << "GLEW v" << glewGetString(GLEW_VERSION) << std::endl;

	int oglMajor, oglMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &oglMajor);
	glGetIntegerv(GL_MINOR_VERSION, &oglMinor);

	std::cout << "OpenGL v" << oglMajor << "." << oglMinor << std::endl;

	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);

	float vertices[] = {
	//  Position      Color             Texcoords
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);

	/*GLuint elements[] =
	{
		0, 1, 2,
		2, 3, 0
	};*/

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLint compileStatus;

	// Create and compiler vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		char logBuffer[512];

		std::cout << "Failed to compile vertex shader!" << std::endl;
		glGetShaderInfoLog(vertexShader, 512, NULL, logBuffer);
		std::cout << "Compile Log:" << std::endl << logBuffer << std::endl;
	}

	// Create and compiler fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		char logBuffer[512];

		std::cout << "Failed to compile fragment shader!" << std::endl;
		glGetShaderInfoLog(fragmentShader, 512, NULL, logBuffer);
		std::cout << "Compile Log:" << std::endl << logBuffer << std::endl;
	}

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	unsigned char* image;
	int width, height;

	// Load texture
	GLuint textureIndex[2];
	glGenTextures(2, textureIndex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIndex[0]);
	image = SOIL_load_image("sample.png", &width, &height, NULL, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureIndex[1]);
	image = SOIL_load_image("sample2.png", &width, &height, NULL, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint alpha = glGetUniformLocation(shaderProgram, "alpha");
	glUniform1f(alpha, 1.0f);*/

	std::cout << "Running..." << std::endl;

	while(glfwGetWindowParam(GLFW_OPENED))
	{
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;

		if (glfwGetKey(GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			SOIL_save_screenshot("screenshot.png", SOIL_SAVE_TYPE_BMP, 0, 0, 800, 600);
			break;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//float time = glfwGetTime();
		//glUniform1f(alpha, (sin(time * 4.0f) + 1.0f) / 2.0f);

		glfwSwapBuffers();
	}

	std::cout << "Shutting down..." << std::endl;

	glDeleteTextures(2, textureIndex);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}
