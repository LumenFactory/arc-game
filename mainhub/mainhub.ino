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
uint32_t wait_update = 500; // This is the number of MS between frames. e.g. 500 = 2 fps

uint32_t last_buttoncheck = 0;
uint32_t wait_buttoncheck = 50;

///////////////////////////////////////
// Game Logic
///////////////////////////////////////

bool is_button_pressed = false;

int cursor_index = 0;
int cursor_index_max = NUM_LEDS;

int target_index = NUM_LEDS / 2;
int target_width = 5;

void setup()
{

  Serial.begin(115200);
  delay(100); // Put a small delay to ensure everything is setup properly
  Serial.println("Starting ArcGame...");

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH); // Turn the built-in led on to show there is power

  // Additional Setup steps go here

  Serial.println("Init Complete...");
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

  // Increment the cursor, but reset to zero if it is greater than the number of leds (Circle Path)
  cursor_index = cursor_index + 1;
  if (cursor_index >= NUM_LEDS)
  {
    cursor_index = 0;
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

  //  delay(10);
}
