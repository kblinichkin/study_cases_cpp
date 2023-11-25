# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
	# list the source files of this component
    SRCS     	
    	main.cpp 
    	../Tasks/TestTask.cpp
    	../Wrappers/I2CPort.cpp
    INCLUDE_DIRS     
    	"../Tasks"
    	"../Wrappers"
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
