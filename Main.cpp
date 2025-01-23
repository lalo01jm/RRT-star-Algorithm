#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <time.h>

#define NUMNODOS 10000

unsigned int shader = 0;

float gamma_rrtstar = 1.0f;

GLFWwindow* window;

// Clase prisma
class Prisma
{
public: 
    glm::vec3 position;
    float width, height, depth;
    glm::vec3 direction;

    // Constructor
    Prisma(glm::vec3 position, glm::vec3 direction, float scale, float size)
    {
        if (abs(direction.x) > 0.8)
        {
			this->width = direction.x * size * scale;
			this->height = 0.5f * scale;
			this->depth = 0.5f * scale;
		}
        else if (abs(direction.y) > 0.8)
        {
			this->width = 0.5f * scale;
			this->height = direction.y * size * scale;
			this->depth = 0.5f * scale;
		}
        else if (abs(direction.z) > 0.8)
        {
			this->width = 0.5f * scale;
			this->height = 0.5f * scale;
			this->depth = direction.z * size * scale;
		}
        else
        {
			this->width = 0.5f * scale;
			this->height = 0.5f * scale;
			this->depth = 0.5f * scale;
		}
		this->position = position;
		this->direction = direction;
		// Normalizar direccion
		this->direction = glm::normalize(this->direction);
	}

    // Constructor con dimensiones especificas
    Prisma(glm::vec3 position, glm::vec3 direction, float width, float height, float depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->position = position;
        this->direction = direction;

        // Normalizar direccion
        this->direction = glm::normalize(this->direction);
    }

    // Dibujar prisma
    void draw()
    {
		// Vertices
        float positions[] = {
            position.x - (width / 2) + (width / 2 - 0.25) * direction.x, position.y - (height / 2) + (height / 2 - 0.25) * direction.y, position.z - (depth / 2) + (depth / 2 - 0.25) * direction.z, // 0
            position.x + (width / 2) + (width / 2 - 0.25) * direction.x, position.y - (height / 2) + (height / 2 - 0.25) * direction.y, position.z - (depth / 2) + (depth / 2 - 0.25) * direction.z, // 1
            position.x + (width / 2) + (width / 2 - 0.25) * direction.x, position.y + (height / 2) + (height / 2 - 0.25) * direction.y, position.z - (depth / 2) + (depth / 2 - 0.25) * direction.z, // 2
            position.x - (width / 2) + (width / 2 - 0.25) * direction.x, position.y + (height / 2) + (height / 2 - 0.25) * direction.y, position.z - (depth / 2) + (depth / 2 - 0.25) * direction.z, // 3
            position.x - (width / 2) + (width / 2 - 0.25) * direction.x, position.y - (height / 2) + (height / 2 - 0.25) * direction.y, position.z + (depth / 2) + (depth / 2 - 0.25) * direction.z, // 4
            position.x + (width / 2) + (width / 2 - 0.25) * direction.x, position.y - (height / 2) + (height / 2 - 0.25) * direction.y, position.z + (depth / 2) + (depth / 2 - 0.25) * direction.z, // 5
            position.x + (width / 2) + (width / 2 - 0.25) * direction.x, position.y + (height / 2) + (height / 2 - 0.25) * direction.y, position.z + (depth / 2) + (depth / 2 - 0.25) * direction.z, // 6
            position.x - (width / 2) + (width / 2 - 0.25) * direction.x, position.y + (height / 2) + (height / 2 - 0.25) * direction.y, position.z + (depth / 2) + (depth / 2 - 0.25) * direction.z  // 7
		};

		// Crear buffer de vertices
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

		// Habilitar atributos de vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		// Indices de los vertices de cada cara
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
            1, 5, 6,
            6, 2, 1,
            7, 6, 5,
            5, 4, 7,
            4, 0, 3,
            3, 7, 4,
            4, 5, 1,
            1, 0, 4,
            3, 2, 6,
            6, 7, 3
        };

