#pragma once
#include <iostream>     // cout, cerr
#include <cstdlib>      // EXIT_FAILURE
#include <GL/glew.h>    // GLEW library
#include <GLFW/glfw3.h> // GLFW library
#include <camera.h>     //camera library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // Image loading utility functions


// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const float DEG2RAD = 3.14159 / 180;

    const char* const WINDOW_TITLE = "Module Five Milestone"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        // Handle for plane
        GLuint planeVao;
        GLuint planeVbo;
        GLuint nPlaneVertices;

        // Handle for bottle
        GLuint bottleVao;
        GLuint bottleVbos;
        GLuint nBottleVertices;

        // Handle for bottleneck
        GLuint bottleNeckVao;
        GLuint bottleNeckVbo;
        GLuint nBottleNeckVertices;
        GLuint nBottleNeckIndices;

        // Spatula handle
        GLuint spatulaHandleVao;
        GLuint spatulaHandleVbo;
        GLuint nSpatulaHandleVertices;

        // Spatula flat top
        GLuint spatulaTopVao;
        GLuint spatulaTopVbo;
        GLuint nSpatulaTopVertices;

        // Pot holder
        GLuint potHolderVao;
        GLuint potHolderVbo;
        GLuint nPotHolderVertices;

        // Salt Shaker
        GLuint saltShakerVao;
        GLuint saltShakerVbo;
        GLuint nSaltShakerVertices;

        // Pepper shaker 
        GLuint pepperShakerVao;
        GLuint pepperShakerVbo;
        GLuint nPepperShakerVertices;

        // Watermelon
        GLuint watermelonVao;
        GLuint watermelonVbo;
        GLuint nWatermelonVertices;

        // Lamp
        GLuint lampVao;
        GLuint lampVbo;
        GLuint nLampVertices;
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    // Shader program
    GLuint gProgramId;
    GLuint gLampProgramId;

    // Texture Ids
    GLuint gPlaneTexture;
    GLuint gBottleTexture;
    GLuint gBottleNeckTexture;
    GLuint gSpatulaTexture;
    GLuint gPotHolderTexture;
    GLuint gSaltShakerTexture;
    GLuint gPepperShakerTexture;
    GLuint gWatermelonTexture;

    glm::vec2 gUVScale(5.0f, 5.0f);
    GLint gTexWrapMode = GL_REPEAT;



    // camera
    Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    bool ortho = false;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // Subject position and scale
    glm::vec3 gCubePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 gCubeScale(2.0f);

    // Cube and light color
    glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);
    glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);

    // Light position and scale
    glm::vec3 gLightPosition(2.0f, 2.0f, -5.0f);
    glm::vec3 gLightScale(0.3f);

}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateTexturedMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);


/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader

    out vec4 fragmentColor;

    uniform sampler2D uTexture;

    void main()
    {
        fragmentColor = texture(uTexture, vertexTextureCoordinate);
    }
);

/* Fragment Shader Source Code for Lighted Objects*/
const GLchar* lightFragmentShaderSource = GLSL(440,
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor; // For outgoing cube color to the GPU

    // Uniform / Global variables for object color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D uTexture; // Useful when working with multiple textures
    uniform vec2 uvScale;

    void main()
    {
        /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

        //Calculate Ambient lighting*/
        float ambientStrength = 0.1f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        //Calculate Diffuse lighting*/
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        //Calculate Specular lighting*/
        float specularIntensity = 0.8f; // Set specular light strength
        float highlightSize = 16.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        //Calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // Texture holds the color to be used for all three components
        vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

        // Calculate phong result
        vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

        fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
    }
);


