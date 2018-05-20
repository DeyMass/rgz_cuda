// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#define N 4

#include <fstream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;
GLuint block_index;
void readShaders(string &Shader, string path) {
	string str;
	ifstream in(path);
	while (getline(in, str)) {
		Shader.append(str + "\n");
	}
}

float pos[N*N];
float sec[N*N];
float res[N*N];

GLuint CompileShader(GLuint type, string source) {
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		char *msg = (char*)alloca(len * sizeof(char));
		glGetShaderInfoLog(id, len, &len, msg);
		printf("Compile error: %s\n", msg);
		return 0;
	}
	return id;
}

GLuint CreateShader(string shader, GLuint type) {
	GLuint program = glCreateProgram();
	GLuint sh = CompileShader(type, shader);
	glAttachShader(program, sh);
	glLinkProgram(program);
	glValidateProgram(program);
	GLuint block_index = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "shader_data");
	glShaderStorageBlockBinding(program, block_index, 80);
	return program;
}

int main() {
	string vertexShader, fragmentShader, computeShader;
	readShaders(vertexShader, "vertexShader.txt");
	readShaders(computeShader, "computeShader.glsl");
	readShaders(fragmentShader, "fragmentShader.txt");
	printf("%s", vertexShader.c_str());
	puts("------------------------------------------------------------------");
	printf("%s", fragmentShader.c_str());
	puts("------------------------------------------------------------------");
	printf("%s", computeShader.c_str());
	puts("------------------------------------------------------------------");


	for (int i = 0; i < N*N; i++) {
		pos[i] = (rand() % 4000) / (float)100;
		sec[i] = (rand() % 4000) / (float)100;
	}
	for (int i = 0; i < N*N; i++) {
		if (i % N == 0) puts("");
		printf("%f ", pos[i]);
	}
	puts("\n-------------------------------");
	for (int i = 0; i < N*N; i++) {
		if (i % N == 0) puts("");
		printf("%f ", sec[i]);
	}

	
	//------------------------------------------------------------------------
	if (!glfwInit()) {	puts("ERROR");	return -1;}
	GLFWwindow *window = glfwCreateWindow(640, 480, "HELLO WORLD", NULL, NULL);
	if (!window) {	glfwTerminate(); puts("ERROR");	return -1; }
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	//------------------------------------------------------------------------
	GLuint buffer;
	glGenBuffers(1, &buffer);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * (N * N + N * N + N * N) , 0, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 80, buffer);
	GLfloat *map = (GLfloat *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	for (int i = 0; i < N*N; i++) {
		map[i] = pos[i];
	}

	GLuint shader = CreateShader(computeShader, GL_COMPUTE_SHADER);
	glUseProgram(shader);
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glfwTerminate(); 
	for (int i = N*N + N*N; i < N*N+N*N+N*N; i++) {
		if (i % N == 0) puts("");
		printf("%f ", map[i]);
	}
	
	system("pause");
	system("pause");
}