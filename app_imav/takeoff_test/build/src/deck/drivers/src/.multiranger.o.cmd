cmd_src/deck/drivers/src/multiranger.o := arm-none-eabi-gcc -Wp,-MD,src/deck/drivers/src/.multiranger.o.d    -I/home/ipontes3olavy/crazyflie-firmware/src/deck/drivers/src -Isrc/deck/drivers/src -D__firmware__ -fno-exceptions -Wall -Wmissing-braces -fno-strict-aliasing -ffunction-sections -fdata-sections -Wdouble-promotion -std=gnu11 -DCRAZYFLIE_FW   -I/home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/Core/Include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/CMSIS/CMSIS/DSP/Include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/libdw1000/inc   -I/home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include   -I/home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/portable/GCC/ARM_CM4F   -I/home/ipontes3olavy/crazyflie-firmware/src/config   -I/home/ipontes3olavy/crazyflie-firmware/src/platform/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/deck/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/deck/drivers/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/bosch/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/drivers/esp32/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/hal/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface/kalman_core   -I/home/ipontes3olavy/crazyflie-firmware/src/modules/interface/lighthouse   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/kve   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/lighthouse   -I/home/ipontes3olavy/crazyflie-firmware/src/utils/interface/tdoa   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/FatFS   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/CMSIS/STM32F4xx/Include   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32_USB_Device_Library/Core/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32_USB_OTG_Driver/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/STM32F4xx_StdPeriph_Driver/inc   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1   -I/home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc   -I/home/ipontes3olavy/sky/IMAV2022-NanocopterAi/app_imav/takeoff_test/build/include/generated -fno-delete-null-pointer-checks --param=allow-store-data-races=0 -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=date-time -DCC_HAVE_ASM_GOTO -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -fno-math-errno -DARM_MATH_CM4 -D__FPU_PRESENT=1 -mfp16-format=ieee -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -Os -Werror   -c -o src/deck/drivers/src/multiranger.o /home/ipontes3olavy/crazyflie-firmware/src/deck/drivers/src/multiranger.c

source_src/deck/drivers/src/multiranger.o := /home/ipontes3olavy/crazyflie-firmware/src/deck/drivers/src/multiranger.c

