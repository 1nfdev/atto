#ifndef ATTO_GL_H__DECLARED
#define ATTO_GL_H__DECLARED

#include <atto/platform.h>

#ifdef ATTO_PLATFORM_X11
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#define ATTO_GL_DESKTOP
#endif /* ifdef ATTO_PLATFORM_X11 */

#ifdef ATTO_PLATFORM_RPI
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define ATTO_GL_ES
#endif /* ifdef ATTO_PLATFORM_RPI */

#ifdef ATTO_PLATFORM_WINDOWS
#ifndef ATTO_WINDOWS_H_INCLUDED
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX
#include <Windows.h>
#define ATTO_WINDOWS_H_INCLUDED
#endif /* ifndef ATTO_WINDOWS_H_INCLUDED */
#include <GL/gl.h>
#include "glext.h"
#define ATTO_GL_DESKTOP
#endif /* ifdef ATTO_PLATFORM_WINDOWS */

#ifdef ATTO_PLATFORM_OSX
#include <OpenGL/gl3.h>
#define ATTO_GL_DESKTOP
#endif

/* Initialize GL stuff, like load GL>=2 procs on Windows */
void aGLInit();

/* Textures */

typedef enum {
	AGLTF_Unknown,
	AGLTF_U8_R,
	AGLTF_U8_RA,
	AGLTF_U8_RGB,
	AGLTF_U8_RGBA,
	AGLTF_U565_RGB,
	AGLTF_U5551_RGBA,
	AGLTF_U4444_RGBA
} AGLTextureFormat;

typedef struct {
	GLuint name;
	GLsizei width, height;
	AGLTextureFormat format;
} AGLTexture;

AGLTexture aGLTextureCreate(void);
void aGLTextureUpload(AGLTexture *texture,
	AGLTextureFormat format, int width, int height, const void *pixels);
#define aGLTextureDestroy(t) do{glDeleteTextures(1,&(t)->name);(t)->name=0;}while(0)

/* Shader programs */

typedef enum {
	AGLAT_Float,
	AGLAT_Vec2,
	AGLAT_Vec3,
	AGLAT_Vec4,
	AGLAT_Mat2,
	AGLAT_Mat3,
	AGLAT_Mat4,
	AGLAT_Int,
	AGLAT_IVec2,
	AGLAT_IVec3,
	AGLAT_IVec4,
	AGLAT_Texture
} AGLAttributeType;

typedef struct {
	const char *name;
	AGLAttributeType type;
	union {
		const GLfloat *pf;
		const GLint *pi;
		const AGLTexture *texture;
	} value;
	GLsizei count;
} AGLProgramUniform;

typedef GLint AGLProgram;

AGLProgram aGLProgramCreate(const char * const *vertex, const char * const *fragment);
AGLProgram aGLProgramCreateSimple(const char *vertex, const char *fragment);
#define aGLProgramDestroy(p) do { glDeleteProgram(p); } while(0)

/* Array buffers */

typedef enum {
	AGLBT_Vertex = GL_ARRAY_BUFFER,
	AGLBT_Index = GL_ELEMENT_ARRAY_BUFFER
} AGLBufferType;

typedef struct {
	GLuint name;
	AGLBufferType type;
} AGLBuffer;

AGLBuffer aGLBufferCreate(AGLBufferType type);
void aGLBufferUpload(AGLBuffer *buffer, GLsizei size, const void *data);
#define aGLBufferDestroy(b) do{glDeleteBuffers(1,&(b)->name);(b)->name=0;}while(0)

/* Draw */

typedef struct {
	const char *name;
	const AGLBuffer *buffer;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const GLvoid *ptr;
} AGLAttribute;

typedef enum {
	AGLDF_Never = GL_NEVER,
	AGLDF_Less = GL_LESS, /* default */
	AGLDF_Equal = GL_EQUAL,
	AGLDF_LessOrEqual = GL_LEQUAL,
	AGLDF_Greater = GL_GREATER,
	AGLDF_NotEqual = GL_NOTEQUAL,
	AGLDF_GreaterOrEqual = GL_GEQUAL,
	AGLDF_Always = GL_ALWAYS
} AGLDepthFunc;

