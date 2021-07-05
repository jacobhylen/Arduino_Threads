/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <ThreadsafeIO.h>

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static int     const BMP388_CS_PIN  = 2;
static int     const BMP388_INT_PIN = 6;
static uint8_t const BMP388_CHIP_ID_REG_ADDR = 0x00;

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

void bmp388_select();
void bmp388_deselect();

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

SpiBusDevice bmp388{"SPI",
                    SpiBusDeviceConfig {
                     SPISettings{1000000, MSBFIRST, SPI_MODE0},
                     /* bmp388_select   or ... */ [](){ digitalWrite(BMP388_CS_PIN, LOW ); },
                     /* bmp388_deselect or ... */ [](){ digitalWrite(BMP388_CS_PIN, HIGH); }
                    }
                   };

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(9600);
  while (!Serial) { }

  pinMode(BMP388_CS_PIN, OUTPUT);
  digitalWrite(BMP388_CS_PIN, HIGH);

  auto bmp388_read_reg = [](uint8_t const reg_addr) -> uint8_t
  {
    uint8_t const tx_buf[3] = {static_cast<uint8_t>(0x80 | reg_addr), 0, 0};
    uint8_t rx_buf[3] = {0};
    
    SpiIoRequest req(tx_buf, sizeof(tx_buf), rx_buf, sizeof(rx_buf));

    TSharedIoResponse rsp = bmp388.transfer(req);

    /* Do other stuff */

    rsp->wait();
    uint8_t const reg_val = rsp->read_buf[2];

    Serial.print(rsp->bytes_written);
    Serial.print("  bytes written, ");
    Serial.print(rsp->bytes_read);
    Serial.println(" bytes read");

    return reg_val;
  };

  uint8_t const chip_id = bmp388_read_reg(BMP388_CHIP_ID_REG_ADDR);
  Serial.print("BMP388 CHIP ID = 0x");
  Serial.println(chip_id, HEX);
}

void loop()
{

}

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void bmp388_select()
{
  digitalWrite(BMP388_CS_PIN, LOW);
}

void bmp388_deselect()
{
  digitalWrite(BMP388_CS_PIN, HIGH);
}
