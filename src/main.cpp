#include <Octree.h>
#include <happly.h>
#include <util.h>
#include <iostream>

extern float deltaTime;
extern float lastFrame;
extern Camera camera;

using glm::vec3;
using glm::mat4;
using glm::translate;

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
                        ""
                        "void main()\n"
                        "{"
                        "   gl_Position = projection * view * model * vec4(aPos, 1.0f);"
                        "}";

const static char *fs = "#version 330 core\n"
                        "out vec4 FragColor;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "   FragColor = vec4(0.5f, 0.3f, 1.0f, 1.0f);\n"
                        "}";


const static char *point_fs = "#version 330 core\n"
                        "out vec4 FragColor;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "   FragColor = vec4(0.8f, 0.0f, 0.0f, 1.0f);\n"
                        "}";

const unsigned int getCircularConeVAO();

const vec3 random_in_unit_sphere();

bool other_detection(const float x, const float y, const float z, const float radius);

vector<float> x;
vector<float> y;
vector<float> z;

int main()
{
    Octree::Octree<> tree(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    happly::PLYData plyIn("./resources/bunny.ply", true);

    x = plyIn.getElement("vertex").getProperty<float>("x");
    y = plyIn.getElement("vertex").getProperty<float>("y");
    z = plyIn.getElement("vertex").getProperty<float>("z");

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Octree", NULL, NULL);
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
    Shader point_shader(vs, point_fs);

    const unsigned int circularConeVAO = getCircularConeVAO();
    
    unsigned int VAO;
    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    const mat4 bunny_model = translate(mat4(1.0f), vec3(0.0f, -0.0936f, 0.0f));
    const mat4 other_bunny_model = translate(mat4(1.0f), vec3(2.0f, -0.0936f, 0.0f));

    float currentFrame;

    vec3 random_sphere_point[20];

    mat4 random_sphere_point_model[20];
    mat4 other_random_sphere_point_model[20];

    vec3 crosses[20];

    float angles[20];

    bool flag[20];
    bool other_flag[20];

    float log[20];
    float other_log[20];

    vector<float> one_log;
    vector<float> other_one_log;

    const vec3 point(0.0f, 0.0f, 1.0f);

    for (size_t i = 0; i < 20; ++i)
    {
        log[i] = other_log[i] = 0;
        flag[i] = false;
        const vec3 & tmp = random_in_unit_sphere();
        const vec3 normal = -glm::normalize(tmp);

        angles[i] = glm::radians(-acos(normal.z) * (180/M_PI));

        random_sphere_point[i] = normal * 0.001f;

        random_sphere_point_model[i] = translate(mat4(1.0f), tmp);
        other_random_sphere_point_model[i] = translate(random_sphere_point_model[i], vec3(2.0f, 0.0f, 0.0f));

        crosses[i] = glm::cross(normal, point);
    }


    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();

        point_shader.use();
        point_shader.setMatrix4fv("projection", projection);
        point_shader.setMatrix4fv("view", view);

        for (size_t i = 0; i < 20; ++i)
        {
            if (flag[i]) continue;

            mat4 & model = random_sphere_point_model[i];

            model = translate(model, random_sphere_point[i]);
            try
            {
                float time = glfwGetTime();
                if (tree.detection(model[3][0], model[3][1]+0.0936f, model[3][2], 0.05f))
                {
                    time = glfwGetTime() - time;
                    log[i] += time;
                    flag[i] = true;
                    cout << i << ": "
                            << "X: " << model[3][0] 
                            << "\tY: " << model[3][1]
                            << "\tZ: " << model[3][2] << endl;    
                }
                else
                {
                    time = glfwGetTime() - time;
                    log[i] += time;
                }
                one_log.push_back(time);

            }
            catch(Octree::OctreeExpection &e)
            {
                cerr << e.what() << endl;
                flag[i] = true;
                // glfwTerminate();
                // return -1;
            }

            const mat4 tmp = glm::rotate(model, angles[i], crosses[i]);
            shader.setMatrix4fv("model", tmp);

            glBindVertexArray(circularConeVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 198);
        }

        for (size_t i = 0; i < 20; ++i)
        {
            if (other_flag[i]) continue;

            mat4 & model = other_random_sphere_point_model[i];

            model = translate(model, random_sphere_point[i]);
            try
            {
                float time = glfwGetTime();
                if (other_detection(model[3][0] - 2.0f, model[3][1]+0.0936f, model[3][2], 0.05f))
                {
                    time = glfwGetTime() - time;
                    other_log[i] += time;
                    other_flag[i] = true;
                    cout << i << ": "
                            << "X: " << model[3][0] 
                            << "\tY: " << model[3][1]
                            << "\tZ: " << model[3][2] << endl;    
                }
                else
                {
                    time = glfwGetTime() - time;
                    other_log[i] += time;
                }
                other_one_log.push_back(time);
            }
            catch(Octree::OctreeExpection &e)
            {
                cerr << e.what() << endl;
                other_flag[i] = true;
                // glfwTerminate();
                // return -1;
            }

            const mat4 tmp = glm::rotate(model, angles[i], crosses[i]);
            shader.setMatrix4fv("model", tmp);

            glBindVertexArray(circularConeVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 198);
        }

        shader.use();
        shader.setMatrix4fv("projection", projection);
        shader.setMatrix4fv("view", view);
        shader.setMatrix4fv("model", bunny_model);
    
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, x.size());

        shader.setMatrix4fv("model", other_bunny_model);
        glDrawArrays(GL_POINTS, 0, x.size());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    std::ofstream fout("output.log", std::ios::app);

    if (fout.is_open())
    {
        float sum = 0, other_sum = 0;

        fout << "\nOctree\t\tNormal\n";

        for(size_t index = 0; index < 20; ++index)
        {
            sum += log[index];
            other_sum += other_log[index];
            fout << log[index] << "\t" << other_log[index] << endl;
        }
        fout << "---------------------------\n";
        
        fout << "sum:" << endl;
        fout << sum << "\t" << other_sum << endl;
        fout << "avager:" << endl;
        fout << sum/20 << "\t" << other_sum/20 << endl;

        sum = other_sum = 0;

        for (float f : one_log)
        {
            sum += f;
        }

        for (float f : other_one_log)
        {
            other_sum += f;
        }

        sum /= one_log.size();
        other_sum /= other_one_log.size();

        fout << "one ave:" << endl;
        fout << sum << "\t" << other_sum << endl;
    }

    return 0;
}

