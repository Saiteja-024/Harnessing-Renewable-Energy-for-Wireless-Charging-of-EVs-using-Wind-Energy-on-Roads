#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

const int batteryPin = A0;           // Analog pin to read battery voltage
float batteryVoltage;                 // To store the voltage
float batteryPercentage;              // To store battery percentage
String chargingStatus;                // To indicate charging status

unsigned long scrollTimer = 0;        // Timer for scrolling display
int scrollPosition = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();                         // Initialize the LCD
  lcd.backlight();                    // Turn on the LCD backlight
}

void loop() {
  readBattery();
  displayBatteryInfo();
  delay(5000);                        // Update every 5 seconds
}

void readBattery() {
  int sensorValue = analogRead(batteryPin);

  // Convert the analog reading to battery voltage assuming 5V as reference
  batteryVoltage = sensorValue * (5.0 / 1023.0);

  // Calculate battery percentage assuming 5V is 100% charged
  batteryPercentage = (batteryVoltage / 5.0) * 100;

  // Determine charging status based on voltage threshold (e.g., 4.2V as charging threshold)
  if (batteryVoltage >= 4.2) {
    chargingStatus = "Charging ON";
  } else {
    chargingStatus = "Charging OFF";
  }

  // Debugging output
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print(" V, Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.print("%, ");
  Serial.println(chargingStatus);
}

void displayBatteryInfo() {
  // Clear the LCD screen
  lcd.clear();

  // Format the display message
  String line1 = "Volt: " + String(batteryVoltage, 2) + "V ";
  String line2 = "Perc: " + String(batteryPercentage, 0) + "% " + chargingStatus;

  // Display static content or scroll if content is too long
  if (line1.length() > 16 || line2.length() > 16) {
    scrollDisplay(line1, line2);
  } else {
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}

// Function to scroll long messages on the LCD
void scrollDisplay(String line1, String line2) {
  unsigned long currentMillis = millis();
  if (currentMillis - scrollTimer >= 500) { // Scroll every 500ms
    scrollTimer = currentMillis;
    lcd.clear();

    // Print scrolling portion of each line
    lcd.setCursor(0, 0);
    lcd.print(line1.substring(scrollPosition, scrollPosition + 16));
    lcd.setCursor(0, 1);
    lcd.print(line2.substring(scrollPosition, scrollPosition + 16));

    // Update scroll position
    scrollPosition++;
    if (scrollPosition > line1.length() - 16 && scrollPosition > line2.length() - 16) {
      scrollPosition = 0; // Reset scroll position after full message has been displayed
    }
  }
}
