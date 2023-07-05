idf_component_register(SRCS "ebyte.c"
                            "ebyte/ebyte_port.c"
                            "ebyte/ebyte_core.c"
                            "ebyte/ebyte_callback.c"
                            "ebyte/ebyte_e220x.c"
                            "OBD/gatts_demo.c"
                            "OBD/OBD_detect.c" 
                    INCLUDE_DIRS "."
                                 "ebyte/include"
                                 "OBD/include"
                    PRIV_REQUIRES driver
                    PRIV_REQUIRES driver ebyte_driver
                    REQUIRES nvs_flash
                    REQUIRES bt)
