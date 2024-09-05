macro(disable_target_compiler_warnings _target)

target_compile_options(${_target} PRIVATE 
    $<$<C_COMPILER_ID:MSVC>:
        /wd4101
        /wd4102
        /wd4244
        /wd4267
        /wd4477
        /wd4700
        /wd4996 >
)

endmacro()