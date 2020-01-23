


void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int k=0;
  //offset=0;
  int inside;
  while(1)
  {
    /*inside=(((float)(k % 256))/255) *3.14;
     analogWrite(4,255*abs(cos(inside+PI/3)*cos(inside)));
     analogWrite(3,255*abs(cos(inside+PI/3)*sin(inside)));
     analogWrite(5, 255*abs(cos(inside+PI/3)));*/
    k = k % (3*256);
    if (k>=0 && k< 256)
    {
      //y=255-x
      analogWrite(6,k);
      analogWrite(5,255-k);
      analogWrite(3,255);
      
      analogWrite(9,k);
      analogWrite(10,255-k);
      analogWrite(11,255);
    }
    if(k>=256 && k< (256*2))
    {
      //z=255-Y
      inside=k-256;

      analogWrite(6,255);
      analogWrite(3,255-inside);
      analogWrite(5,inside);
      
      analogWrite(9,255);
      analogWrite(11,255-inside);
      analogWrite(10,inside);
    }
    if(k>=(256*2))
    {
      inside=k-256-256;
      analogWrite(6,255-inside);
      analogWrite(3,inside);
      analogWrite(5,255);
       analogWrite(9,255-inside);
      analogWrite(11,inside);
      analogWrite(10,255);
    }
    delay(0);
    // put your main code here, to run repeatedly: 
    k=k+1;
    Serial.println(inside);
    // Serial.print(k);
  }
}

