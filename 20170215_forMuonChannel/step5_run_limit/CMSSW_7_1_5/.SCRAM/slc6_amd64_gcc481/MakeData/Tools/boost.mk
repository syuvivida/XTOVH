ALL_TOOLS      += boost
boost_EX_INCLUDE := /cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/boost/1.51.0-cms2/include
boost_EX_LIB := boost_thread boost_signals boost_date_time
boost_EX_USE := sockets
boost_EX_FLAGS_CPPDEFINES  := -DBOOST_SPIRIT_THREADSAFE -DPHOENIX_THREADSAFE

