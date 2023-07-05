/*
This is the code for the brain of the project. If controls the game and is
responsible for controlling lights etc.
*/

///////////////////////////////////////
// Hardware definitions
///////////////////////////////////////

#define PIN_LED 13
#define NUM_LEDS 176

///////////////////////////////////////
// Timing Variables
///////////////////////////////////////

uint32_t last_draw = 0;
uint32_t wait_draw = 10;

uint32_t last_update = 0;
uint32_t wait_update = 100; // This is the number of MS between frames. e.g. 500 = 2 fps

uint32_t last_buttoncheck = 0;
uint32_t wait_buttoncheck = 50;

///////////////////////////////////////
// Game Logic
///////////////////////////////////////

bool is_button_pressed = false;

int cursor_index = 0;
int cursor_index_max = NUM_LEDS;

int target_index = NUM_LEDS / 2;
int target_width = 10;
int min_index = target_index - target_width;
int max_index = target_index + target_width;
int direction = 1;

void setup()
{

  Serial.begin(115200);
  delay(1000); // Put a small delay to ensure everything is setup properly
  Serial.println("Starting ArcGame...");
  delay(1000);

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
  Serial.print("Cursor Value: ");
  Serial.println(cursor_index);

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
  if (is_button_pressed) {
    if (cursor_index >= min_index && cursor_index <= max_index) {
        // Game won!
      Serial.println("Game won!");
      delay(1000);
      cursor_index = 0;
      target_width -= 1;
      wait_update = wait_update/2;
      Serial.print("Next level! Speed: ");
      Serial.println(wait_update);
      Serial.print("Target Range: ");
      Serial.println(target_width);

      delay(1000);
      } else {
        // Game lost!
      Serial.println("Game lost! Back to level 1"); 
      delay(1000);
      cursor_index = 0;
      wait_update = 100;
      target_width = 10;
      }
    }
  
}