typedef enum {
	AGLDM_Disabled = 0, /* default */
	AGLDM_TestOnly = 0x01,
	AGLDM_TestAndWrite = 0x03
} AGLDepthMode;

typedef struct {
	AGLDepthMode mode;
	AGLDepthFunc func;
} AGLDepthParams;

typedef enum {
	AGLBE_Add = GL_FUNC_ADD, /* default */
	AGLBE_Subtract = GL_FUNC_SUBTRACT,
	AGLBE_ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT
} AGLBlendEquation;

typedef enum {
	AGLBF_Zero = GL_ZERO, /* dst default */
	AGLBF_One = GL_ONE, /* src default */
	AGLBF_SrcColor = GL_SRC_COLOR,
	AGLBF_OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
	AGLBF_DstColor = GL_DST_COLOR,
	AGLBF_OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
	AGLBF_SrcAlpha = GL_SRC_ALPHA,
	AGLBF_OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
	AGLBF_DstAlpha = GL_DST_ALPHA,
	AGLBF_OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
	AGLBF_Color = GL_CONSTANT_COLOR,
	AGLBF_OneMinusColor = GL_ONE_MINUS_CONSTANT_COLOR,
	AGLBF_Alpha = GL_CONSTANT_ALPHA,
	AGLBF_OneMinusAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
	AGLBF_SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE /* src only */
} AGLBlendFunc;

typedef struct {
	int enable;
	struct { GLclampf r, g, b, a; } color;
	struct {
		AGLBlendEquation rgb, a;
	} equation;
	struct {
		AGLBlendFunc src_rgb, src_a, dst_rgb, dst_a;
	} func;
} AGLBlendParams;

typedef enum {
	AGLCM_Disable = 0,
	AGLCM_Front = GL_FRONT,
	AGLCM_Back = GL_BACK, /* default */
	AGLCM_FrontAndBack = GL_FRONT_AND_BACK
} AGLCullMode;

typedef enum {
	AGLFF_Clockwise = GL_CW,
	AGLFF_CounterClockwise = GL_CCW /* default */
} AGLFrontFace;

/* \todo
typedef struct {
	AGLTexture *color;
	int ncolors;
	int depth;
} AGLFramebufferParams;
*/

/* The entire state required for one draw call */
typedef struct {
	struct {
		AGLAttribute *attribs;
		int nattribs;
		struct {
			GLenum mode;
			GLsizei count;
			GLint first; /* -1 for indexed */
			GLenum index_type;
			const AGLBuffer *index_buffer;
			const GLvoid *indices_ptr;
			AGLCullMode cull_mode;
			AGLFrontFace front_face;
		} draw;
	} src;

	struct {
		AGLProgram program;
		AGLProgramUniform *uniforms;
		int nuniforms;
	} proc;

	struct {
		/* \todo AGLFramebufferParams framebuffer; */
		AGLBlendParams blend;
		AGLDepthParams depth;
	} dst;
} AGLDrawParams;

void aGLDrawParamsSetDefaults(AGLDrawParams *params);
void aGLDraw(const AGLDrawParams *params);

extern const char *a_gl_extensions;
extern char a_gl_error[];

/***************************************************************************************/
/* IMPLEMENTATION BENEATH */
/***************************************************************************************/

