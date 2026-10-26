#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <time.h>
#include <math.h>

#define TAG "ThiefAndroid"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static GLuint g_program = 0;
static GLint g_attrPos = -1;
static GLint g_attrColor = -1;
static GLint g_uniAngle = -1;
static GLint g_uniAspect = -1;
static float g_aspect = 1.0f;

static const char* kVertexSrc =
    "attribute vec2 aPos;\n"
    "attribute vec3 aColor;\n"
    "uniform float uAngle;\n"
    "uniform float uAspect;\n"
    "varying vec3 vColor;\n"
    "void main() {\n"
    "  float c = cos(uAngle), s = sin(uAngle);\n"
    "  vec2 p = vec2(aPos.x * c - aPos.y * s, aPos.x * s + aPos.y * c);\n"
    "  p.x /= uAspect;\n"
    "  gl_Position = vec4(p, 0.0, 1.0);\n"
    "  vColor = aColor;\n"
    "}\n";

static const char* kFragmentSrc =
    "precision mediump float;\n"
    "varying vec3 vColor;\n"
    "void main() { gl_FragColor = vec4(vColor, 1.0); }\n";

static GLuint compileShader(GLenum type, const char* src) {
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);
    GLint ok = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[512];
        glGetShaderInfoLog(sh, sizeof(buf), nullptr, buf);
        LOGE("Shader compile failed: %s", buf);
        glDeleteShader(sh);
        return 0;
    }
    return sh;
}

static double nowSeconds() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

extern "C" {

// Kept from the original skeleton so existing references still link.
__attribute__((visibility("default")))
void thief_android_init() {
    LOGI("Fresh Thief Android native runtime initialized.");
}

JNIEXPORT void JNICALL
Java_com_example_thiefandroid_MainActivity_nativeInit(JNIEnv*, jclass) {
    thief_android_init();
}

JNIEXPORT void JNICALL
Java_com_example_thiefandroid_MainActivity_nativeSurfaceCreated(JNIEnv*, jclass) {
    LOGI("GL_VERSION: %s", (const char*)glGetString(GL_VERSION));
    LOGI("GL_RENDERER: %s", (const char*)glGetString(GL_RENDERER));

    GLuint vs = compileShader(GL_VERTEX_SHADER, kVertexSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, kFragmentSrc);
    if (!vs || !fs) return;

    g_program = glCreateProgram();
    glAttachShader(g_program, vs);
    glAttachShader(g_program, fs);
    glLinkProgram(g_program);

    GLint ok = 0;
    glGetProgramiv(g_program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[512];
        glGetProgramInfoLog(g_program, sizeof(buf), nullptr, buf);
        LOGE("Program link failed: %s", buf);
        g_program = 0;
        return;
    }
    g_attrPos = glGetAttribLocation(g_program, "aPos");
    g_attrColor = glGetAttribLocation(g_program, "aColor");
    g_uniAngle = glGetUniformLocation(g_program, "uAngle");
    g_uniAspect = glGetUniformLocation(g_program, "uAspect");
    LOGI("Renderer ready.");
}

JNIEXPORT void JNICALL
Java_com_example_thiefandroid_MainActivity_nativeSurfaceChanged(JNIEnv*, jclass, jint w, jint h) {
    glViewport(0, 0, w, h);
    g_aspect = (h > 0) ? (float)w / (float)h : 1.0f;
    LOGI("Surface changed: %dx%d", w, h);
}

JNIEXPORT void JNICALL
Java_com_example_thiefandroid_MainActivity_nativeDrawFrame(JNIEnv*, jclass) {
    glClearColor(0.05f, 0.07f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (!g_program) return;

    static const GLfloat verts[] = {
        // x,     y,     r,    g,    b
         0.0f,  0.6f,  1.0f, 0.3f, 0.3f,
        -0.5f, -0.4f,  0.3f, 1.0f, 0.3f,
         0.5f, -0.4f,  0.3f, 0.4f, 1.0f,
    };

    glUseProgram(g_program);
    glUniform1f(g_uniAngle, (float)fmod(nowSeconds(), 2.0 * M_PI));
    glUniform1f(g_uniAspect, g_aspect);
    glVertexAttribPointer(g_attrPos, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), verts);
    glEnableVertexAttribArray(g_attrPos);
    glVertexAttribPointer(g_attrColor, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), verts + 2);
    glEnableVertexAttribArray(g_attrColor);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

} // extern "C"
