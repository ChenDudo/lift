# mm32_lift

This workspace include:
- MB039:
    - 子板程序
    - KEY1 / LED1: UP
    - KEY2 / LED2: Down
    - LED3: PHY linked status

- Master: 
    - 母板程序
    - 对比MB039项目新增 `master.c` 文件, 宏定义`__MasterTest`
    - 定时发送广播包，广播发包间隔: `[..\app\eth\master.c Line 48]`
    ```c
    #define BCPEIROD 200    //200ms
    ```

# Note：
MDM-2803 LCD display has 2 different driver:
- `__MDM2803_N`: new
- `__MDM2803`: old 
--------------------


[1] 本项目通信帧定义参见[《呼梯通信帧定义》](.//app//eth//readme.md)

update： 2021/06/03