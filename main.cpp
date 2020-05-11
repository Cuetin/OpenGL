#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

using namespace std;

int main(){
    if(!glfwInit()){ //Iniciamos la libreria glfw
        cout << "Error al iniciar GLFW" << endl;
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
        cout << "Fallo al crear la ventana" << endl;
        glfwTerminate();
        return -1;
    }
    //Poniendola en su contexto de OpenGL
    glfwMakeContextCurrent(window);
    glewExperimental = true; //Esto parece ser que es para un tal perfil de base
    if(glewInit() != GLEW_OK){
        cout << "Fallo al inicializar GLEW" << endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //Intuyo que esto es para que glfw se ponga a leer las teclas

    //Esto hace que el fondo sea azul
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    //Creamos un VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //Cargamos los shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

    //Declaramos un vector que representa los tres vertices
    static const GLfloat g_vertex_buffer_data[] ={
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    //Creamos el vertex buffer
    GLuint vertexbuffer;
    //Generamos un buffer y ponerlo en el vertexbuffer creado
    glGenBuffers(1, &vertexbuffer);
    //Le otorgamos caracteristicas
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //Le damos los vertices a OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //Bucle Principal
    do{
        //Limpiamos la pantalla para poner el fondo
        glClear(GL_COLOR_BUFFER_BIT);
        //Le decimos que use los shaders
        glUseProgram(programID);

        //Atributos del buffer: vertices
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
        //Dibujamos el triangulo
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        //Funciones de la ventana
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); //Aqui es donde detecta si se pulsa ESC
    //El ShouldClose creo que es para si pulsas la x de la ventana para cerrar
    return 0;
}

