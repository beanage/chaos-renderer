#ifndef __CHAOS_RENDERER_SHADER_PROGRAM_H__
#define __CHAOS_RENDERER_SHADER_PROGRAM_H__

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace chaos;

/*-------------- shader helper ----------------*/

namespace chaos
{
	template <class T>
	struct type_to_gl_type {};

	template <>
	struct type_to_gl_type<int>
	{
		static const GLint value = GL_INT;
	};

	template <>
	struct type_to_gl_type<float>
	{
		static const GLint value = GL_FLOAT;
	};

	static void gl_uniform_1tv(GLint location, GLsizei size, GLfloat const* value)
	{
		glUniform1fv(location, size, value);
	}
	
	static void gl_uniform_1tv(GLint location, GLsizei size, GLint const* value)
	{
		glUniform1iv(location, size, value);
	}

	class program
	{
		friend class scoped_program;
		friend class renderer;
	public:
		class var
		{
			friend class program;
		public:
			var() : location(-1) {}
			
			template <class T>
			var& operator=(T value)
			{
				gl_uniform_1tv(location, 1, &value);
				return *this;
			}

			template <class T, int size>
			var& operator=(T value[size])
			{
				gl_uniform_1tv(location, size, value);
				return *this;
			}
			
			var& operator=(glm::mat4 const& value)
			{
				glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(value));
				GLenum error_code = glGetError();
				return *this;
			}
			
		private:
			GLint location;
		};
		
		program(string vertex, string fragment) : fragment_shader(0), vertex_shader(0), fragment_src(fragment), vertex_src(vertex), program_id(0) {}

		void compile()
		{
			if (program_id == 0)
				program_id = glCreateProgram();
			else
				return;

			fragment_shader = build_shader(GL_FRAGMENT_SHADER, fragment_src);
			vertex_shader = build_shader(GL_VERTEX_SHADER, vertex_src);
			glLinkProgram(program_id);
			GLint linked = GL_FALSE;
			glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
			if(linked != GL_TRUE)
				throw runtime_error("Program linking failed.");
		}

		GLuint build_shader(GLuint type, string const& source)
		{
			const char* cstr_source = source.c_str();

			GLuint shader_id = glCreateShader(type);
			glShaderSource(shader_id, 1, &cstr_source, NULL);
			glCompileShader(shader_id);

			GLint compiled = GL_FALSE;
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
			if (compiled != GL_TRUE)
				dump_log(shader_id);
			glAttachShader(program_id, shader_id);

			return shader_id;
		}

		var& operator[](string const& name)
		{
			this->operator()();
			var& var = shader_input[name];
			if (var.location == -1) {
				var.location = glGetUniformLocation(program_id, name.c_str());
			}
			if (var.location == -1)
				throw runtime_error("Could not resolve variable.");
			return var;
		}

		void dump_log(GLuint shader)
		{
			//Make sure name is shader
			if(glIsShader(shader))
			{
				//Shader log length
				int infoLogLength = 0;
				int maxLength = infoLogLength;
				//Get info string length
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				vector<char> infoLog(maxLength + 1, '\0');
				//Get info log
				glGetShaderInfoLog(shader, maxLength, &infoLogLength, &infoLog[0]);
				if(infoLogLength > 0)
					throw runtime_error(&infoLog[0]);
			}
			else
			{
				throw runtime_error("Name is not a shader.");
			}
		}

		void operator() ()
		{
			glUseProgram(program_id);
		}

	private:
		GLuint program_id;
		GLuint fragment_shader;
		GLuint vertex_shader;
		string fragment_src;
		string vertex_src;
		map<string, var> shader_input;
	};
}

#endif