        // Crear buffer de indices
        unsigned int indexBuffer;
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Pasar normales
        float normals[] = {
			0.0f, 0.0f, -1.0f, // 0
			0.0f, 0.0f, -1.0f, // 1
			0.0f, 0.0f, -1.0f, // 2
			0.0f, 0.0f, -1.0f, // 3
			0.0f, 0.0f, 1.0f,  // 4
			0.0f, 0.0f, 1.0f,  // 5
			0.0f, 0.0f, 1.0f,  // 6
			0.0f, 0.0f, 1.0f,  // 7
			-1.0f, 0.0f, 0.0f, // 8
			-1.0f, 0.0f, 0.0f, // 9
			-1.0f, 0.0f, 0.0f, // 10
			-1.0f, 0.0f, 0.0f, // 11
			1.0f, 0.0f, 0.0f,  // 12
			1.0f, 0.0f, 0.0f,  // 13
			1.0f, 0.0f, 0.0f,  // 14
			1.0f, 0.0f, 0.0f,  // 15
			0.0f, -1.0f, 0.0f, // 16
			0.0f, -1.0f, 0.0f, // 17
			0.0f, -1.0f, 0.0f, // 18
			0.0f, -1.0f, 0.0f, // 19
			0.0f, 1.0f, 0.0f,  // 20
            0.0f, 1.0f, 0.0f,  // 21
            0.0f, 1.0f, 0.0f,  // 22
            0.0f, 1.0f, 0.0f   // 23
        };

        // Crear buffer de normales
        unsigned int normalBuffer;
        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        // Habilitar atributos de normales
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        // Dibujar prisma
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    // Cambiar posicion
    void setPosition(glm::vec3 position)
    {
        this->position = position;
    }
};

// Funcion que verifica si dos prismas colisionan
bool checkCollisionPrismas(Prisma prisma1, Prisma prisma2)
{
    // Obtener centro de cada prisma dependiendo de la direccion
    glm::vec3 center1;
    glm::vec3 center2;

    if (abs(prisma1.direction.x) > 0.8)
    {
		center1 = glm::vec3(prisma1.position.x + prisma1.width / 2.0f, prisma1.position.y, prisma1.position.z);
	}
    else if (abs(prisma1.direction.y) > 0.8)
    {
        center1 = glm::vec3(prisma1.position.x, prisma1.position.y + prisma1.height / 2.0f, prisma1.position.z);
	}
    else if (abs(prisma1.direction.z) > 0.8)
    {
        center1 = glm::vec3(prisma1.position.x, prisma1.position.y, prisma1.position.z + prisma1.depth / 2.0f);
	}

    if (abs(prisma2.direction.x) > 0.8)
    {
		center2 = glm::vec3(prisma2.position.x + prisma2.width / 2.0f, prisma2.position.y, prisma2.position.z);
    }
    else if (abs(prisma2.direction.y) > 0.8)
    {
		center2 = glm::vec3(prisma2.position.x, prisma2.position.y + prisma2.height / 2.0f, prisma2.position.z);
    }
    else if (abs(prisma2.direction.z) > 0.8)
    {
        center2 = glm::vec3(prisma2.position.x, prisma2.position.y, prisma2.position.z + prisma2.depth / 2.0f);
    }

    // Obtener distancia entre centros
    float distance = glm::distance(center1, center2);

    // Dibujar prismas

    // Para cada coordenada verificar si la distancia es menor a la suma de las mitades de los lados
    if (abs(center1.x - center2.x) < (abs(prisma1.width) / 2.0f + abs(prisma2.width) / 2.0f) &&
        abs(center1.y - center2.y) < (abs(prisma1.height) / 2.0f + abs(prisma2.height) / 2.0f) &&
        abs(center1.z - center2.z) < (abs(prisma1.depth) / 2.0f + abs(prisma2.depth) / 2.0f)) {
        return true;
    }
    else {
        return false;
    }
}

// Clase obstaculo (hereda de prisma)
class Obstaculo : public Prisma
{
public:
    glm::vec3 centro;
    // Constructor
    Obstaculo(glm::vec3 centro, float width, float height, float depth) : Prisma(centro - (width / 2.0f), glm::vec3(1.0f, 0.0f, 0.0f), width, height, depth)
    {
		this->centro = centro;
	}

    // Constructor predeterminado
    Obstaculo() : Prisma(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f)
	{
        this->centro = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Regresar prisma
    Prisma getPrisma()
    {
        return Prisma(this->centro - (width / 2.0f), this->direction, this->width, this->height, this->depth);
    }

	// Dibujar obstaculo
    void draw()
    {
		// Color naranja
        glUniform3f(glGetUniformLocation(shader, "ColorIn"), 1.0f, 0.5f, 0.0f);

		// Dibujar prisma
		Prisma::draw();
	}
};

// Clase articulacion
class Articulacion
{
public:
	float a, alpha, d, theta;
    Articulacion* padre, * hijo;

    // Tamanio de las articulaciones
    float size;

    // Matriz de transformacion de Denavit-Hartenberg
    glm::mat4 DH;

