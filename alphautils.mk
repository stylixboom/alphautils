##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=alphautils
ConfigurationName      :=Release
WorkspacePath          := "/home/stylix/webstylix/code/lib"
ProjectPath            := "/home/stylix/webstylix/code/lib/alphautils"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Siriwat Kasamwattanarote
Date                   :=28/09/15
CodeLitePath           :="/home/stylix/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="alphautils.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/home/stylix/local/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/home/stylix/local/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcu
CXX      := g++
CC       := gcc
CXXFLAGS :=  -O3 -std=c++11 $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/alphautils.cpp$(ObjectSuffix) $(IntermediateDirectory)/hdf5_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/imtools.cpp$(ObjectSuffix) $(IntermediateDirectory)/lapwrap.cpp$(ObjectSuffix) $(IntermediateDirectory)/linear_tree.cpp$(ObjectSuffix) $(IntermediateDirectory)/report.cpp$(ObjectSuffix) $(IntermediateDirectory)/tsp.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/stylix/webstylix/code/lib/.build-release"
	@echo rebuilt > "/home/stylix/webstylix/code/lib/.build-release/alphautils"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/alphautils.cpp$(ObjectSuffix): alphautils.cpp $(IntermediateDirectory)/alphautils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/alphautils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/alphautils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/alphautils.cpp$(DependSuffix): alphautils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/alphautils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/alphautils.cpp$(DependSuffix) -MM "alphautils.cpp"

$(IntermediateDirectory)/alphautils.cpp$(PreprocessSuffix): alphautils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/alphautils.cpp$(PreprocessSuffix) "alphautils.cpp"

$(IntermediateDirectory)/hdf5_io.cpp$(ObjectSuffix): hdf5_io.cpp $(IntermediateDirectory)/hdf5_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/hdf5_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hdf5_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hdf5_io.cpp$(DependSuffix): hdf5_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hdf5_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/hdf5_io.cpp$(DependSuffix) -MM "hdf5_io.cpp"

$(IntermediateDirectory)/hdf5_io.cpp$(PreprocessSuffix): hdf5_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hdf5_io.cpp$(PreprocessSuffix) "hdf5_io.cpp"

$(IntermediateDirectory)/imtools.cpp$(ObjectSuffix): imtools.cpp $(IntermediateDirectory)/imtools.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/imtools.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imtools.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imtools.cpp$(DependSuffix): imtools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imtools.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imtools.cpp$(DependSuffix) -MM "imtools.cpp"

$(IntermediateDirectory)/imtools.cpp$(PreprocessSuffix): imtools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imtools.cpp$(PreprocessSuffix) "imtools.cpp"

$(IntermediateDirectory)/lapwrap.cpp$(ObjectSuffix): lapwrap.cpp $(IntermediateDirectory)/lapwrap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/lapwrap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lapwrap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lapwrap.cpp$(DependSuffix): lapwrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lapwrap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lapwrap.cpp$(DependSuffix) -MM "lapwrap.cpp"

$(IntermediateDirectory)/lapwrap.cpp$(PreprocessSuffix): lapwrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lapwrap.cpp$(PreprocessSuffix) "lapwrap.cpp"

$(IntermediateDirectory)/linear_tree.cpp$(ObjectSuffix): linear_tree.cpp $(IntermediateDirectory)/linear_tree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/linear_tree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/linear_tree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/linear_tree.cpp$(DependSuffix): linear_tree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/linear_tree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/linear_tree.cpp$(DependSuffix) -MM "linear_tree.cpp"

$(IntermediateDirectory)/linear_tree.cpp$(PreprocessSuffix): linear_tree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/linear_tree.cpp$(PreprocessSuffix) "linear_tree.cpp"

$(IntermediateDirectory)/report.cpp$(ObjectSuffix): report.cpp $(IntermediateDirectory)/report.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/report.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/report.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/report.cpp$(DependSuffix): report.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/report.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/report.cpp$(DependSuffix) -MM "report.cpp"

$(IntermediateDirectory)/report.cpp$(PreprocessSuffix): report.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/report.cpp$(PreprocessSuffix) "report.cpp"

$(IntermediateDirectory)/tsp.cpp$(ObjectSuffix): tsp.cpp $(IntermediateDirectory)/tsp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stylix/webstylix/code/lib/alphautils/tsp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tsp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tsp.cpp$(DependSuffix): tsp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tsp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/tsp.cpp$(DependSuffix) -MM "tsp.cpp"

$(IntermediateDirectory)/tsp.cpp$(PreprocessSuffix): tsp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tsp.cpp$(PreprocessSuffix) "tsp.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


