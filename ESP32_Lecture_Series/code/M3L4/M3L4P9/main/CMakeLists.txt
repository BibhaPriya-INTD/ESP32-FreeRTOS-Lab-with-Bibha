idf_component_register(SRCS "Two_pin_intr.c"
                       PRIV_REQUIRES spi_flash
                       INCLUDE_DIRS ""
                       REQUIRES esp_driver_gpio)
