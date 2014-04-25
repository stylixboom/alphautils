#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I/home/stylix/local/include
CFLAGS = -Wall -fexceptions
RESINC = 
LIBDIR = -L/home/stylix/local/lib
LIB = ../sifthesaff/libsifthesaff.a -lopencv_world -lopencv_stitching -lopencv_videostab -lopencv_gpu -lopencv_legacy -lopencv_ts -lopencv_nonfree -lopencv_contrib -lopencv_calib3d -lopencv_objdetect -lopencv_features2d -lopencv_video -lopencv_photo -lopencv_highgui -lopencv_flann -lopencv_imgproc -lopencv_ml -lopencv_core -llapacke -llapack -lblas -ltmglib
LDFLAGS = -lhdf5 -lhdf5_hl_cpp -lhdf5_cpp -lhdf5_hl -lmpi_cxx -lmpi `pkg-config --libs lapacke` -lrt -lpthread -ldl

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -Wall -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = ./libalphautils.a

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O3 -Wall
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = ./libalphautils.a

OBJ_DEBUG = $(OBJDIR_DEBUG)/alphautils.o $(OBJDIR_DEBUG)/hdf5_io.o $(OBJDIR_DEBUG)/imtools.o $(OBJDIR_DEBUG)/lapwrap.o $(OBJDIR_DEBUG)/linear_tree.o $(OBJDIR_DEBUG)/tsp.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/alphautils.o $(OBJDIR_RELEASE)/hdf5_io.o $(OBJDIR_RELEASE)/imtools.o $(OBJDIR_RELEASE)/lapwrap.o $(OBJDIR_RELEASE)/linear_tree.o $(OBJDIR_RELEASE)/tsp.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d . || mkdir -p .
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(AR) rcs $(OUT_DEBUG) $(OBJ_DEBUG)

$(OBJDIR_DEBUG)/alphautils.o: alphautils.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c alphautils.cpp -o $(OBJDIR_DEBUG)/alphautils.o

$(OBJDIR_DEBUG)/hdf5_io.o: hdf5_io.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c hdf5_io.cpp -o $(OBJDIR_DEBUG)/hdf5_io.o

$(OBJDIR_DEBUG)/imtools.o: imtools.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c imtools.cpp -o $(OBJDIR_DEBUG)/imtools.o

$(OBJDIR_DEBUG)/lapwrap.o: lapwrap.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c lapwrap.cpp -o $(OBJDIR_DEBUG)/lapwrap.o

$(OBJDIR_DEBUG)/linear_tree.o: linear_tree.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c linear_tree.cpp -o $(OBJDIR_DEBUG)/linear_tree.o

$(OBJDIR_DEBUG)/tsp.o: tsp.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tsp.cpp -o $(OBJDIR_DEBUG)/tsp.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d . || mkdir -p .
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(AR) rcs $(OUT_RELEASE) $(OBJ_RELEASE)

$(OBJDIR_RELEASE)/alphautils.o: alphautils.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c alphautils.cpp -o $(OBJDIR_RELEASE)/alphautils.o

$(OBJDIR_RELEASE)/hdf5_io.o: hdf5_io.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c hdf5_io.cpp -o $(OBJDIR_RELEASE)/hdf5_io.o

$(OBJDIR_RELEASE)/imtools.o: imtools.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c imtools.cpp -o $(OBJDIR_RELEASE)/imtools.o

$(OBJDIR_RELEASE)/lapwrap.o: lapwrap.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c lapwrap.cpp -o $(OBJDIR_RELEASE)/lapwrap.o

$(OBJDIR_RELEASE)/linear_tree.o: linear_tree.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c linear_tree.cpp -o $(OBJDIR_RELEASE)/linear_tree.o

$(OBJDIR_RELEASE)/tsp.o: tsp.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tsp.cpp -o $(OBJDIR_RELEASE)/tsp.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

