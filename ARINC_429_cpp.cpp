#include <Arduino.h>
#include <bitset>
#include <iostream>
//Im Pandora, hi, some notes are for my class, I will clean this later from the github, maybe, idk.
//Aviso: Unicamente funciona para enviar numeros, por favor no envien data como caracteres
//                  0   1   2   3   4   5  6    7   8   9   10  11 12  13  14  15  16   17  18  19  20  21 22  23   24  25  26  27  28  29  30  31
//                 P  | SMM   |LSB         DATA                                                        MSB|SDI    |LSB           Label          MSB
bool signal[32] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
const int valor=100;
//works 100% gg ez subanle la dificultad a los bots
void paritycheck(){
  int count = 0;
 for(int i=1;i<=31;i++){
  count = count + signal[i];
  }
  if ((count/2)*2 == count){
    signal[0]=1;
  }
  else {
    signal[0]=0;
  }
  
}

void smm(){
  signal[1]=1;
  signal[2]=1;
}

void Data(int valordata){//the LSB to MSB is on inverse to the wikipedia order due to me not realising during coding, may change later lol
  if (valordata<0){
    signal[3]=1; //negative
  }
  else{
    signal[3]=0; //positive
  }
  int absvalordata=abs(valordata);
 std::bitset<18> b(absvalordata);
 for (int i=4, j=0 ;i<22;i++, j++){
  signal[i]=b.test(j);
  
 }
}

void sdi(int bituno, int bitdos){
  //Enter manually which one is which, this ocupies 2 bits
  //por default el mcu con los sensores sera 00, el mcu con los actuadores 11, si estos son el mismo, recomiendo asumir 00
  signal[22]=bituno;
  signal[23]=bitdos;
}

void label(int numerouno, int numerodos, int numerotres){
  //numero uno y numero dos del identificador tienen rango de 0-7 en decimal, numero 3 tiene rango 0-3
  std::bitset<3> b(numerouno); 
  
  for (int i=24,j=0;j<3;i++,j++){
    signal[i]=b.test(j);
    
    
  }
  
  std::bitset<3> c(numerodos); 
  for (int i=27,j=0;j<3;i++,j++){
    signal[i]=c.test(j);
    
  }
  
  std::bitset<2> d(numerotres); 
  for (int i=30,j=0;j<2;i++,j++){
    signal[i]=d.test(j);
    
  }
}

void check(){
  Serial.print("\n String: ");
  for (int i=0;i<32;i++){
    Serial.print(signal[i]);
    
  }
  
}

void setup(){
  Serial.begin(115200);
}

void loop(){
  //set label, first and second number have scope of 0-7, 3rd has scope of 0-3
  label(0,0,0);

  //manually set source/destination, accepts 1 or 0 for both imputs
  sdi(0,0);

  //inset the data into the string, replace 0 with value or variable that want sent, max values of 262,144 acounting for sign 
  Data(0);

  //designs the type of data, set the smm encoding, for purposes of the practice both stay in 11, normal operation (NO)
  smm();

  //checks for the parity so that the sum of on bits is odd
  paritycheck();

  //prints into console the string of data
  check();
  delay(10000);//delay is only for testing on a live esp, can be deleted.
}
