# 以太网通信

## ModBus 

- ModBus 通信栈
![ModBus通信栈](https://pic1.zhimg.com/80/v2-fcb02a1ab065dc079d425a19ab9401ac_720w.jpg)


- ModBus 网络体系结构
![](https://pic4.zhimg.com/80/v2-9a42553452c676e6f92b7b7ac2f77467_720w.jpg) 

## 协议描述
- ModBus 通用帧
![](https://pic3.zhimg.com/80/v2-6fb92a88544ef21aefb0d66d07fc88e6_720w.jpg)
    - ADU：应用数据单元
    - PDU：载荷数据单元
- 正常处理
![](https://pic1.zhimg.com/80/v2-122e3cf2c070e6af67431d1421996a90_720w.jpg)

- 异常处理
![](https://pic4.zhimg.com/80/v2-e806ddc87c2cc46d9e7f72b86c20c6e3_720w.jpg)

- 服务器端事务处理状态图
![](https://pic1.zhimg.com/80/v2-188d2ec3c0fe8625e8b96a1a39bf5850_720w.jpg)
    - 正常 响应
        - `响应功能码 = 请求功能码`
    - 异常 响应
        - `相应功能码 = 请求功能码 + 0x80`
        - 提供对应异常码及其他差错相关信息

- 异常响应
-- 从主站询问中出现下列四种可能事件之一：
    - 服务器设备接收到无通信错误的请求，并且可以正常地处理询问，那么服务器设备将返回一个正常响应。
    - 由于通信错误，服务器没有接收到请求，那么不能返回响应。客户机程序将最终处理请求的超时状态。
    - 服务器接收到请求，但是检测到一个通信错误（奇偶校验、LRC、CRC、...），那么不能返回响应。客户机程序将最终处理请求的超时状态。
    - 服务器接收到无通信错误的请求，但不能处理这个请求（例如，如果请求读一个不存在的输出或寄存器），服务器将返回一个异常响应，通知用户错误的本质特性。
- 异常码的列表：
![](https://pic2.zhimg.com/80/v2-4a3af69228fd5d7f2e221a3c9238b74d_720w.jpg)


## EtherCAT

- 七层协议
EtherCAT系统中只用了3层：物理层、数据链路层、应用层。
![](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9waWM0LnpoaW1nLmNvbS92Mi1iZTk3NTdlZTE0MGQxMTBkOTEyYjhhMTI4ZWFiMmY5N19iLmpwZWc?x-oss-process=image/format,png)

- 帧结构
![](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9waWMzLnpoaW1nLmNvbS92Mi0xNWFhYmUxMzI3NDJiMzJhY2VkMDg0Mjc2ZjBmMjEzNl9iLnBuZw?x-oss-process=image/format,png)
