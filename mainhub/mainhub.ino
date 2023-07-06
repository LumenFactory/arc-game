/*
This is the code for the brain of the project. If controls the game and is
responsible for controlling lights etc.
*/

#include <FastLED.h>

///////////////////////////////////////
// Hardware definitions
///////////////////////////////////////

#define PIN_LED 13
#define NUM_LEDS 23


#define DATA_PIN 5


///////////////////////////////////////
// Timing Variables
///////////////////////////////////////

uint32_t last_draw = 0;
uint32_t wait_draw = 10;

uint32_t last_update = 0;
uint32_t wait_update = 200; // This is the number of MS between frames. e.g. 500 = 2 fps

uint32_t last_buttoncheck = 0;
uint32_t wait_buttoncheck = 50;

///////////////////////////////////////
// Game Logic
///////////////////////////////////////

bool is_button_pressed = false;

int cursor_index = 0;
int cursor_index_max = NUM_LEDS;

int target_index = NUM_LEDS / 2;
int target_width = 2;
int min_index = target_index - target_width;
int max_index = target_index + target_width;
int direction = 1;
int currentLevel = 0;    // Current level
int numLevels = 7;       // Number of levels
// Define the rainbow colors in GRB format
uint32_t rainbowColors[] = {
  0xFF0000, // Red
  0xFF7F00, // Orange
  0xFFFF00, // Yellow
  0x00FF00, // Green
  0x0000FF, // Blue
  0x4B0082, // Indigo
  0x8B00FF  // Violet
};

//Define the array of LEDs
CRGB leds[NUM_LEDS];

void setup()
{
  
  Serial.begin(115200);
  delay(1000); // Put a small delay to ensure everything is setup properly
  Serial.println("Starting ArcGame...");
  delay(1000);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(25);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH); // Turn the built-in led on to show there is power

  // Additional Setup steps go here

  Serial.println("Init Complete...");
  delay(1000);
}

void check_button_state()
{

  // Create a fake button press for testing. This looks for data in the SerialMonitor then simulates
  // a button press
  if (Serial.available() > 0)
  {
    int incomingByte = Serial.read();
    is_button_pressed = !is_button_pressed; // Toggle the button pressed
  }
}

void draw()
{

  // Turn the built-in LED off when the button is pressed
  if (is_button_pressed)
  {
    digitalWrite(PIN_LED, LOW);
  }
  else
  {
    digitalWrite(PIN_LED, HIGH);
  }
}

void update_game()
{
  cursor_index += direction; // Move to the next LED based on the direction

  // Check if the LED reached the end of the strip
  if (cursor_index >= NUM_LEDS) {
    cursor_index = NUM_LEDS - 2; // Move to the second-to-last LED
    direction = -1; // Change the direction to left
  } else if (cursor_index < 0) {
    cursor_index = 1; // Move to the second LED
    direction = 1; // Change the direction to right
  }
  //LEDs
  //Light the cursor
  leds[cursor_index] = rainbowColors[currentLevel];
  FastLED.show();
  // Now turn the LED off, then pause
  leds[cursor_index] = CRGB::Black;
  FastLED.show();

   //Light the target range
  leds[min_index] = CRGB::White;
  FastLED.show();
  leds[max_index] = CRGB::White;
  FastLED.show();
  
  Serial.print("Cursor Value: ");
  Serial.println(cursor_index);

}

void game_logic()
//WIN CASE
{   if (is_button_pressed) {
    if (cursor_index >= min_index && cursor_index <= max_index) {
        // Game won!
      win_animation(2);
      Serial.println("Game won!");
      delay(1000);
      // Update the game logic
      cursor_index = 0;
      wait_update = wait_update/2;
      currentLevel++; 
      if (currentLevel >= numLevels) {
          currentLevel = 0; // Wrap around to the first level if all levels are completed
        }
      // Print message
      Serial.print("Next level! Speed: ");
        Serial.println(wait_update);
      Serial.print("Target Range: ");
        Serial.println(target_width);
      Serial.print("Level: ");
        Serial.print(currentLevel);
        Serial.print(", Color: 0x");
        Serial.println(rainbowColors[currentLevel], HEX);

      //Run win animation

      delay(1000);
      } 
      //LOSS CASE
      else {
        // Game lost!
      Serial.println("Game lost! Back to level 1"); 

      //Run loss animation
      loss_animation(2);

      delay(1000);
      //Update game logic
      cursor_index = 0;
      wait_update = 100;
      currentLevel = 0;


      }
    }
}

void win_animation(int numFlashes)
{
  for (int i = 0; i < numFlashes; i++) {
    fill_solid(leds, NUM_LEDS, 0xFFFFFF); // Set all LEDs to white
    FastLED.show();
    delay(200);

    fill_solid(leds, NUM_LEDS, 0x000000); // Turn off all LEDs
    FastLED.show();
    delay(200);
  }
}

void loss_animation(int numFlashes)
{
  for (int i = 0; i < numFlashes; i++) {
    fill_solid(leds, NUM_LEDS, 0xFF0000); // Set all LEDs to white
    FastLED.show();
    delay(200);

    fill_solid(leds, NUM_LEDS, 0x000000); // Set all LEDs to white
    FastLED.show();
    delay(200);

  }
}

void loop()
{
  // put your main code here, to run repeatedly:

  // Update the ButtonState every N Milliseconds
  if (last_buttoncheck + wait_buttoncheck < millis())
  {
    check_button_state();
    last_buttoncheck = millis();
  }

  // Update the GameState every N Milliseconds
  if (last_update + wait_update < millis())
  {
    update_game();
    last_update = millis();
  }

  // Update the GameState every N Milliseconds
  if (last_draw + wait_draw < millis())
  {
    draw();
    last_draw = millis();
  }
  game_logic();
}
