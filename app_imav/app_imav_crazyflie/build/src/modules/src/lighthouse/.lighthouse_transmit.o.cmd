cmd_src/modules/src/lighthouse/lighthouse_transmit.o := arm-none-eabi-gcc -Wp,-MD,src/modules/src/lighthouse/.lighthouse_transmit.o.d    -I/home/default/crazyflie-firmware/src/modules/src/lighthouse -Isrc/modules/src/lighthouse -D__firmware__ -fno-exceptions -Wall -Wmissing-braces -fno-strict-aliasing -ffunction-sections -fdata-sections -Wdouble-promotion -std=gnu11 -DCRAZYFLIE_FW   -I/home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include   -I/home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include   -I/home/default/crazyflie-firmware/vendor/libdw1000/inc   -I/home/default/crazyflie-firmware/vendor/FreeRTOS/include   -I/home/default/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F   -I/home/default/crazyflie-firmware/src/config   -I/home/default/crazyflie-firmware/src/platform/interface   -I/home/default/crazyflie-firmware/src/deck/interface   -I/home/default/crazyflie-firmware/src/deck/drivers/interface   -I/home/default/crazyflie-firmware/src/drivers/interface   -I/home/default/crazyflie-firmware/src/drivers/bosch/interface   -I/home/default/crazyflie-firmware/src/drivers/esp32/interface   -I/home/default/crazyflie-firmware/src/hal/interface   -I/home/default/crazyflie-firmware/src/modules/interface   -I/home/default/crazyflie-firmware/src/modules/interface/kalman_core   -I/home/default/crazyflie-firmware/src/modules/interface/lighthouse   -I/home/default/crazyflie-firmware/src/modules/interface/cpx   -I/home/default/crazyflie-firmware/src/utils/interface   -I/home/default/crazyflie-firmware/src/utils/interface/kve   -I/home/default/crazyflie-firmware/src/utils/interface/lighthouse   -I/home/default/crazyflie-firmware/src/utils/interface/tdoa   -I/home/default/crazyflie-firmware/src/lib/FatFS   -I/home/default/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include   -I/home/default/crazyflie-firmware/src/lib/STM32_USB_Device_Library/Core/inc   -I/home/default/crazyflie-firmware/src/lib/STM32_USB_OTG_Driver/inc   -I/home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc   -I/home/default/crazyflie-firmware/src/lib/vl53l1   -I/home/default/crazyflie-firmware/src/lib/vl53l1/core/inc   -I/home/default/IMAV2022-NanocopterAi/app_imav/app_imav_crazyflie/build/include/generated -fno-delete-null-pointer-checks --param=allow-store-data-races=0 -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=date-time -DCC_HAVE_ASM_GOTO -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -fno-math-errno -DARM_MATH_CM4 -D__FPU_PRESENT=1 -mfp16-format=ieee -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -Os -Werror   -c -o src/modules/src/lighthouse/lighthouse_transmit.o /home/default/crazyflie-firmware/src/modules/src/lighthouse/lighthouse_transmit.c

source_src/modules/src/lighthouse/lighthouse_transmit.o := /home/default/crazyflie-firmware/src/modules/src/lighthouse/lighthouse_transmit.c

deps_src/modules/src/lighthouse/lighthouse_transmit.o := \
  /home/default/crazyflie-firmware/src/modules/interface/crtp.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdint.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdbool.h \
  /home/default/crazyflie-firmware/src/modules/interface/lighthouse/lighthouse_transmit.h \
  /home/default/crazyflie-firmware/src/modules/interface/lighthouse/lighthouse_core.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/pulse_processor.h \
    $(wildcard include/config/deck/lighthouse/max/n/bs.h) \
  /usr/include/newlib/stdlib.h \
  /usr/include/newlib/machine/ieeefp.h \
  /usr/include/newlib/_ansi.h \
  /usr/include/newlib/newlib.h \
  /usr/include/newlib/_newlib_version.h \
  /usr/include/newlib/sys/config.h \
    $(wildcard include/config/h//.h) \
  /usr/include/newlib/sys/features.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stddef.h \
  /usr/include/newlib/sys/reent.h \
  /usr/include/newlib/_ansi.h \
  /usr/include/newlib/sys/_types.h \
  /usr/include/newlib/machine/_types.h \
  /usr/include/newlib/machine/_default_types.h \
  /usr/include/newlib/sys/lock.h \
  /usr/include/newlib/sys/cdefs.h \
  /usr/include/newlib/machine/stdlib.h \
  /usr/include/newlib/alloca.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/ootx_decoder.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_calibration.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_types.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_geometry.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include/arm_math.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_compiler.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_gcc.h \
  /usr/include/newlib/string.h \
  /usr/include/newlib/sys/_locale.h \
  /usr/include/newlib/strings.h \
  /usr/include/newlib/sys/string.h \
  /usr/include/newlib/math.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/float.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include-fixed/limits.h \
  /home/default/crazyflie-firmware/src/modules/interface/stabilizer_types.h \
  /home/default/crazyflie-firmware/src/hal/interface/imu_types.h \
  /home/default/crazyflie-firmware/src/modules/interface/param.h \
  /home/default/crazyflie-firmware/src/modules/interface/param_logic.h \
    $(wildcard include/config/param/silent/updates.h) \
  /home/default/crazyflie-firmware/src/utils/interface/debug.h \
    $(wildcard include/config/debug/print/on/uart1.h) \
  /home/default/crazyflie-firmware/src/config/config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/block/address.h) \
  /home/default/crazyflie-firmware/src/drivers/interface/nrf24l01.h \
  /home/default/crazyflie-firmware/src/drivers/interface/nRF24L01reg.h \
  /home/default/crazyflie-firmware/src/config/trace.h \
  /home/default/crazyflie-firmware/src/hal/interface/usec_time.h \
  /home/default/crazyflie-firmware/src/modules/interface/console.h \
  /home/default/crazyflie-firmware/src/utils/interface/eprintf.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdarg.h \

src/modules/src/lighthouse/lighthouse_transmit.o: $(deps_src/modules/src/lighthouse/lighthouse_transmit.o)

$(deps_src/modules/src/lighthouse/lighthouse_transmit.o):