deps_src/deck/drivers/src/multiranger.o := \
  /home/ipontes3olavy/crazyflie-firmware/src/deck/interface/deck.h \
  /home/ipontes3olavy/crazyflie-firmware/src/deck/interface/deck_core.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdint.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdbool.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/estimator.h \
    $(wildcard include/config/estimator/kalman/enable.h) \
    $(wildcard include/config/estimator/oot.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/stabilizer_types.h \
  /home/ipontes3olavy/crazyflie-firmware/src/hal/interface/imu_types.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/lighthouse/lighthouse_types.h \
  /home/ipontes3olavy/crazyflie-firmware/src/deck/interface/deck_constants.h \
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
  /home/ipontes3olavy/crazyflie-firmware/src/deck/interface/deck_digital.h \
  /home/ipontes3olavy/crazyflie-firmware/src/deck/interface/deck_analog.h \
  /home/ipontes3olavy/crazyflie-firmware/src/deck/interface/deck_spi.h \
  /usr/include/newlib/string.h \
  /usr/include/newlib/_ansi.h \
  /usr/include/newlib/newlib.h \
  /usr/include/newlib/_newlib_version.h \
  /usr/include/newlib/sys/config.h \
    $(wildcard include/config/h//.h) \
  /usr/include/newlib/machine/ieeefp.h \
  /usr/include/newlib/sys/features.h \
  /usr/include/newlib/sys/reent.h \
  /usr/include/newlib/_ansi.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stddef.h \
  /usr/include/newlib/sys/_types.h \
  /usr/include/newlib/machine/_types.h \
  /usr/include/newlib/machine/_default_types.h \
  /usr/include/newlib/sys/lock.h \
  /usr/include/newlib/sys/cdefs.h \
  /usr/include/newlib/sys/_locale.h \
  /usr/include/newlib/strings.h \
  /usr/include/newlib/sys/string.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/param.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/param_logic.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/crtp.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/system.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/debug.h \
    $(wildcard include/config/debug/print/on/uart1.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/config/config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/block/address.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/nrf24l01.h \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/nRF24L01reg.h \
  /home/ipontes3olavy/crazyflie-firmware/src/config/trace.h \
  /home/ipontes3olavy/crazyflie-firmware/src/hal/interface/usec_time.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/console.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/eprintf.h \
  /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdarg.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/log.h \
  /home/ipontes3olavy/crazyflie-firmware/src/hal/interface/pca95x4.h \
    $(wildcard include/config/reg.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/vl53l1x.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_ll_def.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_ll_device.h \
    $(wildcard include/config/vhv.h) \
    $(wildcard include/config/phasecal.h) \
    $(wildcard include/config/reference/phase.h) \
    $(wildcard include/config/dss1.h) \
    $(wildcard include/config/dss2.h) \
    $(wildcard include/config/mm1.h) \
    $(wildcard include/config/mm2.h) \
    $(wildcard include/config/range.h) \
    $(wildcard include/config/timeout/us.h) \
    $(wildcard include/config/target/total/rate/mcps.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/vl53l1_types.h \
  /usr/include/newlib/stdio.h \
  /usr/include/newlib/sys/types.h \
  /usr/include/newlib/sys/_stdint.h \
  /usr/include/newlib/machine/endian.h \
  /usr/include/newlib/machine/_endian.h \
  /usr/include/newlib/sys/select.h \
  /usr/include/newlib/sys/_sigset.h \
  /usr/include/newlib/sys/_timeval.h \
  /usr/include/newlib/sys/timespec.h \
  /usr/include/newlib/sys/_timespec.h \
  /usr/include/newlib/sys/_pthreadtypes.h \
  /usr/include/newlib/sys/sched.h \
  /usr/include/newlib/machine/types.h \
  /usr/include/newlib/sys/stdio.h \
  /usr/include/newlib/stdlib.h \
  /usr/include/newlib/machine/stdlib.h \
  /usr/include/newlib/alloca.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/vl53l1_platform_user_config.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_error_codes.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_register_structs.h \
    $(wildcard include/config//spad/enables/ref/0.h) \
    $(wildcard include/config/i2c/index.h) \
    $(wildcard include/config//target/total/rate/mcps.h) \
    $(wildcard include/config//stream/count/update/value.h) \
    $(wildcard include/config//timeout/macrop/a/hi.h) \
    $(wildcard include/config//roi/mode/control.h) \
    $(wildcard include/config/i2c/size/bytes.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_register_map.h \
    $(wildcard include/config//vhv/ref/sel/vddpix.h) \
    $(wildcard include/config//vhv/ref/sel/vquench.h) \
    $(wildcard include/config//reg/avdd1v2/sel.h) \
    $(wildcard include/config//fast/osc//trim.h) \
    $(wildcard include/config//timeout/macrop/loop/bound.h) \
    $(wildcard include/config//count/thresh.h) \
    $(wildcard include/config//offset.h) \
    $(wildcard include/config//init.h) \
    $(wildcard include/config//spad/enables/ref/1.h) \
    $(wildcard include/config//spad/enables/ref/2.h) \
    $(wildcard include/config//spad/enables/ref/3.h) \
    $(wildcard include/config//spad/enables/ref/4.h) \
    $(wildcard include/config//spad/enables/ref/5.h) \
    $(wildcard include/config//ref/en/start/select.h) \
    $(wildcard include/config//inner/offset/mm.h) \
    $(wildcard include/config//inner/offset/mm/hi.h) \
    $(wildcard include/config//inner/offset/mm/lo.h) \
    $(wildcard include/config//outer/offset/mm.h) \
    $(wildcard include/config//outer/offset/mm/hi.h) \
    $(wildcard include/config//outer/offset/mm/lo.h) \
    $(wildcard include/config//target/total/rate/mcps/hi.h) \
    $(wildcard include/config//target/total/rate/mcps/lo.h) \
    $(wildcard include/config//spad/sel/pswidth.h) \
    $(wildcard include/config//vcsel/pulse/width/offset.h) \
    $(wildcard include/config//fast/osc//config/ctrl.h) \
    $(wildcard include/config/ctrl.h) \
    $(wildcard include/config//static/config/spare/0.h) \
    $(wildcard include/config/spare/0.h) \
    $(wildcard include/config//static/config/spare/1.h) \
    $(wildcard include/config/spare/1.h) \
    $(wildcard include/config//static/config/spare/2.h) \
    $(wildcard include/config/spare/2.h) \
    $(wildcard include/config//reset/stages/msb.h) \
    $(wildcard include/config//reset/stages/lsb.h) \
    $(wildcard include/config//stream/divider.h) \
    $(wildcard include/config/gpio.h) \
    $(wildcard include/config//vcsel/start.h) \
    $(wildcard include/config//repeat/rate.h) \
    $(wildcard include/config//repeat/rate/hi.h) \
    $(wildcard include/config//repeat/rate/lo.h) \
    $(wildcard include/config//vcsel/width.h) \
    $(wildcard include/config//timeout/macrop.h) \
    $(wildcard include/config//target.h) \
    $(wildcard include/config//override.h) \
    $(wildcard include/config//manual/effective/spads/select.h) \
    $(wildcard include/config//manual/effective/spads/select/hi.h) \
    $(wildcard include/config//manual/effective/spads/select/lo.h) \
    $(wildcard include/config//manual/block/select.h) \
    $(wildcard include/config//aperture/attenuation.h) \
    $(wildcard include/config//max/spads/limit.h) \
    $(wildcard include/config//min/spads/limit.h) \
    $(wildcard include/config//timeout/macrop/a/lo.h) \
    $(wildcard include/config//timeout/macrop/b/hi.h) \
    $(wildcard include/config//timeout/macrop/b/lo.h) \
    $(wildcard include/config//vcsel/period/a.h) \
    $(wildcard include/config//vcsel/period/b.h) \
    $(wildcard include/config//sigma/thresh.h) \
    $(wildcard include/config//sigma/thresh/hi.h) \
    $(wildcard include/config//sigma/thresh/lo.h) \
    $(wildcard include/config//min/count/rate/rtn/limit/mcps.h) \
    $(wildcard include/config//min/count/rate/rtn/limit/mcps/hi.h) \
    $(wildcard include/config//min/count/rate/rtn/limit/mcps/lo.h) \
    $(wildcard include/config//valid/phase/low.h) \
    $(wildcard include/config//valid/phase/high.h) \
    $(wildcard include/config//woi/sd0.h) \
    $(wildcard include/config//woi/sd1.h) \
    $(wildcard include/config//initial/phase/sd0.h) \
    $(wildcard include/config//initial/phase/sd1.h) \
    $(wildcard include/config//first/order/select.h) \
    $(wildcard include/config//quantifier.h) \
    $(wildcard include/config//user/roi/centre/spad.h) \
    $(wildcard include/config//user/roi/requested/global/xy/size.h) \
    $(wildcard include/config//powerdown/go1.h) \
    $(wildcard include/config//ref/bg/ctrl.h) \
    $(wildcard include/config//regdvdd1v2/ctrl.h) \
    $(wildcard include/config//osc/slow/ctrl.h) \
    $(wildcard include/config//fast/osc//trim/max.h) \
    $(wildcard include/config//fast/osc//freq/set.h) \
    $(wildcard include/config//vcsel/trim.h) \
    $(wildcard include/config//vcsel/selion.h) \
    $(wildcard include/config//vcsel/selion/max.h) \
    $(wildcard include/config//spad/enables/rtn/0.h) \
    $(wildcard include/config//spad/enables/rtn/1.h) \
    $(wildcard include/config//spad/enables/rtn/2.h) \
    $(wildcard include/config//spad/enables/rtn/3.h) \
    $(wildcard include/config//spad/enables/rtn/4.h) \
    $(wildcard include/config//spad/enables/rtn/5.h) \
    $(wildcard include/config//spad/enables/rtn/6.h) \
    $(wildcard include/config//spad/enables/rtn/7.h) \
    $(wildcard include/config//spad/enables/rtn/8.h) \
    $(wildcard include/config//spad/enables/rtn/9.h) \
    $(wildcard include/config//spad/enables/rtn/10.h) \
    $(wildcard include/config//spad/enables/rtn/11.h) \
    $(wildcard include/config//spad/enables/rtn/12.h) \
    $(wildcard include/config//spad/enables/rtn/13.h) \
    $(wildcard include/config//spad/enables/rtn/14.h) \
    $(wildcard include/config//spad/enables/rtn/15.h) \
    $(wildcard include/config//spad/enables/rtn/16.h) \
    $(wildcard include/config//spad/enables/rtn/17.h) \
    $(wildcard include/config//spad/enables/rtn/18.h) \
    $(wildcard include/config//spad/enables/rtn/19.h) \
    $(wildcard include/config//spad/enables/rtn/20.h) \
    $(wildcard include/config//spad/enables/rtn/21.h) \
    $(wildcard include/config//spad/enables/rtn/22.h) \
    $(wildcard include/config//spad/enables/rtn/23.h) \
    $(wildcard include/config//spad/enables/rtn/24.h) \
    $(wildcard include/config//spad/enables/rtn/25.h) \
    $(wildcard include/config//spad/enables/rtn/26.h) \
    $(wildcard include/config//spad/enables/rtn/27.h) \
    $(wildcard include/config//spad/enables/rtn/28.h) \
    $(wildcard include/config//spad/enables/rtn/29.h) \
    $(wildcard include/config//spad/enables/rtn/30.h) \
    $(wildcard include/config//spad/enables/rtn/31.h) \
    $(wildcard include/config//mode/roi/centre/spad.h) \
    $(wildcard include/config//mode/roi/xy/size.h) \
    $(wildcard include/config//a0.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/vl53l1_platform_user_defines.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_error_exceptions.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/vl53l1_platform_user_data.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_def.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_ll_def.h \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/i2cdev.h \
  /home/ipontes3olavy/crazyflie-firmware/src/drivers/interface/i2c_drv.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/FreeRTOS.h \
  /home/ipontes3olavy/crazyflie-firmware/src/config/FreeRTOSConfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/debug/queue/monitor.h) \
  /home/ipontes3olavy/crazyflie-firmware/src/config/config.h \
  /home/ipontes3olavy/crazyflie-firmware/src/utils/interface/cfassert.h \
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
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_api.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_api_strings.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_def.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/core/inc/vl53l1_api_core.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/vl53l1_platform.h \
  /home/ipontes3olavy/crazyflie-firmware/src/lib/vl53l1/vl53l1_platform_log.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/range.h \
  /home/ipontes3olavy/crazyflie-firmware/src/modules/interface/static_mem.h \
  /home/ipontes3olavy/crazyflie-firmware/vendor/FreeRTOS/include/task.h \

src/deck/drivers/src/multiranger.o: $(deps_src/deck/drivers/src/multiranger.o)

$(deps_src/deck/drivers/src/multiranger.o):
