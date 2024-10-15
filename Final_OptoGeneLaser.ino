//Este programa se usa con la app OptoGeneLaser desarrollados ambos por Braulio Ramírez Ramos
//permite controlar el laser de optogenética alimentado con la señal de 5 volts proveniente del arduino
//ofrece control sobre la frecuencia de parpadeo de manera independiente milisegundos de encendido y apagado
//Y también permite controlar los minutos funcionando y de reposo para realizar programas cíclicos de estimulación
#include <SoftwareSerial.h>//Para que funcione BT

SoftwareSerial miBT(10, 11); //pins para comunicación BT
const int laserPin = 7;
char id;
String string_on_time;
String string_off_time;
String string_high_cycle_time;
String string_low_cycle_time;
String string_number_of_cycles;
int on_time = 45;
int off_time = 5;
unsigned long high_cycle_time = 60000;
unsigned long low_cycle_time = 60000;
int number_of_cycles = 1;
int cycle_counter = 0;
unsigned long previousMillis = 0;
unsigned long previousInst = 0;
boolean gate = true;

void setup() {
  //Serial.begin(9600); //solo para comunicación serial si se quieren hacer pruebas con el monitor
  //Serial.println("Listo");
  miBT.begin(38400); // con esto se puede establecer comunicacio´n via BT
  pinMode(laserPin, OUTPUT);
}

void loop() {
  if (miBT.available()){
    id = miBT.read(); //lee el caracter para definir que función se ejecuta

    if (id == 'A'){
      on_func();
    }
    if (id == 'B'){
      off_func();
    }
    if (id == 'C'){
      number_of_cycles_func();
    }
    if (id == 'D'){
      high_cycle_func();
    }
    if (id == 'E'){
      low_cycle_func();
    }
    if (id == 'P'){
      play_func();
    }
    if (id == 'R'){
      reset_func();
    }
  }
}

void on_func(){ //estas funciones: on, off, cycles, high y low, son para editar los parámetros desde la app
  while (miBT.available()){
  char c = miBT.read();//como ya se leyó el caracer id para identificar la función a ejecutar, lo que sigue son números
  string_on_time += c;//estos números se almacenarán en una cadena
  }
  if (string_on_time.length() > 0){
    on_time = string_on_time.toInt();//cuando la cadena construida tenga algún número, este se convertirá en entero
    //Serial.print("On time updated: ");
    //Serial.println(on_time);
    miBT.print(on_time);//se envía el entero construido y la app reenviará el mismo número hasta que sean iguales, 
    //esto permite asegurar que se están comunicando, se recibió la información y por tanto se editó la variable como 
    //el usuario lo desea y evitar ejecutar un programa con variables distintas a las que especificó el usuario sin 
    //que este se diera cuenta
    string_on_time = "";//se limpia la cadena para tener el espacio libre cuando se edite de nuevo la variable
  }
}

