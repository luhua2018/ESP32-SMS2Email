# idf_component_register 是注册组件的核心命令
idf_component_register(
    # SRCS: 列出组件的所有源文件 (.c, .cpp, .S)
    # 这些文件将被编译成组件的静态库 (xxx.a)
    SRCS "blink_led.c"

    # INCLUDE_DIRS: 公共头文件目录
    # 这里的路径会被添加到所有依赖此组件的其他组件的 include 搜索路径中
    INCLUDE_DIRS "."

    # PRIV_INCLUDE_DIRS: 私有头文件目录 (可选)
    # 如果你有一些只在组件内部使用的头文件，可以放在这里，这个路径只会添加到本组件的 include 搜索路径中
    # PRIV_INCLUDE_DIRS "private_include"

    # REQUIRES: 公共依赖
    # 如果你的公共头文件 (xxx.h) #include 了其他组件的头文件，那么必须在这里声明依赖。
    REQUIRES driver

    # PRIV_REQUIRES: 私有依赖
    # 如果你的源文件 (.c) 使用了其他组件，但公共头文件没有，就在这里声明。这有助于加快编译速度。
    # PRIV_REQUIRES i2c log
)
