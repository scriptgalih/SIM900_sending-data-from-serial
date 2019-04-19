/*
  Galih Setyawan
  30 Des 2018
*/
#define length 20

//mustaqim
//#define tinggi 18
//#define lebar 6

#define tinggi 16
#define lebar 7
float alpha = 0.75;
double LPFSensor, lastLPFSensor;
int rawSensor, afterFilter;

int temp[length];
int varCycleDetector[tinggi];
int temp_bpm[15];
bool statusSiklus, statusSiklusSebelumnya;

int beat, bpm;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
}

void loop() {

  // Membaca data analog mentah
  rawSensor = analogRead(A0);

  //Low Pas Filter
  LPFSensor  = alpha * rawSensor + (1 - alpha) * lastLPFSensor;
  lastLPFSensor = LPFSensor;


  //Moving Average
  afterFilter = movAvg(LPFSensor);

  //pendeteksi awal siklus
  int awalSiklus = deteksiAwalSiklus(LPFSensor);


  //pendeteksi RISING
  if (statusSiklus == true && statusSiklusSebelumnya == false) {
    beat ++;
  }
  statusSiklusSebelumnya = statusSiklus;

  //perhitungan
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      int sum;
      for (int z = 14; z >= 1; z--) {
        temp_bpm[z] = temp_bpm[z - 1];
   //      Serial.print(temp_bpm[z]); Serial.print("\t");
        sum += temp_bpm[z];
      }
      temp_bpm[0] = beat;
      sum += beat;
    //      Serial.print(sum);
    //    Serial.println();
    bpm = sum*4;
      beat = 0;
    }

//  unsigned long currentMillis = millis();
//  if (currentMillis - previousMillis >= interval) {
//    previousMillis = currentMillis;
//    bpm = beat * 4;
//    beat = 0;
//  }

  //Print to monitor
      Serial.print(rawSensor);
  //    Serial.print("\t");
//    Serial.print(LPFSensor);
  //  //  Serial.print("\t");
  //  Serial.print(afterFilter);
  //  Serial.print("\t");
  //  Serial.print(awalSiklus);
  Serial.print("\t");
  Serial.print(beat);
  Serial.print("\t");
  Serial.print(bpm);



  Serial.println();
  delay(5);
}

int movAvg(int in) {
  double sum = 0;
  for (int i = length - 1; i >= 1; i--) {
    temp[i] = temp[i - 1];
    sum += temp[i];
  }
  temp[0] = in;
  sum += in;
  return (sum / length);
}

int deteksiAwalSiklus(int in) {
  for (int i = tinggi - 1; i >= 1; i--) {
    varCycleDetector[i] = varCycleDetector[i - 1];
//    Serial.print(varCycleDetector[i]); Serial.print("\t");
  }
//  Serial.println();
  varCycleDetector[0] = in;

  if (varCycleDetector[0] - varCycleDetector[tinggi-1] >= lebar) {
//    Serial.println(varCycleDetector[0] - varCycleDetector[tinggi-1]);
    statusSiklus = true;
    return 690;
  } else {
    statusSiklus = false;
    return 680;
  }
}