#ifdef ATTO_PLATFORM_WINDOWS
#define ATTO__FUNCLIST \
	ATTO__FUNCLIST_DO(PFNGLACTIVETEXTUREPROC, ActiveTexture) \
	ATTO__FUNCLIST_DO(PFNGLCREATESHADERPROC, CreateShader) \
	ATTO__FUNCLIST_DO(PFNGLSHADERSOURCEPROC, ShaderSource) \
	ATTO__FUNCLIST_DO(PFNGLCOMPILESHADERPROC, CompileShader) \
	ATTO__FUNCLIST_DO(PFNGLGENFRAMEBUFFERSPROC, GenFramebuffers) \
	ATTO__FUNCLIST_DO(PFNGLBINDFRAMEBUFFERPROC, BindFramebuffer) \
	ATTO__FUNCLIST_DO(PFNGLFRAMEBUFFERTEXTURE2DPROC, FramebufferTexture2D) \
	ATTO__FUNCLIST_DO(PFNGLUSEPROGRAMPROC, UseProgram) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM1FVPROC, Uniform1fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM2FVPROC, Uniform2fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM3FVPROC, Uniform3fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM4FVPROC, Uniform4fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORMMATRIX2FVPROC, UniformMatrix2fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORMMATRIX3FVPROC, UniformMatrix3fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORMMATRIX4FVPROC, UniformMatrix4fv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM1IPROC, Uniform1i) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM1IVPROC, Uniform1iv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM2IVPROC, Uniform2iv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM3IVPROC, Uniform3iv) \
	ATTO__FUNCLIST_DO(PFNGLUNIFORM4IVPROC, Uniform4iv) \
	ATTO__FUNCLIST_DO(PFNGLCREATEPROGRAMPROC, CreateProgram) \
	ATTO__FUNCLIST_DO(PFNGLATTACHSHADERPROC, AttachShader) \
	ATTO__FUNCLIST_DO(PFNGLBINDATTRIBLOCATIONPROC, BindAttribLocation) \
	ATTO__FUNCLIST_DO(PFNGLLINKPROGRAMPROC, LinkProgram) \
	ATTO__FUNCLIST_DO(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation) \
	ATTO__FUNCLIST_DO(PFNGLDELETESHADERPROC, DeleteShader) \
	ATTO__FUNCLIST_DO(PFNGLGETPROGRAMINFOLOGPROC, GetProgramInfoLog) \
	ATTO__FUNCLIST_DO(PFNGLDELETEPROGRAMPROC, DeleteProgram) \
	ATTO__FUNCLIST_DO(PFNGLGETSHADERIVPROC, GetShaderiv) \
	ATTO__FUNCLIST_DO(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog) \
	ATTO__FUNCLIST_DO(PFNGLGETPROGRAMIVPROC, GetProgramiv) \
	ATTO__FUNCLIST_DO(PFNGLCHECKFRAMEBUFFERSTATUSPROC, CheckFramebufferStatus) \
	ATTO__FUNCLIST_DO(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray) \
	ATTO__FUNCLIST_DO(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer)

#define ATTO__FUNCLIST_DO(T,N) T gl##N = 0;
ATTO__FUNCLIST
#undef ATTO__FUNCLIST_DO

#endif /* ifdef ATTO_PLATFORM_WINDOWS */

#endif /* ifndef __ATTO_GL_H__DECLARED */

#ifdef ATTO_GL_H_IMPLEMENT
#ifdef __ATTO_GL_H_IMPLEMENTED
#error atto_gl.h must be implemented only once
#endif /* ifdef __ATTO_GL_H_IMPLEMENTED */
#define __ATTO_GL_H_IMPLEMENTED

