# CG-project 
This repo is our course project of Computer Graphics   
As team members love racing games, we reach agreement on this topic.   
This project is based on opengl4.x with glfw and glad.   
✨ finished on Dec. 20   

## Group Members 
Special thanks to my dear teammates! 
+  zyw [@SleepinWei](https://github.com/SleepinWei) 
+  ppz [@jinxes61](https://github.com/jinxes61)
+  cjy [@bakacheng](https://github.com/bakacheng) 
+  wpy @ 
+  yht @ 
+  fx [@outlier-21](https://github.com/outlier-21) 
## Structure 
+  shader 
    +  **atmosphere**
    +  **grass**
    +  **normal_mapping**
    +  **parrallel_light** 
    +  **point_light** 
    +  **sky**
    +  other shaders  
+  resources (textures & objects & heightmaps) 
+  CG
    +  all kinds of .h & .cpp files
## Plans / Progress 
- [ ] Terrain 
    - [ ] lod system 
    - [x] rendering of height map  
    - [x] integrating into bullet3 engine 
- [x] Shadows (PCF,PCSS):PCSS for ortho is done. For perspective is in progress. 
- [ ] Cascaded Shadow Mapping   
- [ ] deferred rendering 
- [x] imGui   
- [x] 3D model   
- [x] Controlling   
- [x] Physics engine   
- [x] better sky  
- [ ] better vegetation
## Naming Rules 
+   Class: 
```C++
class DefineSomeClassesHere{
};
```
+   functions:
```C++
void createWindowOrSomethingElse();
```
+   variables:
`int numberOfSomeVariable = 10;`
+   MACRO and enum type:
`define DEFINE_SOME_MACRO`
`enum defineSomeEnum{ENUM_ONE,ENUM_TWO,ENUM_THREE}`
+   shaders: 
    +   vertex shader:`filename.vs`
    +   fragment shader:`filename.fs`
