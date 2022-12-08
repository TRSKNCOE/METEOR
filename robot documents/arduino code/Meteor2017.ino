#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "Meteor17-20";
const char* password = "trskncoe";
String cmd ="\0"; // Command received from Android device
String oldc="";
boolean inv1=false;
boolean inv2=false;
boolean inv3=false;

//new pins
#define m1pwm 12  //d6   right motor
#define m1dir 13 //d7   right motor
#define m2pwm 4 //d2   left motor
#define m2dir 14 //d5   left motor

//old pins
/*#define m1pwm 14  //d5   right motor
#define m1dir 15 //d8   right motor
#define m2pwm 4 //d2   left motor
#define m2dir 16 //d0   left motor
*/
int lmid,lright,lleft,obst,ai;

void setup() {
  Serial.begin(115200);

  pinMode(m1pwm,OUTPUT);
  pinMode(m1dir,OUTPUT);
  pinMode(m2pwm,OUTPUT);
  pinMode(m2dir,OUTPUT);
  
  digitalWrite(m1pwm,0);
  digitalWrite(m1dir,0);
  digitalWrite(m2pwm,0);
  digitalWrite(m2dir,0);
      
  connectWiFi();
  server.begin();
}

void loop() {
  pinMode(16,OUTPUT);//d0
  pinMode(5,OUTPUT);// d1
  pinMode(15,OUTPUT);//d8
  
    client = server.available();
    while(!client){
      client = server.available();
    server.begin();
    delay(1);
    CmpAndRun();
    }    
    cmd = checkClient ();
    CmpAndRun();
    homeauto();
}
void homeauto(){
  if(cmd=="m"||cmd=="n"||cmd=="o"||cmd=="M"||cmd=="N"||cmd=="O")
    {
      oldc=cmd;
    }    
    else
    {
    return;
    }
    if (oldc == "n"){ 
  inv1=false;
  digitalWrite(5,inv1);  
    }
    else if (oldc == "o"){   
  inv2=false;
  digitalWrite(16,inv2);
    }
    else if (oldc == "m"){   
  inv3=false;
  digitalWrite(15,inv3);
    }
    else if (oldc == "N"){ 
  inv1=true;
  digitalWrite(5,inv1);  
    }
    else if (oldc == "O"){   
  inv2=true;
  digitalWrite(16,inv2);
    }
    else if (oldc == "M"){   
  inv3=true;
  digitalWrite(15,inv3);
    }
    sendBackEcho(oldc); 
    //oldc = "\0";   
    delay(1);
    client.flush();
    yield();
  }
