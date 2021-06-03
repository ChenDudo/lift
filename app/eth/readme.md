# 呼梯通信帧定义
## 广播发起帧
### 完整帧
```
【dat】: 55 55 55 55 55 55   66 66 66 66 66 66   AE 88   01 01 01   01 00 00   2C 73
【idx】: 01 02 03 04 05 06   07 08 09 10 11 12   13 14   15 16 17   18 19 20   21 22
```
### 格式定义
| DirAddr | SrcAddr | FrameType | Head | data | FCS |
|:-:|:---:|:--:|:--:|:--:|:--:|
| 55 55 55 55 55 55 | 66 66 66 66 66 66 | AE 88 | 01 01 `01` | `01 00 00` | 2C 73 |
| 6 Bytes | 6 Bytes | 2 Bytes | 3 Bytes | `3*1` Bytes | 2 Bytes |

### 注解
- head
    - head【0】：Board Index：主控制板默认为 0x00，其余为 `myDev.id`
    - head【1】：Command Value
        - Master send: 0x01
        - Slave ack: 0x81
    - head【2】：sub-Frame Total number：主控制板默认为 0x01

- data
    - data【0】：当前楼层
    - data【1】：楼层状态
        - 0x00：停留
        - 0x01：上行
        - 0x02：下行
    - data【2】：保留

## 广播回应帧
### 完整帧
```
【dat】: 55 55 55 55 55 55   66 66 66 66 66 66   AE 88   01 81 02   03 01 00  06 00 01   34 89
【idx】: 01 02 03 04 05 06   07 08 09 10 11 12   13 14   15 16 17   18 19 20  21 22 23   24 25
```
### 格式定义
| DirAddr | SrcAddr | FrameType | Head | data | FCS |
|:-:|:---:|:--:|:--:|:--:|:--:|
| 55 55 55 55 55 55 | 66 66 66 66 66 66 | AE 88 | `X1` 81 `N1` | `X2 U2 D2 X3 U3 D3` | 34 89 |
| 6 Bytes | 6 Bytes | 2 Bytes | 2 Bytes | `3*N1` Bytes | 2 Bytes |

### 注解
- `X1`: my board idx---if anser to master, ack the max boardIdx value
- `N1`: data frame number, datalen = 3*N1
- data: board information
    - `X2 X3 ..`：请求楼层
    - `U2 U3 ..`：上行请求
    - `D2 D3 ..`：下行请求
## 出错处理
- CheckSum check Error        ------------> quit receive Handle
- Frame Type != 0xAE 0x88      ------------> quit receive Handle
- ......