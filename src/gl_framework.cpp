#include "gl_framework.hpp"
#include "window.hpp"

#include <glm/glm.hpp>

// extern GLfloat xrot, yrot, zrot;

extern WindowManager windowManager;

namespace csX75 {
	//! Initialize GL State
	void initGL(void) {
		//Set framebuffer clear color
		glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
		//Set depth buffer furthest depth
		glClearDepth(1.0);
		//Set depth test to less-than
		glDepthFunc(GL_LESS);
		//Enable depth testing
		glEnable(GL_DEPTH_TEST);
	}

	//!GLFW Error Callback
	void error_callback(int error, const char* description) {
		std::cerr << description << std::endl;
	}

	//!GLFW framebuffer resize callback
	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		//!Resize the viewport to fit the window size - draw to entire window
		glViewport(0, 0, width, height);
	}

	enum mode_t {
		ROTATE,
		TRANSLATE,
		SCALE,
		NONE,
	};

	enum axis_t {
		X,
		Y,
		Z
	};

	enum home_mode_t {
		MODELLING,
		INSPECTION
	};

	mode_t current_mode = NONE;
	axis_t current_axis = X;
	home_mode_t current_home_mode = INSPECTION;

	//!GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		//!Close the window if the ESC key was pressed
		if ((action == GLFW_PRESS || action == GLFW_REPEAT)) {
			// -----MODELLING MODE-----
			if(current_home_mode == MODELLING)
				switch (key) {
					case GLFW_KEY_I:
						current_home_mode = INSPECTION;
						break;
					case GLFW_KEY_ESCAPE:
						glfwSetWindowShouldClose(window, GL_TRUE);
						break;
					case GLFW_KEY_R:
						if (current_mode != ROTATE) {
							current_mode = ROTATE;
						}
						break;
					case GLFW_KEY_T:
						if (current_mode != TRANSLATE) {
							current_mode = TRANSLATE;
						}
						break;
					case GLFW_KEY_G:
						if (current_mode != SCALE) {
							current_mode = SCALE;
						}
						break;
					case GLFW_KEY_C:
						if (windowManager.model->selected_node.lock()) {
							float r, g, b;
							std::cout << "Enter RGB values between 0 and 1 seperated with spaces: ";
							std::cin >> r >> g >> b;
							std::cout << "Color set to (" << r << ", " << g << ", " << b << ")\n";
							auto node = windowManager.model->selected_node.lock();
							node->set_color(glm::vec4(r, g, b, 1.0f));
						}
						break;
					case GLFW_KEY_S:
						if (!windowManager.model) {
							std::cerr << "No model to serialize\n";
						} else {
							std::string filename;
							std::cout << "Enter filename to serialize to: ";
							std::cin >> filename;
							filename = "models/" + filename;
							std::ofstream ofs(filename);
							if (!ofs) {
								std::cerr << "Error opening file " << filename << " for writing\n";
							} else {
								ofs << *(windowManager.model);
								ofs.close();
								std::cout << "Model serialized to " << filename << "\n";
							}
						}
						break;
					case GLFW_KEY_1:
						windowManager.addNode(shape_type_t::SPHERE_SHAPE, 4);
						break;
					case GLFW_KEY_2:
						windowManager.addNode(shape_type_t::CYLINDER_SHAPE, 4);
						break;
					case GLFW_KEY_3:
						windowManager.addNode(shape_type_t::BOX_SHAPE, 4);
						break;
					case GLFW_KEY_4:
						windowManager.addNode(shape_type_t::CONE_SHAPE, 4);
						break;
					case GLFW_KEY_5:
						windowManager.model->removeSelectedNode();
						break;
					case GLFW_KEY_Q:
						current_mode = NONE;
						break;
				case GLFW_KEY_X:
					current_axis = X;
					break;
				case GLFW_KEY_Y:
					current_axis = Y;
					break;
				case GLFW_KEY_Z:
					current_axis = Z;
					break;
				case GLFW_KEY_UP:
				case GLFW_KEY_EQUAL:
					if (current_mode == ROTATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xrot += M_PI / 10;
								break;
							case Y:
								node->yrot += M_PI / 10;
								break;
							case Z:
								node->zrot += M_PI / 10;
								break;
							}
						}
					} else if (current_mode == SCALE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xscale += 0.1f;
								break;
							case Y:
								node->yscale += 0.1f;
								break;
							case Z:
								node->zscale += 0.1f;
								break;
							}
						}
					} else if (current_mode == TRANSLATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xpos += 0.1f;
								node->centroid += glm::vec4(0.1f, 0, 0, 0);
								break;
							case Y:
								node->ypos += 0.1f;
								node->centroid += glm::vec4(0, 0.1f, 0, 0);
								break;
							case Z:
								node->zpos += 0.1f;
								node->centroid += glm::vec4(0, 0, 0.1f, 0);
								break;
							}
						}
					} else if (current_mode == NONE) {	
						switch(current_axis) {
						case X:
							windowManager.xrot += M_PI / 10;
							break;
						case Y:
							windowManager.yrot += M_PI / 10;
							break;
						case Z:
							windowManager.zrot += M_PI / 10;
							break;
						}
					}
					break;
				case GLFW_KEY_DOWN:
				case GLFW_KEY_MINUS:
					if (current_mode == ROTATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xrot -= M_PI / 10;
								break;
							case Y:
								node->yrot -= M_PI / 10;
								break;
							case Z:
								node->zrot -= M_PI / 10;
								break;
							}
						}
					} else if (current_mode == SCALE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xscale -= 0.1f;
								if (node->xscale < 0.1f) node->xscale = 0.1f;
								break;
							case Y:
								node->yscale -= 0.1f;
								if (node->yscale < 0.1f) node->yscale = 0.1f;
								break;
							case Z:
								node->zscale -= 0.1f;
								if (node->zscale < 0.1f) node->zscale = 0.1f;
								break;
							}
						}
					} else if (current_mode == TRANSLATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xpos -= 0.1f;
								node->centroid -= glm::vec4(0.1f, 0, 0, 0);
								break;
							case Y:
								node->ypos -= 0.1f;
								node->centroid -= glm::vec4(0, 0.1f, 0, 0);
								break;
							case Z:
								node->zpos -= 0.1f;
								node->centroid -= glm::vec4(0, 0, 0.1f, 0);
								break;
							}
						}
					} else if (current_mode == NONE) {
						switch(current_axis) {
						case X:
							windowManager.xrot -= M_PI / 10;
							break;
						case Y:
							windowManager.yrot -= M_PI / 10;
							break;
						case Z:
							windowManager.zrot -= M_PI / 10;
							break;
						}
					}
					break;
					default:
						break;
				}
			// -----INSPECTION MODE-----
			else
			switch (key){
				case GLFW_KEY_M:
					current_home_mode = MODELLING;
					break;
				case GLFW_KEY_L:
					{	
						std::string filename;
						std::cout << "Enter filename to load from: ";
						std::cin >> filename;
						filename = "models/" + filename;
					
						std::ifstream ifs(filename);
						if (!ifs) {
							std::cerr << "Error opening file " << filename << " for reading\n";
						} else {
							windowManager.model = std::make_shared<model_t>(windowManager.vPosition, windowManager.vColor);
							ifs >> *(windowManager.model);
							ifs.close();
							std::cout << "Model loaded from " << filename << "\n";
						}
					}
				case GLFW_KEY_X:
					current_axis = X;
					break;
				case GLFW_KEY_Y:
					current_axis = Y;
					break;
				case GLFW_KEY_Z:
					current_axis = Z;
					break;
				case GLFW_KEY_UP:
				case GLFW_KEY_EQUAL:
					if (current_mode == ROTATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xrot += M_PI / 10;
								break;
							case Y:
								node->yrot += M_PI / 10;
								break;
							case Z:
								node->zrot += M_PI / 10;
								break;
							}
						}
					} else if (current_mode == SCALE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xscale += 0.1f;
								break;
							case Y:
								node->yscale += 0.1f;
								break;
							case Z:
								node->zscale += 0.1f;
								break;
							}
						}
					} else if (current_mode == TRANSLATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xpos += 0.1f;
								break;
							case Y:
								node->ypos += 0.1f;
								break;
							case Z:
								node->zpos += 0.1f;
								break;
							}
						}
					} else if (current_mode == NONE) {	
						switch(current_axis) {
						case X:
							windowManager.xrot += M_PI / 10;
							break;
						case Y:
							windowManager.yrot += M_PI / 10;
							break;
						case Z:
							windowManager.zrot += M_PI / 10;
							break;
						}
					}
					break;
				case GLFW_KEY_DOWN:
				case GLFW_KEY_MINUS:
					if (current_mode == ROTATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xrot -= M_PI / 10;
								break;
							case Y:
								node->yrot -= M_PI / 10;
								break;
							case Z:
								node->zrot -= M_PI / 10;
								break;
							}
						}
					} else if (current_mode == SCALE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xscale -= 0.1f;
								if (node->xscale < 0.1f) node->xscale = 0.1f;
								break;
							case Y:
								node->yscale -= 0.1f;
								if (node->yscale < 0.1f) node->yscale = 0.1f;
								break;
							case Z:
								node->zscale -= 0.1f;
								if (node->zscale < 0.1f) node->zscale = 0.1f;
								break;
							}
						}
					} else if (current_mode == TRANSLATE) {
						if (windowManager.model->selected_node.lock()) {
							auto node = windowManager.model->selected_node.lock();
							switch (current_axis) {
							case X:
								node->xpos -= 0.1f;
								break;
							case Y:
								node->ypos -= 0.1f;
								break;
							case Z:
								node->zpos -= 0.1f;
								break;
							}
						}
					} else if (current_mode == NONE) {
						switch(current_axis) {
						case X:
							windowManager.xrot -= M_PI / 10;
							break;
						case Y:
							windowManager.yrot -= M_PI / 10;
							break;
						case Z:
							windowManager.zrot -= M_PI / 10;
							break;
						}
					}
					break;
				default:
					break;
			}
		}
	}
};
