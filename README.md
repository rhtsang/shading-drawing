# shading-drawing
The Phong lighting model, Gouraud shading, Half-toning, and the Painter's Algorithm Surface Removal

###### Raymond Tsang
###### 912868864
###### rhtsang@ucdavis.edu

Compilation and name of executable
----------
In the project's root directory, run 'make' to compile the executable named 'draw'

How to run, command syntax, input/output file format
---------
The syntax to run the program is "./draw polygons environment" where "polygons" is a file that describes polygons and follows the same file format as described in discussion, and "environment" is a file that contains coefficients and other details about the scene in this format:

PhongConstant  
k_a k_d k_s  
AmbientLightIntensity LightSourceIntensity  
LightSourceLocation
ViewSourceLocation

For example, for Phong constant = 4, k_a = 0.5, k_d = 0.25, k_s = 0.75, ambient intensity = 5, light source intensity = 9, light source location = (500,500,500), and viewing point location = (200, 200, 200)

4  
0.5 0.25 0.75  
5 9  
500 500 500  
200 200 200

After the user enters the viewport dimensions, he/she will continually be able to enter whether half-tone mode should be on or off, and which plane to project to.  
Enter 0 to turn off half-tone or 1 to turn on.  
Enter 0 for the xy plane, 1 for the yz plane, or 2 for the xz plane.

Location of algorithm implementations
-----------------

Phong lighting model: phong.cpp    

Gouraud shading: line.cpp, in the function dda() line 40, and rasterize.cpp lines 196-208  

Half-toning: rasterize.cpp, in the function rasterize(), lines 232-274  

Hidden surface removal with the painter's algorithm: rasterize.cpp, in functions swap() and bubbleSort() which were based on https://www.geeksforgeeks.org/bubble-sort/ and rasterize.cpp lines 134-231


Other
--------------
After the user enters the half-tone on/off mode and which plane to project to, it may take more than a couple seconds for the polygons to be displayed on the screen. Please be patient with my inefficient code!
