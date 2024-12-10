#include <Wire.h> // I2C iletişimi için gerekli kütüphane
#include <LiquidCrystal_I2C.h> // I2C arayüzlü LCD ekran kontrolü için kütüphane

// STM32F103C8 Kartı'nın USART1 (PA9 - TX, PA10 - RX)

char data = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD'nin I2C adresi 0x27, 16x2 boyutunda

// Pin tanımlamaları
#define echoPin PB12 // Ultrasonik sensörün echo pini
#define trigPin PB13 // Ultrasonik sensörün trig pini
#define MotorR1 PB0 // Sağ motorun birinci pin tanımı
#define MotorR2 PA7 // Sağ motorun ikinci pin tanımı
#define MotorL1 PA2 // Sol motorun birinci pin tanımı
#define MotorL2 PA1 // Sol motorun ikinci pin tanımı
#define EN2 PA0 // Sağ motorun hız kontrol pini
#define EN1 PA6 // Sol motorun hız kontrol pini

long sure, uzaklik;

// İleri hareket fonksiyonu
void ileri() {
  digitalWrite(MotorR1, HIGH); // Sağ motor ileri yönde döner
  digitalWrite(MotorR2, LOW);
  analogWrite(EN1, 150); // Hız kontrolü
  digitalWrite(MotorL1, LOW); 
  digitalWrite(MotorL2, HIGH); // Sol motor ileri yönde döner
  analogWrite(EN2, 180); // Hız kontrolü
}

// Sağa dönüş fonksiyonu
void sag() {
  digitalWrite(MotorR1, LOW); // Sağ motor geri yönde döner
  digitalWrite(MotorR2, HIGH);
  analogWrite(EN1, 150); // Hız kontrolü
  digitalWrite(MotorL1, LOW); 
  digitalWrite(MotorL2, HIGH); // Sol motor ileri yönde döner
  analogWrite(EN2, 200); // Hız kontrolüa
}

// Sola dönüş fonksiyonu
void sol() {
  digitalWrite(MotorR1, HIGH); // Sağ motor ileri yönde döner
  digitalWrite(MotorR2, LOW);
  analogWrite(EN1, 150); // Hız kontrolü
  digitalWrite(MotorL1, HIGH); // Sol motor geri yönde döner
  digitalWrite(MotorL2, LOW);
  analogWrite(EN2, 150); // Hız kontrolü
}

// Geri hareket fonksiyonu
void geri() {
  digitalWrite(MotorR1, LOW); // Sağ motor geri yönde döner
  digitalWrite(MotorR2, HIGH);
  analogWrite(EN1, 150); // Hız kontrolü
  digitalWrite(MotorL1, HIGH); // Sol motor geri yönde döner
  digitalWrite(MotorL2, LOW);
  analogWrite(EN2, 160); // Hız kontrolü
}

// Durdurma fonksiyonu
void dur() {
  digitalWrite(MotorR1, LOW); // Sağ motoru durdur
  digitalWrite(MotorR2, LOW);
  digitalWrite(MotorL1, LOW); // Sol motoru durdur
  digitalWrite(MotorL2, LOW);
}

void setup() {
  // Pin modlarını ayarla
  pinMode(echoPin, INPUT); // Echo pini giriş olarak ayarla
  pinMode(trigPin, OUTPUT); // Trig pini çıkış olarak ayarla
  pinMode(MotorL1, OUTPUT); // Sol motorun birinci pini çıkış olarak ayarla
  pinMode(MotorL2, OUTPUT); // Sol motorun ikinci pini çıkış olarak ayarla
  pinMode(MotorR1, OUTPUT); // Sağ motorun birinci pini çıkış olarak ayarla
  pinMode(MotorR2, OUTPUT); // Sağ motorun ikinci pini çıkış olarak ayarla
  pinMode(EN1, OUTPUT); // Sol motorun hız kontrol pini çıkış olarak ayarla
  pinMode(EN2, OUTPUT); // Sağ motorun hız kontrol pini çıkış olarak ayarla
 
  Serial1.begin(9600); // USART1'i başlat
  Serial.begin(9600); // Seri portu başlat

  // LCD ekranını başlat
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Mesafe:");
}

void loop() {
  // Ultrasonik sensör ile mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  uzaklik = sure / 29.1 / 2;
  Serial.print("Uzaklık: ");
  Serial.print(uzaklik);
  
  // LCD ekranına mesafeyi yazdır
  lcd.setCursor(9, 0);
  lcd.print(uzaklik);
  lcd.print(" cm    ");

  // Eğer engel 15 cm'den kısa mesafedeyse geri git
  if (uzaklik < 15) {
    geri();
    delay(1000); // 1 saniye geri git
    dur(); // Durdur
  }

  // Bluetooth verisini kontrol et
  if (Serial1.available()) {
    data = Serial1.read();

    if (data == 'F') { // İleri komutu
      ileri();
    } 
    else if (data == 'B') { // Geri komutu
      geri();
    } 
    else if (data == 'L') { // Sola dönüş komutu
      sol();
    } 
    else if (data == 'R') { // Sağa dönüş komutu
      sag();
    } 
    else if (data == 'S') { // Dur komutu
      dur();
    }
  }
}
