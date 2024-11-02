idf_component_register(

    INCLUDE_DIRS "esp-os"
    SRCS "esp-os/eos.c"
    
    INCLUDE_DIRS "app"
    SRCS "app/app.cpp"
         "app/task_list.cpp"
         "app/task_shell.cpp"    
         "app/shell.cpp"
         "app/task_device.cpp"  

    INCLUDE_DIRS "common"
    SRCS "common/cmd_line.c"    

    INCLUDE_DIRS "sys"
    SRCS "sys/sys_io.c"
    )
