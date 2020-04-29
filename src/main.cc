#include <GL/glew.h>

#include "bone_geometry.h"
#include "procedure_geometry.h"
#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "chrono"
#include "texture_to_render.h"

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

const char* blending_shader =
#include "shaders/blending.vert"
;

const char* geometry_shader =
#include "shaders/default.geom"
;

const char* fragment_shader =
#include "shaders/default.frag"
;

const char* floor_fragment_shader =
#include "shaders/floor.frag"
;

const char* bone_vertex_shader =
#include "shaders/bone.vert"
;

const char* bone_fragment_shader =
#include "shaders/bone.frag"
;

const char* cylinder_vertex_shader =
#include "shaders/cylinder.vert"
		;

const char* cylinder_fragment_shader =
#include "shaders/cylinder.frag"
        ;

const char* preview_vertex_shader =
#include "shaders/preview.vert"
        ;

const char* preview_fragment_shader =
#include "shaders/preview.frag"
        ;

const char* shadow_vertex_shader =
#include "shaders/shadow.vert"
        ;

const char* shadow_geom_shader =
#include "shaders/shadow.geom"
        ;

const char* shadow_fragment_shader =
#include "shaders/shadow.frag"
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

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Input model file is missing" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <PMD file>" << std::endl;
		return -1;
	}
	GLFWwindow *window = init_glefw();
    gui = new GUI(window, main_view_width, main_view_height, preview_height);

    //Generates textures and initializes them
    GLuint depthTexture[6];
    glGenTextures(6, depthTexture);

    for (int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, depthTexture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 960, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    }

    //Generates buffers
    GLuint depthfb;
    glGenFramebuffers(1, &depthfb);

    //Generates samplers and initializes them
    GLuint depthsampler[6];
    glGenSamplers(6, depthsampler);

    for (int i = 0; i < 6; i++) {
        CHECK_GL_ERROR(glSamplerParameteri(depthsampler[i], GL_TEXTURE_WRAP_S, GL_REPEAT));
        CHECK_GL_ERROR(glSamplerParameteri(depthsampler[i], GL_TEXTURE_WRAP_T, GL_REPEAT));
        CHECK_GL_ERROR(glSamplerParameteri(depthsampler[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        CHECK_GL_ERROR(glSamplerParameteri(depthsampler[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    }

    //Lambda functions for accessing the sampler and depth texture
    std::function<int()> depthsamplerdata0 = [depthsampler] {return depthsampler[0]; };
    std::function<int()> depthsamplerdata1 = [depthsampler] {return depthsampler[1]; };
    std::function<int()> depthsamplerdata2 = [depthsampler] {return depthsampler[2]; };
    std::function<int()> depthsamplerdata3 = [depthsampler] {return depthsampler[3]; };
    std::function<int()> depthsamplerdata4 = [depthsampler] {return depthsampler[4]; };
    std::function<int()> depthsamplerdata5 = [depthsampler] {return depthsampler[5]; };

    std::function<int()> depthtexturedata0 = [depthTexture] {return depthTexture[0]; };
    std::function<int()> depthtexturedata1 = [depthTexture] {return depthTexture[1]; };
    std::function<int()> depthtexturedata2 = [depthTexture] {return depthTexture[2]; };
    std::function<int()> depthtexturedata3 = [depthTexture] {return depthTexture[3]; };
    std::function<int()> depthtexturedata4 = [depthTexture] {return depthTexture[4]; };
    std::function<int()> depthtexturedata5 = [depthTexture] {return depthTexture[5]; };

    auto shadow0 = make_texture("sampler0", depthsamplerdata0, 0, depthtexturedata0);
    auto shadow1 = make_texture("sampler1", depthsamplerdata1, 1, depthtexturedata1);
    auto shadow2 = make_texture("sampler2", depthsamplerdata2, 2, depthtexturedata2);
    auto shadow3 = make_texture("sampler3", depthsamplerdata3, 3, depthtexturedata3);
    auto shadow4 = make_texture("sampler4", depthsamplerdata4, 4, depthtexturedata4);
    auto shadow5 = make_texture("sampler5", depthsamplerdata5, 5, depthtexturedata5);
    
    
    
    
    
    std::vector<glm::vec4> floor_vertices;
	std::vector<glm::uvec3> floor_faces;
	create_floor(floor_vertices, floor_faces);

	Mesh mesh;

    std::chrono::time_point<std::chrono::steady_clock> previous = mesh.clock->now();
    glGenFramebuffers(1, &mesh.Panel);
    glBindFramebuffer(GL_FRAMEBUFFER, mesh.Panel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



	/*
	 * GUI object needs the mesh object for bone manipulation.
	 */
	gui->assignMesh(&mesh);
    mesh.skeleton.refreshCache(const_cast<Configuration *>(mesh.getCurrentQ()));
    MatrixPointers mats; // Define MatrixPointers here for lambda to capture


    GUI gui2 = *gui;
	// FIXME: add more lambdas for data_source if you want to use RenderPass.
	//        Otherwise, do whatever you like here
	std::function<const glm::mat4*()> model_data = [&mats]() {
		return mats.model;
	};

	std::function<glm::mat4()> view_data = [&mats]() { return *mats.view; };
	std::function<glm::mat4()> proj_data = [&mats]() { return *mats.projection; };
	std::function<glm::mat4()> identity_mat = [](){ return glm::mat4(1.0f); };
	std::function<glm::vec3()> cam_data = [&gui2](){ return gui->getCamera(); };
    float cylinder_rad = kCylinderRadius;
    std::function<float()> radius_data = [&cylinder_rad] () {
        return cylinder_rad;
    };

	auto std_model = std::make_shared<ShaderUniform<const glm::mat4*>>("model", model_data);
    auto floor_model = make_uniform("model", identity_mat);
	auto std_view = make_uniform("view", view_data);
	auto std_camera = make_uniform("camera_position", cam_data);
	auto std_proj = make_uniform("projection", proj_data);
	auto std_radius = make_uniform("radius", radius_data);


	// Floor render pass
	RenderDataInput floor_pass_input;
	floor_pass_input.assign(0, "vertex_position", floor_vertices.data(), floor_vertices.size(), 4, GL_FLOAT);
	floor_pass_input.assignIndex(floor_faces.data(), floor_faces.size(), 3);
	RenderPass floor_pass(-1,
			floor_pass_input,
			{ vertex_shader, geometry_shader, floor_fragment_shader},
             { floor_model, std_view, std_proj},
			{ "fragment_color" }
			);


	float aspect = 0.0f;
	std::cout << "center = " << mesh.getCenter() << "\n";

	bool draw_floor = true;
	bool draw_skeleton = true;
	bool draw_object = true;
	bool draw_cylinder = true;
    gui->floor = &floor_pass;
    gui->floor_faces = &floor_faces;
    if (argc >= 3) {
        mesh.loadAnimationFrom(argv[2]);
    }

   
    GLuint multiTexture;
    glGenTextures(1, &multiTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 960, 720, 0);

    GLuint multifb;
    glGenFramebuffers(1, &multifb);
    glBindFramebuffer(GL_FRAMEBUFFER, multifb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        // Setup some basic window stuff.
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glViewport(0, 0, main_view_width, main_view_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

        
        glBindFramebuffer(GL_FRAMEBUFFER, depthfb);
        
        
        for (int i = 0; i < 6; i++) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture[i], 0);

            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, floor_faces.size() * 3, GL_UNSIGNED_INT, 0));
        
           
        }
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        
        glBindFramebuffer(GL_FRAMEBUFFER, multifb);

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        
        

        gui->updateMatrices();
        mats = gui->getMatrixPointers();

        std::stringstream title;
        float cur_time = gui->getCurrentPlayTime();
        title << window_title;

        glfwSetWindowTitle(window, title.str().data());

		// Then draw floor.
		if (draw_floor) {
			floor_pass.setup();
			// Draw our triangles.
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
			                              floor_faces.size() * 3,
			                              GL_UNSIGNED_INT, 0));
		}

		float floor = int(gui->scrollBar / 240.0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multifb);
        glBlitFramebuffer(0, 0, 960, 720, 0, 0, 960, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        


		// Poll and swap.
        // FIXME: Draw previews here, note you need to call glViewport
        
        glViewport(main_view_width, 0, preview_bar_width, preview_bar_height);


        
        glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
