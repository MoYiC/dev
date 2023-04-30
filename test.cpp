// Configure SPI communication
spi.begin();
spi.setClockSpeed(1000000); // Set SPI clock speed to 1 MHz

// Read the STATUS register
uint8_t status_reg = 0x01; // STATUS register address
uint8_t read_cmd = 0x80; // Read command
spi.beginTransaction();
digitalWrite(SS, LOW); // Select the device
spi.transfer(read_cmd | status_reg); // Send the read command and register address
uint8_t status_value = spi.transfer(0
);
digitalWrite(SS, HIGH); // Deselect the device
spi.endTransaction();

// Check if a new measurement is ready
if (status_value & 0x01) {
// New measurement is ready, read the respiratory rate
uint16_t respiratory_rate = 0;
spi.beginTransaction();
digitalWrite(SS, LOW); // Select the device
spi.transfer(0x80 | 0x04); // Send the read command and respiratory rate register address
respiratory_rate |= spi.transfer(0x00) << 8; // Read the high byte of respiratory rate
respiratory_rate |= spi.transfer(0x00); // Read the low byte of respiratory rate
digitalWrite(SS, HIGH); // Deselect the device
spi.endTransaction();

// Convert respiratory rate to beats per minute
uint16_t bpm = respiratory_rate * 60 / sample_rate;

// Print the result
Serial.print("Respiratory rate: ");
Serial.print(bpm);
Serial.println(" bpm");
} else {
// No new measurement is ready
Serial.println("No new measurement is available");
}