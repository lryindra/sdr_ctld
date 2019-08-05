# Add the source supdirectories
add_subdirectory(application)
add_subdirectory(audio)
add_subdirectory(receivers)
add_subdirectory(sdr)
add_subdirectory(transmitters)

# bring in the global properties
get_property(${PROJECT_NAME}_SOURCE GLOBAL PROPERTY SRCS_LIST)

# Build the program
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE} )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
# The pulse libraries are only needed on Linux. On other platforms they will not be found, so having them here is fine.
target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${LOG4CPP_LIBRARIES}
    ${GNURADIO_ALL_LIBRARIES}
    ${GNURADIO_LIMESDR_LIBRARIES}
    ${LIMESUITE_LIBRARIES}
    ${ALSA_LIBRARY}
)

message("target link library Thread: ${CMAKE_THREAD_LIBS_INIT}")
message("target link library boost: ${Boost_LIBRARIES}")
message("target link library log4cpp: ${LOG4CPP_LIBRARIES}")
foreach(GNURADIO_LIBRARY ${GNURADIO_ALL_LIBRARIES})
    message("target link library: ${GNURADIO_LIBRARY}")
endforeach(GNURADIO_LIBRARY)
message("target link library limesdr: ${GNURADIO_LIMESDR_LIBRARIES}")
message("target link library LimeSuite: ${LIMESUITE_LIBRARIES}")
message("target link library alsa: ${ALSA_LIBRARY}")