    // Constructor predeterminado
    Articulacion()
    {
        this->a = 0.0f;
        this->alpha = 0.0f;
        this->d = 0.0f;
        this->theta = 0.0f;
        this->size = 0.0f;
    }

    // Constructor
    Articulacion(float a, float alpha, float d, float theta, float size)
    {
		this->a = a;
		this->alpha = alpha;
		this->d = d;
		this->theta = theta;
        this->size = size;

		// Inicializar matriz de transformacion
		this->DH = glm::mat4(1.0f);

        // Inicializar punteros
		this->padre = nullptr;
		this->hijo = nullptr;

        // Calcular matriz de transformacion
		this->calcularDH();
	}

    // Cambiar parametros de Denavit-Hartenberg
    void setDH(float a, float alpha, float d, float theta, float size)
    {
        this->a = a;
        this->alpha = alpha;
        this->d = d;
        this->theta = theta;
        this->size = size;

        // Calcular matriz de transformacion
        this->calcularDH();
    }

    // Calcular matriz de transformacion
    void calcularDH()
    {
        // Matriz de transformacion (Asignar valores manualmente)
        glm::mat4 A = glm::mat4(1.0f);
        A[0][0] = cos(glm::radians(this->theta));
        A[0][1] = -sin(glm::radians(this->theta)) * cos(glm::radians(this->alpha));
        A[0][2] = sin(glm::radians(this->theta)) * sin(glm::radians(this->alpha));
        A[0][3] = this->a * cos(glm::radians(this->theta));
        A[1][0] = sin(glm::radians(this->theta));
        A[1][1] = cos(glm::radians(this->theta)) * cos(glm::radians(this->alpha));
        A[1][2] = -cos(glm::radians(this->theta)) * sin(glm::radians(this->alpha));
        A[1][3] = this->a * sin(glm::radians(this->theta));
        A[2][0] = 0.0f;
        A[2][1] = sin(glm::radians(this->alpha));
        A[2][2] = cos(glm::radians(this->alpha));
        A[2][3] = this->d;
        A[3][0] = 0.0f;
        A[3][1] = 0.0f;
        A[3][2] = 0.0f;
        A[3][3] = 1.0f;


		// Matriz de transformacion
		this->DH = A;
	}

    // Cambiar articulacion padre
    void setPadre(Articulacion* padre)
    {
		this->padre = padre;
	}

	// Cambiar articulacion hijo
    void setHijo(Articulacion* hijo)
    {
		this->hijo = hijo;
	}

	// Obtener matriz de transformacion
    glm::mat4 getDH()
    {
		return this->DH;
	}

    // Obtener matriz de transformacion global
    glm::mat4 getDHGlobal()
    {
		// Si no tiene padre, la matriz de transformacion global es la misma
		if (this->padre == nullptr)
			return this->DH;
		// Si tiene padre, la matriz de transformacion global es la matriz de transformacion del padre por la matriz de transformacion local
		else
			return this->DH * this->padre->getDHGlobal();
	}

    // Obtener posicion
    glm::vec3 getPosition()
    {
        // Obtener matriz de transformacion global
        glm::mat4 DHGlobal = this->getDHGlobal();
        // Obtener posicion
        glm::vec3 position = glm::vec3(DHGlobal[0][3], DHGlobal[1][3], DHGlobal[2][3]);

        return position;
    }

    // Obtener direccion
    glm::vec3 getDirection()
    {
        // Obtener matriz de transformacion global
        glm::mat4 DHGlobal = this->getDHGlobal();
        // Obtener direccion
        glm::vec3 direction = glm::vec3(DHGlobal[0][2], DHGlobal[1][2], DHGlobal[2][2]);
        return direction;
    }

    // Cambiar tamanio
    void setSize(float size)
    {
		this->size = size;
	}

    // Dibujar parte del robot
    void draw()
    {
        // Obtener posicion
        glm::vec3 position = this->getPosition();

        // Obtener direccion
        glm::vec3 direction = this->getDirection();

        // Dibujar prisma
        Prisma prisma = Prisma(position, direction, 1.0f, size);
        prisma.draw();

        // Si tiene hijo, dibujar hijo
        /*if (this->hijo != nullptr)
			this->hijo->draw();*/
    }

    // Dibujar un poco más ancho
    void drawbig()
    {
		// Obtener posicion
		glm::vec3 position = this->getPosition();

		// Obtener direccion
		glm::vec3 direction = this->getDirection();

		// Dibujar prisma
		Prisma prisma = Prisma(position, direction, 1.2f, size);
		prisma.draw();
    }
};

// Robot cartesiano
class Robot
{
public:
    // Articulaciones
	Articulacion articulacion0, articulacion1, articulacion2, articulacion3;

