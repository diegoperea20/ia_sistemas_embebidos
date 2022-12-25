/******************************************************************
 * Programa que genera una onda senoidal usando una red neuronal
 * MLP superficial
 * Autor:
 *      Jesús Alfonso López 
 *      jalopez@uao.edu.co
 ******************************************************************/

#include <math.h>

/******************************************************************
 * Definición estructura de la red
 ******************************************************************/

const int HiddenNodes = 10;
const int InputNodes = 1;
const int OutputNodes = 1;

// Los pesos de la red fueron obtenidos en TensorFlow y se copiaron a este programa
// Pesos capa oculta
const float HiddenWeights[InputNodes+1][HiddenNodes]= {

    };

// Pesos capa de salida
const float OutputWeights[HiddenNodes+1][OutputNodes]  = {
    }; 


int i, j, p, q, r;
float Accum;
float Hidden[HiddenNodes];
float Output[OutputNodes];
float Input[InputNodes];
 

void setup(){
  //start serial connection
  Serial.begin(9600);
}

void loop(){
  
  float Entrada;
  float Salida;
  float Tiempo;

Tiempo=millis();
//Artificio para generar una entrada entre 0 y 2*pi
Entrada=(fmod(Tiempo,6283))/1000; 

Input[0]=Entrada;
/******************************************************************
* Cálculo de la salida de capa oculta
******************************************************************/
    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += HiddenWeights[j][i]*Input[j];
      }
      // función de activación tangente hiperbólica
        Hidden[i] = (exp(Accum)-exp(-Accum))/(exp(Accum)+exp(-Accum));
    }

/******************************************************************
* Cálculo de la salida de capa de salida
******************************************************************/

    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum +=  OutputWeights[j][i]*Hidden[j];
      }
      // función de activación lineal
        Output[i] = Accum; 
    }

Salida=Output[0];

Serial.print("SalidaDes:"); 
Serial.println(Salida);      

delay(50);  

}