/* Lamp Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

        //Uniform / Global variables for the  transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
    }
);


/* Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

    void main()
    {
        fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
    }
);

const GLchar* spotlightFragmentShaderSource = GLSL(330,

    );

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}
class Circle
{
public:
    float red, green, blue;
    float radius;
    float x;
    float y;
    float speed = 0.01;
    int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left

    Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
    {
        x = xx;
        y = yy;
        radius = rr;
        red = r;
        green = g;
        blue = b;
        radius = rad;
        direction = dir;
    }
    void DrawCircle()
    {
        glColor3f(red, green, blue);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float degInRad = i * DEG2RAD;
            glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
        }
        glEnd();
    }
};

int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateTexturedMesh(gMesh); // Calls the function to create the Vertex Buffer Object

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;


    //Load textures
    const char* texFileName = "../images/countertop.jpg";
    if (!UCreateTexture(texFileName, gPlaneTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/bottle.jpg";
    if (!UCreateTexture(texFileName, gBottleTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/bottleTop.jpg";
    if (!UCreateTexture(texFileName, gBottleNeckTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/spatula.jpg";
    if (!UCreateTexture(texFileName, gSpatulaTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/saltShaker.jpg";
    if (!UCreateTexture(texFileName, gSaltShakerTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/pepperShaker.jpg";
    if (!UCreateTexture(texFileName, gPepperShakerTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/potHolder.jpg";
    if (!UCreateTexture(texFileName, gPotHolderTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }
    texFileName = "../images/watermelon.jpg";
    if (!UCreateTexture(texFileName, gWatermelonTexture)) {
        cout << "Failed to load texture " << texFileName << endl;
        return EXIT_FAILURE;
    }

    glUseProgram(gProgramId);

    // Set texture units
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePlane"), 0);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureBottle"), 1);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureBottleNeck"), 2);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureSpatula"), 3);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureSaltShaker"), 4);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePepperShaker"), 5);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePotHolder"), 6);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureWatermelon"), 7);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        UProcessInput(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gMesh);

    // Release texture
    UDestroyTexture(gPlaneTexture);
    UDestroyTexture(gBottleTexture);
    UDestroyTexture(gBottleNeckTexture);
    UDestroyTexture(gSpatulaTexture);
    UDestroyTexture(gSaltShakerTexture);
    UDestroyTexture(gPepperShakerTexture);
    UDestroyTexture(gPotHolderTexture);
    UDestroyTexture(gWatermelonTexture);

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Function called to render a frame
void URender()
{

    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Scale, rotate, and translate object
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 4.0f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a perspective projection
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 100.0f, -100.0f, 100.0f);
    if (!ortho) {
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    if (ortho) {
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
    };

    glm::mat4 transformation(1.0f);

    // Set the shader to be used
    glUseProgram(gProgramId);

    GLuint UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //// Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(gProgramId, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformation));

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


    // Plane
    glBindVertexArray(gMesh.planeVao);
    model = glm::mat4(1.0f);

    // Bind textures 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPlaneTexture);

    // Draw Plane
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nPlaneVertices);

    //////////////////////////////////

    // Bottle
    // Scale, rotate, and translate bottle
    glm::mat4 bottleScale = glm::scale(glm::vec3(1.0f, 2.0f, 1.0f));
    glm::mat4 bottleRotation = glm::rotate(-25.0f, glm::vec3(0.0, 1.0f, 0.0f));
    glm::mat4 bottleTranslation = glm::translate(glm::vec3(-2.0f, 0.0f, -1.5f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 bottleModel = bottleTranslation * bottleRotation * bottleScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint bottleModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint bottleViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint bottleProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(bottleModelLoc, 1, GL_FALSE, glm::value_ptr(bottleModel));
    glUniformMatrix4fv(bottleViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(bottleProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draws bottle
    glBindVertexArray(gMesh.bottleVao);
    model = glm::mat4(1.0f);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBottleTexture);

    // Draw bottle
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nBottleVertices);
    /////////////////

    //// Bottle Neck
    // Scale, rotate, and translate bottle neck
    glm::mat4 bottleNeckScale = glm::scale(glm::vec3(0.06f, 0.1f, 0.06f));
    glm::mat4 bottleNeckRotation = glm::rotate(-25.0f, glm::vec3(0.0, 1.0f, 0.0f));
    glm::mat4 bottleNeckTranslation = glm::translate(glm::vec3(-2.05f, 1.0f, -1.5f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 bottleNeckModel = bottleNeckTranslation * bottleNeckRotation * bottleNeckScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint bottleNeckModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint bottleNeckViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint bottleNeckProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(bottleNeckModelLoc, 1, GL_FALSE, glm::value_ptr(bottleNeckModel));
    glUniformMatrix4fv(bottleNeckViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(bottleNeckProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.bottleNeckVao);
    model = glm::mat4(1.0f);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBottleNeckTexture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nBottleNeckVertices);
    /////////////////////////////////

    ////Spatula Handle
    //// ----------------------------------------------------
    // Scale, rotate, and translate
    glm::mat4 spatulaHandleScale = glm::scale(glm::vec3(0.35f, 3.0f, 0.35f));
    glm::mat4 spatulaHandleRotation = glm::rotate(glm::radians(90.0f), glm::vec3(2.0, 0.0f, 0.0f));
    glm::mat4 spatulaHandleRotation1 = glm::rotate(glm::radians(45.0f), glm::vec3(0.0, 0.0f, 2.0f));
    glm::mat4 spatulaHandleTranslation = glm::translate(glm::vec3(-0.5f, -0.8f, 3.0f));
    glm::mat4 spatulaHandleModel = spatulaHandleTranslation * spatulaHandleRotation * spatulaHandleRotation1 * spatulaHandleScale;

    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc1 = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc1 = glGetUniformLocation(gProgramId, "view");
    GLint projLoc1 = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(spatulaHandleModel));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc1, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.spatulaHandleVao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gSpatulaTexture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nSpatulaHandleVertices);
    /////////////////////////

    // Spatula Top
    //----------------------------------------------------
    // Scale, rotate, and translate
    glm::mat4 spatulaTopScale = glm::scale(glm::vec3(1.0f, 1.5f, 0.2f));
    glm::mat4 SpatulaTopRotation = glm::rotate(glm::radians(90.0f), glm::vec3(2.0, 0.0f, 0.0f));
    glm::mat4 SpatulaTopRotation1 = glm::rotate(glm::radians(45.0f), glm::vec3(0.0, 0.0f, 2.0f));
    glm::mat4 spatulaTopTranslation = glm::translate(glm::vec3(1.0f, -0.8f, 1.5f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 spatulaTopModel = spatulaTopTranslation * SpatulaTopRotation * SpatulaTopRotation1 * spatulaTopScale;

    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc2 = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc2 = glGetUniformLocation(gProgramId, "view");
    GLint projLoc2 = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(spatulaTopModel));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.spatulaTopVao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gSpatulaTexture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nSpatulaTopVertices);
    /////////////////////////

    // Salt Shaker
    //----------------------------------------------------
    // Scale, rotate, and translate bottle neck
    glm::mat4 saltShakerScale = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
    glm::mat4 saltShakerRotation = glm::rotate(-25.0f, glm::vec3(0.0, 1.0f, 0.0f));
    glm::mat4 saltShakerTranslation = glm::translate(glm::vec3(3.0f, -1.0f, -1.5f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 saltShakerModel = saltShakerTranslation * saltShakerRotation * saltShakerScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint saltShakerModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint saltShakerViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint saltShakerProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(saltShakerModelLoc, 1, GL_FALSE, glm::value_ptr(saltShakerModel));
    glUniformMatrix4fv(saltShakerViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(saltShakerProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.saltShakerVao);
    model = glm::mat4(1.0f);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gSaltShakerTexture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nSaltShakerVertices);
    /////////////////////////

    // Pepper Shaker
    //----------------------------------------------------
    // Scale, rotate, and translate bottle neck
    glm::mat4 pepperShakerScale = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
    glm::mat4 pepperShakerRotation = glm::rotate(-25.0f, glm::vec3(0.0, 1.0f, 0.0f));
    glm::mat4 pepperShakerTranslation = glm::translate(glm::vec3(2.5f, -1.0f, -3.0));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 pepperShakerModel = pepperShakerTranslation * pepperShakerRotation * pepperShakerScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint pepperShakerModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint pepperShakerViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint pepperShakerProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(pepperShakerModelLoc, 1, GL_FALSE, glm::value_ptr(pepperShakerModel));
    glUniformMatrix4fv(pepperShakerViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(pepperShakerProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.pepperShakerVao);
    model = glm::mat4(1.0f);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPepperShakerTexture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nPepperShakerVertices);
    /////////////////////////

    // Pot Holder
    //----------------------------------------------------
    // Scale, rotate, and translate bottle neck
    glm::mat4 potHolderScale = glm::scale(glm::vec3(4.25f, 0.1f, 5.5f));
    glm::mat4 potHolderRotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 potHolderTranslation = glm::translate(glm::vec3(0.5f, -1.0f, -1.0f));
    glm::mat4 potHolderModel = potHolderTranslation * potHolderRotation * potHolderScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint potHoldermodelLoc = glGetUniformLocation(gProgramId, "model");
    GLint potHolderviewLoc = glGetUniformLocation(gProgramId, "view");
    GLint potHolderprojLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(potHoldermodelLoc, 1, GL_FALSE, glm::value_ptr(potHolderModel));
    glUniformMatrix4fv(potHolderviewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(potHolderprojLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.potHolderVao);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPotHolderTexture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nPotHolderVertices);
    ////////////////////////////
    // Lamp
    /*glUseProgram(gLampProgramId);*/

    glm::mat4 lampModel = glm::translate(gLightPosition) * glm::scale(gLightScale);

    // Reference matrix uniforms from the Lamp Shader program
    GLint lampModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint lampViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint lampProjLoc = glGetUniformLocation(gProgramId, "projection");

    // Pass matrix data to the Lamp Shader program's matrix uniforms
    glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(lampModel));
    glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(lampProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, gSaltShakerTexture);
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nLampVertices);


    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Implements the UCreateMesh function
