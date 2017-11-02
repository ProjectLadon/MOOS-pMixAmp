#--------------------------------------------------------
# The CMakeLists.txt for:                       pMixAmp
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  MixAmp.cpp
  MixAmp_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pMixAmp ${SRC})

TARGET_LINK_LIBRARIES(pMixAmp
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

