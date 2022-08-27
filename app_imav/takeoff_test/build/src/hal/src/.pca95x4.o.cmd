cmd_src/hal/src/pca95x4.o := arm-none-eabi-gcc -Wp,-MD,src/hal/src/.pca95x4.o.d    -I/home/ipontes3olavy/crazyflie-firmware/src/hal/src -Isrc/hal/src -D__firmware__ -fno-exceptions -Wall -Wmissing-braces -fno-strict-aliasing -ffunction-sections -fdata-sections -Wdouble-promotion -std=gnu11 -DCRAZYFLIE_FW   -I/home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/libdw1000/inc   -I/home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F   -I/home/ipontes3olavy/crazyflie-firmware/src/config   -I/home/ipontes3olavy/crazyflie-firmware/src/platform/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/deck/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/deck/drivers/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/bosch/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/esp32/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/hal/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface/kalman_core   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface/lighthouse   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/kve   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/lighthouse   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/tdoa   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/FatFS   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32_USB_Device_Library/Core/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32_USB_OTG_Driver/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc   -I/home/ipontes3olavy/sky/IMAV2022-NanocopterAi/app_imav/takeoff_test/build/include/generated -fno-delete-null-pointer-checks --param=allow-store-data-races=0 -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=date-time -DCC_HAVE_ASM_GOTO -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -fno-math-errno -DARM_MATH_CM4 -D__FPU_PRESENT=1 -mfp16-format=ieee -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -Os -Werror   -c -o src/hal/src/pca95x4.o /home/ipontes3olavy/crazyflie-firmware/src/hal/src/pca95x4.c

source_src/hal/src/pca95x4.o := /home/ipontes3olavy/crazyflie-firmware/src/hal/src/pca95x4.c

deps_src/hal/src/pca95x4.o := \
    $(wildcard include/config/reg.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/i2cdev.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdint.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdbool.h \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/i2c_drv.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/FreeRTOS.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stddef.h \
  /home/ipontes3olavy/crazyflie-firmware/src/config/FreeRTOSConfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/debug/queue/monitor.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/config/config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/block/address.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/nrf24l01.h \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/nRF24L01reg.h \
  /home/ipontes3olavy/crazyflie-firmware/src/config/trace.h \
  /home/ipontes3olavy/crazyflie-firmware/src/hal/interface/usec_time.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/cfassert.h \
  /home/ipontes3olavy/crazyflie-firmware/src/config/stm32fxxx.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include/stm32f4xx.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/core_cm4.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_version.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_compiler.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_gcc.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/mpu_armv7.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include/system_stm32f4xx.h \
  /home/ipontes3olavy/crazyflie-firmware/src/config/stm32f4xx_conf.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_adc.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_crc.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dbgmcu.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dma.h \
    $(wildcard include/config/it.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_exti.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_flash.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_i2c.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_iwdg.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_pwr.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rcc.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rtc.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_sdio.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_spi.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_syscfg.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_tim.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_usart.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_wwdg.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_misc.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_cryp.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_hash.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rng.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_can.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dac.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dcmi.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_fsmc.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/console.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/eprintf.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdarg.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/projdefs.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/portable.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/deprecated_definitions.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/mpu_wrappers.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/semphr.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/queue.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/task.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/list.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/queue.h \
  /home/ipontes3olavy/crazyflie-firmware/src/hal/interface/pca95x4.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/debug.h \
    $(wildcard include/config/debug/print/on/uart1.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/config/config.h \

src/hal/src/pca95x4.o: $(deps_src/hal/src/pca95x4.o)

$(deps_src/hal/src/pca95x4.o):
