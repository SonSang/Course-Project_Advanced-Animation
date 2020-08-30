CC = g++ -std=c++11
LD_OPENGL = -lGL -lGLU -lglut -lGLEW -lgomp
DEBUG = -g
TARGET = main.exe

IMGUI_OBJ = imgui_draw.o imgui_widgets.o imgui.o imgui_impl_glut.o imgui_impl_opengl2.o
IMGUI_LIB = libimgui.a
LD_IMGUI = -L./ -limgui

MATH_OBJ = converter.o math.o matrix.o quaternion.o transformation.o vector.o timer.o
MATH_LIB = libmath.a
LD_MATH = -L./ -lmath

SCENE_OBJ = camera.o geometry.o mouse.o object_manager.o object.o property_render_geometry.o property_render.o property.o render_basic.o render_manager.o scene_manager.o property_imgui.o
SCENE_LIB = libscene.a
LD_SCENE = -L./ -lscene

UI_OBJ = ui_camera.o ui_object.o ui.o
UI_LIB = libui.a
LD_UI = -L./ -lui

$(TARGET) : $(MATH_LIB) $(IMGUI_LIB) $(SCENE_LIB) $(UI_LIB)
	#HW 1
	#$(CC) main2.cpp body.cpp joint.cpp link.cpp $(LD_SCENE) $(LD_UI) $(LD_MATH) $(LD_IMGUI) $(LD_OPENGL) $(DEBUG) -o $(TARGET) 

	#HW 2
	$(CC) main2.cpp bvh_parser.cpp body.cpp joint.cpp link.cpp $(LD_SCENE) $(LD_UI) $(LD_MATH) $(LD_IMGUI) $(LD_OPENGL) $(DEBUG) -o $(TARGET) 

$(IMGUI_OBJ) : imgui/imgui-master/imgui_draw.cpp imgui/imgui-master/imgui_widgets.cpp imgui/imgui-master/imgui.cpp imgui/imgui-master/imgui_impl_glut.cpp imgui/imgui-master/imgui_impl_opengl2.cpp
	$(CC) -c $^

$(IMGUI_LIB) : $(IMGUI_OBJ)
	ar rc $@ $^

$(MATH_OBJ) : 
	$(CC) -c -fopenmp ./math/*.cpp

$(MATH_LIB) : $(MATH_OBJ)
	ar rc $@ $^

$(SCENE_OBJ) : 
	$(CC) -c ./scene/*.cpp

$(SCENE_LIB) : $(SCENE_OBJ)
	ar rc $@ $^

$(UI_OBJ) : 
	$(CC) -c ./ui/*.cpp

$(UI_LIB) : $(UI_OBJ)
	ar rc $@ $^

clean :
	rm *.o *.a *.exe