#ifndef ATTO_ASSERT
#define ATTO_ASSERT(cond) \
	if (!(cond)) { \
		aAppDebugPrintf("ERROR @ %s:%d: (%s) failed", __FILE__, __LINE__, #cond); \
		aAppTerminate(-1); \
	}
#endif /* ifndef ATTO_ASSERT */

#ifndef ATTO_GL_ERROR_BUFFER_SIZE
#define ATTO_GL_ERROR_BUFFER_SIZE 1024
#endif /* ifndef ATTO_GL_ERROR_BUFFER_SIZE */

char a_gl_error[ATTO_GL_ERROR_BUFFER_SIZE];

static struct {
	/* \todo GLuint framebuffer;
	AGLFramebufferParams framebuffer; */
	AGLProgram program;
	AGLCullMode cull_mode;
	AGLFrontFace front_face;
	AGLBlendParams blend;
	AGLDepthParams depth;
} a__gl_state;

static GLuint a__GLCreateShader(int type, const char * const *source);
static void a__GLProgramBind(AGLProgram program,
	const AGLProgramUniform *uniforms, int nuniforms);
static void a__GLTextureBind(const AGLTexture *texture, GLint unit);
static void a__GLAttribsBind(const AGLAttribute *attrs, int nattrs, AGLProgram program);
static void a__GLAttribsUnbind(const AGLAttribute *attrs, int nattrs, AGLProgram program);
static void a__GLDepthBind(AGLDepthParams depth);
static void a__GLBlendBind(const AGLBlendParams *blend);
static void	a__GLCullingBind(AGLCullMode cull, AGLFrontFace front);

#ifdef ATTO_PLATFORM_WINDOWS
static PROC a__check_get_proc_address(const char *name) {
	PROC ret = wglGetProcAddress(name);
	ATTO_ASSERT(ret);
	return ret;
}
#endif /* ifdef ATTO_PLATFORM_WINDOWS */

void aGLInit() {
#ifdef ATTO_PLATFORM_WINDOWS
#define ATTO__FUNCLIST_DO(T, N) gl##N = (T)a__check_get_proc_address("gl" #N);
	ATTO__FUNCLIST
#undef ATTO__FUNCLIST_DO
#endif /* ifdef ATTO_PLATFORM_WINDOWS */

	/* default initial GL state */
	a__gl_state.cull_mode = AGLCM_Disable;
	a__gl_state.front_face = AGLFF_CounterClockwise;
	a__gl_state.blend.equation.rgb = a__gl_state.blend.equation.a = AGLBE_Add;
	a__gl_state.blend.func.src_rgb = a__gl_state.blend.func.src_a = AGLBF_One;
	a__gl_state.blend.func.dst_rgb = a__gl_state.blend.func.dst_a = AGLBF_Zero;
	a__gl_state.depth.mode = AGLDM_Disabled;
	a__gl_state.depth.func = AGLDF_Less;
}

GLint aGLProgramCreate(const char * const *vertex, const char * const *fragment) {
	GLuint program;
	GLuint vertex_shader, fragment_shader;
	fragment_shader = a__GLCreateShader(GL_FRAGMENT_SHADER, fragment);
	if (fragment_shader == 0)
		return -1;

	vertex_shader = a__GLCreateShader(GL_VERTEX_SHADER, vertex);
	if (vertex_shader == 0) {
		glDeleteShader(fragment_shader);
		return -2;
	}

	program = glCreateProgram();
	glAttachShader(program, fragment_shader);
	glAttachShader(program, vertex_shader);
	glLinkProgram(program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	{
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			glGetProgramInfoLog(program, sizeof(a_gl_error), 0, a_gl_error);
			glDeleteProgram(program);
			return -3;
		}
	}

	return program;
}

GLint aGLProgramCreateSimple(const char *vertex, const char *fragment) {
	const char *vvertex[2] = { 0 };
	const char *vfragment[2] = { 0 };
	vvertex[0] = vertex;
	vfragment[0] = fragment;
	return aGLProgramCreate(vvertex, vfragment);
}

AGLTexture aGLTextureCreate(void) {
	AGLTexture tex;
	glGenTextures(1, &tex.name);
	tex.width = tex.height = 0;
	tex.format = AGLTF_Unknown;
	return tex;
}

void aGLTextureUpload(AGLTexture *tex,
		AGLTextureFormat aformat, int width, int height, const void *pixels) {
	GLenum internal, format, type;
	switch (aformat) {
		case AGLTF_U8_R:
			internal = format = GL_LUMINANCE; type = GL_UNSIGNED_BYTE; break;
		case AGLTF_U8_RA:
			internal = format = GL_LUMINANCE_ALPHA; type = GL_UNSIGNED_BYTE; break;
		case AGLTF_U8_RGB:
			internal = format = GL_RGB; type = GL_UNSIGNED_BYTE; break;
		case AGLTF_U8_RGBA:
			internal = format = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
		case AGLTF_U565_RGB:
			internal = format = GL_RGB; type = GL_UNSIGNED_SHORT_5_6_5; break;
		case AGLTF_U5551_RGBA:
			internal = format = GL_RGBA; type = GL_UNSIGNED_SHORT_5_5_5_1; break;
		case AGLTF_U4444_RGBA:
			internal = format = GL_RGBA; type = GL_UNSIGNED_SHORT_4_4_4_4; break;
		default: ATTO_ASSERT(!"Unknown format");
	}
	glBindTexture(GL_TEXTURE_2D, tex->name);
	glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0,
		format, type, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	tex->width = width;
	tex->height = height;
	tex->format = format;
}

AGLBuffer aGLBufferCreate(AGLBufferType type) {
	AGLBuffer buf;
	glGenBuffers(1, &buf.name);
	buf.type = type;
	return buf;
}

void aGLBufferUpload(AGLBuffer *buffer, GLsizei size, const void *data) {
	glBindBuffer(buffer->type, buffer->name);
	glBufferData(buffer->type, size, data, GL_STATIC_DRAW);
}

void aGLDrawParamsSetDefaults(AGLDrawParams *params) {
	params->src.draw.cull_mode = AGLCM_Disable;
	params->src.draw.front_face = AGLFF_CounterClockwise;
	params->dst.blend.enable = 0;
	params->dst.blend.color.r =
		params->dst.blend.color.g =
		params->dst.blend.color.b =
		params->dst.blend.color.a = 0;
	params->dst.blend.equation.rgb = params->dst.blend.equation.a = AGLBE_Add;
	params->dst.blend.func.src_rgb = params->dst.blend.func.src_a = AGLBF_One;
	params->dst.blend.func.dst_rgb = params->dst.blend.func.dst_a = AGLBF_Zero;
	params->dst.depth.mode = AGLDM_Disabled;
	params->dst.depth.func = AGLDF_Less;
}

void aGLDraw(const AGLDrawParams *p) {
	/* \todo a__GLFramebufferBind(&p->dst.framebuffer); */
	a__GLDepthBind(p->dst.depth);
	a__GLBlendBind(&p->dst.blend);

	a__GLProgramBind(p->proc.program, p->proc.uniforms, p->proc.nuniforms);
	a__GLAttribsBind(p->src.attribs, p->src.nattribs, p->proc.program);
	a__GLCullingBind(p->src.draw.cull_mode, p->src.draw.front_face);
	if (p->src.draw.first < 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->src.draw.index_buffer->name);
		glDrawElements(p->src.draw.mode, p->src.draw.count,
			p->src.draw.index_type, p->src.draw.indices_ptr);
	} else
		glDrawArrays(p->src.draw.mode, p->src.draw.first, p->src.draw.count);

	a__GLAttribsUnbind(p->src.attribs, p->src.nattribs, p->proc.program);
}

