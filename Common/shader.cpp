/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for shader implementation.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma warning(disable: 4996)

#include "Shader.h"
#include <fstream>
#include <sstream>



Shader::Shader(const char* vertexPath, const char* fragPath)
{
	GLint is_succeed = 1;

	std::ifstream stream(fragPath);
	std::string file;
	std::stringstream f_shader_stream;

	std::ifstream v_stream(vertexPath);
	std::string vfile;
	std::stringstream v_shader_stream;

	if (stream.is_open())
	{
		f_shader_stream << stream.rdbuf();
		stream.close();
		file = f_shader_stream.str();
	}
	else
	{
		std::string error = "Failed to open fragment shader file";
		error += fragPath;
		throw std::runtime_error(error);
	}

	//const char* f_shader_code = file.c_str();

	if (v_stream.is_open())
	{
		v_shader_stream << v_stream.rdbuf();
		v_stream.close();
		vfile = v_shader_stream.str();
	}
	else
	{
		std::string error = "Failed to open vertex shader file";
		error += vertexPath;
		throw std::runtime_error(error);
	}

	//const char* v_shader_code = vfile.c_str();

	/*
	 * Create & Compile fragment shader.
	 * Check the compile status.
	 */
	//const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	fragmentShaderId = Load(fragPath, GL_FRAGMENT_SHADER, true);
	/*fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &f_shader_code, 0);
	glCompileShader(fragmentShaderId);*/

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &is_succeed);
	if (is_succeed == GL_FALSE)
	{
		glDeleteShader(fragmentShaderId);
		std::string error = "Failed to compile fragment shader file";
		error += fragPath;
		throw std::runtime_error(error);
	}

	/*
	 * Create & Compile vertex shader.
	 * Check the compile status.
	 */
	//const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	vertexShaderId = Load(vertexPath, GL_VERTEX_SHADER, true);
	/*vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &v_shader_code, 0);
	glCompileShader(vertexShaderId);*/

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &is_succeed);
	if (is_succeed == GL_FALSE)
	{
		glDeleteShader(vertexShaderId);
		std::string error = "Failed to compile vertex shader file";
		error += fragPath;
		throw std::runtime_error(error);
	}

	/*
	 * 1.Create program, attach shader to the program.
	 * 2.Link & Set use.
	 */
	programId = glCreateProgram();
	glAttachShader(programId, fragmentShaderId);
	glAttachShader(programId, vertexShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	/*
	 * Set the ucolor & aposition uniform variable.
	 */
	 //ucolor = glGetUniformLocation(program, "color");
	 //aposition = glGetAttribLocation(program, "position");

	glGetProgramiv(programId, GL_LINK_STATUS, &is_succeed);
	if (is_succeed == GL_FALSE)
	{
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		throw std::runtime_error("failed to link");
	}

	//glEnable(GL_DEPTH_TEST);


	/*
	 * Done all stuff, delete both vertex and fragment shader.
	 */
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	//unsigned int uniformBlockIndex = glGetUniformBlockIndex(programId, "Matrices");
	//glUniformBlockBinding(programId, uniformBlockIndex, 0);
}

Shader::Shader(const char* computeShader)
{
	std::ifstream stream(computeShader);
	std::string file;
	std::stringstream shader_stream;

	if (stream.is_open())
	{
		shader_stream << stream.rdbuf();
		stream.close();
		file = shader_stream.str();
	}
	else
	{
		throw std::runtime_error("Failed to open compute shader file");
	}

	//const char* v_shader_code = file.c_str();
	
	GLint is_succeed = 1;
	/*computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShaderId, 1, &v_shader_code, NULL);
	glCompileShader(computeShaderId);*/
	computeShaderId = Load(computeShader, GL_COMPUTE_SHADER, true);

	glGetShaderiv(computeShaderId, GL_COMPILE_STATUS, &is_succeed);
	if (is_succeed == GL_FALSE)
	{
		glDeleteShader(computeShaderId);
		throw std::runtime_error("failed to compile compute");
	}

	programId = glCreateProgram();
	glAttachShader(programId, computeShaderId);
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &is_succeed);

	if(is_succeed == GL_FALSE)
	{
		throw std::runtime_error("compute shader link fail");
	}

	glDeleteShader(computeShaderId);
}

Shader::Shader(const char* vertex, const char* frag, const char* tessControl, const char* tessEval)
{
	GLint vs = Load(vertex, GL_VERTEX_SHADER, false);
	GLint fs = Load(frag, GL_FRAGMENT_SHADER, false);
	GLint tes = Load(tessEval, GL_TESS_EVALUATION_SHADER, false);
	GLint tcs = Load(tessControl, GL_TESS_CONTROL_SHADER, false);
	GLint is_succeed = 1;
	programId = glCreateProgram();

	glAttachShader(programId, vs);
	glAttachShader(programId, fs);
	glAttachShader(programId, tes);
	glAttachShader(programId, tcs);

	glLinkProgram(programId);
	
	glGetProgramiv(programId, GL_LINK_STATUS, &is_succeed);

	if (is_succeed == GL_FALSE)
	{
		throw std::runtime_error("compute shader link fail");
	}
}

