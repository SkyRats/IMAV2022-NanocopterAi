cmd_src/drivers/bosch/src/bmi088_fifo.o := arm-none-eabi-gcc -Wp,-MD,src/drivers/bosch/src/.bmi088_fifo.o.d    -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/bosch/src -Isrc/drivers/bosch/src -D__firmware__ -fno-exceptions -Wall -Wmissing-braces -fno-strict-aliasing -ffunction-sections -fdata-sections -Wdouble-promotion -std=gnu11 -DCRAZYFLIE_FW   -I/home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/libdw1000/inc   -I/home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F   -I/home/ipontes3olavy/crazyflie-firmware/src/config   -I/home/ipontes3olavy/crazyflie-firmware/src/platform/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/deck/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/deck/drivers/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/bosch/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/esp32/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/hal/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface/kalman_core   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface/lighthouse   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/kve   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/lighthouse   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/tdoa   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/FatFS   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32_USB_Device_Library/Core/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32_USB_OTG_Driver/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc   -I/home/ipontes3olavy/sky/IMAV2022-NanocopterAi/app_imav/takeoff_test/build/include/generated -fno-delete-null-pointer-checks --param=allow-store-data-races=0 -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=date-time -DCC_HAVE_ASM_GOTO -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -fno-math-errno -DARM_MATH_CM4 -D__FPU_PRESENT=1 -mfp16-format=ieee -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -Os -Werror   -c -o src/drivers/bosch/src/bmi088_fifo.o /home/ipontes3olavy/crazyflie-firmware/src/drivers/bosch/src/bmi088_fifo.c

source_src/drivers/bosch/src/bmi088_fifo.o := /home/ipontes3olavy/crazyflie-firmware/src/drivers/bosch/src/bmi088_fifo.c

deps_src/drivers/bosch/src/bmi088_fifo.o := \
    $(wildcard include/config/1/reg.h) \
    $(wildcard include/config/0/reg.h) \
    $(wildcard include/config/check.h) \

src/drivers/bosch/src/bmi088_fifo.o: $(deps_src/drivers/bosch/src/bmi088_fifo.o)

$(deps_src/drivers/bosch/src/bmi088_fifo.o):
