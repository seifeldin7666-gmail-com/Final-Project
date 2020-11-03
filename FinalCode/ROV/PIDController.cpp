#include "library.h"
#include "PIDController.h"
PIDController::PIDController () {
  // Variables - double
  double output;
  double lastErr;
  double errSum;

  // Variables - long
  unsigned long lastTime;

  // Variables - bool
  bool doConstrain;
  bool init;

  // Variables - double - tuining
  double Kp;
  double Ki;
  double Kd;
  double divisor;
  double minOut;
  double maxOut;
  double setPoint;
}

void PIDController::begin () {
  Kp = 1;
  Ki = 1;
  Kd = 1;
  divisor = 10;
  doLimit = false;
  init = true;
}

void PIDController::setpoint (double newSetpoint) {
  setPoint = newSetpoint;
}

void PIDController::tune (double _Kp, double _Ki, double _Kd) {
  if (_Kp < 0 || _Ki < 0 || _Kd < 0) return;
  Kp = _Kp;
  Ki = _Ki;
  Kd = _Kd;
}

void PIDController::limit(double min, double max) {
  minOut = min;
  maxOut = max;
  doLimit = true;
}

/*void PIDController::printGraph (double sensorInput, String verbose) {
  Serial.print(sensorInput);
  if (verbose == VERBOSE) {
    Serial.print(",");
    Serial.print(output);
  }
  Serial.print(",");
  Serial.println(setPoint);
}

*/
void PIDController::minimize (double newMinimize) {
  divisor = newMinimize;
}

// Getters
double PIDController::getOutput () {
  return output;
}


double PIDController::compute (double sensor) {
  // Return false if it could not execute;
  // This is the actual PID algorithm executed every loop();

  // Failsafe, return if the begin() method hasn't been called
  if (!init) return 0;

  // Calculate time difference since last time executed
  unsigned long now = millis(); 
  double timeChange = (double)(now - lastTime);
  double error = setPoint - sensor;
  
  
 
   double dErr = (error - lastErr) ;
   dErr/= now-lastTime;
  double output = error * Kp + dErr * Kd;

 if(output>255) output = 255;
 else if (output <-255) output = -255;

  // Update lastErr and lastTime to current values for use in next execution
  lastErr = error;
  lastTime = now;
  /*  double error = pid->sp - input;
    double derError = error - pid->previousErr;
    derError /= (currentTime - pid->previousTime); //Derivative is rate of change of error.

    pid->output = error * kp + derError * kd; //PID equation (+100) as the reference value is 100.
      if(pid->output > 255)
        pid->output = 255;
      else if (pid->output < -255)
        pid->output = -255;
      
    //Now we make new time and error as previous for the next case.
    pid->previousTime = currentTime;
    pid->previousErr =error;*/

  return output;
}
