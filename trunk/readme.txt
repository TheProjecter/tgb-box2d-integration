TGB-Box2D-Integration - readme.txt


Installation Instruction:

1) There are some steps of preparation needed to integrate the wrapper into TGB. These steps 
can be found in the chapter "Compiling and Linking TGB and Box2D together" of the original 
thesis about the integration. The thesis can be found on the project Google Code page:
http://tgb-box2d-integration.googlecode.com/files/Integrating%20Box2D%20into%20TGB%201.0.pdf

2) Once the steps in the paper are executed, copy the "Box2D" folder in the "Cpp" directory
into the "engine/source/T2D" directory of your TGB installation.

3) Add all files in the just copied directory to the "TGBGame" project in your Visual 
Studio Solution. The recommendation here obviously is to mimic the file system directory
structure in the "TGBGame" project filter structure, although this is not mandatory.

4) Build the solution (or only the TGBGame project).