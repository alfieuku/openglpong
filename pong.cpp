#include<iostream>
#include<cmath>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void updateOctagonVertices(float* arr, float centerX, float centerY, float radius) {

	arr[0] = centerX;
	arr[1] = centerY;

	float angleStep = 3.1415 / 4.0f;
	for (int i = 0; i < 8; i++) {

		float angle = i * angleStep;
		float x = centerX + radius * cos(angle);
		float y = centerY + radius * sin(angle);

		arr[2 + i * 2] = x;
		arr[2 + i * 2 + 1] = y;
	}

	arr[18] = centerX + radius * cos(0.0f);
	arr[19] = centerY + radius * sin(0.0f);
}

void updatePaddleVertices(float* change, float paddleX, float paddleY) {


	change[0] = paddleX; //x1topright
	change[1] = paddleY; //y1topright
	change[2] = paddleX - 0.4f; //x1topleft
	change[3] = paddleY; //y1topleft
	change[4] = paddleX; //x1bottomright
	change[5] = paddleY - 0.05f; //y1bottomright
	change[6] = paddleX - 0.4f; //x2topleft
	change[7] = paddleY; //y2topleft
	change[8] = paddleX; //x2bottomright
	change[9] = paddleY - 0.05f; //y2bottomright
	change[10] = paddleX - 0.4f; //x2bottomleft
	change[11] = paddleY - 0.05f; //y2bottomleft
}




int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Pong", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glfwSwapBuffers(window);



	float paddlevertices[12];

	float paddleX = 0.2f;
	float paddleY = -0.65f;

	float paddlelength = 0.4f;
	float paddleVelX = 0.01f;


	float octagonvertices[20];
	
	float octX = 0.0f;
	float octY = 0.6f;
	float octRadius = 0.05f;
	float octVelX = 0.001f;
	float octVelY = 0.001f;


	updateOctagonVertices(octagonvertices, octX, octY, octRadius);

	updatePaddleVertices(paddlevertices, paddleX, paddleY);

	unsigned int VAO1, VBO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(paddlevertices), paddlevertices, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int VAO2, VBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(octagonvertices), octagonvertices, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		const float minX = -0.6f;
		const float maxX = 1.0f;


		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			paddleX = fmin(paddleX + paddleVelX * 0.1f, maxX);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			paddleX = fmax(paddleX - paddleVelX * 0.1f, minX);
		}

		octX += octVelX;
		octY += octVelY;

		if (octY - octRadius < paddleY) {
			if (octX + octRadius < (paddleX + 0.1) && octX - octRadius > (paddleX - 0.5f) ) {
				octVelY = -octVelY;
			}
		}

		if (octX + octRadius > 1.0f) {
			octX = 1.0f - octRadius;
			octVelX = -octVelX;
		}

		if (octX - octRadius < -1.0f) {
			octX = -1.0f + octRadius;
			octVelX = -octVelX;
		}

		if (octY + octRadius > 1.0f) {
			octY = 1.0f - octRadius;
			octVelY = -octVelY;
		}

		if (octY - octRadius < -1.0f) {
			glfwDestroyWindow(window);
			glfwTerminate();
		}



		/* if (octX - octRadius < ponglength && octY - octRadius < pongheight) {
			octY = -1.0f + octRadius;
			octVelY = -octVelY;
		}*/


		updateOctagonVertices(octagonvertices, octX, octY, octRadius);
		

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(octagonvertices), octagonvertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		updatePaddleVertices(paddlevertices, paddleX, paddleY);

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(paddlevertices), paddlevertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 10);

		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}