void off_func(){//es prácticamente la misma función, pero para editar off, number of cycles, high y low
  while (miBT.available()){
  char c = miBT.read();
  string_off_time += c;
  }
  if (string_off_time.length() > 0){
    off_time = string_off_time.toInt();
    //Serial.print("Off time updated: ");
    //Serial.println(off_time);
    miBT.print(off_time);
    string_off_time = "";
  }
}
void high_cycle_func(){
  while (miBT.available()){
  char c = miBT.read();
  string_high_cycle_time += c;
  }
  if (string_high_cycle_time.length() > 0){
    high_cycle_time = string_high_cycle_time.toInt();
    miBT.print(high_cycle_time);
    //Serial.print("High time updated: ");
    //Serial.println(high_cycle_time);
    high_cycle_time *= 60000UL;//se ajusta el tiempo ingresado por el usuario en miutos y se convierte a milisegundos
    //Como se multiplica por 60 mil, el resultado puede ser muy grande y se usa UL para forzar al número a convertirse en
    // unsigned long que aguanta hasta millones, según yo, a lo mejor puede omitirse y especificar en al app un máximo de
    //minutos en caso de ser necesario
    string_high_cycle_time = "";
  }
}
void low_cycle_func(){
  while (miBT.available()){
  char c = miBT.read();
  string_low_cycle_time += c;
  }
  if (string_low_cycle_time.length() > 0){
    low_cycle_time = string_low_cycle_time.toInt();
    miBT.print(low_cycle_time);
    //Serial.print("Low time updated: ");
    //Serial.println(low_cycle_time);
    low_cycle_time *= 60000UL;
    string_low_cycle_time = "";
  }
}
void number_of_cycles_func(){
  while (miBT.available()){
  char c = miBT.read();
  string_number_of_cycles += c;
  }
  if (string_number_of_cycles.length() > 0){
    number_of_cycles = string_number_of_cycles.toInt();
    //Serial.print("Number of cycles updated: ");
    //Serial.println(number_of_cycles);
    miBT.print(number_of_cycles);
    string_number_of_cycles = "";
  }
}
void play_func(){
  int cycle_counter = 0;
  gate = true;//esta puerta permite asegurar que se incremente el counter solo una vez cada que se termina una fase high del ciclo
  previousMillis = millis();//se pide el SystemTime que se usa para los ciclos macro de tiempo
  previousInst = millis();//se pide el SystemTime que se usa para los ciclos pequeños de tiempo
  //Serial.println("Inicia función play");
  //Serial.println(high_cycle_time + low_cycle_time);
  while (id != 'S' && cycle_counter < number_of_cycles){//mientras no se mande la señal de "Stop" o se alcanze el límite del
    //contador, es decir, se termine de ejecutar el programa, se estará ejecutando el ciclo
    unsigned long currentMillis = millis();//se pide el System time que se estará actualizando y se usará para macro tiempos
    unsigned long currentInst = millis();//System time a actualizarse para tiempos micro (es decir, el parpadeo del laser)
    if (currentMillis - previousMillis <= high_cycle_time){//mientras que se este en un instante del tiempo menor al de la
      //fase high del programa, se estará ejecutanto este ciclo
            if (currentInst - previousInst <= on_time){
              digitalWrite(laserPin, HIGH);//se prende el laser cuando se está en un instante del tiempo comprendido en la duración especificada de milisegundos de encendido
            }
            if ((currentInst - previousInst > on_time) && (currentInst - previousInst <= (on_time + off_time))){
              digitalWrite(laserPin, LOW);//cuando el instante del tiempo esta en el periodo de off, se apaga el laser
            }
            if (currentInst - previousInst > (on_time + off_time)){
              previousInst = millis();//cuando se supera el tiempo después del off, se reinicia el marco de referencia tomado para marcar el inicio del tiempo transcurrido
              //Serial.println("Primer condicional");
            }
    }
    
    if ((currentMillis - previousMillis) > high_cycle_time && currentMillis - previousMillis <= high_cycle_time + low_cycle_time && gate == true){
      digitalWrite(laserPin, LOW);//si está en fase low, se asegura que se apague el laser
      cycle_counter +=1;//se adiciona 1 al counter, y esto se hace aquí, porque una vez que termine el programa, no hay que pasar por la última fase low, sino terminarlo en realidad en cuanto termine la última fase high
      gate = false;//pero se asegura de que solo suceda una vez, ya que la condición pide gate == true
      //Serial.println("Segundo condicional");
    }
    
    if ((currentMillis - previousMillis) > high_cycle_time + low_cycle_time){
      previousMillis = millis();//cuando se supera el periodo después del low, se reinicia el marco de referencia temporal para tiempos macro
      gate = true;//se vuelve a abrir la puerta para que cuando se acabe la siguiente fase high, se sume 1 al contador solo una vez
      //Serial.print("Ingreso al último condicional, contador: ");
      //Serial.println(cycle_counter);
    }
    id = miBT.read();//se lee cualquier mensaje de la App esperando un posible caracter de "Stop" que termine el ciclo
  }
  digitalWrite(laserPin, LOW);//si termina el ciclo porque terminó el programa o se recibió "Stop", se asegura de que se apague el laser
  //Serial.println("término de la función play");
}
void reset_func(){//esta función se activa cuando se inicia o se reestablece la conexión BT, se reinician las variables
  //tanto en la App como aquí, para asegurarse de que el usuario sea conciente de que al iniciar o reestablecer conexión, los
  //ajustes previos no se guardan y vuelva a editar manualmente las variables que desee cambiar
  on_time = 45;
  off_time = 5;
  high_cycle_time = 60000;
  low_cycle_time = 60000;
  number_of_cycles = 1;
  cycle_counter = 0;
  previousMillis = 0;
  previousInst = 0;
  gate = true;
}
