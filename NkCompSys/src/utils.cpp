/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "utils.h"

namespace shader_utils
{
	char * loadShader(const char * filename)
	{
		char * source;
		long size;
		errno_t err;
		FILE* fp;
		err =fopen_s(&fp, filename, "r");
		if (err)
			return NULL;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		//source = (char*)malloc((size+1)*sizeof(char));
		source = new char[size+1];
		memset(source, 0, size+1);
		fread(source, sizeof(char), size, fp);
		source[size] = 0;
		fclose(fp);
		return source;
	}

	GLuint createShaderFromFile(const char* filename, GLenum type)
	{
		GLchar* source =  loadShader(filename);
		GLuint shaderID;
		if (source)
		{
			shaderID = glCreateShader(type);
		}
		else
		{
			std::cout << "Could not find shader: " << filename << std::endl;
			return -1; 
		}

		glShaderSource(shaderID, 1, (const GLchar**)&source, NULL);
		if (source)
			delete[] source;

		GLint compiled;
		glCompileShader(shaderID);
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
		if (compiled == false)
		{
			 std::cout << std::endl << "Could not compile shader: " << filename << std::endl;
			 int loglen = 0;
			 int charswritten = 0;
			 GLchar* infolog;
			 glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &loglen);
			 infolog = new GLchar[loglen];
			 glGetShaderInfoLog(shaderID, loglen, &charswritten, infolog);
			 std::cout << "InfoLog: " << std::endl << infolog << std::endl;
				delete[] infolog;
			return -1;
		}
		else {
			printf("Shader: '%s' compiled successfully!\n", filename);
		}
	
		return shaderID;
	}

	bool compileShader(GLuint ID, GLchar*& source)
	{
		glShaderSource(ID, 1, (const GLchar**)&source, NULL);
		GLint compiled;
		glCompileShader(ID);
		glGetShaderiv(ID, GL_COMPILE_STATUS, &compiled);
		if (compiled == false)
		{
			 std::cout << "Could not compile shader: " << ID << std::endl;
			 int loglen = 0;
			 int charswritten = 0;
			 GLchar* infolog;
			 glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &loglen);
			 infolog = new GLchar[loglen];
			 glGetShaderInfoLog(ID, loglen, &charswritten, infolog);
			 std::cout << "InfoLog: " << std::endl << infolog << std::endl;
				delete[] infolog;
			return 0;
		}
		return 1;
	}

	bool checkLinkError(const GLuint& prog)
	{
		GLint islinked;

		glGetProgramiv(prog, GL_LINK_STATUS, &islinked);
		if (islinked == false){
			std::cout << "failed to link shaders" << std::endl;

			GLint maxlength;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxlength);
			if (maxlength > 0)
			{
				char* linkinfolog = new char[maxlength];
				glGetProgramInfoLog(prog, maxlength, &maxlength, linkinfolog);
				std::cout << linkinfolog << std::endl;
				delete [] linkinfolog;
			}
			return 0;
		}
		return 1;
	}

};


namespace bullet_utils 
{
	glm::mat4 btTransform2GLM(const btTransform& transform) {
		btScalar mat[16] ;
		transform.getOpenGLMatrix(mat);
		return glm::mat4(mat[0], mat[1], mat[2], mat[3],
						mat[4], mat[5], mat[6], mat[7],
						mat[8], mat[9], mat[10], mat[11],
						mat[12], mat[13], mat[14],  mat[15]);
	}

	glm::quat btQuat2GLM(const btQuaternion& quat) {
		
		return glm::quat(quat.w(), glm::vec3(quat.x(), quat.y(), quat.z()));
	}
};


void logGLError(const std::string &msg)
{
    switch(glGetError())
    {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM: ";
        break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_VALUE: ";
        break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION: ";
        break;
        case GL_STACK_OVERFLOW:
            std::cout << "GL_STACK_OVERFLOW: "; 
        break;
        case GL_STACK_UNDERFLOW:
            std::cout << "GL_STACK_UNDERFLOW: ";
        break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY: ";
        break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION: ";
        break;
        default:
        std::cout << "OK: ";
    }
	 std::cout << msg << std::endl;
}