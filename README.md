On my honor, I, Kushan Gupta, completed the ECIS survey.
On my honor, I, Andrew Li, completed the ECIS survey.

Radiosity.pdf is our presentation.
Graphics Writeup.pdf is our writeup.

Our code is run by:

1) Creating folder build
2) cmake .. within build
3) make -j8
4) cd bin
5) ./skinning + arguments
    There are 2 required arguments and 1 additional argument
    arg1 = tesselation level
    arg2 = .obj file
    arg3(optional) = num threads(default 4)
    Example:
    
    In bin folder, ./skinning 2 cornell_box_multimaterial.obj 2
    
Additionally, obj files must be at the root directory level, at the same level as CMakeLists.txt

In order to edit light positions beyond the given default cornell box light positions, you must edit the code from lines
173 to 188, following the code example given for the cornell box. Light positions for the beacon file are centered around
or at the point (-6.0, 548.0, 375.0).

We don't suggest a level higher than 2 if running on a quad core i7 Macbook pro level machine, or if on gdc computers,
level 3 with 8 threads minimum(this one will still take 30 minutes).

To move around, use WASD + arrow keys

The png files in the root directory are our resulting images.

CornellL1: Level 1 tessellation, takes 1 second to run, soft shadows and light bleed present but very tessellated. 
           Form factors less than .00005 culled. Faces = 576.
CornellL2: Level 2 tessellation, takes 2 minutes to run, soft shadows and light bleed present and less tessellated.
           Form factors less than .00005 culled. Faces = 2304.
CornellL3: Level 3 tessellation, takes 2 minutes to run, soft shadows and light bleed present and much less tessellation.
           Form factors less than .00005 culled. Faces = 9216
CornellL2LessCulling: Level 2 tesselation, takes 1.5 hours to run, much softer shadows and more light bleed, could look
                      better with more tuning but increases runtime exponentially.
           Form factors less than .000005 culled. Faces = 2304.
BeaconL1:  Level 1 tesselation, took 3.5 hours to run on GCP 16 core Haswell machine. Mostly as a check out of curiosity,
           this is a heavily modified beacon scene found online that had 8500 faces, but significantly more form factors
           on average than the cornell box with equivalent number of faces.
           Form Factors less than .00005 culled. Faces = 8224.
