cmake_minimum_required(VERSION 3.28)
project(arp_listener C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Wextra")

add_executable(arp_scanner
        main.c
        interface/interface.h
        interface/interface.c
        utils/utils.h
        utils/net.c
        arp_proto/arp_proto.h
        arp_proto/arp_probe.c
        send.c
        arp_scanner.h
        receive.c
        interface_selector.c
        futex.c
        sync_objects.c
        arguments.c
)

target_link_libraries(arp_scanner ncurses)
