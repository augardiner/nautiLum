int stdFlashTime = 700;               // standard flash high time
int groupFlashTime = 450;             // group flash high time

// buoy class declaration
class Buoy {
  public:
    // all buoy attributes
    int outputPin;                    // output pin
    int state = 0;                        // current state of pin
    int period;                       // time of total flash cycle
    int highInterval = stdFlashTime;  // how long led is on for
    int lowInterval;                  // how long led is off for
    int previousMillis;               // time since last state change
    
    // group flash attributes
    boolean isGroup;                  // is a group flash?
    int group1;                       // number of flashes in group 1
    int group2;                       // number of flashes in group 2
    int periodRemainTime;             // time remaining in period, after flashes
    int numFlash_1 = 0;               // number of times group 1 has flashed 
    int numFlash_2 = 0;               // number of times group 2 has flashed 

    // constructor
    Buoy(int pin, int buoyType, int _period, int _group1=0, int _group2=0) {
      outputPin = pin;
      period = _period;
      // set up intervals for each buoy type
      switch (buoyType){
        // fixed
        case 0: 
          lowInterval = period;
          break;
        // flashing
        case 1:         
          lowInterval = period - highInterval;
          break;
        // occulting
        case 2:
          lowInterval = stdFlashTime;
          highInterval = period - lowInterval;
          break;
        // quick flash or very quick flash or iso
        case 3:
          lowInterval = highInterval = period;
          break;
        // group flash
        case 4:
          isGroup = true;
          group1 = _group1;
          group2 = _group2;
          periodRemainTime = period - (2 * (group1+group2) * groupFlashTime);
          if (group2 != 0) {
            periodRemainTime += groupFlashTime;
          }
          break;                
      }
    }

    void flash(int currentMillis) {
      // not group flash
      if (!isGroup) {
        Serial.println(outputPin);
        Serial.println(state);
        // always on
        if (lowInterval == 0) {
          digitalWrite(outputPin, HIGH);
        } 
        // flashing
        else {
          // if led is currently off
          if (state == 0) {
            if (currentMillis - previousMillis >= lowInterval) {
              state = 1;
              digitalWrite(outputPin, HIGH);
              previousMillis = currentMillis; 
            }
          } 
          // if led is currently on
          else {
              if (currentMillis - previousMillis >= highInterval) {
                state = 0;
                digitalWrite(outputPin, LOW);
                previousMillis = currentMillis; 
              }
          }
        }
      }
      // group flash
      else {
        // if led is currently off
        // composite flash group 1
        if (state == 0) {
          if (numFlash_1 < group1
              && currentMillis - previousMillis >= groupFlashTime) {
              digitalWrite(outputPin, HIGH);
              state = 1;
              numFlash_1++;
              previousMillis = currentMillis;
          }
          // composite flash group 2 (optional)
          else if (group2 > 0
                  && currentMillis - previousMillis >= periodRemainTime/2) {
            if (numFlash_2 < group2) {
              state = 1;
              digitalWrite(outputPin, HIGH);
              numFlash_2++;
              previousMillis = currentMillis;
            }
            else {
              numFlash_1 = 0;
              numFlash_2 = 0;
              previousMillis = currentMillis;
            }
          }
          // no group 2
          else if (currentMillis - previousMillis >= periodRemainTime) {
            numFlash_1 = 0;
            previousMillis = currentMillis;
          }
        }
        // if led is currently on
        else {
          if (currentMillis - previousMillis >= groupFlashTime) {
            state = 0;
            digitalWrite(outputPin, LOW);
            previousMillis = currentMillis; 
          }          
        }
      }
    }
};

// instantiate each buoy type

// buoyType   value
// F          0
// Fl         1
// Occ        2
// Q          3
// Fl(x)      4

// instanceName(pin, buoyType, period, group1=0, group2=0)
Buoy led0(2, 0, 0);           // F
Buoy led1(3, 1, 2500);        // Fl 2.5
Buoy led2(4, 2, 4000);        // Occ 4
Buoy led3(5, 3, 500);         // Q
Buoy led4(6, 4, 6000, 3);     // Fl(3) 6s
Buoy led5(7, 4, 6000, 2, 1);  // Fl(2+1) 6s

void setup() {
  // output pins for shift register
  for (int i = 2; i < 8; i++)
  {
    pinMode(i, OUTPUT);
  }

  // open serial port
  Serial.begin(9600);
  Serial.println("reset");
}

void loop() {
  
  // get time since we started
  int currentMillis = millis();
  
  // load register array with state for each buoy
  led0.flash(currentMillis);
  led1.flash(currentMillis);
  led2.flash(currentMillis);
  led3.flash(currentMillis);
  led4.flash(currentMillis);
  led5.flash(currentMillis);
  
}