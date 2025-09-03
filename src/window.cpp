#include "window.hpp"

WindowManager::WindowManager(int width, int height, const char* title) {

    //! Setting up the GLFW Error callback
    glfwSetErrorCallback(csX75::error_callback);

    //! Initialize GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);

    //We want OpenGL 4.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //This is for MacOSX - can be omitted otherwise
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //We don't want the old OpenGL 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //! Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1024, 1024, "Assignment 1", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //! Make the window's context current 
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    //Turn this on to get Shader based OpenGL
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        //Problem: glewInit failed, something is seriously wrong.
        std::cerr << "GLEW Init Failed : %s" << std::endl;
    }

    //Print and see what context got enabled
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //Keyboard Callback
    glfwSetKeyCallback(window, csX75::key_callback);
    //Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    enable_culling = true;
    enable_wireframe = false;


    //Initialize GL state
    csX75::initGL();
}

void WindowManager::initBuffersGL() {
    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    shaderProgram = csX75::CreateProgramGL(shaderList);
    glUseProgram(shaderProgram);

    vPosition = glGetAttribLocation(shaderProgram, "vPosition");
    vColor = glGetAttribLocation(shaderProgram, "vColor");
    uModelViewProjectMatrix = glGetUniformLocation(shaderProgram, "uModelViewProjectMatrix");

    // shapes.push_back(std::make_shared<sphere_t>(4, vPosition, vColor));
    // selected_shape = shapes.back();
    model = std::make_shared<model_t>(vPosition, vColor);
}

void WindowManager::addNode(shape_type_t type, uint32_t level) {
    model->addNode(type, level);
}

void WindowManager::renderGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    matrixStack.clear();

    c_rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f, 0.0f, 0.0f));
    c_rotation_matrix = glm::rotate(c_rotation_matrix, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
    c_rotation_matrix = glm::rotate(c_rotation_matrix, zrot, glm::vec3(0.0f, 0.0f, 1.0f));

    c_view_matrix = glm::lookAt(glm::vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    c_ortho_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    matrixStack.push_back(c_ortho_matrix * c_view_matrix * c_rotation_matrix);

    glUseProgram(shaderProgram);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Toggle culling and wireframe
    if (enable_culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    if (enable_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (model) {
        model->draw(matrixStack, uModelViewProjectMatrix);
    }
    glFlush();
}

void WindowManager::mainLoop() {
    //! Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        renderGL();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}

WindowManager::~WindowManager() {
    glfwDestroyWindow(window);
    //Terminate GLFW
    glfwTerminate();
}