    // Configuracion
    glm::vec3 conf;

	// Constructor
    Robot(glm::vec3 confI)
    {
        // Configuracion
		conf = confI;

		// Crear articulaciones
		articulacion0 = Articulacion(0.0f, 0.0f, 0.0f, 0.0f, 0.5f);
		articulacion1 = Articulacion(0.0f, 90.0f, conf[0], 0.0f, 0.0f);
		articulacion2 = Articulacion(0.0f, 90.0f, -conf[1], 90.0f, 0.5f);
		articulacion3 = Articulacion(0.0f, 0.0f, conf[2], 0.0f, 0.5f);

		// Cambiar articulaciones padre e hijo
		articulacion1.setPadre(&articulacion0);
		articulacion0.setHijo(&articulacion1);
		articulacion2.setPadre(&articulacion1);
		articulacion1.setHijo(&articulacion2);
		articulacion3.setPadre(&articulacion2);
		articulacion2.setHijo(&articulacion3);
	}

    // Cambiar configuracion
    void setConf(glm::vec3 conf)
    {
        this->conf = conf;

        // Cambiar parametros de Denavit-Hartenberg
        articulacion0.setDH(0.0f, 0.0f, 0.0f, 0.0f, conf[0] + 0.5f);
        articulacion1.setDH(0.0f, 90.0f, conf[0], 0.0f, conf[1]);
        articulacion2.setDH(0.0f, 90.0f, -conf[1], 90.0f, conf[2] + 0.5f); //No se porque se invierte el signo
        articulacion3.setDH(0.0f, 0.0f, conf[2], 0.0f, 0.5f);
    }

    // Cambiar configuracion delta
    void setConfDelta(glm::vec3 delta)
    {
        this->setConf(glm::vec3(this->conf[0] + delta[0], this->conf[1] + delta[1], this->conf[2] + delta[2]));
    }

    // Obtener configuracion
    glm::vec3 getConf()
    {
		return this->conf;
	}

    // Dibujar robot
    void draw()
    {
		// Dibujar articulacion 0 roja
		glUniform3f(glGetUniformLocation(shader, "ColorIn"), 1.0f, 0.0f, 0.0f);
		articulacion0.draw();

		// Dibujar articulacion 1 verde
		glUniform3f(glGetUniformLocation(shader, "ColorIn"), 0.0f, 1.0f, 0.0f);
		articulacion1.draw();

		// Dibujar articulacion 2 azul
		glUniform3f(glGetUniformLocation(shader, "ColorIn"), 0.0f, 0.0f, 1.0f);
		articulacion2.draw();

		// Dibujar articulacion 3 amarilla
		glUniform3f(glGetUniformLocation(shader, "ColorIn"), 1.0f, 1.0f, 0.0f);
		articulacion3.drawbig();
    }
};

// Funcion de colision entre robot y varios obstaculos
bool checkCollision(Robot robot, Obstaculo* obst, int n)
{
	// Para cada obstaculo
    for (int i = 0; i < n; i++)
    {
        // Crear prisma del obstaculo
        Prisma prismaObst = obst[i].getPrisma();

        // Dibujar obstaculo
        glUniform3f(glGetUniformLocation(shader, "ColorIn"), 1.0f, 0.5f, 0.0f);
        //prismaObst.draw();

		// Para cada articulacion
        for (int j = 0; j < 4; j++)
        {
			// Obtener posicion de la articulacion
			glm::vec3 position = robot.articulacion0.getPosition();
			if (j == 1)
				position = robot.articulacion1.getPosition();
			else if (j == 2)
				position = robot.articulacion2.getPosition();
			else if (j == 3)
				position = robot.articulacion3.getPosition();

			// Obtener direccion de la articulacion
			glm::vec3 direction = robot.articulacion0.getDirection();
			if (j == 1)
				direction = robot.articulacion1.getDirection();
			else if (j == 2)
				direction = robot.articulacion2.getDirection();
			else if (j == 3)
				direction = robot.articulacion3.getDirection();

            // Obtener tamanio de la articulacion
            float size = robot.articulacion0.size;
            if (j == 1)
				size = robot.articulacion1.size;
			else if (j == 2)
				size = robot.articulacion2.size;
			else if (j == 3)
				size = robot.articulacion3.size;

			// Crear prisma
			Prisma prisma = Prisma(position, direction, 1.0f, size);

            // dibujar prisma
            //prisma.draw();

			// Si colisiona con el obstaculo
            if (checkCollisionPrismas(prisma, prismaObst))
				return true;
		}
	}
	return false;
}

// Funcion que verifica si un estado de configuración colisiona con un obstaculo
bool collides(glm::vec3 conf, Obstaculo* obst)
{
	// Crear robot
	Robot robot = Robot(conf);

    return checkCollision(robot, obst, 8);
}

// Función que verifica si un camino colisiona con un obstaculo
bool ObstacleFree(glm::vec3 confA, glm::vec3 confB, Obstaculo* obst)
{
    // Crear robot
	Robot robot = Robot(confA);

	// Obtener distancia entre configuraciones
	float distancia = glm::distance(confA, confB);

	// Obtener direccion
	glm::vec3 direction = glm::normalize(confB - confA);

	// Obtener numero de pasos
	int nPasos = distancia / 0.1f;

	// Para cada paso
    for (int i = 0; i < nPasos; i++)
    {
		// Obtener configuracion
		glm::vec3 conf = confA + direction * (i * 0.1f);

		// Cambiar configuracion
		robot.setConf(conf);

		// Si colisiona con el obstaculo
		if (checkCollision(robot, obst, 8))
			return false;
	}

	return true;
}

// Clase nodo
class Nodo
{
public:
// Configuracion
	glm::vec3 conf;