void UCreateTexturedMesh(GLMesh& mesh)
{

    GLfloat planeVerts[] = {
        // Vertex Positions    // Texture coordinates
        -5.0f, -5.0f, -5.0f,   0.0f, 0.0f,
         5.0f, -5.0f, -5.0f,   1.0f, 0.0f,
         5.0f, -5.0f,  5.0f,   1.0f, 1.0f,
         5.0f, -5.0f,  5.0f,   1.0f, 1.0f,
        -5.0f, -5.0f,  5.0f,   0.0f, 1.0f,
        -5.0f, -5.0f, -5.0f,   0.0f, 0.0f

    };

    GLfloat cubeVerts[] = {
        // Vertex Positions    // Texture coordinates
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f

    };


    GLfloat cylinderVerts[] = {
        -3.0f, 0.0f,  3.0f,     0.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  4.0f,     1.0f, 1.0f,
         0.0f, 0.0f,  4.0f,     1.0f, 1.0f,
         0.0f, 0.0f,  0.0f,     0.0f, 1.0f,
         3.0f, 0.0f,  3.0f,     0.0f, 0.0f,
         3.0f, 0.0f,  3.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         4.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         4.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         3.0f, 0.0f, -3.0f,     1.0f, 0.0f,
         3.0f, 0.0f, -3.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         0.0f, 0.0f, -4.0f,     1.0f, 0.0f,
         0.0f, 0.0f, -4.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        -3.0f, 0.0f, -3.0f,     1.0f, 0.0f,
        -3.0f, 0.0f, -3.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        -4.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        -4.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        -3.0f, 0.0f,  3.0f,     1.0f, 0.0f,



        -3.0f, 10.0f, 3.0f,     0.0f, 0.0f,
        -3.0f, 0.0f,  3.0f,     1.0f, 0.0f,
         0.0f, 0.0f,  4.0f,     1.0f, 1.0f,
         0.0f, 0.0f,  4.0f,     1.0f, 1.0f,
         0.0f, 10.0f, 4.0f,     0.0f, 1.0f,
        -3.0f, 10.0f, 3.0f,     0.0f, 0.0f,

         0.0f, 10.0f, 4.0f,     0.0f, 0.0f,
         0.0f, 0.0f,  4.0f,     1.0f, 0.0f,
         3.0f, 0.0f,  3.0f,     1.0f, 1.0f,
         3.0f, 0.0f,  3.0f,     1.0f, 1.0f,
         3.0f, 10.0f, 3.0f,     0.0f, 1.0f,
         0.0f, 10.0f, 4.0f,     0.0f, 0.0f,

         3.0f, 10.0f, 3.0f,     0.0f, 0.0f,
         3.0f, 0.0f,  3.0f,     1.0f, 0.0f,
         4.0f, 0.0f,  0.0f,     1.0f, 1.0f,
         4.0f, 0.0f,  0.0f,     1.0f, 1.0f,
         4.0f, 10.0f, 0.0f,     0.0f, 1.0f,
         3.0f, 10.0f, 3.0f,     0.0f, 0.0f,

         4.0f, 10.0f, 0.0f,     0.0f, 0.0f,
         4.0f, 0.0f,  0.0f,     1.0f, 0.0f,
         3.0f, 0.0f,  -3.0f,    1.0f, 1.0f,
         3.0f, 0.0f,  -3.0f,    1.0f, 1.0f,
         3.0f, 10.0f, -3.0f,    0.0f, 1.0f,
         4.0f, 10.0f,  0.0f,    0.0f, 0.0f,

         3.0f, 10.0f, -3.0f,    0.0f, 0.0f,
         3.0f, 0.0f,  -3.0f,    1.0f, 0.0f,
         0.0f, 0.0f,  -4.0f,    1.0f, 1.0f,
         0.0f, 0.0f,  -4.0f,    1.0f, 1.0f,
         0.0f, 10.0f, -4.0f,    0.0f, 1.0f,
         3.0f, 10.0f, -3.0f,    0.0f, 0.0f,

         0.0f, 10.0f, -4.0f,    0.0f, 0.0f,
         0.0f, 0.0f,  -4.0f,    1.0f, 0.0f,
        -3.0f, 0.0f,  -3.0f,    1.0f, 1.0f,
        -3.0f, 0.0f,  -3.0f,    1.0f, 1.0f,
        -3.0f, 10.0f, -3.0f,    0.0f, 1.0f,
         0.0f, 10.0f, -4.0f,    0.0f, 0.0f,

        -3.0f, 10.0f, -3.0f,    0.0f, 0.0f,
        -3.0f, 0.0f,  -3.0f,    1.0f, 0.0f,
        -4.0f, 0.0f,  0.0f,     1.0f, 1.0f,
        -4.0f, 0.0f,  0.0f,     1.0f, 1.0f,
        -4.0f, 10.0f, 0.0f,     0.0f, 1.0f,
        -3.0f, 10.0f, -3.0f,    0.0f, 0.0f,

        -4.0f, 10.0f, 0.0f,     0.0f, 0.0f,
        -4.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        -3.0f, 0.0f,  3.0f,     1.0f, 1.0f,
        -3.0f, 0.0f,  3.0f,     1.0f, 1.0f,
        -3.0f, 10.0f, 3.0f,     0.0f, 1.0f,
        -4.0f, 10.0f, 0.0f,     0.0f, 0.0f,



        -3.0f, 10.0f,  3.0f,     0.0f, 0.0f,
         0.0f, 10.0f,  0.0f,     1.0f, 0.0f,
         0.0f, 10.0f,  4.0f,     1.0f, 1.0f,
         0.0f, 10.0f,  4.0f,     1.0f, 1.0f,
         0.0f, 10.0f,  0.0f,     0.0f, 1.0f,
         3.0f, 10.0f,  3.0f,     0.0f, 0.0f,
         3.0f, 10.0f,  3.0f,     0.0f, 0.0f,
         0.0f, 10.0f,  0.0f,     1.0f, 0.0f,
         4.0f, 10.0f,  0.0f,     1.0f, 1.0f,
         4.0f, 10.0f,  0.0f,     1.0f, 1.0f,
         0.0f, 10.0f,  0.0f,     0.0f, 1.0f,
         3.0f, 10.0f, -3.0f,     0.0f, 0.0f,
         3.0f, 10.0f, -3.0f,     0.0f, 0.0f,
         0.0f, 10.0f,  0.0f,     1.0f, 0.0f,
         0.0f, 10.0f, -4.0f,     1.0f, 1.0f,
         0.0f, 10.0f, -4.0f,     1.0f, 1.0f,
         0.0f, 10.0f,  0.0f,     0.0f, 1.0f,
        -3.0f, 10.0f, -3.0f,     0.0f, 0.0f,
        -3.0f, 10.0f, -3.0f,     0.0f, 0.0f,
         0.0f, 10.0f,  0.0f,     1.0f, 0.0f,
        -4.0f, 10.0f,  0.0f,     1.0f, 1.0f,
        -4.0f, 10.0f,  0.0f,     1.0f, 1.0f,
         0.0f, 10.0f,  0.0f,     0.0f, 1.0f,
        -3.0f, 10.0f,  3.0f,     0.0f, 0.0f,
    };

    // Position and Color data
    GLfloat verts[] = {
        //Positions          //Normals
        // ------------------------------------------------------
        //Back Face          //Negative Z Normal  Texture Coords.
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       //Front Face         //Positive Z Normal
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

      //Left Face          //Negative X Normal
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Right Face         //Positive X Normal
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Bottom Face        //Negative Y Normal
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    //Top Face           //Positive Y Normal
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };




    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    // Strides between vertex coordinates 
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);

    // Plane
    // --------------------------
    mesh.nPlaneVertices = sizeof(planeVerts) / (sizeof(planeVerts[0]) * (floatsPerVertex + floatsPerUV));
    glGenVertexArrays(1, &mesh.planeVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.planeVao);

    // Create VBO
    glGenBuffers(1, &mesh.planeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.planeVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts), planeVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    ////////////////////////////////

    // Bottle
    // --------------------------
    mesh.nBottleVertices = sizeof(cubeVerts) / (sizeof(cubeVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.bottleVao);
    glBindVertexArray(mesh.bottleVao);

    //Create VBO
    glGenBuffers(1, &mesh.bottleVbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.bottleVbos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    /////////////////

    // Bottleneck
    // --------------------------
    mesh.nBottleNeckVertices = sizeof(cylinderVerts) / (sizeof(cylinderVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.bottleNeckVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.bottleNeckVao);


    // Create VBO
    glGenBuffers(1, &mesh.bottleNeckVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.bottleNeckVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVerts), cylinderVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    ////////////////////

    // Spatula Handle
    // --------------------------
    mesh.nSpatulaHandleVertices = sizeof(cubeVerts) / (sizeof(cubeVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.spatulaHandleVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.spatulaHandleVao);

    // Create VBO
    glGenBuffers(1, &mesh.spatulaHandleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.spatulaHandleVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    /////////////////

    // Spatula top
    mesh.nSpatulaTopVertices = sizeof(cubeVerts) / (sizeof(cubeVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.spatulaTopVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.spatulaTopVao);

    // Create VBO
    glGenBuffers(1, &mesh.spatulaTopVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.spatulaTopVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU


    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    /////////////////

    // Salt Shaker
    // --------------------------
    mesh.nSaltShakerVertices = sizeof(cylinderVerts) / (sizeof(cylinderVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.saltShakerVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.saltShakerVao);


    // Create VBO
    glGenBuffers(1, &mesh.saltShakerVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.saltShakerVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVerts), cylinderVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    ////////////////////

    // Pepper Shaker
    // --------------------------
    mesh.nPepperShakerVertices = sizeof(cylinderVerts) / (sizeof(cylinderVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.pepperShakerVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.pepperShakerVao);


    // Create VBO
    glGenBuffers(1, &mesh.pepperShakerVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.pepperShakerVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVerts), cylinderVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    ////////////////////

    // PotHolder
    mesh.nPotHolderVertices = sizeof(cubeVerts) / (sizeof(cubeVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.potHolderVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.potHolderVao);


    // Create VBO
    glGenBuffers(1, &mesh.potHolderVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.potHolderVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);

    // Lamp 1
    
    mesh.nLampVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.lampVao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.lampVao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.lampVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.lampVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    GLint stride1 = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride1, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride1, (void*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride1, (void*)(sizeof(float)* (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}



void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.planeVao);
    glDeleteBuffers(1, &mesh.planeVbo);

    glDeleteVertexArrays(1, &mesh.bottleVao);
    glDeleteBuffers(2, &mesh.bottleVbos);

    glDeleteVertexArrays(1, &mesh.bottleNeckVao);
    glDeleteBuffers(1, &mesh.bottleNeckVbo);

    glDeleteVertexArrays(1, &mesh.spatulaHandleVao);
    glDeleteBuffers(1, &mesh.spatulaHandleVbo);

    glDeleteVertexArrays(1, &mesh.lampVao);
    glDeleteBuffers(1, &mesh.lampVbo);
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}

// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

    // If window creatin fails, terminate 
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;

    // Terminates program if escape pressed 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Moves left, right, forward, and backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);

    // Moves camera up and down
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);
    }

    // Changes orthographic/perspective camera view
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        ortho = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        ortho = false;
    }

    //reset camera to default speed and setting
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        gCamera = glm::vec3(0.0f, 0.0f, 3.0f);
        //cameraSpeed = 2.5f;
    }


}


// GLFW: Whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// GLFW: whenever the mouse moves, this callback is called
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// GLFW: whenever the mouse scroll wheel scrolls, this callback is called
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// GLFW: handle mouse button events
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
        // Left mouse button clicked 
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    // Middle mouse button clicked 
    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    // Right mouse button clicked 
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    // Unknown mouse button clicked
    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}
