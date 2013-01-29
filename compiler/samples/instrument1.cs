#include "output.hs"

class Instrument 
{
public:
  void play() 
  {
    print("Instrument::play");
  }
};

// Wind objects are Instruments
// because they have the same interface:
class Wind : Instrument 
{
public:
  // Redefine interface function:
  void play()
  {
    print("Wind::play");
  }
};

void tune(Instrument & i) 
{
  i.play();
}

int main() 
{
  Wind flute;
  tune(flute); // Upcasting
  
  return 0;
}


