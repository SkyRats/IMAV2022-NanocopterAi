cmd_src/modules/src/stabilizer.o := arm-none-eabi-gcc -Wp,-MD,src/modules/src/.stabilizer.o.d    -I/home/default/crazyflie-firmware/src/modules/src -Isrc/modules/src -D__firmware__ -fno-exceptions -Wall -Wmissing-braces -fno-strict-aliasing -ffunction-sections -fdata-sections -Wdouble-promotion -std=gnu11 -DCRAZYFLIE_FW   -I/home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include   -I/home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include   -I/home/default/crazyflie-firmware/vendor/libdw1000/inc   -I/home/default/crazyflie-firmware/vendor/FreeRTOS/include   -I/home/default/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F   -I/home/default/crazyflie-firmware/src/config   -I/home/default/crazyflie-firmware/src/platform/interface   -I/home/default/crazyflie-firmware/src/deck/interface   -I/home/default/crazyflie-firmware/src/deck/drivers/interface   -I/home/default/crazyflie-firmware/src/drivers/interface   -I/home/default/crazyflie-firmware/src/drivers/bosch/interface   -I/home/default/crazyflie-firmware/src/drivers/esp32/interface   -I/home/default/crazyflie-firmware/src/hal/interface   -I/home/default/crazyflie-firmware/src/modules/interface   -I/home/default/crazyflie-firmware/src/modules/interface/kalman_core   -I/home/default/crazyflie-firmware/src/modules/interface/lighthouse   -I/home/default/crazyflie-firmware/src/modules/interface/cpx   -I/home/default/crazyflie-firmware/src/utils/interface   -I/home/default/crazyflie-firmware/src/utils/interface/kve   -I/home/default/crazyflie-firmware/src/utils/interface/lighthouse   -I/home/default/crazyflie-firmware/src/utils/interface/tdoa   -I/home/default/crazyflie-firmware/src/lib/FatFS   -I/home/default/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include   -I/home/default/crazyflie-firmware/src/lib/STM32_USB_Device_Library/Core/inc   -I/home/default/crazyflie-firmware/src/lib/STM32_USB_OTG_Driver/inc   -I/home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc   -I/home/default/crazyflie-firmware/src/lib/vl53l1   -I/home/default/crazyflie-firmware/src/lib/vl53l1/core/inc   -I/home/default/crazyflie-firmware/code_test/test_app/build/include/generated -fno-delete-null-pointer-checks --param=allow-store-data-races=0 -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=date-time -DCC_HAVE_ASM_GOTO -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -fno-math-errno -DARM_MATH_CM4 -D__FPU_PRESENT=1 -mfp16-format=ieee -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -Os -Werror   -c -o src/modules/src/stabilizer.o /home/default/crazyflie-firmware/src/modules/src/stabilizer.c

source_src/modules/src/stabilizer.o := /home/default/crazyflie-firmware/src/modules/src/stabilizer.c

