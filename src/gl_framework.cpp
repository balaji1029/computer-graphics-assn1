#include "gl_framework.hpp"
#include "utils.hpp"
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

	mode_t current_mode = NONE;
	axis_t current_axis = X;

	//!GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		//!Close the window if the ESC key was pressed
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_R:
				if (current_mode != ROTATE) {
					current_mode = ROTATE;
				}
				else {
					current_mode = NONE;
				}
				break;
			case GLFW_KEY_T:
				if (current_mode != TRANSLATE) {
					current_mode = TRANSLATE;
				}
				else {
					current_mode = NONE;
				}
				break;
			case GLFW_KEY_G:
				if (current_mode != SCALE) {
					current_mode = SCALE;
				}
				else {
					current_mode = NONE;
				}
				break;
			case GLFW_KEY_C:
				if (windowManager.selected_shape.lock()) {
					float r, g, b;
					std::cout << "Enter RGB values between 0 and 1 seperated with spaces: ";
					std::cin >> r >> g >> b;
					std::cout << "Color set to (" << r << ", " << g << ", " << b << ")\n";
					auto shape = windowManager.selected_shape.lock();
					shape->set_color(glm::vec4(r, g, b, 1.0f));
				}
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
					if (windowManager.selected_shape.lock()) {
						auto shape = windowManager.selected_shape.lock();
						switch (current_axis) {
						case X:
							shape->xrot += M_PI / 10;
							break;
						case Y:
							shape->yrot += M_PI / 10;
							break;
						case Z:
							shape->zrot += M_PI / 10;
							break;
						}
					}
				} else if (current_mode == SCALE) {
					if (windowManager.selected_shape.lock()) {
						auto shape = windowManager.selected_shape.lock();
						switch (current_axis) {
						case X:
							shape->xscale += 0.1f;
							break;
						case Y:
							shape->yscale += 0.1f;
							break;
						case Z:
							shape->zscale += 0.1f;
							break;
						}
					}
				} else if (current_mode == TRANSLATE) {
					if (windowManager.selected_shape.lock()) {
						auto shape = windowManager.selected_shape.lock();
						switch (current_axis) {
						case X:
							shape->xpos += 0.1f;
							break;
						case Y:
							shape->ypos += 0.1f;
							break;
						case Z:
							shape->zpos += 0.1f;
							break;
						}
					}
				}
				break;
			case GLFW_KEY_DOWN:
			case GLFW_KEY_MINUS:
				if (current_mode == ROTATE) {
					if (windowManager.selected_shape.lock()) {
						auto shape = windowManager.selected_shape.lock();
						switch (current_axis) {
						case X:
							shape->xrot -= M_PI / 10;
							break;
						case Y:
							shape->yrot -= M_PI / 10;
							break;
						case Z:
							shape->zrot -= M_PI / 10;
							break;
						}
					}
				} else if (current_mode == SCALE) {
					if (windowManager.selected_shape.lock()) {
						auto shape = windowManager.selected_shape.lock();
						switch (current_axis) {
						case X:
							shape->xscale -= 0.1f;
							if (shape->xscale < 0.1f) shape->xscale = 0.1f;
							break;
						case Y:
							shape->yscale -= 0.1f;
							if (shape->yscale < 0.1f) shape->yscale = 0.1f;
							break;
						case Z:
							shape->zscale -= 0.1f;
							if (shape->zscale < 0.1f) shape->zscale = 0.1f;
							break;
						}
					}
				} else if (current_mode == TRANSLATE) {
					if (windowManager.selected_shape.lock()) {
						auto shape = windowManager.selected_shape.lock();
						switch (current_axis) {
						case X:
							shape->xpos -= 0.1f;
							break;
						case Y:
							shape->ypos -= 0.1f;
							break;
						case Z:
							shape->zpos -= 0.1f;
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
};
