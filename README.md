# RRT* for a cartesian Robot
### Eduardo Jaziel Juárez Martínez
## Introduction
The RRT* (Rapidly-exploring Random Tree Star) algorithm is an extension of the basic RRT algorithm used in trajectory planning for robotics and other fields. Its main goal is to find an optimal path between a starting point and a target point in a configuration space.

RRT* improves upon the basic RRT algorithm by optimizing the found trajectory. It uses a tree structure to efficiently sample the configuration space, expanding the tree towards unexplored regions. As it progresses, it improves existing trajectories if a shorter or more efficient route is found.

In this project, a Cartesian Robot is implemented using OpenGL, along with the RRT* algorithm to move it from one configuration to another in an environment with obstacles.

## Cartesian Robot
A Cartesian robot is a specific type of robot used in industrial and manufacturing applications. It is characterized by its Cartesian coordinate system for movement. This type of robot moves along linear axes (X, Y, Z) that follow a coordinate system similar to the Cartesian system in mathematics.

Its structure resembles the following image:
![image](https://github.com/user-attachments/assets/6b076791-ffda-400c-8b0a-3d947946c049)

## Denavit–Hartenberg parameters
We define the following axes: 

![image](https://github.com/user-attachments/assets/31d321d2-4231-4185-bf8d-17794cfa48af)

Then, the Denavit–Hartenberg parameter table is as follows:

| A  | θᵢ   | dᵢ  | aᵢ | αᵢ   |  
|----|-------|------|-----|------|  
| 1  | 0°    | L₁  | 0   | 90°  |  
| 2  | 90°   | L₂  | 0   | 90°  |  
| 3  | 0°    | L₃  | 0   | 0°   |

## Implementation

To implement, we create a "Prism" structure, which is created with the following attributes:  
- **Position**: A vector that contains the midpoint of its base.  
- **Direction**: The direction of the height (in this case, I only considered the cases (1, 0, 0), (0, 1, 0), and (0, 0, 1)).  
- **Dimensions**: 3 parameters for width, height, and depth.  

I also created a "Joint" object with the properties of the robot's joints. These have methods to map them (in the form of a prism) with the respective DH transformations.  
The collision function was implemented using a collision function between prisms, so obstacles will not be able to pass through the robot's arms.

# Requirements

The code is in a single file, Main.cpp, and requires the OpenGL libraries (GLFW and GLEW), as well as the glm library for matrix usage.  
When running the program, the graphical part will be paused, and the process will be printed in the console while the nodes are being added. Once this process finishes, the graphical part will be executed.  
It has two functions: the camera moves sideways with the Q and E keys to observe the environment, and to execute the path found by RRT*, the spacebar is pressed.  
If the program does not reach the graphical part, it means that with the given number of nodes, a solution could not be found.