const unsigned int getCircularConeVAO()
{
    unsigned int VAO;
    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    vector<float> tmp;
    tmp.push_back(0);
    tmp.push_back(0);
    tmp.push_back(0);

    for (float angle = 0; angle <= (2.001f * M_PI); angle += (M_PI / 32.0f))
    {
        float x = 0.005f * sin(angle);
        float y = 0.005f * cos(angle);

        tmp.push_back(x);
        tmp.push_back(y);
        tmp.push_back(-0.1f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(float), &tmp[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return VAO;
}

const vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = vec3(drand48(), drand48(), drand48());
        p.x *= 2;
        p.y *= 2;
        p.z *= 2;
        p -= vec3(1,1,1);
    } while(dot(p,p) >= 1.0); 
    
    return p;
}

bool other_detection(const float x_point, const float y_point, const float z_point, const float radius)
{
    const float xMin = x_point - radius, xMax = x_point + radius,
                yMin = y_point - radius, yMax = y_point + radius,
                zMin = z_point - radius, zMax = z_point + radius;

    for(size_t index = 0; index < x.size(); ++index)
    {
        if (
            xMin < x[index] && x[index] < xMax &&
            yMin < y[index] && y[index] < yMax &&
            zMin < z[index] && z[index] < zMax
        ) return true;
    }

    return false;
}
