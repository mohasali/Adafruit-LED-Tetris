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

    Shape(int startX, int startY, int red, int green, int blue, int m[4][3][3]) 
        : x(startX), y(startY), r(red), g(green), b(blue){
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
};

Shape test1 = Shape(6,18,225,65,175,LShape);
Shape test2 = Shape(5,14,225,65,175,JShape);
Shape test3 = Shape(5,10,225,65,175,SShape);
Shape test4 = Shape(5,6,225,65,175,ZShape);
Shape test5 = Shape(5,2,225,65,175,TShape);


void setup() {
  strip.begin();  // Initialize the NeoPixel strip
  strip.show();
  Serial.begin(9600);
}


void loop(){
  test1.draw(strip);
  
  delay(800);
  strip.clear();

  test1.move_right();
}