	// Padre
	Nodo* padre;

    // Hijos
    Nodo** hijos;
    int nHijos;

    // Costo
    float costo;

    // Constructor
    Nodo(glm::vec3 conf, Nodo* padre, float costo)
    {
        this->conf = conf;
        this->padre = padre;
        this->costo = costo;
        this->hijos = nullptr;
        this->nHijos = 0;
    }

    // Agrergar hijo
    void addHijo(Nodo* hijo)
    {
		// Si no tiene hijos
        if (this->hijos == nullptr)
        {
            this->hijos = new Nodo*[1];
			this->hijos[0] = hijo;
			this->nHijos = 1;
		}
		// Si ya tiene hijos
        else
        {
            // Crear arreglo temporal
			Nodo** temp = new Nodo*[this->nHijos + 1];
			// Copiar hijos
            for (int i = 0; i < this->nHijos; i++)
            {
				temp[i] = this->hijos[i];
			}
			// Agregar hijo
			temp[this->nHijos] = hijo;
			// Actualizar hijos
			this->hijos = temp;
			// Actualizar numero de hijos
			this->nHijos++;
		}

        // Actualizar costo
        hijo->setCosto(this->costo + glm::distance(this->conf, hijo->conf));
	}

    // Quitar hijo
    void removeHijo(Nodo* hijo)
    {
        // Si no tiene hijos
        if (this->hijos == nullptr)
        {
            return;
        }

        // Si tiene hijos
        else
        {
			// Crear arreglo temporal
			Nodo** temp = new Nodo*[this->nHijos - 1];
			// Copiar hijos
			int j = 0;
            for (int i = 0; i < this->nHijos; i++)
            {
                if (this->hijos[i] != hijo)
                {
					temp[j] = this->hijos[i];
					j++;
				}
			}

			// Actualizar hijos
			this->hijos = temp;
			// Actualizar numero de hijos
			this->nHijos--;
		}
    }

    // Cambiar padre
    void setPadre(Nodo* padre)
    {
        // Si tiene padre
        if (this->padre != nullptr)
        {
            // Quitar hijo del padre
            this->padre->removeHijo(this);
        }

        // Cambiar padre
        this->padre = padre;

        // Actualizar costo
        this->costo = padre->costo + glm::distance(this->conf, padre->conf);

        // Actualizar hijos del padre
        padre->addHijo(this);
    }

    // Obtener hijos
    Nodo** getHijos()
    {
		return this->hijos;
	}

	// Obtener padre
    Nodo* getPadre()
    {
		return this->padre;
	}

	// Obtener configuracion
    glm::vec3 getConf()
    {
		return this->conf;
	}

	// Obtener costo
    float getCosto()
    {
		return this->costo;
	}

	// Cambiar costo
    void setCosto(float costo)
    {
		this->costo = costo;
	}

