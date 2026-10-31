
add_library(fips202 STATIC
        fips202/fips202.c
)
target_include_directories(fips202 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/fips202
)

# if (BUILD_X86_64)
#         enable_language(ASM)
#         file(GLOB XKCP_AVX2_SRCS
#                 XKCP/x86_64/*.c
#                 XKCP/x86_64/*.S
#         )
#         add_library(xkcp_avx2 STATIC ${XKCP_AVX2_SRCS})
#         target_include_directories(xkcp_avx2 PUBLIC
#                 ${CMAKE_CURRENT_SOURCE_DIR}/XKCP/x86_64
#         )
#         target_compile_options(xkcp_avx2 PRIVATE
#                 $<$<COMPILE_LANGUAGE:C>:-mavx2>
#         )
#         if (CMAKE_C_COMPILER_ID MATCHES "AppleClang")
#                 target_compile_options(xkcp_avx2 PRIVATE
#                         $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
#                 )
#         endif()
#         message(STATUS " xkcp_avx2 (AVX2) implementation: ENABLED")
# endif()

        file(GLOB XKCP_SRCS
                XKCP/ref/*.c
        )
        add_library(xkcp STATIC ${XKCP_SRCS})
        target_include_directories(xkcp PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/XKCP/ref
        )


add_library(djbsort32 STATIC
        djbsort-common-32/uint32_sort.c
)
target_include_directories(djbsort32
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/djbsort-common-32
)
if (BUILD_X86_64)
        target_sources(djbsort32
                PRIVATE djbsort-avx2-32/sort.c
        )
        target_compile_options(djbsort32 PRIVATE ${X86_64_C_FLAGS})
else()
        target_sources(djbsort32
                PRIVATE djbsort-ref-32/sort.c
        )
endif()

add_library(djbsort64 STATIC
        djbsort-ref-64/uint64_sort.c
)
target_include_directories(djbsort64
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/djbsort-ref-64
)
