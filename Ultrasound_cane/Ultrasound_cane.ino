#define TRIG1  4   /* Триггер 1-го датчика */
#define ECHO1  3   /* Сигнал с 1-го датчика */
#define PULSE1 A3    /* Мотор №1 */
#define LED 13

#define pulse_ms 100  /* Длительность импульса мотора (мс) */

#define L1 15  
#define L2 50
#define L3 100

int Level1 = 0;

int i = 0;

int t1st;
int dist1 = 0;
int count1 = 0;
int low1 = 0;
int pulse1_stp = 0;
 
void setup()
{
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(PULSE1, OUTPUT);
  pinMode(2, OUTPUT); // pin for buzzer
  
  Timer1_Init(32, 250);	 
}

void Timer1_Init(int prescaler1, int prescaler2)
{
  TCCR1A = 0;
  switch (prescaler1)
  {
    case (0):     TCCR1B = 0<<CS12 | 0<<CS11 | 0<<CS10; break;   // Timer stopped
    case (1):     TCCR1B = 0<<CS12 | 0<<CS11 | 1<<CS10; break;   // CLK
    case (8):     TCCR1B = 0<<CS12 | 1<<CS11 | 0<<CS10; break;   // CLK/8
    case (32):    TCCR1B = 0<<CS12 | 1<<CS11 | 1<<CS10; break;   // CLK/32
    case (64):    TCCR1B = 1<<CS12 | 0<<CS11 | 0<<CS10; break;   // CLK/64
    case (128):   TCCR1B = 1<<CS12 | 0<<CS11 | 1<<CS10; break;   // CLK/128
    case (256):   TCCR1B = 1<<CS12 | 1<<CS11 | 0<<CS10; break;   // CLK/256
    case (1024):  TCCR1B = 1<<CS12 | 1<<CS11 | 1<<CS10; break;   // CLK/1024
  }
  TIMSK1 = 1<<TOIE1;  // Overflov interrupt enabled
  t1st = 65535 - prescaler2;
  TCNT1 = t1st;       // Start count value
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = t1st;  
  count1++;
  switch(pulse1_stp)
  {
    case (0):  digitalWrite(PULSE1, LOW); digitalWrite(LED, LOW); break;
    case (1):  digitalWrite(PULSE1, HIGH); tone(2, 440, 200); digitalWrite(LED, HIGH); count1 = 0; pulse1_stp++; break;
    case (2):  (count1 >= pulse_ms) ? (digitalWrite(PULSE1, LOW), digitalWrite(LED, LOW), count1 = 0, pulse1_stp++) : (0); break;
    case (3):  (count1 >= low1) ? (pulse1_stp = 1) : (0); break;    
  }
}

int GetDist (int trig, int echo)
{
  int dist;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW); 
  dist = pulseIn(echo, HIGH)/58;     
  return(dist);
}

void GetLevel (int &dist, int &level)
{
  (dist >= 0  && dist < L1) ? (level = 1):
  (dist >= L1 && dist < L2) ? (level = 2):
  (dist >= L2 && dist < L3) ? (level = 3):  
  (dist >= L3) ? (level = 0) : (level = 0);
}

void GetInterval (int level, int &low, int &stp)
{
  switch(level)
  {
    case (1):  low = pulse_ms*1; break;
    case (2):  low = pulse_ms*4; break;
    case (3):  low = pulse_ms*7; break;    
  }
  
  if (level != 0 && stp == 0)
    stp = 1;
  else
    if (level == 0)
      stp = 0;     
}

void loop()
{  
  delay(100);    
  dist1 = GetDist(TRIG1, ECHO1);
  GetLevel(dist1, Level1);
  GetInterval(Level1, low1, pulse1_stp);
}
