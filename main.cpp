#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/shader.hpp"

using namespace std;

int main(){
    if(!glfwInit()){ //Iniciamos la libreria glfw
        fprintf(stderr, "Error al iniciar GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Para OpenGL 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Para tener el nuevo OpenGL el de core
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Para que funcione bien en mac, nice steve jobs

    //Creamos una variable ventana
    GLFWwindow* window = glfwCreateWindow(1024, 768, "PLATAFORMA", NULL, NULL);
    if(window == NULL){
        fprintf(stderr, "Fallo al crear la ventana\n");
        glfwTerminate();
        return -1;
    }
    //Poniendola en su contexto de OpenGL
    glfwMakeContextCurrent(window);

    //Se inicia GLEW
    glewExperimental = true; //Esto parece ser que es para un tal perfil de base
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "Fallo al inicializar GLEW\n");
        return -1;
    }

    //Intuyo que esto es para que glfw se ponga a leer las teclas
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); 

    //Esto hace que el fondo sea azul
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Habilitar el test de profundidad
    glEnable(GL_DEPTH_TEST);
    // Aceptar el fragmento si está más cerca de la cámara que el fragmento anterior
    glDepthFunc(GL_LESS);

    //Creamos un VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //Cargamos los shaders
    GLuint programID = LoadShaders( "shaders/transformVertexShader.vertexshader", "shaders/colorFragmentShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    //Declaramos un cubo formado por 6 caras dos triangulos por cara
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triángulo 1 : comienza
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triángulo 1 : termina
        1.0f, 1.0f,-1.0f, // triángulo 2 : comienza
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triángulo 2 : termina
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // Un color por vértice. Fueron generados al azar.
    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    //Creamos el vertex buffer
    GLuint vertexbuffer;
    //Generamos un buffer y ponerlo en el vertexbuffer creado
    glGenBuffers(1, &vertexbuffer);
    //Le otorgamos caracteristicas
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //Le damos los vertices a OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //Buffer de los colores
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    //Bucle Principal
    do{
        //Limpiamos la pantalla para poner el fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Le decimos que use los shaders
        glUseProgram(programID);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        //Primer atributo del buffer: vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0, //shader
            3, //Tamaño
            GL_FLOAT, //Tipo
            GL_FALSE, //Normalizado?
            0, //Paso
            (void*)0 //Desfase del buffer
        );

        // 2do atributo del buffer : colores
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // Atributo. No hay razón especial para el 1, pero debe corresponder al número en el shader.
            3,                                // tamaño
            GL_FLOAT,                         // tipo
            GL_FALSE,                         // normalizado?
            0,                                // corrimiento
            (void*)0                          // corrimiento de buffer
        );

        //Dibujamos el triangulo
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(0);

        //Funciones de la ventana
        glfwSwapBuffers(window);
        glfwPollEvents();

    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); //Aqui es donde detecta si se pulsa ESC
    //El ShouldClose creo que es para si pulsas la x de la ventana para cerrar

    // Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

    return 0;
}