static GLuint a__GLCreateShader(int type, const char * const *source) {
	int n;
	GLuint shader = glCreateShader(type);

	for (n = 0; source[n] != 0; ++n);

	glShaderSource(shader, n, (const GLchar **)source, 0);
	glCompileShader(shader);

	{
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			glGetShaderInfoLog(shader, sizeof(a_gl_error), 0, a_gl_error);
			glDeleteShader(shader);
			shader = 0;
		}
	}

	return shader;
}

void a__GLProgramBind(AGLProgram program,
		const AGLProgramUniform *uniforms, int nuniforms) {
	int i, texture_unit = 0;
	glUseProgram(program);
	for (i = 0; i < nuniforms; ++i) {
		int loc = glGetUniformLocation(program, uniforms[i].name);
		if (loc == -1) continue;
		switch(uniforms[i].type) {
			case AGLAT_Float:
				glUniform1fv(loc, uniforms[i].count, uniforms[i].value.pf);
				break;
			case AGLAT_Vec2:
				glUniform2fv(loc, uniforms[i].count, uniforms[i].value.pf);
				break;
			case AGLAT_Vec3:
				glUniform3fv(loc, uniforms[i].count, uniforms[i].value.pf);
				break;
			case AGLAT_Vec4:
				glUniform4fv(loc, uniforms[i].count, uniforms[i].value.pf);
				break;
			case AGLAT_Mat2:
				glUniformMatrix2fv(loc, uniforms[i].count, GL_FALSE, uniforms[i].value.pf);
				break;
			case AGLAT_Mat3:
				glUniformMatrix3fv(loc, uniforms[i].count, GL_FALSE, uniforms[i].value.pf);
				break;
			case AGLAT_Mat4:
				glUniformMatrix4fv(loc, uniforms[i].count, GL_FALSE, uniforms[i].value.pf);
				break;
			case AGLAT_Int:
				glUniform1iv(loc, uniforms[i].count, uniforms[i].value.pi);
				break;
			case AGLAT_IVec2:
				glUniform2iv(loc, uniforms[i].count, uniforms[i].value.pi);
				break;
			case AGLAT_IVec3:
				glUniform3iv(loc, uniforms[i].count, uniforms[i].value.pi);
				break;
			case AGLAT_IVec4:
				glUniform4iv(loc, uniforms[i].count, uniforms[i].value.pi);
				break;
			case AGLAT_Texture:
				a__GLTextureBind(uniforms[i].value.texture, texture_unit);
				glUniform1i(loc, texture_unit);
				++texture_unit;
		}
	}
}

