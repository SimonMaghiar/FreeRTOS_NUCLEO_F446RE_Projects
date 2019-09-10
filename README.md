# FreeRTOS_NUCLEO_F446RE_Projects

# STM32_HelloWorld used cooperative scheduling which means that the System Handler doesn't trigger the PendSV Handler every 1ms. In cooperative scheduling the task manually should yield it's execution to another task. In this case we used taskYIELD Api to do it. We used SEGGER software to trace the signals and the operation behavior.