Shader::Shader(const char* vertex, const char* frag, const char* geometry)
{
	GLint vs = Load(vertex, GL_VERTEX_SHADER, false);
	GLint fs = Load(frag, GL_FRAGMENT_SHADER, false);
	GLint gs = Load(geometry, GL_GEOMETRY_SHADER, false);
	GLint is_succeed = 1;
	
	programId = glCreateProgram();

	glAttachShader(programId, vs);
	glAttachShader(programId, fs);
	glAttachShader(programId, gs);
	
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &is_succeed);

	if (is_succeed == GL_FALSE)
	{
		throw std::runtime_error("compute shader link fail");
	}
}

unsigned Shader::Load(const char* fileName, GLenum type, bool checkError)
{
	GLuint result = 0;
	FILE* fp;
	size_t filesize;
	char* data;

	fp = fopen(fileName, "rb");

	if (!fp)
		return 0;

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data = new char[filesize + 1];

	if (!data)
		goto fail_data_alloc;

	fread(data, 1, filesize, fp);
	data[filesize] = 0;
	fclose(fp);

	result = glCreateShader(type);

	if (!result)
		goto fail_shader_alloc;

	glShaderSource(result, 1, &data, NULL);

	delete[] data;

	glCompileShader(result);

	if (checkError)
	{
		GLint status = 0;
		glGetShaderiv(result, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			char buffer[4096];
			glGetShaderInfoLog(result, 4096, NULL, buffer);
//#ifdef _WIN64
//			OutputDebugStringA(fileName);
//			OutputDebugStringA(":");
//			OutputDebugStringA(buffer);
//			OutputDebugStringA("\n");
//#else
//			fprintf(stderr, "%s: %s\n", filename, buffer);
//#endif
//			goto fail_compile_shader;
		}
	}

	return result;

//fail_compile_shader:
//	glDeleteShader(result);

fail_shader_alloc:;
fail_data_alloc:
	return result;
}

unsigned Shader::GetShaderId()
{
	return programId;
}

unsigned Shader::GetUniformLocation(const char* name)
{
	return glGetUniformLocation(programId, name);
}

void Shader::Use()
{
	glUseProgram(programId);
}

void Shader::SendUniformMat(std::string uniformName, void* val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniformMatrix4fv(loc, 1, GL_TRUE, valInFloat);
}

void Shader::SendUniformMatGLM(std::string uniformName, void* val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniformMatrix4fv(loc, 1, GL_FALSE, valInFloat);
}

void Shader::SendUniformMatGLM(const std::string& uniformName, glm::mat4 val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
}

void Shader::SendUniformInt(std::string uniformName, void* val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	int* valInInt = static_cast<int*>(val);
	glUniform1i(loc, *valInInt);
}

void Shader::SendUniformInt(std::string uniformName, int val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	glUniform1i(loc, val);
}

void Shader::SendUniformFloat(std::string uniformName, void* val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniform1f(loc, *valInFloat);
}

void Shader::SendUniformFloat(std::string uniformName, float val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	glUniform1f(loc, val);
}

void Shader::SendUniformVec3(std::string uniformName, void* val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniform3f(loc, valInFloat[0], valInFloat[1], valInFloat[2]);
}

void Shader::SendUniformVec4(std::string uniformName, void* val) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniform4f(loc, valInFloat[0], valInFloat[1], valInFloat[2], valInFloat[3]);
}

void Shader::SendUniform3fv(std::string uniformName, void* val, int count) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniform3fv(loc, count, valInFloat);
}

void Shader::SendUniform4fv(std::string uniformName, void* val, int count) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniform4fv(loc, count, valInFloat);
}

void Shader::SendUniform1fv(std::string uniformName, void* val, int count) const
{
	const int loc = glGetUniformLocation(programId, uniformName.c_str());
	float* valInFloat = static_cast<float*>(val);
	glUniform1fv(loc, count, valInFloat);
}

void Shader::SendUniformValues()
{
	for(const auto& val : uniformValues)
	{
		switch(val.second.type)
		{
		case ShaderValueType::Int:
			SendUniformInt(val.first, val.second.data);
			break;
		case ShaderValueType::Float:
			SendUniformFloat(val.first, val.second.data);
			break;
		case ShaderValueType::Vec3:
			SendUniformVec3(val.first, val.second.data);
			break;
		case ShaderValueType::Vec4:
			SendUniformVec4(val.first, val.second.data);
			break;
		case ShaderValueType::Matrix4x4:
			SendUniformMat(val.first, val.second.data);
			break;
		default:
			break;
		}
	}
}

void Shader::AddUniformValues(std::string name, ShaderValueType type, void* data)
{
	ShaderUniformValue value{type, data};
	uniformValues.insert(std::make_pair(name, value));
}

Shader::~Shader()
{
	glDeleteProgram(programId);
}
