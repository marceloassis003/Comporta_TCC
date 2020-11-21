#include <SoftwareSerial.h>
//SoftwareSerial btSerial(30, 31); //rx / tx 


// ----------- Nomenclatura das entradas e saídas ------------//
#define SensordeChuva A0
#define FimdeCursoComportaFechada 3
#define FimdeCursoComportaAberta 4
#define BotaoManualAutomatico 5
#define BotaoFechaManual 6
#define BotaoAbreManual 7
#define LedAutomatico 8
#define LedManual 9
#define LedControl 2
#define FechaComporta 10
#define AbreComporta 11
#define presenca 12
#define boia 13



// ----------- Declaração de variáveis ------------//
short PARA = 0;
short CLIQUES = 0 ;

short EstadoComporta = 0;
short FechandoaComporta = 0;
short AbrindoaComporta = 0;

void setup() {


// ----------- Definições de entradas e saídas ------------//
pinMode(FimdeCursoComportaFechada, INPUT);
pinMode(FimdeCursoComportaAberta, INPUT);
pinMode(BotaoManualAutomatico, INPUT);
pinMode(BotaoFechaManual, INPUT);
pinMode(BotaoAbreManual, INPUT);
pinMode(boia, INPUT);


pinMode(LedAutomatico, OUTPUT);
pinMode(LedManual, OUTPUT);
pinMode(LedControl, OUTPUT);
pinMode(FechaComporta, OUTPUT);
pinMode(AbreComporta, OUTPUT);


// ----------- Iniciando a comunicação serial com velocidade de 9600 bits/segundo ------------//

//btSerial.begin(9600);
Serial.begin(9600);

}

void loop() {
  //lendo variavel estado para o sensor anti-esmagamento 
int estado;
  estado = digitalRead(presenca);
  
  //lendo o sesor boia para saber seu estado 
int estado_b;
  estado_b= digitalRead(boia);
  
  
  char command;
  
  


// ----------- Estratégia para incrementar CLIQUES a cada clique ------------//
if( digitalRead(BotaoManualAutomatico) == HIGH && PARA == 0){
PARA = 1; 
}

if( digitalRead(BotaoManualAutomatico) == LOW && PARA == 1){
CLIQUES++;
PARA = 0;
}


// Se cliques for igual a 0
if (CLIQUES == 0){// Automático
  
// Liga LED Automático e desliga Manual 
digitalWrite( LedAutomatico, HIGH); 
digitalWrite( LedManual, LOW); 
digitalWrite( LedControl, LOW); 



if(analogRead(SensordeChuva) <= 800   and estado_b == 0 and estado == 1 ){// Se chover e o sensor for igual a 0 que significa algamento manda fechar  //
FechandoaComporta = 1;
// Manda fechar a comporta
digitalWrite( FechaComporta, HIGH); 
digitalWrite( AbreComporta, LOW); 
delay(2000);
}



if (FechandoaComporta == 1 && EstadoComporta == 0){// Se a comporta chegar no fim de curso da janela fechada
 //Desliga o motor
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, LOW); 
FechandoaComporta = 0 ;
 
}


if(analogRead(SensordeChuva) > 900 and estado_b == 1){// Se parar de chover e o sensor for igual a 1 nao possui alagamento manda abrir  
AbrindoaComporta = 1;
// Manda abrir a comporta
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, HIGH); 
}



if (AbrindoaComporta == 1 && EstadoComporta == 1){// Se a comporta chegar no fim de curso da comporta aberta

// Desliga o motor
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, LOW); 
AbrindoaComporta = 0 ;
 
}


}


// Se cliques for igual a 1
if (CLIQUES == 1){// Manual


// Liga LED Manual e desliga Automático  
digitalWrite( LedAutomatico, LOW); 
digitalWrite( LedManual, HIGH); 
digitalWrite( LedControl, LOW); 



if(digitalRead(BotaoAbreManual) == HIGH){ // Se o botão para abrir a comporta for precionado
// Manda abrir a janela
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, HIGH); 
}


if(digitalRead(BotaoFechaManual) == HIGH and estado == 1 ){ // Se o botão para fechar a comporta for precionado
// Manda fechar a janela  
digitalWrite( FechaComporta, HIGH); 
digitalWrite( AbreComporta, LOW); 
}




if(digitalRead(BotaoAbreManual) == LOW && digitalRead(BotaoFechaManual) == LOW ){// Se os dois botões estiverem solto

// Desliga o motor  
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, LOW);  
}

if (digitalRead(BotaoFechaManual) == LOW && EstadoComporta == 1){// Se a comporta chegar no fim de curso da comporta aberta
// Desliga o motor    
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, LOW); 
}

if (digitalRead(BotaoAbreManual) == LOW && EstadoComporta == 0){// Se a comporta chegar no fim de curso da comporta fechada
// Desliga o motor    
digitalWrite( FechaComporta, LOW); 
digitalWrite( AbreComporta, LOW); 
}
 
}


if (CLIQUES == 2){

// desliga LED Manual e desliga Automático  e liga cntrole bluethooth 
digitalWrite( LedAutomatico, LOW); 
digitalWrite( LedManual, LOW); 
digitalWrite( LedControl, HIGH); 

//logica controle 


while (Serial.available())
  command = Serial.read();

 

Serial.print(command);

if (command == 'A' ){
  digitalWrite( AbreComporta, HIGH); // Manda abrir a comporta
}
if (command == 'a'){
    //digitalWrite( FechaComporta, LOW);
    digitalWrite( AbreComporta, LOW);
}


 if (command == 'B' and estado == 1){
  digitalWrite(FechaComporta, HIGH);// Manda fechar a comporta 
}
if (command == 'b'){
    //digitalWrite(AbreComporta, LOW); 
    digitalWrite(FechaComporta, LOW);

}


 
}

if (CLIQUES == 3){// Se cliques for igual a 3 ele mesmo se volta para 0

CLIQUES = 0;

  
}



// ----------- Mapeando estados dos fim de cursos ------------//

if( digitalRead(FimdeCursoComportaFechada) == HIGH && digitalRead(FimdeCursoComportaAberta) == LOW){

EstadoComporta = 0; // 0 Fechada  1 Aberta  2 Em movimento
}

if( digitalRead(FimdeCursoComportaFechada) == LOW && digitalRead(FimdeCursoComportaAberta) == HIGH){

EstadoComporta = 1; // 0 Fechada  1 Aberta  2 Em movimento

}

if( digitalRead(FimdeCursoComportaFechada) == LOW && digitalRead(FimdeCursoComportaAberta) == LOW){

EstadoComporta = 2; // 0 Fechada  1 Aberta  2 Em movimento

}
// --------------------------------------------------------------------------------------------------------// 


// ----------- Escrevendo no monitor serial ------------//
Serial.print("Valor Sensor:");
Serial.print(analogRead(SensordeChuva));
Serial.print("|");
Serial.print("Estado da Comporta:");
Serial.print(EstadoComporta);
Serial.print("|");
Serial.print("Fechado a Comporta:");
Serial.print(FechandoaComporta);
Serial.print("|");
Serial.print("Abrindo a Comporta:");
Serial.println(AbrindoaComporta);
Serial.print("|");
Serial.print("Nivel da agua:");
if (estado_b == 0){
  Serial.print("Alagado");
  Serial.print("|");
}
else{
    Serial.print("Normal");
}
delay(500);
/*
//Serial.println(estado_b);
*/


// -------------------------------------------------------------------------------------------------------------- //

}
