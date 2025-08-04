This project was started May 19th 2025.
The goal of this project is to try writing a graphics renderer on the CPU (for now).
This project will be capable of rendering wavefront object files (.obj) files, with more formats to come in the future.

A similar, yet simpler, rendering pipeline to the one present in OpenGL is used for displaying images.
Z-buffering is used to decrease the number of triangles to be drawn to decrease rendering times.
For rendering UV textures, the renderer supports both Phong and Gouraud shading.
Lighting over the model is handled with diffuse and specular reflection for all types of materials.

![Diablo3_front](images/Diablo3_front.jpg)
![Diablo3_left](images/Diablo3_left.jpg)
![Diablo3_top](images/Diablo3_top_lighting.jpg)


![Head_front](images/Head_front.jpg)
![Head_left](images/Head_left.jpg)
![Head_top](images/Head_top_lighting.jpg)