deps_src/modules/src/stabilizer.o := \
    $(wildcard include/config/deck/usd.h) \
    $(wildcard include/config/motors/esc/protocol/dshot.h) \
  /usr/include/newlib/math.h \
  /usr/include/newlib/sys/reent.h \
  /usr/include/newlib/_ansi.h \
  /usr/include/newlib/newlib.h \
  /usr/include/newlib/_newlib_version.h \
  /usr/include/newlib/sys/config.h \
    $(wildcard include/config/h//.h) \
  /usr/include/newlib/machine/ieeefp.h \
  /usr/include/newlib/sys/features.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stddef.h \
  /usr/include/newlib/sys/_types.h \
  /usr/include/newlib/machine/_types.h \
  /usr/include/newlib/machine/_default_types.h \
  /usr/include/newlib/sys/lock.h \
  /usr/include/newlib/sys/cdefs.h \
  /usr/include/newlib/_ansi.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/FreeRTOS.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdint.h \
  /home/default/crazyflie-firmware/src/config/FreeRTOSConfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/debug/queue/monitor.h) \
  /home/default/crazyflie-firmware/src/config/config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/block/address.h) \
  /home/default/crazyflie-firmware/src/drivers/interface/nrf24l01.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdbool.h \
  /home/default/crazyflie-firmware/src/drivers/interface/nRF24L01reg.h \
  /home/default/crazyflie-firmware/src/config/trace.h \
  /home/default/crazyflie-firmware/src/hal/interface/usec_time.h \
  /home/default/crazyflie-firmware/src/utils/interface/cfassert.h \
  /home/default/crazyflie-firmware/src/config/stm32fxxx.h \
  /home/default/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include/stm32f4xx.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/core_cm4.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_version.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_compiler.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_gcc.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/mpu_armv7.h \
  /home/default/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include/system_stm32f4xx.h \
  /home/default/crazyflie-firmware/src/config/stm32f4xx_conf.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_adc.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_crc.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dbgmcu.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dma.h \
    $(wildcard include/config/it.h) \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_exti.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_flash.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_i2c.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_iwdg.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_pwr.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rcc.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rtc.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_sdio.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_spi.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_syscfg.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_tim.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_usart.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_wwdg.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_misc.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_cryp.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_hash.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rng.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_can.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dac.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dcmi.h \
  /home/default/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_fsmc.h \
  /home/default/crazyflie-firmware/src/modules/interface/console.h \
  /home/default/crazyflie-firmware/src/utils/interface/eprintf.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdarg.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/projdefs.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/portable.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/deprecated_definitions.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/mpu_wrappers.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/task.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/list.h \
  /home/default/crazyflie-firmware/src/modules/interface/system.h \
  /home/default/crazyflie-firmware/src/modules/interface/log.h \
  /home/default/crazyflie-firmware/src/modules/interface/param.h \
  /home/default/crazyflie-firmware/src/modules/interface/param_logic.h \
    $(wildcard include/config/param/silent/updates.h) \
  /home/default/crazyflie-firmware/src/modules/interface/crtp.h \
  /home/default/crazyflie-firmware/src/utils/interface/debug.h \
    $(wildcard include/config/debug/print/on/uart1.h) \
  /home/default/crazyflie-firmware/src/config/config.h \
  /home/default/crazyflie-firmware/src/drivers/interface/motors.h \
    $(wildcard include/config/motors/esc/protocol/oneshot125.h) \
    $(wildcard include/config/motors/esc/protocol/oneshot42.h) \
    $(wildcard include/config/motors/dshot/pwm/150khz.h) \
    $(wildcard include/config/motors/dshot/pwm/300khz.h) \
    $(wildcard include/config/motors/dshot/pwm/600khz.h) \
  /home/default/crazyflie-firmware/src/hal/interface/pm.h \
  /home/default/crazyflie-firmware/src/drivers/interface/adc.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/semphr.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/queue.h \
  /home/default/crazyflie-firmware/vendor/FreeRTOS/include/task.h \
  /home/default/crazyflie-firmware/src/hal/interface/syslink.h \
  /home/default/crazyflie-firmware/src/deck/interface/deck.h \
  /home/default/crazyflie-firmware/src/deck/interface/deck_core.h \
  /home/default/crazyflie-firmware/src/modules/interface/estimator.h \
    $(wildcard include/config/estimator/kalman/enable.h) \
    $(wildcard include/config/estimator/oot.h) \
  /home/default/crazyflie-firmware/src/modules/interface/stabilizer_types.h \
  /home/default/crazyflie-firmware/src/hal/interface/imu_types.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_types.h \
  /home/default/crazyflie-firmware/src/deck/interface/deck_constants.h \
  /home/default/crazyflie-firmware/src/deck/interface/deck_digital.h \
  /home/default/crazyflie-firmware/src/deck/interface/deck_analog.h \
  /home/default/crazyflie-firmware/src/deck/interface/deck_spi.h \
  /usr/include/newlib/string.h \
  /usr/include/newlib/sys/_locale.h \
  /usr/include/newlib/strings.h \
  /usr/include/newlib/sys/string.h \
  /home/default/crazyflie-firmware/src/platform/interface/platform.h \
    $(wildcard include/config/sensors/bmi088/bmp388.h) \
    $(wildcard include/config/sensors/bmi088/spi.h) \
    $(wildcard include/config/sensors/mpu9250/lps25h.h) \
    $(wildcard include/config/sensors/bosch.h) \
  /home/default/crazyflie-firmware/src/modules/interface/stabilizer.h \
  /home/default/crazyflie-firmware/src/hal/interface/sensors.h \
  /home/default/crazyflie-firmware/src/modules/interface/stabilizer_types.h \
  /home/default/crazyflie-firmware/src/modules/interface/commander.h \
  /home/default/crazyflie-firmware/src/modules/interface/crtp_commander_high_level.h \
  /home/default/crazyflie-firmware/src/modules/interface/math3d.h \
  /home/default/crazyflie-firmware/src/modules/interface/crtp_localization_service.h \
    $(wildcard include/config/deck/lighthouse.h) \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/pulse_processor.h \
    $(wildcard include/config/deck/lighthouse/max/n/bs.h) \
  /usr/include/newlib/stdlib.h \
  /usr/include/newlib/machine/stdlib.h \
  /usr/include/newlib/alloca.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/ootx_decoder.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_calibration.h \
  /home/default/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_geometry.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include/arm_math.h \
  /home/default/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include/cmsis_compiler.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/float.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include-fixed/limits.h \
  /home/default/crazyflie-firmware/src/modules/interface/controller.h \
  /home/default/crazyflie-firmware/src/modules/interface/power_distribution.h \
  /home/default/crazyflie-firmware/src/modules/interface/collision_avoidance.h \
  /home/default/crazyflie-firmware/src/modules/interface/health.h \
  /home/default/crazyflie-firmware/src/modules/interface/supervisor.h \
  /home/default/crazyflie-firmware/src/deck/drivers/interface/usddeck.h \
  /home/default/crazyflie-firmware/src/modules/interface/quatcompress.h \
  /home/default/crazyflie-firmware/src/utils/interface/statsCnt.h \
  /home/default/crazyflie-firmware/src/modules/interface/static_mem.h \
  /home/default/crazyflie-firmware/src/utils/interface/rateSupervisor.h \

src/modules/src/stabilizer.o: $(deps_src/modules/src/stabilizer.o)

$(deps_src/modules/src/stabilizer.o):
