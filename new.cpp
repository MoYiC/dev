// 定义常量
const uint8_t STATUS_REG_ADDR = 0x01; // STATUS寄存器地址
const uint8_t READ_CMD_BIT = 0x80; // 读取命令标志位
const uint8_t RESPIRATORY_RATE_REG_ADDR = 0x04; // 呼吸率寄存器地址
const uint16_t SAMPLE_RATE_PER_MIN = 50; // 每分钟测量次数

// 配置SPI通信
spi.begin(); // 初始化SPI接口
spi.setClockSpeed(1000000); // 将SPI时钟速度设置为1 MHz

// 读取STATUS寄存器
spi.beginTransaction(); // 开始事务
digitalWrite(SS, LOW); // 选择设备
spi.transfer(READ_CMD_BIT | STATUS_REG_ADDR); // 发送读取命令和STATUS寄存器地址
uint8_t status_value = spi.transfer(0); // 读取STATUS寄存器值
digitalWrite(SS, HIGH); // 取消设备选择
spi.endTransaction(); // 结束事务

// 检查是否有新的测量准备好
if (status_value & 0b00000001) { // 测试第0位以检查新的测量准备是否就绪
    // 新的测量准备好了，读取呼吸率
    uint16_t respiratory_rate = 0;
    spi.beginTransaction();
    digitalWrite(SS, LOW); // 选择设备
    spi.transfer(READ_CMD_BIT | RESPIRATORY_RATE_REG_ADDR); // 发送读取命令和呼吸率寄存器地址
    respiratory_rate |= spi.transfer(0x00) << 8; // 读取呼吸率高字节
    respiratory_rate |= spi.transfer(0x00); // 读取呼吸率低字节
    digitalWrite(SS, HIGH); // 取消设备选择
    spi.endTransaction();

    // 将呼吸率转换为每分钟的心跳数
    uint16_t bpm = respiratory_rate * 60 / SAMPLE_RATE_PER_MIN;

    // 输出结果
    Serial.print("Respiratory rate: ");
    Serial.print(bpm);
    Serial.println(" bpm");
} else {
    // 没有新的测量准备好
    Serial.println("No new measurement is available");
}
