find_path( BOOST_INCLUDE_PATH boost/config.hpp
	/usr/include
	/usr/local/include
	/opt/local/include
	DOC "The directory where boost/config.hpp resides")
set(BOOST_SEARCH_PATHS 
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib 
	NO_DEFAULT_PATH
)
find_library(BOOST_SERIALIZATION NAMES boost_serialization-mt boost_serialization PATHS	${BOOST_SEARCH_PATHS} DOC "The boost serialization library")
find_library(BOOST_FILESYSTEM NAMES boost_filesystem-mt boost_filesystem PATHS ${BOOST_SEARCH_PATHS} DOC "The boost filesystem library")
find_library(BOOST_SYSTEM NAMES boost_system-mt boost_system PATHS ${BOOST_SEARCH_PATHS} DOC "The boost system library")
find_library(BOOST_TIMERS NAMES boost_timer boost_timer-mt PATHS ${BOOST_SEARCH_PATHS} DOC "The boost timer library")
find_library(BOOST_SIGNALS NAMES boost_signals boost_signals-mt PATHS ${BOOST_SEARCH_PATHS} DOC "The boost signals library")

set(BOOST_LIBRARIES ${BOOST_FILESYSTEM} ${BOOST_SYSTEM} ${BOOST_TIMERS} ${BOOST_COROUTINE} ${BOOST_PROGRAMOPTIONS} ${BOOST_CONTEXT} ${BOOST_SIGNALS})
if(BOOST_INCLUDE_PATH)
	set(BOOST_FOUND 1)
else(BOOST_INCLUDE_PATH)
	set(BOOST_FOUND 0)
	message(FATAL_ERROR "Boost not found.  Either set BOOST_INCLUDE_PATH and BOOST_LIBRARIES or make sure Boost is included in your PATH")
endif(BOOST_INCLUDE_PATH)

MARK_AS_ADVANCED(BOOST_FOUND)
