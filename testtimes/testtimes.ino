
hw_timer_t * timerPacman = NULL;
hw_timer_t * timerGhost = NULL;
void r()
{
  Serial.printf("1r");
}
void r2()
{
  Serial.println("2r");
}
int     pacmanInterval=90000;
int     gohstInterval=320000;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
timerPacman = timerBegin(0, 80, true);
     timerAttachInterrupt(timerPacman, &r, true);
     timerAlarmWrite(timerPacman, pacmanInterval, true);
     timerAlarmEnable(timerPacman);
     timerGhost = timerBegin(3, 80, true);
     timerAttachInterrupt(timerGhost, &r2, true);
     timerAlarmWrite(timerGhost, gohstInterval, true);
     timerAlarmEnable(timerGhost);
}

void loop() {
  // put your main code here, to run repeatedly:

}
