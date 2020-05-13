On my honor, I, Kushan Gupta, completed the ECIS report.
On my honor, I, Andrew Li, completed the ECIS report.

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
173 to 188, following the code example given for the cornell box.

We don't suggest a level higher than 2 if running on a quad core i7 Macbook pro level machine, or if on gdc computers,
level 3 with 8 threads minimum(this one will still take 30 minutes).

To move around, use WASD + arrow keys