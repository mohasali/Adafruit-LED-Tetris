#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Grove_LED_Matrix_Driver_HT16K33.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

Matrix_8x8 score_matrix;
Matrix_8x8 shape_matrix;


#define PIN 12      
#define NUMPIXELS 320

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int LShape[4][4][4] = {
  {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},
  {{0,0,1,0},{0,0,1,0},{0,0,1,1},{0,0,0,0}},
  {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
  {{0,0,0,0},{1,1,0,0},{0,1,0,0},{0,1,0,0}}
};
int TShape[4][4][4] = {
  {{0,0,0,0},{0,1,1,1},{0,0,1,0},{0,0,0,0}},
  {{0,0,1,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,0,1,0},{0,1,1,1},{0,0,0,0}},
  {{0,0,1,0},{0,0,1,1},{0,0,1,0},{0,0,0,0}}
};
int OShape[4][4][4] = { 
  {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}
};
int ZShape[4][4][4] = {
  {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
  {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}} 
};
int SShape[4][4][4] = {
  {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
  {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
  {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}
};
int JShape[4][4][4] = {
  {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
  {{0,0,1,0},{0,0,1,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
  {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}
};
int IShape[4][4][4] = {
  {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
  {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
  {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
  {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}}
};

const uint64_t SHAPE_IMAGES[] = {
  0x007e7e0606060606,
  0x007e7e6060606060,
  0x6060607e7e060606,
  0x0606067e7e606060,
  0x00007e7e18181800,
  0x007e7e7e7e7e7e00,
  0x1818181818181818
};


const int colors[8][3] = {
  {120, 0, 0},    // 1. Red
  {0, 120, 0},    // 2. Green
  {0, 0, 120},    // 3. Blue
  {120, 120, 0},    // 4. Yellow
  {0, 120, 120},    // 5. Cyan
  {120, 0,120},    // 6. Magenta
  {100, 100, 100},   // 7. White
  {120, 0, 80}     // 8. Purple
};

// Color structure
struct Color {
  int r, g, b;
  Color() : r(0), g(0), b(0) {}
  Color(int red, int green, int blue) : r(red), g(green), b(blue) {}

  bool operator==(const Color& other) const {
    return r == other.r && g == other.g && b == other.b;
  }
};

// Game board
Color gameBoard[BOARD_WIDTH][BOARD_HEIGHT];

int score = 0;

// Remap Function Prototype
uint16_t remap(uint16_t x_param, uint16_t y_param);

// Shape class
class Shape {
public:
  int matrix[4][4][4];
  int x, y; // Position of the object
  int r_color, g_color, b_color;
  int rotationIndex = 0;

  Shape(int startX, int startY, int red, int green, int blue, int m[4][4][4], int rotation)
    : x(startX), y(startY), r_color(red), g_color(green), b_color(blue), rotationIndex(rotation) {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        for (int k = 0; k < 4; ++k)
          matrix[i][j][k] = m[i][j][k];
  }

  // Default constructor for placeholder
  Shape() : x(0), y(0), r_color(0), g_color(0), b_color(0), rotationIndex(0) {
  }

  // It calls the global remap function.
  uint16_t remap_pixel(uint16_t board_x_one_based, uint16_t board_y_one_based);

  // Checks if the shape at a potential new position (testX, testY, testRotIdx) is valid
  bool isPositionValid(int testX, int testY, int testRotIdx, Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int shape_r = 0; shape_r < 4; ++shape_r) {    // row in shape's 3x3 matrix
      for (int shape_c = 0; shape_c < 4; ++shape_c) { // col in shape's 3x3 matrix
        if (matrix[testRotIdx][shape_r][shape_c] == 1) {
          int boardX = testX + shape_c;
          // y is the 'top' row for the shape on the board (e.g., 19).
          // shape_r increases downwards in shape matrix (0, 1, 2).
          // So, actual board row is y - shape_r.
          int boardY = testY - shape_r;

          // 1. Check board boundaries
          if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT) {
            return false; // Part of the shape is out of bounds
          }

          // 2. Check collision with existing blocks on the gameBoard
          if (!(gb[boardX][boardY] == Color(0, 0, 0))) {
            return false; // Collision with an existing block
          }
        }
      }
    }
    return true; // Position is valid
  }

  void rotateClockwise(Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    int newRotation = (rotationIndex + 1) % 4;
    if (isPositionValid(x, y, newRotation, gb)) 
    {
      rotationIndex = newRotation;
      Serial.print("Rotated to: "); Serial.println(rotationIndex);
    } 
    else if(isPositionValid(x + 1, y, newRotation, gb))
    {
      x += 1;
      rotationIndex = newRotation;
    }
    else if(isPositionValid(x - 1, y, newRotation, gb))
    {
      x -= 1;
      rotationIndex = newRotation;
    }
    else 
    {
      Serial.println("Rotation blocked");
      // Optional: Implement wall kicks here if desired
      // Try shifting left/right by 1 unit and re-check isPositionValid
    }
  }

  void draw(Adafruit_NeoPixel& strip_ref) {
    for (int shape_r = 0; shape_r < 4; ++shape_r) {
      for (int shape_c = 0; shape_c < 4; ++shape_c) {
        if (matrix[rotationIndex][shape_r][shape_c] == 1) {
          int boardX = x + shape_c;
          int boardY = y - shape_r;

          // Ensure the pixel is on the board before trying to draw
          if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
            // Draw the shape on the strip
            strip_ref.setPixelColor(remap_pixel(boardX + 1, boardY + 1), r_color, g_color, b_color);
          }
        }
      }
    }
  }

  void saveToBoard(Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int shape_r = 0; shape_r < 4; ++shape_r) {
      for (int shape_c = 0; shape_c < 4; ++shape_c) {
        if (matrix[rotationIndex][shape_r][shape_c] == 1) {
          int boardX = x + shape_c;
          int boardY = y - shape_r;
          if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
            gb[boardX][boardY] = Color(r_color, g_color, b_color);
          }
        }
      }
    }
  }

  void move_right(Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    if (isPositionValid(x + 1, y, rotationIndex, gb)) {
      x += 1;
    }
  }

  void move_left(Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    if (isPositionValid(x - 1, y, rotationIndex, gb)) {
      x -= 1;
    }
  }
  
  // Attempts to move the shape down. Returns true if successful, false if blocked.
  bool try_move_down(Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    if (isPositionValid(x, y - 1, rotationIndex, gb)) {
      y -= 1;
      return true; // Moved down successfully
    }
    return false; // Cannot move down (collision or bottom)
  }
};

// Global current shape
Shape currentShape;
Shape nextShape;

// Global remap function.
uint16_t remap(uint16_t x_param, uint16_t y_param) {  
  if (x_param <= 6) { 
    if (x_param % 2) {
      return (x_param - 1) * 40 + (y_param * 2) - 1; 
    } else {
      return x_param * 40 - (y_param * 2); 
    }
  } else {
    if (x_param % 2) {
      return 240 + (x_param - 7) * 20 + y_param - 1; 
    } else {
      return 240 + (x_param - 6) * 20 - y_param;
    }
  }
}

uint16_t Shape::remap_pixel(uint16_t board_x_one_based, uint16_t board_y_one_based) {
    return remap(board_x_one_based, board_y_one_based);
}


Shape SpawnShape() {
  int rotation = random(0, 4);
  int color = random(0,8);
  int shapeType = random(0,7);
  // Initial position:
  int initialX = BOARD_WIDTH / 2 - 2;
  int initialY = BOARD_HEIGHT - 1;

  Serial.print("Spawning shape type: "); Serial.println(shapeType);
  shape_matrix.writeOnePicture(SHAPE_IMAGES[shapeType]);
  shape_matrix.display();

  switch (shapeType) {
    case 0: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], LShape, rotation);
    case 1: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], JShape, rotation);
    case 2: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], SShape, rotation);
    case 3: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], ZShape, rotation);
    case 4: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], TShape, rotation);
    case 5: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], OShape, rotation);
    case 6: return Shape(initialX, initialY, colors[color][0], colors[color][1], colors[color][2], IShape, rotation);
  }
  return Shape(initialX, initialY, 100, 100, 100, LShape, 0);
}


