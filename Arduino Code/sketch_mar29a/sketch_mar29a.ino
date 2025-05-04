#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

#define PIN            12  // Define the pin to which the NeoPixel data line is connected
#define NUMPIXELS      320 // Define the number of pixels in the NeoPixel chain

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int LShape[4][3][3] = {
  {
      {0, 0, 0},
      {1, 1, 1},
      {0, 0, 1}
  },
  {
      {0, 1, 0},
      {0, 1, 0},
      {1, 1, 0}
  },
  {
      {1, 0, 0},
      {1, 1, 1},
      {0, 0, 0}
  },
  {
      {0, 1, 1},
      {0, 1, 0},
      {0, 1, 0}
  }
};
int TShape[4][3][3] = {
    {
        {0, 1, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 1, 0},
        {0, 1, 1},
        {0, 1, 0}
    },
    {
        {0, 0, 0},
        {1, 1, 1},
        {0, 1, 0}
    },
    {
        {0, 1, 0},
        {1, 1, 0},
        {0, 1, 0}
    }
};
int OShape[4][3][3] = {
    {
        {0, 1, 1},
        {0, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 1, 1},
        {0, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 1, 1},
        {0, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 1, 1},
        {0, 1, 1},
        {0, 0, 0}
    }
};
int ZShape[4][3][3] = {
    {
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 0, 1},
        {0, 1, 1},
        {0, 1, 0}
    },
    {
        {0, 0, 0},
        {1, 1, 0},
        {0, 1, 1}
    },
    {
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0}
    }
};
int SShape[4][3][3] = {
    {
        {0, 1, 1},
        {1, 1, 0},
        {0, 0, 0}
    },
    {
        {0, 1, 0},
        {0, 1, 1},
        {0, 0, 1}
    },
    {
        {0, 0, 0},
        {0, 1, 1},
        {1, 1, 0}
    },
    {
        {1, 0, 0},
        {1, 1, 0},
        {0, 1, 0}
    }
};
int JShape[4][3][3] = {
    {
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 1, 1},
        {0, 1, 0},
        {0, 1, 0}
    },
    {
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 1}
    },
    {
        {0, 1, 0},
        {0, 1, 0},
        {1, 1, 0}
    }
};

struct Shape {
public:
    int matrix[4][3][3];
    int x;
    int y;
    int r, g, b;
    int rotationIndex = 0;

    Shape(int startX, int startY, int red, int green, int blue, int m[4][3][3],int rotation) 
        : x(startX), y(startY), r(red), g(green), b(blue),rotationIndex(rotation){
          for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    matrix[i][j][k] = m[i][j][k];
    }

    virtual void rotateClockwise() {
        rotationIndex+=1;
        rotationIndex = rotationIndex%4;
        Serial.println(rotationIndex);
    }
    virtual void rotateAntiClockwise() {
        rotationIndex-=1;
        if(rotationIndex<0) rotationIndex=3;
        Serial.println(rotationIndex);
    }

    uint16_t remap(uint16_t x, uint16_t y) {
      if (x <= 6) 
      {
        if(x%2)
        {
          //Odd rows, They go up in counting
          return (x-1)*40 + (y*2)-1;
        }
        else
        {
          //Positive rows, They go down in counting
          return x*40 - (y*2);
        }
      } 
      else 
      {
        if(x%2)
        {
          return 240 + (x-7)*20 + y-1;
        }
        else {
        {
          return 240 + (x-6)*20 - (y);
        }
        }
      }
    }

    void draw(Adafruit_NeoPixel& strip){
      for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
          if(matrix[rotationIndex][i][j]==1){
            int xPos = x + i;
            int yPos = y + j;
            strip.setPixelColor(remap(xPos, yPos), r, g, b);
          }
        }
        strip.show();
        }
    }    

    void move_right()
    {
      if(x < 8)
      {
        x += 1;
      }
    }

    void move_left()
    {
      if(x > 0)
      {
        x -= 1;
      }
    }

    void move_down()
    {
      if(y > 1)
      {
        y -= 1;
      }
    }
    bool isBottom(){
      return y<=1;
    }
};
void setup() {
  strip.begin();  // Initialize the NeoPixel strip
  strip.show();
  Serial.begin(9600);
}
  Shape test1 = SpawnShape();


void loop(){
  if(test1.isBottom()){
    test1 = SpawnShape();
  }
  test1.draw(strip);
  strip.clear();
  MoveShape(test1);
  delay(200);
  strip.clear();
  test1.move_down();

}

Shape SpawnShape(){
  int shapeMatrix = random(0,6);
  int r = random(0,180);
  int g = random(0,180-r);
  int b = 180-r-g;
  int rotation = random(0,4);
  switch (shapeMatrix){
    case 0: return Shape(5,18,r,g,b,LShape,rotation);
    case 1: return Shape(5,18,r,g,b,JShape,rotation);
    case 2: return Shape(5,18,r,g,b,SShape,rotation);
    case 3: return Shape(5,18,r,g,b,ZShape,rotation);
    case 4: return Shape(5,18,r,g,b,TShape,rotation);
    case 5: return Shape(5,18,r,g,b,OShape,rotation);
  }
}

void MoveShape(Shape &shape){
  int joyX = analogRead(A0);
  int joyY = analogRead(A1);

    if(joyX > 600){
      shape.move_right();
    }
    else if(joyX < 300){
      shape.move_left();
    }
    if(joyY < 300){
      shape.move_down();
    }

}
/*TO DO:
  
*/