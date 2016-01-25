Add the pthread headers from the include directory to your project.

Go to the projects's property page and set the following stuff:
  c/c++->General->Additional Include Libraries = $(SolutionDir)include
  Linker->General->Additional Library Directories = $(SolutionDir)lib\$(PlatformShortName)
  Linker->Input->Additional Dependencies += pthreadVC2.lib 

after the build copy the pthreadVC2.dll to the output directory