void DrawGrid(Color gb[BOARD_WIDTH][BOARD_HEIGHT], Adafruit_NeoPixel& strip_ref) {
  for (int r_board = 0; r_board < BOARD_HEIGHT; ++r_board) {
    for (int c_board = 0; c_board < BOARD_WIDTH; ++c_board) {
      Color current_pixel_color = gb[c_board][r_board];
      if (!(current_pixel_color == Color(0, 0, 0))) { 
        strip_ref.setPixelColor(remap(c_board + 1, r_board + 1), current_pixel_color.r, current_pixel_color.g, current_pixel_color.b);
      }
    }
  }
}

void CheckLineClears(Color gb[BOARD_WIDTH][BOARD_HEIGHT]) {
    int linesClearedThisTurn = 0;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        bool lineFull = true;
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (gb[c][r] == Color(0,0,0)) {
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            linesClearedThisTurn++;
            Serial.print("Line clear at row: "); Serial.println(r);
            // Line is full, clear it and move rows above down
            for (int r_clear = r; r_clear < BOARD_HEIGHT - 1; ++r_clear) { // Shift rows down
                for (int c = 0; c < BOARD_WIDTH; ++c) {
                    gb[c][r_clear] = gb[c][r_clear + 1];
                }
            }
            // Clear the top-most line after shifting
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                gb[c][BOARD_HEIGHT - 1] = Color(0,0,0);
            }
            r--; // Re-check the current row index as it now contains the row from above
        }
    }
    if (linesClearedThisTurn > 0) {
        Serial.print(linesClearedThisTurn); Serial.println(" lines cleared!");
        score += (linesClearedThisTurn*10)*(1+((linesClearedThisTurn-1)/5));
    }
}


