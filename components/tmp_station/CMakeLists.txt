# idf_component_register 是注册组件的核心命令
idf_component_register(
    # SRCS: 列出组件的所有源文件 (.c, .cpp, .S)
    SRCS "tmp_station.c"

    # INCLUDE_DIRS: 公共头文件目录
    INCLUDE_DIRS "."

    # PRIV_INCLUDE_DIRS: 私有头文件目录 (可选)
    PRIV_INCLUDE_DIRS "../../main"

    # PRIV_REQUIRES: 私有依赖
    PRIV_REQUIRES esp_wifi nvs_flash
)
