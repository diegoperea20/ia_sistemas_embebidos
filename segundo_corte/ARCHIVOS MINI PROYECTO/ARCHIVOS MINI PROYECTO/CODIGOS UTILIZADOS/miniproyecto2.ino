#include <mini_deportista_inferencing.h>

/* Includes ---------------------------------------------------------------- */

#include <Arduino_LSM9DS1.h> //Click here to get the library: http://librarymanager/All#Arduino_LSM9DS1

/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2    9.80665f
#define MAX_ACCEPTED_RANGE  2.0f        // starting 03/2022, models are generated setting range to +-2, but this example use Arudino library which set range to +-4g. If you are using an older model, ignore this value and use 4.0f instead

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

/* Private variables ------------------------------------------------------- */
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
//COMIENZA PUENTE

int tenis_pin=D2;
int golf_pin=D3;
int baloncesto_pin=D4;
int correr_pin=D5;
int arco_pin=D6;

/**
* @brief      Arduino setup function
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");

    // *****************
// Lo Nuevo
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);

    //MODO PUENTE DIEGO........
    
    pinMode(arco_pin,OUTPUT);
    
    pinMode(baloncesto_pin,OUTPUT);
    
    pinMode(tenis_pin,OUTPUT);
    
    pinMode(correr_pin,OUTPUT);
    
    pinMode(golf_pin,OUTPUT);


    //----------
    
    // Ensure the LED is off by default.
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
// *****************
// Lo Nuevo

    if (!IMU.begin()) {
        ei_printf("Failed to initialize IMU!\r\n");
    }
    else {
        ei_printf("IMU initialized\r\n");
    }

    if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 3) {
        ei_printf("ERR: EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME should be equal to 3 (the 3 sensor axes)\n");
        return;
    }
}

// *****************
// Lo Nuevo
void turn_off_leds(){
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
}

void apagar(){
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
  }
// *****************
// Lo Nuevo
/*
 * Bodega:      [0] ==> All OFF
 * Carga:       [1] ==> Green ON
 * maritimo   [2] ==> Red ON
 * terrestre:[3] ==> Blue ON 
 * Anomaly         ==> LED_BUILTIN ON
 */

// *****************
void puente(int pred_index){
switch (pred_index)
  {
    case 0:     // Idle:      [0] ==> All OFF arco
    apagar();
      digitalWrite(D6, HIGH);
      break;

    case 1:     // lift:       [1] ==> Green ON  baloncesto
    apagar();
        digitalWrite(D4, HIGH);
      
      break;
    
    case 2:     // maritime:   [2] ==> Red ON   correr
    apagar();
      digitalWrite(D5, HIGH);
      break;

    case 3:      //terrestrial:[3] ==> Blue ON  golf
    apagar();
      digitalWrite(D3, HIGH);;
      break;

    case 4:      //terrestrial:[3] ==> Blue ON  teniss
    apagar();
      digitalWrite(D2, HIGH);
      break;

  }
}
// Lo Nuevo
void turn_on_leds(int pred_index) {
  switch (pred_index)
  {
    case 0:     // Idle:      [0] ==> All OFF arco
      turn_off_leds();
      break;

    case 1:     // lift:       [1] ==> Green ON  baloncesto
      turn_off_leds();         
      digitalWrite(LEDG, LOW);
      
      break;
    
    case 2:     // maritime:   [2] ==> Red ON   correr
      turn_off_leds();
      digitalWrite(LEDR, LOW);
      break;

    case 3:      //terrestrial:[3] ==> Blue ON  golf
      turn_off_leds();
      digitalWrite(LEDB, LOW);
      break;
  }
}
// *****************
// Lo Nuevo

/**
 * @brief Return the sign of the number
 * 
 * @param number 
 * @return int 1 if positive (or 0) -1 if negative
 */
float ei_get_sign(float number) {
    return (number >= 0.0) ? 1.0 : -1.0;
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop()
{
    ei_printf("\nStarting inferencing in 2 seconds...\n");

    delay(2000);

    ei_printf("Sampling...\n");

    // Allocate a buffer here for the values we'll read from the IMU
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
        // Determine the next tick (and then sleep later)
        uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

        IMU.readAcceleration(buffer[ix], buffer[ix + 1], buffer[ix + 2]);

        for (int i = 0; i < 3; i++) {
            if (fabs(buffer[ix + i]) > MAX_ACCEPTED_RANGE) {
                buffer[ix + i] = ei_get_sign(buffer[ix + i]) * MAX_ACCEPTED_RANGE;
            }
        }

        buffer[ix + 0] *= CONVERT_G_TO_MS2;
        buffer[ix + 1] *= CONVERT_G_TO_MS2;
        buffer[ix + 2] *= CONVERT_G_TO_MS2;

        delayMicroseconds(next_tick - micros());
    }

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", err);
        return;
    }

    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");

 // *****************
// Lo Nuevo 
    int pred_index = 0;
    float pred_value = result.classification[0].value;   
 // *****************
// Lo Nuevo
   
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
           // *****************
// Lo Nuevo 
        if (result.classification[ix].value > pred_value){
          pred_index = ix;
          pred_value = result.classification[ix].value;
        }  
    
             
    }
    ei_printf("  Prediction: %s with probability %.2f\n", result.classification[pred_index].label, pred_value);
    turn_on_leds (pred_index);
    puente(pred_index);

// *****************
// Lo Nuevo    

#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
// *****************
// Lo Nuevo 
    if (result.anomaly > 0.5)
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);
// *****************
// Lo Nuevo   
#endif
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_ACCELEROMETER
#error "Invalid model for current sensor"
#endif