void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);
  Serial.println("Tetris NeoPixel Game Starting...");

  // Initialize random seed
  unsigned long seed = 0;
  seed += analogRead(A0);
  seed += analogRead(A1);
  randomSeed(seed);

  // Initialize game board to empty
  for (int r = 0; r < BOARD_HEIGHT; ++r) {
    for (int c = 0; c < BOARD_WIDTH; ++c) {
      gameBoard[c][r] = Color(0, 0, 0);
    }
  }

  nextShape = SpawnShape();
  currentShape = nextShape;
  nextShape = SpawnShape();
  Serial.println("Setup complete. Game running.");
  
  Wire.begin();
  score_matrix.init(0x70);
  score_matrix.setBrightness(10);
  score_matrix.setBlinkRate(BLINK_OFF);
  
  shape_matrix.init(0x72);
  shape_matrix.setBrightness(10);
  shape_matrix.setBlinkRate(BLINK_OFF);}

unsigned long lastFallTime = 0;
unsigned long lastMoveTime = 0;
unsigned long lastDebounceTime = 0;
unsigned long moveInterval = 100; // Debounce for horizontal moves/rotations
unsigned long debounceDelay = 300;
bool gameOver = false;
bool pause = false;

void loop() {
  int joyX = analogRead(A0);
  int joyY = analogRead(A1); 

  if (gameOver) {
    if(joyX>=1020){
      for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
          gameBoard[c][r] = Color(0, 0, 0);
        }
      }
      strip.clear();
      score=0;
      gameOver=false;
    }
    strip.clear();
    for(int i=0; i<NUMPIXELS; i++) {
        strip.setPixelColor(i, 60,1,1);
    }
    strip.show();
    delay(500);
    strip.clear();
    strip.show();
    delay(500);
    return;
  }

  unsigned long currentTime = millis();

  // --- Handle Pause ---

  if((currentTime -debounceDelay) > lastDebounceTime){
      if(joyX >= 1020){
        pause = ! pause;
        lastDebounceTime = currentTime;
        Serial.println(pause);
      }
  }

  if(pause){
    return;
  }
  // --- Handle Input ---

  if (currentTime - lastMoveTime > moveInterval) {
    bool moved = false;
    if (joyX > 600 && joyX < 900) {
      currentShape.move_right(gameBoard);
      moved = true;
    } else if (joyX < 400) {
      currentShape.move_left(gameBoard);
      moved = true;
    }
    if (joyY > 600) {
      currentShape.rotateClockwise(gameBoard);
            moved = true;
  }
    if (moved) {
        lastMoveTime = currentTime;
    }

  }

  

  // --- Game Logic: Piece Falling ---
  float currentFallInterval =  map(score,0,200,200,50);
  if (joyY < 400) {
    currentFallInterval *=0.6;
  }

  if (currentTime - lastFallTime > currentFallInterval) {
    if (currentShape.try_move_down(gameBoard)) {
    } else {
      currentShape.saveToBoard(gameBoard);
      CheckLineClears(gameBoard);

      currentShape = nextShape;
      nextShape = SpawnShape();

      // Check for game over: if new piece is not valid in its spawn position
      if (!currentShape.isPositionValid(currentShape.x, currentShape.y, currentShape.rotationIndex, gameBoard)) {
        Serial.println("GAME OVER!");
        gameOver = true; 
        // Display game over message, freeze game, etc.
      }
    }
    lastFallTime = currentTime;
  }

  // --- Drawing ---
  strip.clear();
  currentShape.draw(strip);
  DrawGrid(gameBoard, strip);
  strip.show();
  score_matrix.writeNumber(score, 300);
  score_matrix.display();
}
