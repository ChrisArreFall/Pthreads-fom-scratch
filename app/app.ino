#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "El Quijongo";            // Enter SSID //!Change Value
const char *password = "ThatsWhatSheSaid13"; // Enter Password //!Change Value
int num_pixels = 60; // Number of pixels in the strip
String response; // String that hold the http GET answer
/**Declare our NeoPixel strip object:
 * &Argument 1 = Number of pixels in NeoPixel strip
 * &Argument 2 = Arduino pin number (most are valid)
 * &Argument 3 = Pixel type flags, add together as needed:
 */
Adafruit_NeoPixel pixels(num_pixels, D6, NEO_GRB | NEO_KHZ800);
// Capacity to allocate the JSON document in heap
const size_t capacity = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) +
                        3 * JSON_OBJECT_SIZE(4) + 3 * JSON_OBJECT_SIZE(6) + 715;
DynamicJsonDocument doc(capacity); // Alloates memory in the heap

struct myJSON
{
  int position[3];
  int priority[3];
  int band;
};

///////////////////////////// Methods Implementation //////////////////////////////

/**
 * parseJSON: Function that parse the json in the priority and position of each package in the band
 * @param response: string with JSON format
 * @return myJSON: struct with an array with position and priority of each package and band size
 * 
 */
myJSON parseJSON(String response)
{
  // Deserialize the JSON document

  DeserializationError error = deserializeJson(doc, response);

  if (error) // Test if parsing succeeds.
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    struct myJSON empty
    {
      0, 0, 0
    };
    return empty;
  }

  struct myJSON packages;

  int band = doc[0]["tamano"]; // Gets the size of the band

  // Check if the band is bigger than 10
  if (band < 15)
  {
    packages.band = band; // Use the size of smaller than 15
  }
  else
  {
    packages.band = 15; // Max size of the band in the led strip = 15 leds
  }

  Serial.print("Banda ");
  Serial.println(packages.band); //!Debug

  packages.position[0] = floor((packages.band / band) * int(doc[0]["paquete"]["pos"])); // Normalize the band and then the position
  packages.position[1] = floor((packages.band / band) * int(doc[1]["paquete"]["pos"])); // Normalize the band and then the position;
  packages.position[2] = floor((packages.band / band) * int(doc[2]["paquete"]["pos"])); // Normalize the band and then the position;

  packages.priority[0] = int(doc[0]["paquete"]["tipo"]); // Normalize the band and then the position;
  packages.priority[1] = int(doc[1]["paquete"]["tipo"]); // Normalize the band and then the position;;
  packages.priority[2] = int(doc[2]["paquete"]["tipo"]); // Normalize the band and then the position;;

  // Prints to test the results
  Serial.print("Paquete 0: posicion "); //!Debug
  Serial.print(packages.position[0]);   //!Debug
  Serial.print(", prioridad ");         //!Debug
  Serial.println(packages.priority[0]); //!Debug

  Serial.print("Paquete 1: posicion "); //!Debug
  Serial.print(packages.position[1]);   //!Debug
  Serial.print(", prioridad ");         //!Debug
  Serial.println(packages.priority[1]); //!Debug

  Serial.print("Paquete 2: posicion "); //!Debug
  Serial.print(packages.position[2]);   //!Debug
  Serial.print(", prioridad ");         //!Debug
  Serial.println(packages.priority[2]); //!Debug
  return packages;
}

/**
 * setPixelsColor: Function that sets the color of each pixel in the strip
 * @param package: myJSON struct with the position and priority of each package in the band
 */
void setPixelsColor(myJSON packages)
{
  if (packages.band == 0) // If the json is "empty" (failed to deserialize)
  {
    Serial.println("JSON empty");
    return; // Do not refresh the leds
  }

  uint32_t color; // Color in number

  // For that sets the color of all the leds
  for (int i = 0; i < num_pixels; i++)
  {
    // See if a package is in the position
    if (packages.position[0] == i)
    {
      color = getPackageColor(packages.priority[0]);
    }
    else if (packages.position[1] == i - packages.band - 5) // Offset of 5 between the bands (accumulative)
    {
      color = getPackageColor(packages.priority[1]);
    }
    else if (packages.position[2] == i - 2 * packages.band - 10) // Offset of 5 between the bands (accumulative)
    {
      color = getPackageColor(packages.priority[2]);
    }
    else // Turn off the led
    {
      color = pixels.Color(0, 0, 0);
    }

    pixels.setPixelColor(i, color);
  }

  // Sets frotier leds white
  pixels.setPixelColor(packages.band,
                       pixels.Color(100, 100, 100)); // Last led if band 0
  pixels.setPixelColor(packages.band + 4,
                       pixels.Color(100, 100, 100)); // First led of band 1
  pixels.setPixelColor(2 * packages.band + 5,
                       pixels.Color(100, 100, 100)); // Last led of band 1
  pixels.setPixelColor(2 * packages.band + 9,
                       pixels.Color(100, 100, 100)); // First led of band 2
  pixels.setPixelColor(3 * packages.band + 10,
                       pixels.Color(100, 100, 100)); // Last led of band 2

  pixels.show(); // Refresh the strip
}

/**
 * getPackageColor: Function that returns a color (in a number) according to the priority of the package
 * @return: color formated as a number
 */
uint32_t getPackageColor(int priority)
{
  uint32_t color; // Color in number

  switch (priority) // Sets the color according to the priority
  {
  case 0:
    color = pixels.Color(100, 0, 0); // Red
    break;

  case 1:
    color = pixels.Color(100, 100, 0); // Yellow
    break;

  case 2:
    color = pixels.Color(0, 100, 0); // Green
    break;
  default:
    color = pixels.Color(0, 0, 0); // No color (no priority)
  }

  return color;
}

/**
 * setup: Function that initialize the necessary parameters/functions for the arduino
 */
void setup()
{
  Serial.begin(115200); // Sets the baud rate

  WiFi.begin(ssid, password); // Connects to the wifi
  Serial.print("Connecting"); // Print in Serial for status

  // Wait for the wifi to connect
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);       // Wait 1s to try again
    Serial.print("."); // Print in Serial for status
  }

  Serial.println();
  Serial.println("Setup!"); // Print in Serial for status

  pixels.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();             // Turn OFF all pixels ASAP
  pixels.setBrightness(100); // Dim the brightness
}

/**
 * loop: Function thats always executing after the setup in the arduino
 */
void loop()
{
  HTTPClient http;                   // Declare an object of class HTTPClient
  response = "";                     // Resets the response message
  http.begin("192.168.100.8", 4001); // Establishes a connection
  int httpCode = http.GET();         // Send the request

  if (httpCode > 0) // Check the returning code
  {
    response = http.getString(); // Get the request response payload
    //Serial.println(response);    // Print the response payload //!Debug
  }

  http.end(); //Close connection

  myJSON packages = parseJSON(response); // Deserialize the JSON
  setPixelsColor(packages);              // Turn on/off the leds where the package is

  delay(100); // Waits 100ms
}