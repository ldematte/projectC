#include "output.hs"
#include "string.hs"

class Instrument 
{
public:
  virtual void play(int note) 
  {
    print(stradd("Instrument::play ", (string)note));
  }
};

// Wind objects are Instruments
// because they have the same interface:
class Wind : Instrument 
{
public:
  // Redefine interface function:
  void play(int note)
  {
    print(stradd("Wind::play ", (string)note));
  }
};

// Wind objects are Instruments
// because they have the same interface:
class Flute : Wind 
{
public:
  // Redefine interface function:
  void play(int note)
  {
    print(stradd("Flute::play ", (string)note));
  }
};

void tune(Instrument i, int note) 
{
  i.play(note);
}

int main() 
{
  Flute flute;
  tune(flute, 10); // Upcasting
  
  return 0;
}


