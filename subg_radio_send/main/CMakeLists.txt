idf_component_register(SRCS "test.c" 
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver ebyte_driver
                    REQUIRES nvs_flash
                    REQUIRES bt)
