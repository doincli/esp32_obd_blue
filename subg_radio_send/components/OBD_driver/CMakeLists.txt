idf_component_register(SRCS 
                                 "OBD/OBD_detect.c"
                    INCLUDE_DIRS "."
                                 "OBD/include"
                    PRIV_REQUIRES driver
                    REQUIRES nvs_flash
                    REQUIRES bt
)
