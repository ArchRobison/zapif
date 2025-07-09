bison_target(zapif_parser
    gram.ypp
    ${CMAKE_CURRENT_BINARY_DIR}/gram.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/gram.tab.hpp
    OPTIONS -r all $<$<CONFIG:Debug>:-t>
)

flex_target(zapif_lexer
    token.l
    ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c
    OPTIONS $<$<CONFIG:Debug>:--debug>
)

add_flex_bison_dependency(zapif_lexer zapif_parser)
add_custom_target(zapif_parser DEPENDS ${BISON_zapif_parser_OUTPUTS})
add_custom_target(zapif_lexer DEPENDS ${FLEX_zapif_lexer_OUTPUTS})

target_sources(zapif PRIVATE
    ${BISON_zapif_parser_OUTPUT_SOURCE}
    ${FLEX_zapif_lexer_OUTPUTS}
    action.cpp
    action.h
    Chunk.cpp
    Chunk.h
    main.cpp
)

target_include_directories(zapif PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(zapif zapif_parser zapif_lexer)