static void a__GLTextureBind(const AGLTexture *texture, GLint unit) {
	glActiveTexture(GL_TEXTURE0  + unit);
	glBindTexture(GL_TEXTURE_2D, texture->name);
}

static void a__GLAttribsBind(const AGLAttribute *attribs, int nattribs,
		AGLProgram program) {
	int i;
	for (i = 0; i < nattribs; ++i) {
		const AGLAttribute *a = attribs + i;
		GLint loc = glGetAttribLocation(program, a->name);
		if (loc < 0) continue;
		glBindBuffer(GL_ARRAY_BUFFER, a->buffer ? a->buffer->name : 0);
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer(loc, a->size, a->type, a->normalized, a->stride, a->ptr);
	}
}

static void a__GLAttribsUnbind(const AGLAttribute *attribs, int nattribs,
		AGLProgram program) {
	int i;
	for (i = 0; i < nattribs; ++i) {
		const AGLAttribute *a = attribs + i;
		GLint loc = glGetAttribLocation(program, a->name);
		if (loc < 0) continue;
		glDisableVertexAttribArray(loc);
	}
}

static void a__GLDepthBind(AGLDepthParams depth) {
	AGLDepthParams *cur = &a__gl_state.depth;
	if (depth.mode != cur->mode) {
		if (depth.mode == AGLDM_Disabled) {
			glDisable(GL_DEPTH_TEST);
			cur->mode = AGLDM_Disabled;
			return;
		}

		if (cur->mode == AGLDM_Disabled)
			glEnable(GL_DEPTH_TEST);

		glDepthMask(depth.mode == AGLDM_TestAndWrite);

		cur->mode = depth.mode;
	}

	if (depth.func != cur->func)
		glDepthFunc(cur->func = depth.func);
}

static void a__GLBlendBind(const AGLBlendParams *blend) {
	AGLBlendParams *cur = &a__gl_state.blend;
	if (blend->enable != cur->enable) {
		cur->enable = blend->enable;
		if (!blend->enable) {
			glDisable(GL_BLEND);
			return;
		}

		glEnable(GL_BLEND);
	}

	if (blend->color.r != cur->color.r || blend->color.g != cur->color.g ||
			blend->color.b != cur->color.b || blend->color.a != cur->color.a) {
		cur->color = blend->color;
		glBlendColor(cur->color.r, cur->color.g, cur->color.b, cur->color.a);
	}

	if (blend->equation.rgb != cur->equation.rgb
			|| blend->equation.a != cur->equation.a) {
		cur->equation = blend->equation;
		if (cur->equation.rgb == cur->equation.a)
			glBlendEquation(cur->equation.rgb);
		else
			glBlendEquationSeparate(cur->equation.rgb, cur->equation.a);
	}

	if (blend->func.src_rgb != cur->func.src_rgb
			|| blend->func.dst_rgb != cur->func.dst_rgb
			|| blend->func.src_a != cur->func.src_a
			|| blend->func.dst_a != cur->func.dst_a) {
		cur->func = blend->func;
		if (cur->func.src_rgb == cur->func.src_a && cur->func.dst_rgb == cur->func.dst_a)
			glBlendFunc(cur->func.src_rgb, cur->func.dst_rgb);
		else
			glBlendFuncSeparate(cur->func.src_rgb, cur->func.dst_rgb,
				cur->func.src_a, cur->func.dst_a);
	}
}

static void	a__GLCullingBind(AGLCullMode cull, AGLFrontFace front) {
	if (cull != a__gl_state.cull_mode) {
		if (cull == AGLCM_Disable) {
			glDisable(GL_CULL_FACE);
			return;
		}

		if (a__gl_state.cull_mode == AGLCM_Disable)
			glEnable(GL_CULL_FACE);

		glCullFace(a__gl_state.cull_mode = cull);
	}

	if (front != a__gl_state.front_face)
		glFrontFace(a__gl_state.front_face = front);
}

#endif /* ATTO_GL_H_IMPLEMENT */
