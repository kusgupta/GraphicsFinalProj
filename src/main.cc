#include <GL/glew.h>

#include "bone_geometry.h"
#include "procedure_geometry.h"
#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "chrono"
#include "texture_to_render.h"
#include "OBJ_Loader.h"
#include "RadiosityScene.h"
//#include "kdTree.h"

#include <memory>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>




int window_width = 1280;
int window_height = 720;
int main_view_width = 960;
int main_view_height = 720;
int preview_width = window_width - main_view_width; // 320
int preview_height = preview_width / 4 * 3; // 320 / 4 * 3 = 240
int preview_bar_width = preview_width;
int preview_bar_height = main_view_height;
GUI *gui;
const std::string window_title = "Animation";

const char* vertex_shader =
#include "shaders/default.vert"
;

const char* geometry_shader =
#include "shaders/default.geom"
;
const char* floor_fragment_shader =
#include "shaders/floor.frag"
;

const char* object_vertex_shader =
#include "shaders/object.vert"
;

const char* object_geometry_shader =
#include "shaders/object.geom"
;

const char* object_fragment_shader =
#include "shaders/object.frag"
;


//const char* preview_geom_shader =
//#include "shaders/preview_name.geom";

// FIXME: Add more shaders here.

void ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << "\n";
}


GLFWwindow* init_glefw()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwSetErrorCallback(ErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Disable resizing, for simplicity
    glfwWindowHint(GLFW_SAMPLES, 4);
    auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);
    CHECK_SUCCESS(ret != nullptr);
    glfwMakeContextCurrent(ret);
    glewExperimental = GL_TRUE;
    CHECK_SUCCESS(glewInit() == GLEW_OK);
    glGetError();  // clear GLEW's error for it
    glfwSwapInterval(1);
    const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);    // version as a string
    std::cout << "Renderer: " << renderer << "\n";
    std::cout << "OpenGL version supported:" << version << "\n";

    return ret;
}

SceneObject make_box(float size) {
    SceneObject obj;

    obj.vertices.push_back(glm::vec4(-size, -size, -size, 1));
    obj.vertices.push_back(glm::vec4(-size, -size, size, 1));
    obj.vertices.push_back(glm::vec4(size, size, -size, 1));
    obj.vertices.push_back(glm::vec4(size, size, size, 1));
    obj.vertices.push_back(glm::vec4(-size, size, -size, 1));
    obj.vertices.push_back(glm::vec4(-size, size, size, 1));
    obj.vertices.push_back(glm::vec4(size, -size, -size, 1));
    obj.vertices.push_back(glm::vec4(size, -size, size, 1));


    for (int i = 0; i < obj.vertices.size(); i++) {
        obj.vertices[i].x /= 1;
        obj.vertices[i].y /= 1;
        obj.vertices[i].z /= 1;
    }

    obj.faces.push_back(glm::vec3(0, 1, 4));
    obj.faces.push_back(glm::vec3(5, 1, 4));
    obj.faces.push_back(glm::vec3(4, 2, 5));
    obj.faces.push_back(glm::vec3(3, 5, 2));
    obj.faces.push_back(glm::vec3(2, 6, 3));
    obj.faces.push_back(glm::vec3(7, 3, 6));
    obj.faces.push_back(glm::vec3(0, 6, 1));
    obj.faces.push_back(glm::vec3(7, 1, 6));
    obj.faces.push_back(glm::vec3(1, 5, 7));
    obj.faces.push_back(glm::vec3(3, 7, 5));
    obj.faces.push_back(glm::vec3(0, 4, 6));
    obj.faces.push_back(glm::vec3(2, 6, 4));


    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));

    return obj;
}

