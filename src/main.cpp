#include <Octree.h>
#include <happly.h>
#include <util.h>
#include <iostream>

extern float deltaTime;
extern float lastFrame;
extern Camera camera;

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

const static char *vs = "#version 330 core\n"
                        "layout (location = 0) in vec3 aPos;\n"
                        ""
                        "uniform mat4 model;\n"
                        "uniform mat4 view;\n"
                        "uniform mat4 projection;\n"
                        "uniform mat4 transform;\n"
                        ""
                        "void main()\n"
                        "{"
                        "   gl_Position = projection * view * transform * model * vec4(aPos, 1.0f);"
                        "}";

const static char *fs = "#version 330 core\n"
                        "out vec4 FragColor;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "   FragColor = vec4(0.5f, 0.3f, 1.0f, 1.0f);\n"
                        "}";

int main()
{
    Octree::Octree<> tree(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    happly::PLYData plyIn("./resources/bunny.ply", true);

    vector<float> y = plyIn.getElement("vertex").getProperty<float>("y");
    vector<float> z = plyIn.getElement("vertex").getProperty<float>("z");
    vector<float> x = plyIn.getElement("vertex").getProperty<float>("x");

    float vertices[x.size() * 3];

    for (size_t index = 0 ; index < plyIn.getElement("vertex").count; ++index)
    {
        vertices[index * 3] = x[index];
        vertices[index * 3 + 1] = y[index];
        vertices[index * 3 + 2] = z[index];

        Octree::Octree<>::Node node(Octree::Octree<>::Leave(x[index], y[index], z[index]));

        tree.insert(node);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader(vs, fs);
    
    unsigned int VAO;
    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    float currentFrame;
    glm::mat4 trans = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMatrix4fv("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMatrix4fv("view", view);

        shader.setMatrix4fv("model", glm::mat4(1.0f));

        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::translate(trans, glm::vec3(0.01f, 0.0f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMatrix4fv("transform", glm::mat4(1.0f));

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, x.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
