/*
    ---6--
   |      |
   5      1
   |      |
    --7---
   |      |
   4      2
   |      |
   ----3---
*/

// segmenty       1  2  3 4 5 6 7
byte Lcislo[7] = {11,10,9,5,6,7,8};

// segmenty       1 2 3 4  5  6  7
byte Pcislo[7] = {4,3,2,12,0,15,13};

int cislo = 0;
int countLoop = 0;
int beep = 13;
boolean status = true;

void setup()
{
  pinMode(beep, OUTPUT);
  nastavvystup(Lcislo);
  //nastavvystup(Pcislo);
}

void nastavvystup(byte piny[])
{
  for(int i = 0; i < 7; i++)
  {
    pinMode(piny[i], OUTPUT);
  }
}

void loop()
{
  if (countLoop < 2)
  {
    delay(400);
    
    zobrazcislo(Lcislo, cislo);
    cislo++;
    if (cislo == 10) {
      cislo = 0;
      countLoop++;
    }
    
    digitalWrite(beep, HIGH); 
    delay(50);               
    digitalWrite(beep, LOW);
  } 
  if (countLoop == 2 && status == true) {
    zobrazcislo(Lcislo, 0);
    digitalWrite(beep, HIGH); 
    delay(500);               
    digitalWrite(beep, LOW);
    status = false;
  }
}

void zobrazcislo(byte piny[], int hodnota)
{
  byte datasegmentu[11][7] = {
  {1,1,1,1,1,1,0},  //nula        index 0
  {1,1,0,0,0,0,0},  //jedna       index 1
  {1,0,1,1,0,1,1},  //dva         index 2
  {1,1,1,0,0,1,1},  //tri         index 3
  {1,1,0,0,1,0,1},  //ctyri       index 4
  {0,1,1,0,1,1,1},  //pet         index 5
  {0,1,1,1,1,1,1},  //sest        index 6
  {1,1,0,0,0,1,0},  //sedm        index 7
  {1,1,1,1,1,1,1},  //osm         index 8
  {1,1,0,0,1,1,1},  //devet       index 9
  {0,0,0,0,0,0,0},  //nic         index 10
  };

  for(int i = 0; i < 7; i++)
  {
    digitalWrite(piny[i], !datasegmentu[hodnota][i]);      
  }
}
