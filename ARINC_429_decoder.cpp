#include <Arduino.h>

//Usually, a returning 1 will be used as a fail state, while 0 as everything seems fine
//                  0   1   2   3   4   5  6    7   8   9   10  11 12  13  14  15  16   17  18  19  20  21 22  23   24  25  26  27  28  29  30  31
//                 P  | SMM   |MSB         DATA                                                        LSB|SDI    |LSB           Label          MSB
bool signal[32] = { 0 , 1 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 1 };
//                                                                                                                            |           |       
int data = 0;
int label=0;
bool verificaciones[4]={0,0,0,0};
//

bool verifier(bool signal[32]){
  int count=0;
  for (int i=0;i<32;i++){
    count = count+ signal[i];
  }
  if ((count/2)*2==count){
    
    return 1;
  }
  else{
    return 0;
  }
}

bool smm_verifier(bool signal[32], bool smm_code_one, bool smm_code_two){
  if (signal[1]==smm_code_one && signal[2]==smm_code_two){
    

    return 0;
  }
  else{
    
    
    return 1;
  }
}

void datadecoder(bool signal[32]){
  int x = 0;
  for (int i=4,j=0;i<22;i++,j++){
    x=x+(signal[i]*pow(2,j));
  }
  if (signal[3]==true){
    x=x*-1;
  }
  data=x;
  
}

bool sdi_verifier(bool signal[32], bool sdi_code_one, bool sdi_code_two){
  if (signal[1]==sdi_code_one && signal[2]==sdi_code_two){
    

    return 0;
  }
  else{
    
    
    return 1;
  }
}

bool label_verifier(bool signal[32],int label_one,int label_two,int label_three){
  int slabel_one = 0;
  int slabel_two = 0;
  int slabel_three = 0;
  for (int i=24,j=0;i<27;i++,j++){
    slabel_one= slabel_one+(signal[i]*pow(2,j));
  }
  for (int i=27,j=0;i<30;i++,j++){
    slabel_two= slabel_two+(signal[i]*pow(2,j));
  }
  for (int i=30,j=0;i<32;i++,j++){
    slabel_three= slabel_three+(signal[i]*pow(2,j));
  }
  
  if (slabel_one==label_one && slabel_two==label_two && slabel_three == label_three){
    return 0;
  }
  else {
    return 1;
  }
}

void setup(){
  Serial.begin(115200);
}

void loop(){
  verificaciones[0]= verifier(signal);
  verificaciones[1]= smm_verifier(signal,1,1);
  datadecoder(signal); //does not have return signal, is a void. The value of the signal is stored in the data variable.
  verificaciones[2]= sdi_verifier(signal,1,1);
  verificaciones[3]= label_verifier(signal,0,6,2);
  
  
  
  delay(10000);
}