    // Nodo más cercano a la rama
    Nodo* getNodoMasCercanoRama(glm::vec3 conf)
	{
        // Nodo mas cercano
        Nodo* nodoMasCercano = this;
        // Distancia minima
        float distanciaMinima = glm::distance(conf, this->conf);

        // Para cada hijo
        for (int i = 0; i < this->nHijos; i++)
        {
			// Obtener nodo mas cercano de la rama
			Nodo* nodoMasCercanoHijo = this->hijos[i]->getNodoMasCercanoRama(conf);
			// Obtener distancia
			float distancia = glm::distance(conf, nodoMasCercanoHijo->conf);
			// Si la distancia es menor
            if (distancia < distanciaMinima)
            {
				// Actualizar distancia minima
				distanciaMinima = distancia;
				// Actualizar nodo mas cercano
				nodoMasCercano = nodoMasCercanoHijo;
			}
		}

        return nodoMasCercano;
    }
};

Nodo* Steer(Nodo* nodoA, Nodo* nodoB, float delta)
{
	// Obtener configuracion
	glm::vec3 confA = nodoA->getConf();
	glm::vec3 confB = nodoB->getConf();

	// Obtener distancia
	float distancia = glm::distance(confA, confB);

	// Obtener direccion
	glm::vec3 direction = glm::normalize(confB - confA);

	// Obtener configuracion
	glm::vec3 conf = confA + direction * (delta * distancia);

	// Crear nodo
	Nodo* nodo = new Nodo(conf, nullptr, 0.0f);

	return nodo;
}

glm::vec3 SampleFree()
{
    // Configuracion aleatoria
	glm::vec3 confAleatoria = glm::vec3(rand() % 1000 / 100.0f, rand() % 1000 / 100.0f, rand() % 1000 / 100.0f);

	return confAleatoria;
}

// Clase arbol RRT*
class RRTstar
{
public:
    // Raiz
    Nodo* raiz;
    
    // Arreglo de nodos
    Nodo** nodos;
    int nNodos;

    // Configuracion objetivo
    glm::vec3 confObjetivo;

    // Constructor
    RRTstar(glm::vec3 confInicial, glm::vec3 confObjetivo)
    {
        this->raiz = new Nodo(confInicial, nullptr, 0.0f);
        this->nNodos = 1;
        this->nodos = new Nodo*[1];
        this->nodos[0] = this->raiz;
        this->confObjetivo = confObjetivo;
	}

	// Obtener raiz
    Nodo* getRaiz()
    {
		return this->raiz;
	}

	// Obtener nodo mas cercano
    Nodo* getNodoMasCercano(glm::vec3 conf)
    {
        return this->raiz->getNodoMasCercanoRama(conf);
    }

    // Agregar nodo (LA FUNCIÓN DEL ALGORITMO RRT*)
    void addNodo(Obstaculo* obst, float eta)
    {
        // Configuracion aleatoria
        glm::vec3 xrandvec = SampleFree();
        Nodo* xrand = new Nodo(xrandvec, nullptr, 0.0f);

		// Obtener nodo mas cercano
		Nodo* xnearest = this->getNodoMasCercano(xrandvec);

		// Nuevo nodo
        Nodo* xnew = Steer(xnearest, xrand, 0.1f);

        // Si no colisiona con el obstaculo
        if (ObstacleFree(xnearest->getConf(), xnew->getConf(), obst))
        {
            // Calcular el radio de busqueda de padre
            float r = std::fmin(gamma_rrtstar * pow(log(this->nNodos) / this->nNodos, 1.0f / 3.0f), eta);

            // Minimos
            Nodo* xmin = xnearest;
            float cmin = xnearest->getCosto() + glm::distance(xnearest->getConf(), xnew->getConf());

            // Se conecta a lo largo del camino de menor costo dentro del radio
            for (int i = 0; i < this->nNodos; i++)
            {
                Nodo* xnear = this->nodos[i];

                if (glm::distance(xnear->getConf(), xnew->getConf()) < r)
                {
					// Si no colisiona con el obstaculo
                    if (ObstacleFree(xnear->getConf(), xnew->getConf(), obst))
                    {
						// Costo
						float c = xnear->getCosto() + glm::distance(xnear->getConf(), xnew->getConf());

						// Si el costo es menor
                        if (c < cmin)
                        {
							// Actualizar costo minimo
							cmin = c;
							// Actualizar nodo minimo
							xmin = xnear;
						}
					}
				}
            }

            // Cambiar padre
            xnew->setPadre(xmin);

            // Rewire the tree
            for (int i = 0; i < this->nNodos; i++)
            {
                Nodo* xnear = this->nodos[i];

                if (glm::distance(xnear->getConf(), xnew->getConf()) < r)
                {
                    // Si no colisiona con el obstaculo
					if (ObstacleFree(xnew->getConf(), xnear->getConf(), obst))
					{ 
                        // Costo
                        float c = xnew->getCosto() + glm::distance(xnew->getConf(), xnear->getConf());

                        // Si el costo es menor
                        if (c < xnear->getCosto())
                        {
							// Cambiar padre
							xnear->setPadre(xnew);
						}
                    }
                }
            }

            // Actualizar vector de nodos
            Nodo** temp = new Nodo*[this->nNodos + 1];
            for (int i = 0; i < this->nNodos; i++)
            {
                temp[i] = this->nodos[i];
            }
            temp[this->nNodos] = xnew;
            this->nodos = temp;
            this->nNodos++;
        }
	}