void add_box(float size, float scale, int box_num, SceneObject &obj) {
    obj.vertices.push_back(glm::vec4(-size, -size, -size, 1));
    obj.vertices.push_back(glm::vec4(-size, -size, size, 1));
    obj.vertices.push_back(glm::vec4(size, size, -size, 1));
    obj.vertices.push_back(glm::vec4(size, size, size, 1));
    obj.vertices.push_back(glm::vec4(-size, size, -size, 1));
    obj.vertices.push_back(glm::vec4(-size, size, size, 1));
    obj.vertices.push_back(glm::vec4(size, -size, -size, 1));
    obj.vertices.push_back(glm::vec4(size, -size, size, 1));


    for (int i = 0; i < obj.vertices.size(); i++) {
        obj.vertices[i].x /= scale;
        obj.vertices[i].y /= scale;
        obj.vertices[i].z /= scale;
    }

    int num_faces = box_num * 8;
    obj.faces.push_back(glm::vec3(0 + num_faces, 1 + num_faces, 4 + num_faces));
    obj.faces.push_back(glm::vec3(5 + num_faces, 1 + num_faces, 4 + num_faces));
    obj.faces.push_back(glm::vec3(4 + num_faces, 2 + num_faces, 5 + num_faces));
    obj.faces.push_back(glm::vec3(3 + num_faces, 5 + num_faces, 2 + num_faces));
    obj.faces.push_back(glm::vec3(2 + num_faces, 6 + num_faces, 3 + num_faces));
    obj.faces.push_back(glm::vec3(7 + num_faces, 3 + num_faces, 6 + num_faces));
    obj.faces.push_back(glm::vec3(0 + num_faces, 6 + num_faces, 1 + num_faces));
    obj.faces.push_back(glm::vec3(7 + num_faces, 1 + num_faces, 6 + num_faces));
    obj.faces.push_back(glm::vec3(1 + num_faces, 5 + num_faces, 7 + num_faces));
    obj.faces.push_back(glm::vec3(3 + num_faces, 7 + num_faces, 5 + num_faces));
    obj.faces.push_back(glm::vec3(0 + num_faces, 4 + num_faces, 6 + num_faces));
    obj.faces.push_back(glm::vec3(2 + num_faces, 6 + num_faces, 4 + num_faces));


    obj.colors.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
    obj.colors.push_back(glm::vec4(0, 1.0, 0.0, 1.0));
    obj.colors.push_back(glm::vec4(0, 0.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(0, 0.0, 0.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 0.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
    obj.colors.push_back(glm::vec4(0.0, 1.0, 1.0, 1.0));

//    return obj;
}

void readColors(std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& colors, std::vector<glm::uvec3>& faces) {
    std::ifstream fin;
    fin.open("../../geometrydata");

    int vertexSize = 0, faceSize = 0;
    fin >> vertexSize >> faceSize;

    for (int i = 0; i < vertexSize; i++) {
        glm::vec4 vertex;
        fin >> vertex.x >> vertex.y >> vertex.z;
        vertex.w = 1.0;

        vertices.push_back(vertex);
    }

    for (int i = 0; i < vertexSize; i++) {
        glm::vec4 color;
        fin >> color.x >> color.y >> color.z;
        color.w = 1.0;

        colors.push_back(color);
    }

    for (int i = 0; i < faceSize; i++) {
        glm::uvec3 face;
        fin >> face.x >> face.y >> face.z;

        faces.push_back(face);
    }

    fin.close();
}




int main(int argc, char* argv[])
{
//	if (argc < 2) {
//		std::cerr << "Input model file is missing" << std::endl;
//		std::cerr << "Usage: " << argv[0] << " <PMD file>" << std::endl;
//		return -1;
//	}

//    std::cout << argv[0] << std::endl;
    int tess_level = std::stoi(argv[1]);
    char* obj_name = argv[2];

    int threadCount = 4;

    if (argc >= 4) {
        threadCount = std::stoi(argv[3]);
    }


    GLFWwindow *window = init_glefw();
    gui = new GUI(window, main_view_width, main_view_height, preview_height);

    Mesh mesh;
    bool loadColorsFromFile = false;

    std::ifstream file;
    SceneObject box;
    if (loadColorsFromFile) {
        box.vertices.clear();
        box.colors.clear();
        box.faces.clear();
        readColors(box.vertices, box.colors, box.faces);
    } else {
        objl::Loader loader;
        loader.LoadFile(std::string("../../") + obj_name);


        std::chrono::time_point<std::chrono::steady_clock> previous = mesh.clock->now();

//    SceneObject box = make_box(500.0);
        //add_box(500, 1, 0, box);
        //add_box(100, 1, 1, box);
        box.loadScene(loader);
        std::vector<LightSource> lights;
//    LightSource light1;
//    light1.position = glm::vec4(-400, -400, -400, 1);
//    light1.color = glm::vec4(1, 1, 1, 1);
//    light1.intensity = glm::vec4(1, 1, 1, 1);
//    lights.push_back(light1);
        /*
         * GUI object needs the mesh object for bone manipulation.
         */
//    mesh.skeleton.refreshCache(const_cast<Configuration *>(mesh.getCurrentQ()));

//    /*
//    /*
        LightSource light1;
        LightSource light2;
        LightSource light3;
        LightSource light4;
        light1.position = glm::vec4(343.0, 548.0, 227.0, 1);
        light1.intensity = glm::vec4(1, 1, 1, 1);
        light2.position = glm::vec4(343.0, 548.0, 332.0, 1);
        light2.intensity = glm::vec4(1, 1, 1, 1);
        light3.position = glm::vec4(213.0, 548.0, 332.0, 1);
        light3.intensity = glm::vec4(1, 1, 1, 1);
        light4.position = glm::vec4(213.0, 548.0, 227.0, 1);
        light4.intensity = glm::vec4(1, 1, 1, 1);
        lights.push_back(light1);
        lights.push_back(light2);
        lights.push_back(light3);
        lights.push_back(light4);
//    */
        //     */


//	auto diffuse = std::make_shared<ShaderUniform<const glm::vec3*>>("diffuse", diffuse)
        std::cout << box.vertices.size() << std::endl;
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        box.makeTriangles(tess_level);
        box.tree = new kdTree(&(box.triangles));
        //Create bounding box
        box.box = box.tree->createMergedBoundingBox(&(box.triangles), 0, box.triangles.size());
        box.tree->buildTree(*(box.box));
        std::cout << "L" << std::endl;
        //std::cout << box.vertices.size() << std::endl;
        std::cout << box.triangles.size() << std::endl;
        //std::cout << box.faces.size() << std::endl;
        //std:: cout << box.colors.size() << std::endl;
        std::cout << "L" << std::endl;
//    std::cout << box.triangles.size() << std::endl;
//    box.make_form_factors_threads(8);
        box.pre_process(4);
        std::cout << box.triangles[1].form_factors.size() << std::endl;

        std::cout << "Made form factors" << std::endl;
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

// To get the value of duration use the count()
// member function on the duration object

        std::cout << duration.count() / 1000000 << std::endl;
        for (int light_num = 0; light_num < lights.size(); light_num++) {
            for (int passes = 0; passes < 5; passes++) {
                std::cout << "pass " << passes << std::endl;
                box.calculate_light(lights[light_num], 4, true);
                start = stop;
                stop = high_resolution_clock::now();

                duration = duration_cast<microseconds>(stop - start);
                std::cout << duration.count() / 1000000 << std::endl;
            }


        }

        box.updateColors(1);
    }
    gui->assignMesh(&mesh);
    MatrixPointers mats; // Define MatrixPointers here for lambda to capture

    GUI gui2 = *gui;
    // FIXME: add more lambdas for data_source if you want to use RenderPass.
    //        Otherwise, do whatever you like here
    std::function<const glm::mat4 *()> model_data = [&mats]() {
        return mats.model;
    };

    std::function<glm::mat4()> view_data = [&mats]() { return *mats.view; };
    std::function<glm::mat4()> proj_data = [&mats]() { return *mats.projection; };
//    std::function<glm::mat4()> identity_mat = [](){ return glm::mat4(1.0f); };
    std::function<glm::vec3()> cam_data = [&gui2]() { return gui->getCamera(); };
    float cylinder_rad = kCylinderRadius;
    std::function<float()> radius_data = [&cylinder_rad]() {
        return cylinder_rad;
    };

    auto std_model = std::make_shared<ShaderUniform<const glm::mat4 *>>("model", model_data);
//    auto floor_model = make_uniform("model", identity_mat);
    auto std_view = make_uniform("view", view_data);
    auto std_camera = make_uniform("camera_position", cam_data);
    auto std_proj = make_uniform("projection", proj_data);
    auto std_radius = make_uniform("radius", radius_data);
    // Object render pass
    RenderDataInput object_pass_input;
    object_pass_input.assign(0, "vertex_position", box.vertices.data(), box.vertices.size(), 4, GL_FLOAT);
    object_pass_input.assign(1, "diffuse", box.colors.data(), box.colors.size(), 4, GL_FLOAT);
    object_pass_input.assignIndex(box.faces.data(), box.faces.size(), 3);
    RenderPass object_pass(-1,
                           object_pass_input,
                           { object_vertex_shader, nullptr, nullptr, object_geometry_shader, object_fragment_shader},
                           {std_view, std_proj},
                           { "fragment_color" }
    );
//each triangle emits 1/num_triangles light, curtri = sum(light emitted per triangle, up to 1)

    float aspect = 0.0f;
    std::cout << "center = " << mesh.getCenter() << "\n";

    bool draw_objects = true;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        // Setup some basic window stuff.
        //start = stop;
        //stop = high_resolution_clock::now();

        //duration = duration_cast<microseconds>(stop - start);
        //std::cout << duration.count() / 100000.0 << std::endl;


        glfwGetFramebufferSize(window, &window_width, &window_height);
        glViewport(0, 0, main_view_width, main_view_height);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        //glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glCullFace(GL_BACK);

        gui->updateMatrices();
        mats = gui->getMatrixPointers();

        std::stringstream title;
        float cur_time = gui->getCurrentPlayTime();
        title << window_title;

        glfwSetWindowTitle(window, title.str().data());

        if (draw_objects) {
            object_pass.setup();
            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
                                          box.faces.size() * 3,
                                          GL_UNSIGNED_INT, 0));
        }



        // Poll and swap.
        // FIXME: Draw previews here, note you need to call glViewport




        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
