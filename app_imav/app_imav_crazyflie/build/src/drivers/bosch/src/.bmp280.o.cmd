cmd_src/drivers/bosch/src/bmp280.o := arm-none-eabi-gcc -Wp,-MD,src/drivers/bosch/src/.bmp280.o.d    -I/home/default/crazyflie-firmware/src/drivers/bosch/src -Isrc/drivers/bosch/src -D__firmware__ -fno-exceptions -Wall -Wmissing-braces -fno-strict-aliasing -ffunction-sections -fdata-sections -Wdouble-promotion -std=gnu11 -DCRAZYFLIE_FW   -I/home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include   -I/home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include   -I/home/default/crazyflie-firmware/vendor/libdw1000/inc   -I/home/default/crazyflie-firmware/vendor/FreeRTOS/include   -I/home/default/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F   -I/home/default/crazyflie-firmware/src/config   -I/home/default/crazyflie-firmware/src/platform/interface   -I/home/default/crazyflie-firmware/src/deck/interface   -I/home/default/crazyflie-firmware/src/deck/drivers/interface   -I/home/default/crazyflie-firmware/src/drivers/interface   -I/home/default/crazyflie-firmware/src/drivers/bosch/interface   -I/home/default/crazyflie-firmware/src/drivers/esp32/interface   -I/home/default/crazyflie-firmware/src/hal/interface   -I/home/default/crazyflie-firmware/src/modules/interface   -I/home/default/crazyflie-firmware/src/modules/interface/kalman_core   -I/home/default/crazyflie-firmware/src/modules/interface/lighthouse   -I/home/default/crazyflie-firmware/src/modules/interface/cpx   -I/home/default/crazyflie-firmware/src/utils/interface   -I/home/default/crazyflie-firmware/src/utils/interface/kve   -I/home/default/crazyflie-firmware/src/utils/interface/lighthouse   -I/home/default/crazyflie-firmware/src/utils/interface/tdoa   -I/home/default/crazyflie-firmware/src/lib/FatFS   -I/home/default/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include   -I/home/default/crazyflie-firmware/src/lib/STM32_USB_Device_Library/Core/inc   -I/home/default/crazyflie-firmware/src/lib/STM32_USB_OTG_Driver/inc   -I/home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc   -I/home/default/crazyflie-firmware/src/lib/vl53l1   -I/home/default/crazyflie-firmware/src/lib/vl53l1/core/inc   -I/home/default/IMAV2022-NanocopterAi/app_imav/app_imav_crazyflie/build/include/generated -fno-delete-null-pointer-checks --param=allow-store-data-races=0 -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=date-time -DCC_HAVE_ASM_GOTO -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -fno-math-errno -DARM_MATH_CM4 -D__FPU_PRESENT=1 -mfp16-format=ieee -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -Os -Werror   -c -o src/drivers/bosch/src/bmp280.o /home/default/crazyflie-firmware/src/drivers/bosch/src/bmp280.c

source_src/drivers/bosch/src/bmp280.o := /home/default/crazyflie-firmware/src/drivers/bosch/src/bmp280.c

deps_src/drivers/bosch/src/bmp280.o := \
    $(wildcard include/config/reg/spi3/enable//reg.h) \
    $(wildcard include/config/reg/spi3/enable.h) \
    $(wildcard include/config/reg/filter//reg.h) \
    $(wildcard include/config/reg/filter.h) \
    $(wildcard include/config/reg/standby/durn//reg.h) \
    $(wildcard include/config/reg/standby/durn.h) \
  /home/default/crazyflie-firmware/src/drivers/bosch/src/../interface/bmp280.h \
    $(wildcard include/config/reg.h) \
    $(wildcard include/config/reg/standby/durn//pos.h) \
    $(wildcard include/config/reg/standby/durn//msk.h) \
    $(wildcard include/config/reg/standby/durn//len.h) \
    $(wildcard include/config/reg/filter//pos.h) \
    $(wildcard include/config/reg/filter//msk.h) \
    $(wildcard include/config/reg/filter//len.h) \
    $(wildcard include/config/reg/spi3/enable//pos.h) \
    $(wildcard include/config/reg/spi3/enable//msk.h) \
    $(wildcard include/config/reg/spi3/enable//len.h) \
  /home/default/crazyflie-firmware/src/drivers/bosch/src/../interface/bstdr_comm_support.h \
  /home/default/crazyflie-firmware/src/drivers/bosch/src/../interface/bstdr_types.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdint.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdbool.h \

src/drivers/bosch/src/bmp280.o: $(deps_src/drivers/bosch/src/bmp280.o)

$(deps_src/drivers/bosch/src/bmp280.o):
