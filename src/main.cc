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

void updatePose(Mesh &mesh, KeyFrame &target) {
    Skeleton skeleton = mesh.skeleton;
    for (int i = 0; i < skeleton.joints.size(); i++) {
        //Re-initializes skeleton
        Joint *current_joint = skeleton.joints[i];
        current_joint->T_i = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        current_joint->D_i = current_joint->U_i;
    }
    for (int j = 0; j < skeleton.joints.size(); j++) {
//        skeleton.joints[j]->orientation = k.rel_rot[j];
        Joint *current_joint = skeleton.joints[j];
        current_joint->orientation = target.rel_rot[j];
        current_joint->rel_orientation = target.rel_rot[j];

        if (current_joint->parent_index != -1) {
            Joint *parent = skeleton.joints[current_joint->parent_index];
            skeleton.joints[j]->applyTransformations(current_joint, parent, glm::toMat4(current_joint->orientation));
            current_joint->transformRecursive(parent);
        }
        else {
            skeleton.joints[j]->applyTransformations(current_joint, nullptr, glm::toMat4(current_joint->orientation));
            current_joint->transformRecursive(current_joint);
        }
    }
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
	LineMesh cylinder_mesh;
	LineMesh axes_mesh;


	// FIXME: we already created meshes for cylinders. Use them to render
	//        the cylinder and axes if required by the assignment.
	create_cylinder_mesh(cylinder_mesh);
	create_axes_mesh(axes_mesh);

	Mesh mesh;
	mesh.loadPmd(argv[1]);
	std::cout << "Loaded object  with  " << mesh.vertices.size()
		<< " vertices and " << mesh.faces.size() << " faces.\n";

	glm::vec4 mesh_center = glm::vec4(0.0f);
	for (size_t i = 0; i < mesh.vertices.size(); ++i) {
            mesh_center += mesh.vertices[i];
	}
	mesh_center /= mesh.vertices.size();

    std::chrono::time_point<std::chrono::steady_clock> previous = mesh.clock->now();
    glGenFramebuffers(1, &mesh.Panel);
    glBindFramebuffer(GL_FRAMEBUFFER, mesh.Panel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



	/*
	 * GUI object needs the mesh object for bone manipulation.
	 */
	gui->assignMesh(&mesh);
    mesh.skeleton.refreshCache(const_cast<Configuration *>(mesh.getCurrentQ()));

	glm::vec4 light_position = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
	MatrixPointers mats; // Define MatrixPointers here for lambda to capture
    
    glm::mat4 light_proj = glm::perspective((float)(kFov * (M_PI / 180.0f)), (float) 1.0, kNear, kFar);;
    glm::mat4 light_view;

    glm::vec3 xDir(1.0, 0.0, 0.0);
    glm::vec3 yDir(0.0, 1.0, 0.0);
    glm::vec3 zDir(0.0, 0.0, 1.0);
    std::vector<glm::mat4> cubeMapViews;
    //Odd indices contain negative axis, even contain positive, goes from z to x
    cubeMapViews.push_back(glm::inverse(glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { 0, 0, 0, 1 })) * 
        glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { -glm::vec3(light_position), 1 }));
    cubeMapViews.push_back(glm::inverse(glm::mat4({ xDir, 0 }, { yDir, 0 }, { -zDir, 0 }, { 0, 0, 0, 1 })) *
        glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { -glm::vec3(light_position), 1 }));
    cubeMapViews.push_back(glm::inverse(glm::mat4({ xDir, 0 }, { zDir, 0 }, { yDir, 0 }, { 0, 0, 0, 1 })) *
        glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { -glm::vec3(light_position), 1 }));
    cubeMapViews.push_back(glm::inverse(glm::mat4({ xDir, 0 }, { zDir, 0 }, { -yDir, 0 }, { 0, 0, 0, 1 })) *
        glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { -glm::vec3(light_position), 1 }));
    cubeMapViews.push_back(glm::inverse(glm::mat4({ zDir, 0 }, { yDir, 0 }, { xDir, 0 }, { 0, 0, 0, 1 })) *
        glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { -glm::vec3(light_position), 1 }));
    cubeMapViews.push_back(glm::inverse(glm::mat4({ zDir, 0 }, { yDir, 0 }, { -xDir, 0 }, { 0, 0, 0, 1 })) *
        glm::mat4({ xDir, 0 }, { yDir, 0 }, { zDir, 0 }, { -glm::vec3(light_position), 1 }));
    


    glm::mat4 cylinder_transform;
    float height;

	/*
	 * In the following we are going to define several lambda functions as
	 * the data source of GLSL uniforms
	 *
	 * Introduction about lambda functions:
	 *      http://en.cppreference.com/w/cpp/language/lambda
	 *      http://www.stroustrup.com/C++11FAQ.html#lambda
	 *
	 * Note: lambda expressions cannot be converted to std::function directly
	 *       Hence we need to declare the data function explicitly.
	 *
	 * CAVEAT: DO NOT RETURN const T&, which compiles but causes
	 *         segfaults.
	 *
	 * Do not worry about the efficient issue, copy elision in C++ 17 will
	 * minimize the performance impact.
	 *
	 * More details about copy elision:
	 *      https://en.cppreference.com/w/cpp/language/copy_elision
	 */
    GUI gui2 = *gui;
	// FIXME: add more lambdas for data_source if you want to use RenderPass.
	//        Otherwise, do whatever you like here
	std::function<const glm::mat4*()> model_data = [&mats]() {
		return mats.model;
	};
    
    std::function<glm::mat4()> specific_light_view_data = [&light_view] { return light_view; };
    std::function<std::vector<glm::mat4>()> light_view_data = [&cubeMapViews] { return cubeMapViews; };
    std::function<glm::mat4()> light_proj_data = [&light_proj] { return light_proj; };
	std::function<glm::mat4()> view_data = [&mats]() { return *mats.view; };
	std::function<glm::mat4()> proj_data = [&mats]() { return *mats.projection; };
	std::function<glm::mat4()> identity_mat = [](){ return glm::mat4(1.0f); };
	std::function<glm::vec3()> cam_data = [&gui2](){ return gui->getCamera(); };
	std::function<glm::vec4()> lp_data = [&light_position]() { return light_position; };
    std::function<glm::mat4()> cylinder_transform_data = [&cylinder_transform]() { return cylinder_transform;};
    float cylinder_rad = kCylinderRadius;
    std::function<float()> radius_data = [&cylinder_rad] () {
        return cylinder_rad;
    };
    std::function<float()> height_data = [&height]() { return height; };

	auto std_model = std::make_shared<ShaderUniform<const glm::mat4*>>("model", model_data);
    auto std_specific_light_view = make_uniform("light_view", specific_light_view_data);
    auto std_light_view = make_uniform("light_view", light_view_data);
    auto std_light_proj = make_uniform("light_proj", light_proj_data);
    auto floor_model = make_uniform("model", identity_mat);
	auto std_view = make_uniform("view", view_data);
	auto std_camera = make_uniform("camera_position", cam_data);
	auto std_proj = make_uniform("projection", proj_data);
	auto std_light = make_uniform("light_position", lp_data);
    auto std_cy_transform = make_uniform("bone_transform", cylinder_transform_data);
	auto std_radius = make_uniform("radius", radius_data);
    auto std_height = make_uniform("height", height_data);


	std::function<float()> alpha_data = [&gui2]() {
		static const float transparet = 0.5; // Alpha constant goes here
		static const float non_transparet = 1.0;
		if (gui->isTransparent())
			return transparet;
		else
			return non_transparet;
	};
	auto object_alpha = make_uniform("alpha", alpha_data);

	std::function<std::vector<glm::vec3>()> trans_data = [&mesh](){ return mesh.getCurrentQ()->transData(); };
	std::function<std::vector<glm::fquat>()> rot_data = [&mesh](){ return mesh.getCurrentQ()->rotData(); };
	std::function<std::vector<float>()> angle_data = [&mesh](){ return mesh.getCurrentQ()->angleData(); };
	std::function<std::vector<glm::vec3>()> axes_data = [&mesh]() { return mesh.getCurrentQ()->axesData(); };
	auto joint_trans = make_uniform("joint_trans", trans_data);
	auto joint_rot = make_uniform("joint_rot", rot_data);
	auto joint_axis = make_uniform("joint_axis", axes_data);
	auto joint_angle = make_uniform("joint_angle", angle_data);
	// FIXME: define more ShaderUniforms for RenderPass if you want to use it.
	//        Otherwise, do whatever you like here

    
    RenderDataInput shadow_mesh_pass_input;
    shadow_mesh_pass_input.assign(0, "vertex_position", mesh.vertices.data(), mesh.vertices.size(), 4, GL_FLOAT);
    shadow_mesh_pass_input.assignIndex(mesh.faces.data(), mesh.faces.size(), 3);
    RenderPass shadow_mesh_pass(-1,
            shadow_mesh_pass_input,
            {shadow_vertex_shader, shadow_geom_shader, shadow_fragment_shader},
            {std_specific_light_view, std_light_proj, std_light},
            {"fragment_color"}

        );


    RenderDataInput shadow_floor_pass_input;
    shadow_floor_pass_input.assign(0, "vertex_position", floor_vertices.data(), floor_vertices.size(), 4, GL_FLOAT);
    shadow_floor_pass_input.assignIndex(floor_faces.data(), floor_faces.size(), 3);
    RenderPass shadow_floor_pass(-1,
        shadow_floor_pass_input,
        { shadow_vertex_shader, shadow_geom_shader, shadow_fragment_shader },
        { std_specific_light_view, std_light_proj, std_light},
        {"fragment_color"}

    );
    

	// Floor render pass
	RenderDataInput floor_pass_input;
	floor_pass_input.assign(0, "vertex_position", floor_vertices.data(), floor_vertices.size(), 4, GL_FLOAT);
	floor_pass_input.assignIndex(floor_faces.data(), floor_faces.size(), 3);
	RenderPass floor_pass(-1,
			floor_pass_input,
			{ vertex_shader, geometry_shader, floor_fragment_shader},
             { floor_model, std_view, std_proj, std_light, std_light_view, std_light_proj, shadow0, shadow1, shadow2, shadow3, shadow4, shadow5 },
			{ "fragment_color" }
			);


    std::vector<glm::vec4> preview1;
    preview1.push_back(glm::vec4(-1.0f, 1.0f/3, 1.0f, 1));
    preview1.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1));
    preview1.push_back(glm::vec4(1.0f, 1.0f/3, 1.0f, 1));
    preview1.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1));

    std::vector<glm::uvec3> preview_triangles;
    preview_triangles.push_back(glm::uvec3(2, 1, 3));
    preview_triangles.push_back(glm::uvec3(2, 3, 0));
    glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

    std::vector<glm::vec2> tex_coords;
    tex_coords.push_back(glm::vec2(0, 0));
    tex_coords.push_back(glm::vec2(1, 1));
    tex_coords.push_back(glm::vec2(1, 0));
    tex_coords.push_back(glm::vec2(0, 1));

    GLuint sampler2d_; // Check if gl making is necessary
    int texid = 0;
    CHECK_GL_ERROR(glGenSamplers(1, &sampler2d_));
    CHECK_GL_ERROR(glSamplerParameteri(sampler2d_, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler2d_, GL_TEXTURE_WRAP_T, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler2d_, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glSamplerParameteri(sampler2d_, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    std::function<int()> sampler2d_lambda = [&sampler2d_]() { return sampler2d_; };
    std::function<int()> texture_lambda = [&texid]() { return texid; };
    bool show1 = false;
    bool show2 = false;
    bool show3 = false;
    bool show4 = false;

    float scroll;
    auto texture = make_texture("sampler", sampler2d_lambda, 0, texture_lambda);
    std::function<glm::mat4()> orthographic = [&ortho](){ return ortho ;};
    auto orthoUniform = make_uniform("orthomat", orthographic);
    std::function<float()> frame_shift = [&scroll](){ return scroll;};
    auto shiftData = make_uniform("frame_shift", frame_shift);
    std::function<bool()> show_border = [&show1](){ return show1;};
    auto borderData = make_uniform("show_border", show_border);
    std::function<bool()> show_border1 = [&show2](){ return show2;};
    auto borderData1 = make_uniform("show_border", show_border1);
    std::function<bool()> show_border2 = [&show3](){ return show3;};
    auto borderData2 = make_uniform("show_border", show_border2);
    std::function<bool()> show_border3 = [&show4](){ return show4;};
    auto borderData3 = make_uniform("show_border", show_border3);

    RenderDataInput preview1_input;
    preview1_input.assign(0, "vertex_position", preview1.data(), preview1.size(), 4, GL_FLOAT);
    preview1_input.assign(1, "tex_coord", tex_coords.data(), tex_coords.size(), 2, GL_FLOAT);
    preview1_input.assignIndex(preview_triangles.data(), preview_triangles.size(), 3);
    RenderPass preview1_pass(-1,
            preview1_input,
            {
                preview_vertex_shader,
                nullptr,
                preview_fragment_shader
            },
            {texture, orthoUniform, shiftData, borderData},
            {"fragment_color"}
    );


    std::vector<glm::vec4> preview2;
    preview2.push_back(glm::vec4(-1.0f, 1.0f/3 - 2.0/3.0f, 1.0f, 1));
    preview2.push_back(glm::vec4(1.0f, 1.0f - 2.0/3.0f, 1.0f, 1));
    preview2.push_back(glm::vec4(1.0f, 1.0f/3 - 2.0/3.0f, 1.0f, 1));
    preview2.push_back(glm::vec4(-1.0f, 1.0f - 2.0/3.0f, 1.0f, 1));

    GLuint sampler22d_; // Check if gl making is necessary
    int texid2 = 0;
    CHECK_GL_ERROR(glGenSamplers(1, &sampler22d_));
    CHECK_GL_ERROR(glSamplerParameteri(sampler22d_, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler22d_, GL_TEXTURE_WRAP_T, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler22d_, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glSamplerParameteri(sampler22d_, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    std::function<int()> sampler22d_lambda = [&sampler22d_]() { return sampler22d_; };
    std::function<int()> texture_lambda2 = [&texid2]() { return texid2; };


    auto texture2 = make_texture("sampler", sampler22d_lambda, 0, texture_lambda2);

    RenderDataInput preview2_input;
    preview2_input.assign(0, "vertex_position", preview2.data(), preview2.size(), 4, GL_FLOAT);
    preview2_input.assign(1, "tex_coord", tex_coords.data(), tex_coords.size(), 2, GL_FLOAT);
    preview2_input.assignIndex(preview_triangles.data(), preview_triangles.size(), 3);
    RenderPass preview2_pass(-1,
                             preview2_input,
                             {
                                     preview_vertex_shader,
                                     nullptr,
                                     preview_fragment_shader
                             },
                             {texture2, orthoUniform, shiftData, borderData1},
                             {"fragment_color"}
    );

    std::vector<glm::vec4> preview3;
    preview3.push_back(glm::vec4(-1.0f, 1.0f/3 - 4.0/3.0f, 1.0f, 1));
    preview3.push_back(glm::vec4(1.0f, 1.0f - 4.0/3.0f, 1.0f, 1));
    preview3.push_back(glm::vec4(1.0f, 1.0f/3 - 4.0/3.0f, 1.0f, 1));
    preview3.push_back(glm::vec4(-1.0f, 1.0f - 4.0/3.0f, 1.0f, 1));

    GLuint sampler23d_; // Check if gl making is necessary
    int texid3 = 0;
    CHECK_GL_ERROR(glGenSamplers(1, &sampler23d_));
    CHECK_GL_ERROR(glSamplerParameteri(sampler23d_, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler23d_, GL_TEXTURE_WRAP_T, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler23d_, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glSamplerParameteri(sampler23d_, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    std::function<int()> sampler23d_lambda = [&sampler23d_]() { return sampler23d_; };
    std::function<int()> texture_lambda3 = [&texid3]() { return texid3; };


    auto texture3 = make_texture("sampler", sampler23d_lambda, 0, texture_lambda3);

    RenderDataInput preview3_input;
    preview3_input.assign(0, "vertex_position", preview3.data(), preview3.size(), 4, GL_FLOAT);
    preview3_input.assign(1, "tex_coord", tex_coords.data(), tex_coords.size(), 2, GL_FLOAT);
    preview3_input.assignIndex(preview_triangles.data(), preview_triangles.size(), 3);
    RenderPass preview3_pass(-1,
                             preview3_input,
                             {
                                     preview_vertex_shader,
                                     nullptr,
                                     preview_fragment_shader
                             },
                             {texture3, orthoUniform, shiftData, borderData2},
                             {"fragment_color"}
    );

    std::vector<glm::vec4> preview4;
    preview4.push_back(glm::vec4(-1.0f, 1.0f/3 - 6.0/3.0f, 1.0f, 1));
    preview4.push_back(glm::vec4(1.0f, 1.0f - 6.0/3.0f, 1.0f, 1));
    preview4.push_back(glm::vec4(1.0f, 1.0f/3 - 6.0/3.0f, 1.0f, 1));
    preview4.push_back(glm::vec4(-1.0f, 1.0f - 6.0/3.0f, 1.0f, 1));

    GLuint sampler24d_; // Check if gl making is necessary
    int texid4 = 0;
    CHECK_GL_ERROR(glGenSamplers(1, &sampler24d_));
    CHECK_GL_ERROR(glSamplerParameteri(sampler24d_, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler24d_, GL_TEXTURE_WRAP_T, GL_REPEAT));
    CHECK_GL_ERROR(glSamplerParameteri(sampler24d_, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glSamplerParameteri(sampler24d_, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    std::function<int()> sampler24d_lambda = [&sampler24d_]() { return sampler24d_; };
    std::function<int()> texture_lambda4 = [&texid4]() { return texid4; };


    auto texture4 = make_texture("sampler", sampler24d_lambda, 0, texture_lambda4);

    RenderDataInput preview4_input;
    preview4_input.assign(0, "vertex_position", preview4.data(), preview4.size(), 4, GL_FLOAT);
    preview4_input.assign(1, "tex_coord", tex_coords.data(), tex_coords.size(), 2, GL_FLOAT);
    preview4_input.assignIndex(preview_triangles.data(), preview_triangles.size(), 3);
    RenderPass preview4_pass(-1,
                             preview4_input,
                             {
                                     preview_vertex_shader,
                                     nullptr,
                                     preview_fragment_shader
                             },
                             {texture4, orthoUniform, shiftData, borderData3},
                             {"fragment_color"}
    );




	// PMD Model render pass
	// FIXME: initialize the input data at Mesh::loadPmd
	std::vector<glm::vec2>& uv_coordinates = mesh.uv_coordinates;
	RenderDataInput object_pass_input;
	object_pass_input.assign(0, "jid0", mesh.joint0.data(), mesh.joint0.size(), 1, GL_INT);
	object_pass_input.assign(1, "jid1", mesh.joint1.data(), mesh.joint1.size(), 1, GL_INT);
	object_pass_input.assign(2, "w0", mesh.weight_for_joint0.data(), mesh.weight_for_joint0.size(), 1, GL_FLOAT);
	object_pass_input.assign(3, "vector_from_joint0", mesh.vector_from_joint0.data(), mesh.vector_from_joint0.size(), 3, GL_FLOAT);
	object_pass_input.assign(4, "vector_from_joint1", mesh.vector_from_joint1.data(), mesh.vector_from_joint1.size(), 3, GL_FLOAT);
	object_pass_input.assign(5, "normal", mesh.vertex_normals.data(), mesh.vertex_normals.size(), 4, GL_FLOAT);
	object_pass_input.assign(6, "uv", uv_coordinates.data(), uv_coordinates.size(), 2, GL_FLOAT);
	// TIPS: You won't need vertex position in your solution.
	//       This only serves the stub shader.
	object_pass_input.assign(7, "vert", mesh.vertices.data(), mesh.vertices.size(), 4, GL_FLOAT);
	object_pass_input.assignIndex(mesh.faces.data(), mesh.faces.size(), 3);
	object_pass_input.useMaterials(mesh.materials);
	RenderPass object_pass(-1,
			object_pass_input,
			{
			  blending_shader,
			  geometry_shader,
			  fragment_shader
			},
			{ std_model, std_view, std_proj,
			  std_light, std_light_view, std_light_proj,
			  std_camera, object_alpha,
			  joint_trans, joint_rot, joint_angle, joint_axis,
			  shadow0, shadow1, shadow2, shadow3, shadow4, shadow5
            },
			{ "fragment_color" }
			);


	// Setup the render pass for drawing bones
	// FIXME: You won't see the bones until Skeleton::joints were properly
	//        initialized
	std::vector<int> bone_vertex_id;
	std::vector<glm::uvec2> bone_indices;
	for (int i = 0; i < (int)mesh.skeleton.joints.size(); i++) {
		bone_vertex_id.emplace_back(i);
	}
	for (const auto& joint: mesh.skeleton.joints) {
		if (joint->parent_index < 0)
			continue;
		bone_indices.emplace_back(joint->joint_index, joint->parent_index);
	}
	RenderDataInput bone_pass_input;
	bone_pass_input.assign(0, "jid", bone_vertex_id.data(), bone_vertex_id.size(), 1, GL_UNSIGNED_INT);
	bone_pass_input.assignIndex(bone_indices.data(), bone_indices.size(), 2);
	RenderPass bone_pass(-1, bone_pass_input,
			{ bone_vertex_shader, nullptr, bone_fragment_shader},
			{ std_model, std_view, std_proj, joint_trans },
			{ "fragment_color" }
			);

	// FIXME: Create the RenderPass objects for bones here.
	RenderDataInput cylinder_pass_input;
	cylinder_pass_input.assign(0, "vertex_position", cylinder_mesh.vertices.data(), cylinder_mesh.vertices.size(), 4, GL_FLOAT);
	cylinder_pass_input.assignIndex(cylinder_mesh.indices.data(), cylinder_mesh.indices.size(), 2);
	RenderPass cylinder_pass(-1, cylinder_pass_input,
                             {cylinder_vertex_shader, nullptr, cylinder_fragment_shader},
    {std_model, std_view, std_proj, std_cy_transform, std_radius, std_height},
    {"fragment_color"}
                             );


    const char* cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 960x720 -i - "
                      "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

// open pipe to ffmpeg's stdin in binary write mode
// UNCOMMENT THIS CHUNK TO ENABLE MOVIE MODE
//    FILE* ffmpeg = popen(cmd, "w");
//        GLint dims[4] = {0};
//        glGetIntegerv(GL_VIEWPORT, dims);
//        GLint fbWidth = dims[2];
//        GLint fbHeight = dims[3];
//////        unsigned char* buffer = new unsigned char[fbWidth * fbHeight * 3];
//////        std::cout<< "HERE" << std::endl;
//        char *pixels = new char[4 * fbWidth * fbHeight];



	float aspect = 0.0f;
	std::cout << "center = " << mesh.getCenter() << "\n";

	bool draw_floor = true;
	bool draw_skeleton = true;
	bool draw_object = true;
	bool draw_cylinder = true;
    gui->floor = &floor_pass;
    gui->model = &object_pass;
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

            light_view = cubeMapViews[2];

            shadow_mesh_pass.setup();
            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0));

            shadow_floor_pass.setup();
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


        if (gui->recording) {

            std::chrono::time_point<std::chrono::steady_clock> current = mesh.clock->now();
            std::chrono::time_point<std::chrono::steady_clock> previous = mesh.previous;

            title << " Recording: "
                  << std::setprecision(2)
                  << std::setfill('0') << std::setw(6)
                  << cur_time << " sec";

            mesh.updateAnimation(float((current - previous).count()) / 1000000000.0);
            mesh.previous = current;

            glReadBuffer( GL_FRONT );
//            glReadPixels(0, 0, 960, 720, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//            fwrite(pixels, 4 * fbWidth * fbHeight, 1, ffmpeg);
            int currentFrame = int(mesh.totalRunTime);
            if (currentFrame == mesh.keyFrames.size() - 1) {
                            std::cout << "moneyfloat" << std::endl;
                gui->recording = false;
//                pclose(ffmpeg);
                mesh.totalRunTime = 0;
                mesh.previous = mesh.clock->now();
            }

        }
        else if (gui->isPlaying()) {
            std::chrono::time_point<std::chrono::steady_clock> current = mesh.clock->now();
            std::chrono::time_point<std::chrono::steady_clock> previous = mesh.previous;

            title << " Playing: "
                  << std::setprecision(2)
                  << std::setfill('0') << std::setw(6)
                  << cur_time << " sec";

            mesh.updateAnimation(float((current - previous).count()) / 1000000000.0);
            mesh.previous = current;
        } else if (gui->isPoseDirty()) {
            title << " Editing";
            // TODO: CHECK THIS
//            mesh.updateAnimation();
            gui->clearPose();
        }
        else
        {
            title << " Paused: "
                  << std::setprecision(2)
                  << std::setfill('0') << std::setw(6)
                  << cur_time << " sec";
        }

        glfwSetWindowTitle(window, title.str().data());
        


        // FIXME: update the preview textures here

		int current_bone = gui->getCurrentBone();


		// Draw bones first.
		if (draw_skeleton && gui->isTransparent()) {
			bone_pass.setup();
			// Draw our lines.
			// FIXME: you need setup skeleton.joints properly in
			//        order to see th bones.
			CHECK_GL_ERROR(glDrawElements(GL_LINES,
			                              bone_indices.size() * 2,
			                              GL_UNSIGNED_INT, 0));
		}




		draw_cylinder = (current_bone != -1 && gui->isTransparent());
        if (draw_cylinder) {
            std::vector<Bone*> bones = mesh.skeleton.bones;
            Bone* currentBone = bones[gui->getCurrentBone()];
            float distance = currentBone->distance();
            glm::vec3 yDirection = currentBone->direction();
            glm::vec3 zDirection(-yDirection[1], yDirection[0], 0);
            glm::vec3 xDirection = glm::cross(yDirection, zDirection);

            height = distance;
            cylinder_transform = glm::mat4({{1, 0, 0, 0},
                                            {0, 1, 0, 0},
                                            {0, 0, 1, 0},
                                            {currentBone->parent->position, 1}})
                                 * glm::mat4({{xDirection, 0}, {yDirection, 0}, {zDirection, 0}, {0, 0, 0, 1}});
            cylinder_pass.setup();
            CHECK_GL_ERROR(glDrawElements(GL_LINES, cylinder_mesh.vertices.size() * 2, GL_UNSIGNED_INT, 0));

        }



		// Then draw floor.
		if (draw_floor) {
			floor_pass.setup();
			// Draw our triangles.
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
			                              floor_faces.size() * 3,
			                              GL_UNSIGNED_INT, 0));
		}

		// Draw the model
		if (draw_object) {
			object_pass.setup();
			int mid = 0;
			while (object_pass.renderWithMaterial(mid))
				mid++;
		}

		float floor = int(gui->scrollBar / 240.0);
		scroll = ((gui->scrollBar - floor * 240.0 ) / 240.0) * 2.0 / 3.0;

        
        
        
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multifb);
        glBlitFramebuffer(0, 0, 960, 720, 0, 0, 960, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        


		// Poll and swap.
        // FIXME: Draw previews here, note you need to call glViewport
        
        glViewport(main_view_width, 0, preview_bar_width, preview_bar_height);
        
       
        
        if (int(floor) >= 0 && int(floor) < mesh.keyFrames.size()) {
            KeyFrame k0 = mesh.keyFrames[floor];
            if (!k0.rendered) {
                            updatePose(mesh, k0);
                gui->make_texture(k0);
            }
            texid = k0.texture.getTexture();
            mesh.keyFrames[floor] = k0;
                        
            preview1_pass.setup(); 
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, k0.texture.getTexture()));
            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, preview_triangles.size() * 3, GL_UNSIGNED_INT, 0));
            if (gui->currentSelectedKeyframe == floor) {
                show1= true;
            }else {
                show1 = false;
            }
        }
        if (int(floor + 1) >= 0 && int(floor + 1) < mesh.keyFrames.size()) {
            KeyFrame k1 = mesh.keyFrames[floor + 1];
            if (!k1.rendered) {
                updatePose(mesh, k1);
                gui->make_texture(k1);
            }
            texid2 = k1.texture.getTexture();
            mesh.keyFrames[floor + 1] = k1;
            preview2_pass.setup();
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, k1.texture.getTexture()));
            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, preview_triangles.size() * 3, GL_UNSIGNED_INT, 0));
            if (gui->currentSelectedKeyframe == floor + 1) {
                show2= true;
            }else {
                show2 = false;
            }
        }
        if (int(floor + 2) >= 0 && int(floor + 2) < mesh.keyFrames.size()) {
            KeyFrame k2 = mesh.keyFrames[floor + 2];
            if (!k2.rendered) {
                updatePose(mesh, k2);
                gui->make_texture(k2);
            }
            texid3 = k2.texture.getTexture();
            mesh.keyFrames[floor + 2] = k2;
            preview3_pass.setup();
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, k2.texture.getTexture()));
            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, preview_triangles.size() * 3, GL_UNSIGNED_INT, 0));
            if (gui->currentSelectedKeyframe == floor + 2) {
                show3= true;
            }else {
                show3 = false;
            }
        }
        if (int(floor + 3) >= 0 && int(floor + 3) < mesh.keyFrames.size()) {
            KeyFrame k3= mesh.keyFrames[floor + 3];
            if (!k3.rendered) {
                updatePose(mesh, k3);
                gui->make_texture(k3);
            }
            texid4 = k3.texture.getTexture();
            mesh.keyFrames[floor + 3] = k3;
            preview4_pass.setup();
            if (gui->currentSelectedKeyframe == floor + 3) {
                show4= true;
            } else {
                show4 = false;
            }
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, k3.texture.getTexture()));
            CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, preview_triangles.size() * 3, GL_UNSIGNED_INT, 0));
        }
        
        
        

        
        glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
