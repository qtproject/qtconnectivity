add_subdirectory(heartrate-server)
if(TARGET Qt::Widgets)
    add_subdirectory(btchat)
    add_subdirectory(btscanner)
endif()
if(TARGET Qt::Quick)
    add_subdirectory(pingpong)
    add_subdirectory(lowenergyscanner)
    add_subdirectory(heartrate-game)
endif()