void CmpAndRun(){
  
    if(cmd=="c"||cmd=="f"||cmd=="b"||cmd=="r"||cmd=="l"||cmd=="s"||cmd=="w"||cmd=="x"||cmd=="y"||cmd=="z"||cmd=="C"||cmd=="F"||cmd=="R"||cmd=="L"||cmd=="W"||cmd=="X")
    {
      oldc=cmd;
    }    
    else
    {
    return;
    }
    Serial.println(oldc);
    if (oldc == "c"){ 
      linetrace();
      delay(1);
      client.flush();
    }
    if (oldc == "C"){ 
      LINETRACEING();
      delay(1);
      client.flush();
    }    
     if (oldc =="f")  {
         forward(1024);
    }
    else if (oldc == "b"){ 
         back(10);
    }
    else if (oldc == "l") {
         dleft(800);    
    }
    else if (oldc == "r") {
         dright(800);    
    }
    else if (oldc == "s") {    
         stopp(); 
    }
    else if (oldc == "x") {
        leftfront();
    }
    else if (oldc == "w") {
       rightfront();
    }
    else if (oldc == "z") {
      rightback();
    }
    else if (oldc == "y") {
      leftback();      
    }                                   //=================no obstackle in caps commands===============================================
    else if (oldc =="F")  {
         FORWARDING(1024);
    }
    else if (oldc == "L") {
         DLEFTING(800);    
    }
    else if (oldc == "R") {
         DRIGHTING(800);    
    }
    else if (oldc == "X") {
        LEFTFRONTING();
    }
    else if (oldc == "W") {
       RIGHTFRONTING();
    }
    
    sendBackEcho(oldc); 
    //oldc = "\0";   
    delay(1);
    client.flush();
    client = server.available();
    yield();
}
void connectWiFi()
{
  Serial.println("\nConnecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}
String checkClient (void)
{
  char *r;
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  //request.toCharArray[r,10];  
  return request;
}
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  //client.println("<!DOCTYPE HTML>");
  //client.println("<html>");
  client.println(echo);
  Serial.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
  client.flush();
  yield();
}
void LINETRACEING() {
  pinMode(16,INPUT);//lmid d7
  pinMode(5,INPUT);//lleft d1
  pinMode(15,INPUT);//lright d5
  pinMode(A0,INPUT);//obst
  lmid=digitalRead(16);//d0
  lleft=digitalRead(5);//d1
  lright=digitalRead(15);//d8
  //Serial.println("linetracedata====================================================");
  //Serial.print(lmid);
  //Serial.print(lright);
  //Serial.print(lleft);
  if(lleft==1&&lmid==1&&lright==1){
      //all high 111
      //ctr++;
      //if(ctr%2==0)
      //left(900);
      //else
      FORWARDING(800);
      yield();
    }
  else if(lleft==1&&lmid==1&&lright==0){
      //110
      digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 750);
      digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 350);
      ai=1;
      yield();
    }
  else if(lleft==1&&lmid==0&&lright==1){
      //101
      FORWARDING(800);
      yield();
    }
  else if(lleft==1&&lmid==0&&lright==0){
      //100
      digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 350);
      digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 750);
      ai=1;
      yield();
    }
  else if(lleft==0&&lmid==1&&lright==1){
      //011
      digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 350);
      digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 750);
      ai=0;
      yield();
    }
  else if(lleft==0&&lmid==1&&lright==0){
      //010
      FORWARDING(700);
      yield();
    }
  else if(lleft==0&&lmid==0&&lright==1){
      //001
      digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 350);
      digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 750);
      ai=0;
      yield();
    }
  else{
      //000
     if(ai = 0)
     {
      digitalWrite(m2dir, LOW);
      analogWrite(m2pwm, 750);
      digitalWrite(m1dir, HIGH);
      analogWrite(m1pwm, 350);
    }
    else if(ai = 1)
     {
      digitalWrite(m1dir, LOW);
      analogWrite(m1pwm, 750);
      digitalWrite(m2dir, HIGH);
      analogWrite(m2pwm, 350);
    }
    yield();
    }
    yield();
}
void linetrace() {
  pinMode(16,INPUT);//lmid d7
  pinMode(5,INPUT);//lleft d1
  pinMode(15,INPUT);//lright d5
  pinMode(A0,INPUT);//obst
  lmid=digitalRead(16);//d0
  lleft=digitalRead(5);//d1
  lright=digitalRead(15);//d8
  //Serial.println("linetracedata====================================================");
  //Serial.print(lmid);
  //Serial.print(lright);
  //Serial.print(lleft);
  if(lleft==1&&lmid==1&&lright==1){
      //all high 111
      //ctr++;
      //if(ctr%2==0)
      //left(900);
      //else
      forward(800);      
      yield();
    }
  else if(lleft==1&&lmid==1&&lright==0){
      //110
      digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 750);
      digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 350);
      ai=1;
      yield();
    }
  else if(lleft==1&&lmid==0&&lright==1){
      //101
      forward(800);
      yield();
    }
  else if(lleft==1&&lmid==0&&lright==0){
      //100
      digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 350);
      digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 750);
      ai=1;
      yield();
    }
  else if(lleft==0&&lmid==1&&lright==1){
      //011
      digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 350);
      digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 750);
      ai=0;
      yield();
    }
  else if(lleft==0&&lmid==1&&lright==0){
      //010
      forward(700);
      yield();
    }
  else if(lleft==0&&lmid==0&&lright==1){
      //001
      digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 350);
      digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 750);
      ai=0;
      yield();
    }
  else{
      //000
      if(ai = 0)
     {
      digitalWrite(m2dir, LOW);
      analogWrite(m2pwm, 750);
      digitalWrite(m1dir, HIGH);
      analogWrite(m1pwm, 350);
    }
    else if(ai = 1)
     {
      digitalWrite(m1dir, LOW);
      analogWrite(m1pwm, 750);
      digitalWrite(m2dir, HIGH);
      analogWrite(m2pwm, 350);
    }
    yield();
    }
    yield();
}
void DLEFTING(int a){
  digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 1024-a);
      digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, a);
}
void DRIGHTING(int a)
{
  digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 1024-a);
      digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, a);
}
void dleft(int a){
  digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 1024-a);
      digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, a);
    obst=analogRead(A0);
    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
}
void dright(int a)
{
  digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 1024-a);
      digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, a);
    obst=analogRead(A0);
    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
}
void leftback(){
    digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, 1);
    digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, 300);  
}
void rightback(){
  digitalWrite(m1dir, HIGH);
      analogWrite(m1pwm, 300);
      digitalWrite(m2dir, HIGH);
      analogWrite(m2pwm, 1);
}
void LEFTFRONTING(){
  digitalWrite(m1dir, LOW);
        analogWrite(m1pwm, 1024);
        digitalWrite(m2dir, LOW);
        analogWrite(m2pwm, 700);
}
void leftfront(){
  digitalWrite(m1dir, LOW);
        analogWrite(m1pwm, 1024);
        digitalWrite(m2dir, LOW);
        analogWrite(m2pwm, 700);
  obst=analogRead(A0);
    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
  }
void RIGHTFRONTING(){
  digitalWrite(m1dir, LOW);
       analogWrite(m1pwm, 700);
       digitalWrite(m2dir, LOW);
       analogWrite(m2pwm, 1024);
}
void rightfront(){
  digitalWrite(m1dir, LOW);
       analogWrite(m1pwm, 700);
       digitalWrite(m2dir, LOW);
       analogWrite(m2pwm, 1024);
       obst=analogRead(A0);
    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
}
void back(int a)
{
    digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, a);
    digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, a);
}
void FORWARDING(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, a);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, a);
}
void forward(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, a);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, a);
    obst=analogRead(A0);
          Serial.println(obst);

    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
}
void stopp()
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 0);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 0);
}
void BLEFTING(int a)
{
    digitalWrite(m1dir, HIGH);
    analogWrite(m1pwm, a);
    digitalWrite(m2dir,LOW);
    analogWrite(m2pwm, 0);
}
void BRIGHTING(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 0);
    digitalWrite(m2dir, HIGH);
    analogWrite(m2pwm, a);
}
void LEFTING(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, a);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 0);
}
void RIGHTING(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 0);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, a);
}
void left(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, a);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, 0);
    obst=analogRead(A0);
    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
}
void right(int a)
{
    digitalWrite(m1dir, LOW);
    analogWrite(m1pwm, 0);
    digitalWrite(m2dir, LOW);
    analogWrite(m2pwm, a);
    obst=analogRead(A0);
    if(obst<350){
      stopp();
      obst=analogRead(A0);
      Serial.println(obst);
      delay(10);
      client.flush();
    }
}