    // Obtener camino
    std::vector<glm::vec3> getCamino(Obstaculo* obst)
    {
		// Camino
		std::vector<glm::vec3> camino;

        Nodo* nodo = nullptr;
        int flag = 0;

        // Ver si el nodo mas cercano es visible
        if (ObstacleFree(this->getNodoMasCercano(this->confObjetivo)->getConf(), this->confObjetivo, obst))
        {
			nodo = this->getNodoMasCercano(this->confObjetivo);
			flag = 1;
		}

        // Si no era visible
        if (flag == 0)
        {
            return camino;
        }

        // Agregar configuraciones
        camino.push_back(this->confObjetivo);
        while (nodo != nullptr)
        {
            camino.push_back(nodo->getConf());
            nodo = nodo->getPadre();
        }

        // Voltear camino
        std::reverse(camino.begin(), camino.end());

        return camino;
	}
};

// Funcion para discretizar un camino
std::vector<glm::vec3> discretizarCamino(std::vector<glm::vec3> camino, float delta)
{
	// Camino discretizado
	std::vector<glm::vec3> caminoDiscretizado;

	// Para cada configuracion
    for (int i = 0; i < camino.size() - 1; i++)
    {
		// Obtener configuracion
		glm::vec3 confA = camino[i];
		glm::vec3 confB = camino[i + 1];

		// Obtener distancia
		float distancia = glm::distance(confA, confB);

		// Obtener direccion
		glm::vec3 direction = glm::normalize(confB - confA);

		// Obtener numero de pasos
		int nPasos = distancia / delta;

		// Para cada paso
        for (int j = 0; j < nPasos; j++)
        {
			// Obtener configuracion
			glm::vec3 conf = confA + direction * (j * delta);

			// Agregar configuracion
			caminoDiscretizado.push_back(conf);
		}
	}

	// Agregar configuracion objetivo
	caminoDiscretizado.push_back(camino[camino.size() - 1]);

	return caminoDiscretizado;
}

// Funcion para compilar shaders
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// Funcion para crear shaders
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    // Inicializar la libreria
    if (!glfwInit())
        return -1;

    // Crear ventana
    window = glfwCreateWindow(1080, 720, "Robot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Contexto actual
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Si GLEW no se inicializa
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    // Imprimir versiones
    fprintf(stdout, "GLEW version: %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "GL version: %s\n", glGetString(GL_VERSION));

    // Crear shaders
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec3 aNormal;\n"
        "out vec4 transformedPosition;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform vec3 ColorIn;\n"
        "\n"
        "out vec3 Normal;"
        "out vec3 FragPos;"
        "out vec3 ColIn;"
        "void main()\n"
        "{\n"
        "   mat4 MVP = projection * view * model;\n"
        "   transformedPosition = MVP * position;\n"
        "   gl_Position = transformedPosition;\n"
        "   FragPos = vec3(model * position);\n"
        "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        "   ColIn = ColorIn;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "in vec3 Normal;"
        "in vec3 FragPos;"
        "in vec3 ColIn;"
        "uniform vec3 lightPos;"
        "uniform vec3 viewPos;"
        "void main()\n"
        "{\n"
        "   vec3 ambient = vec3(0.5, 0.5, 0.5);\n"
        "   vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
        "   vec3 lightDir = normalize(lightPos - FragPos);\n"
        "   float diff = max(dot(Normal, lightDir), 0.0);\n"
        "   vec3 diffuse = diff * lightColor;\n"
        "   vec3 viewDir = normalize(viewPos - FragPos);\n"
        "   vec3 reflectDir = reflect(-lightDir, Normal);\n"
        "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "   vec3 specular = spec * lightColor;\n"
        "   vec3 result = (ambient + diffuse + specular) * ColIn;\n"
        "   color = vec4(result, 1.0);\n"
        "}\n";

    shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // Matriz de proyeccion
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1080.0f / 720.0f, 0.1f, 100.0f);

    // Posicion de la camara
    glm::vec3 cameraPos = glm::vec3(-20.0f, 5.0f, 4.0f);

    // Matriz de vista
    glm::mat4 view = glm::lookAt(cameraPos,                     // Posicion
        						 glm::vec3(0.0f, 0.0f, 0.0f),   // Hacia donde mira
                                 glm::vec3(0.0f, 0.0f, 1.0f));  // Vector arriba

    // Obtener ubicacion de las matrices
    unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
    unsigned int viewLoc = glGetUniformLocation(shader, "view");
    unsigned int modelLoc = glGetUniformLocation(shader, "model");

    // Enviar matrices
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Enviar posicion de la luz
    glm::vec3 lightPos = glm::vec3(10.0f, 10.0f, 10.0f);
    unsigned int lightPosLoc = glGetUniformLocation(shader, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

    // Enviar posicion de la camara
    unsigned int viewPosLoc = glGetUniformLocation(shader, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));

    // Crear robot
    Robot robot = Robot(glm::vec3(0.0f, 0.0f, 0.0f));

    // Crear obstaculos alearotiamente
    srand(time(NULL));
    Obstaculo obstaculos[2];

    // Crear obstaculo
    float width = 1.0f;
    float height = 100.0f;
    float depth = 1.0f;
    float x = 2.0f;
    float y = 1.5f;
    float z = 1.0f;
    glm::vec3 centro = glm::vec3(x, y, z);
    obstaculos[0] = Obstaculo(centro, width, height, depth);

    // Crear obstaculo
    width = 1.0f;
    height = 100.0f;
    depth = 1.0f;
    x = 2.0f;
    y = 1.5f;
    z = 3.0f;
    centro = glm::vec3(x, y, z);
    obstaculos[1] = Obstaculo(centro, width, height, depth);

    // Configuracion inicial
    glm::vec3 confInicial = glm::vec3(0.0f, 0.0f, 0.0f);
    // Configuracion final
    glm::vec3 confFinal = glm::vec3(1.5f, 1.5f, 4.0f);

    // Crear arbol
    RRTstar arbol = RRTstar(confInicial, confFinal);

    // Agregar nodos
    for (int i = 0; i < NUMNODOS; i++)
    {
        printf("Agregando nodo %d\n", i);

		// Agregar nodo
		arbol.addNodo(obstaculos, 10.0f);

        printf("Nodos: %d\n", arbol.nNodos);
	}

    // Obtener camino
    std::vector<glm::vec3> camino = arbol.getCamino(obstaculos);

    // Imprimir si existe camino
    std::cout << "Cantidad de aristas del camino: " << camino.size() - 1 << std::endl;

    // Discretizar camino si existe
    std::vector<glm::vec3> caminoDiscretizado;
    if (camino.size() > 0)
    {
		// Discretizar camino
		caminoDiscretizado = discretizarCamino(camino, 0.01f);
	}
    // Si no existe camino terminar la ventana
    else
    {
        // Terminar programa
        glfwTerminate();
        return 0;
    }

    // Numero de configuracion actual
    int nConf = 0;

    // Habilitar 3d
    glEnable(GL_DEPTH_TEST);

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        // Calcular el tiempo
        float time = glfwGetTime();

        // Actualizar matriz de transformacion
        glm::mat4 model = glm::mat4(1.0f);

        // Enviar matriz de transformacion
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Enviar matriz de vista
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Color de fondo
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Limpiar pantalla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pasar por el camino
        if (caminoDiscretizado.size() > 0)
        {
            // Obtener configuracion
            glm::vec3 conf = caminoDiscretizado[nConf];

            // Cambiar configuracion
            robot.setConf(conf);
            
            // Aumentar configuracion si se presiona la tecla espacio
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				nConf++;

            // Si se llega al final del camino
            if (nConf >= caminoDiscretizado.size())
				nConf--;
        }

        // Dibujar robot
        robot.draw();

        // Dibujar obstaculos
        for (int i = 0; i < 2; i++)
            obstaculos[i].draw();

        // Obtener posicion de la camara
        glm::vec3 cameraPos = glm::vec3(view[0][3], view[1][3], view[2][3]);
        // Obtener direccion de la camara
        glm::vec3 cameraDirection = glm::vec3(view[0][2], view[1][2], view[2][2]);
        // Obtener vector arriba de la camara
        glm::vec3 cameraUp = glm::vec3(view[0][1], view[1][1], view[2][1]);

        // Si se presiona la tecla E, la camara rota hacia arriba respecto al origen
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			view = glm::rotate(view, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// Si se presiona la tecla Q, la camara rota hacia abajo respecto al origen
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            view = glm::rotate(view, glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Salir con ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}