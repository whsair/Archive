#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/position_sensor.h>
#include <webots/utils/motion.h>
#include <webots/touch_sensor.h>
#include <webots/camera.h>
#define TIME_STEP 16

static WbDeviceTag RShoulderPitchSensor, RShoulderYawSensor, RElbowYawSensor;
static WbDeviceTag LShoulderPitchSensor, LShoulderYawSensor, LElbowYawSensor;
static WbDeviceTag RHipYawSensor, RHipPitchSensor, RKneePitchSensor, RAnklePitchSensor, RAnkleYawSensor;
static WbDeviceTag LHipYawSensor, LHipPitchSensor, LKneePitchSensor, LAnklePitchSensor, LAnkleYawSensor;
static WbDeviceTag NeckRollSensor;

static WbDeviceTag RShoulderPitchMotor, RShoulderYawMotor, RElbowYawMotor;
static WbDeviceTag LShoulderPitchMotor, LShoulderYawMotor, LElbowYawMotor;
static WbDeviceTag RHipYawMotor, RHipPitchMotor, RKneePitchMotor, RAnklePitchMotor, RAnkleYawMotor;
static WbDeviceTag LHipYawMotor, LHipPitchMotor, LKneePitchMotor, LAnklePitchMotor, LAnkleYawMotor;
static WbDeviceTag NeckRollMotor;

static WbDeviceTag camera;
static int camera_width;
static int camera_height;
//static const unsigned char *image;





static void find_and_enable_devices() {
  // Motor Sensors
  RShoulderPitchSensor = wb_robot_get_device("RShoulderPitchSensor");
  RShoulderYawSensor = wb_robot_get_device("RShoulderYawSensor");
  RElbowYawSensor = wb_robot_get_device("RElbowYawSensor");
  
  LShoulderPitchSensor = wb_robot_get_device("LShoulderPitchSensor");
  LShoulderYawSensor = wb_robot_get_device("LShoulderYawSensor");
  LElbowYawSensor = wb_robot_get_device("LElbowYawSensor");
  
  RHipYawSensor = wb_robot_get_device("RHipYawSensor");
  RHipPitchSensor = wb_robot_get_device("RHipPitchSensor");
  RKneePitchSensor = wb_robot_get_device("RKneePitchSensor");
  RAnklePitchSensor = wb_robot_get_device("RAnklePitchSensor");
  RAnkleYawSensor = wb_robot_get_device("RAnkleYawSensor");
  
  LHipYawSensor = wb_robot_get_device("LHipYawSensor");
  LHipPitchSensor = wb_robot_get_device("LHipPitchSensor");
  LKneePitchSensor = wb_robot_get_device("LKneePitchSensor");
  LAnklePitchSensor = wb_robot_get_device("LAnklePitchSensor");
  LAnkleYawSensor = wb_robot_get_device("LAnkleYawSensor");
  
  NeckRollSensor = wb_robot_get_device("NeckRollSensor"); 
  
  wb_position_sensor_enable(RShoulderPitchSensor, TIME_STEP);
  wb_position_sensor_enable(RShoulderYawSensor, TIME_STEP);
  wb_position_sensor_enable(RElbowYawSensor, TIME_STEP);
  
  wb_position_sensor_enable(LShoulderPitchSensor, TIME_STEP);  
  wb_position_sensor_enable(LShoulderYawSensor, TIME_STEP);  
  wb_position_sensor_enable(LElbowYawSensor, TIME_STEP);  
  
  wb_position_sensor_enable(RHipYawSensor, TIME_STEP);  
  wb_position_sensor_enable(RHipPitchSensor, TIME_STEP);  
  wb_position_sensor_enable(RKneePitchSensor, TIME_STEP);  
  wb_position_sensor_enable(RAnklePitchSensor, TIME_STEP);  
  wb_position_sensor_enable(RAnkleYawSensor, TIME_STEP);  
  
  wb_position_sensor_enable(LHipYawSensor, TIME_STEP); 
  wb_position_sensor_enable(LHipPitchSensor, TIME_STEP);  
  wb_position_sensor_enable(LKneePitchSensor, TIME_STEP);  
  wb_position_sensor_enable(LAnklePitchSensor, TIME_STEP);   
  wb_position_sensor_enable(LAnkleYawSensor, TIME_STEP);  
  
  wb_position_sensor_enable(NeckRollSensor, TIME_STEP);
  
  // Motors
  RShoulderPitchMotor = wb_robot_get_device("RShoulderPitchMotor");
  RShoulderYawMotor = wb_robot_get_device("RShoulderYawMotor");
  RElbowYawMotor = wb_robot_get_device("RElbowYawMotor");
  
  LShoulderPitchMotor = wb_robot_get_device("LShoulderPitchMotor");
  LShoulderYawMotor = wb_robot_get_device("LShoulderYawMotor");
  LElbowYawMotor = wb_robot_get_device("LElbowYawMotor");
  
  RHipYawMotor = wb_robot_get_device("RHipYawMotor");
  RHipPitchMotor = wb_robot_get_device("RHipPitchMotor");
  RKneePitchMotor = wb_robot_get_device("RKneePitchMotor");
  RAnklePitchMotor = wb_robot_get_device("RAnklePitchMotor");
  RAnkleYawMotor = wb_robot_get_device("RAnkleYawMotor");
  
  LHipYawMotor = wb_robot_get_device("LHipYawMotor");
  LHipPitchMotor = wb_robot_get_device("LHipPitchMotor");
  LKneePitchMotor = wb_robot_get_device("LKneePitchMotor");
  LAnklePitchMotor = wb_robot_get_device("LAnklePitchMotor");
  LAnkleYawMotor = wb_robot_get_device("LAnkleYawMotor");
  
  NeckRollMotor = wb_robot_get_device("NeckRollMotor");
  
  wb_motor_enable_torque_feedback(RShoulderPitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(RShoulderYawMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(RElbowYawMotor, TIME_STEP);
  
  wb_motor_enable_torque_feedback(LShoulderPitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(LShoulderYawMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(LElbowYawMotor, TIME_STEP);
  
  wb_motor_enable_torque_feedback(RHipYawMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(RHipPitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(RKneePitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(RAnklePitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(RAnkleYawMotor, TIME_STEP);
  
  wb_motor_enable_torque_feedback(LHipYawMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(LHipPitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(LKneePitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(LAnklePitchMotor, TIME_STEP);
  wb_motor_enable_torque_feedback(LAnkleYawMotor, TIME_STEP);
  
  wb_motor_enable_torque_feedback(NeckRollMotor, TIME_STEP);
  
}


// print output
static void print_motor(FILE *fp) {
  //motor torque
  // Torque information degrades performance, commenting so you have more source code to learn from
  /*
  const double TorqueRShoulderPitchMotor = wb_motor_get_torque_feedback(RShoulderPitchMotor);
  const double TorqueRShoulderYawMotor = wb_motor_get_torque_feedback(RShoulderYawMotor);
  const double TorqueRElbowYawMotor = wb_motor_get_torque_feedback(RElbowYawMotor);
  
  const double TorqueLShoulderPitchMotor = wb_motor_get_torque_feedback(LShoulderPitchMotor);
  const double TorqueLShoulderYawMotor = wb_motor_get_torque_feedback(LShoulderYawMotor);
  const double TorqueLElbowYawMotor = wb_motor_get_torque_feedback(LElbowYawMotor);
  
  const double TorqueRHipYawMotor = wb_motor_get_torque_feedback(RHipYawMotor);
  const double TorqueRHipPitchMotor = wb_motor_get_torque_feedback(RHipPitchMotor);
  const double TorqueRKneePitchMotor = wb_motor_get_torque_feedback(RKneePitchMotor);
  const double TorqueRAnklePitchMotor = wb_motor_get_torque_feedback(RAnklePitchMotor);
  const double TorqueRAnkleYawMotor = wb_motor_get_torque_feedback(RAnkleYawMotor);
  
  const double TorqueLHipYawMotor = wb_motor_get_torque_feedback(LHipYawMotor);
  const double TorqueLHipPitchMotor = wb_motor_get_torque_feedback(LHipPitchMotor);
  const double TorqueLKneePitchMotor = wb_motor_get_torque_feedback(LKneePitchMotor);
  const double TorqueLAnklePitchMotor = wb_motor_get_torque_feedback(LAnklePitchMotor);
  const double TorqueLAnkleYawMotor = wb_motor_get_torque_feedback(LAnkleYawMotor);
  
  const double TorqueNeckRollMotor = wb_motor_get_torque_feedback(NeckRollMotor);
  */
  
  // position sensor
  const double PositionRShoulderPitchSensor = wb_position_sensor_get_value(RShoulderPitchSensor);
  const double PositionRShoulderYawSensor = wb_position_sensor_get_value(RShoulderYawSensor);
  const double PositionRElbowYawSensor = wb_position_sensor_get_value(RElbowYawSensor);
  
  const double PositionLShoulderPitchSensor = wb_position_sensor_get_value(LShoulderPitchSensor);
  const double PositionLShoulderYawSensor = wb_position_sensor_get_value(LShoulderYawSensor);
  const double PositionLElbowYawSensor = wb_position_sensor_get_value(LElbowYawSensor);
  
  const double PositionRHipYawSensor = wb_position_sensor_get_value(RHipYawSensor);
  const double PositionRHipPitchSensor = wb_position_sensor_get_value(RHipPitchSensor);
  const double PositionRKneePitchSensor = wb_position_sensor_get_value(RKneePitchSensor);
  const double PositionRAnklePitchSensor = wb_position_sensor_get_value(RAnklePitchSensor);
  const double PositionRAnkleYawSensor = wb_position_sensor_get_value(RAnkleYawSensor);
  
  const double PositionLHipYawSensor = wb_position_sensor_get_value(LHipYawSensor);
  const double PositionLHipPitchSensor = wb_position_sensor_get_value(LHipPitchSensor);
  const double PositionLKneePitchSensor = wb_position_sensor_get_value(LKneePitchSensor);
  const double PositionLAnklePitchSensor = wb_position_sensor_get_value(LAnklePitchSensor);
  const double PositionLAnkleYawSensor = wb_position_sensor_get_value(LAnkleYawSensor);
  
  const double PositionNeckRollSensor = wb_position_sensor_get_value(NeckRollSensor);
  const double time = wb_robot_get_time();
  
  fprintf(fp, "%f ", time);
  
  // Torque information degrades performance, commenting so you have more source code to learn from
  /*
  fprintf(fp, "%f %f %f ", TorqueRShoulderPitchMotor, TorqueRShoulderYawMotor, TorqueRElbowYawMotor);
  fprintf(fp, "%f %f %f ", TorqueLShoulderPitchMotor, TorqueLShoulderYawMotor, TorqueLElbowYawMotor);
  fprintf(fp, "%f %f %f %f %f ", TorqueRHipYawMotor, TorqueRHipPitchMotor, TorqueRKneePitchMotor, TorqueRAnklePitchMotor, TorqueRAnkleYawMotor);
  fprintf(fp, "%f %f %f %f %f ", TorqueLHipYawMotor, TorqueLHipPitchMotor, TorqueLKneePitchMotor, TorqueLAnklePitchMotor, TorqueLAnkleYawMotor);
  fprintf(fp, "%f", TorqueNeckRollMotor);
  */
  
  fprintf(fp, "%f %f %f ", PositionRShoulderPitchSensor, PositionRShoulderYawSensor, PositionRElbowYawSensor);
  fprintf(fp, "%f %f %f ", PositionLShoulderPitchSensor, PositionLShoulderYawSensor, PositionLElbowYawSensor);
  fprintf(fp, "%f %f %f %f %f ", PositionRHipYawSensor, PositionRHipPitchSensor, PositionRKneePitchSensor, PositionRAnklePitchSensor, PositionRAnkleYawSensor);
  fprintf(fp, "%f %f %f %f %f ", PositionLHipYawSensor, PositionLHipPitchSensor, PositionLKneePitchSensor, PositionLAnklePitchSensor, PositionLAnkleYawSensor);
  fprintf(fp, "%f\n", PositionNeckRollSensor);

}

// Set velocities to avoid yeeting the robot into orbit
// Values taken from data sheet and converted from sec/deg to rad/sec
static void set_motor_velocities() {
  wb_motor_set_velocity(RShoulderPitchMotor, 5.28887);
  wb_motor_set_velocity(RShoulderYawMotor, 5.28887);
  wb_motor_set_velocity(RElbowYawMotor, 5.28887);
  
  wb_motor_set_velocity(LShoulderPitchMotor, 5.28887);
  wb_motor_set_velocity(LShoulderYawMotor, 5.28887);
  wb_motor_set_velocity(LElbowYawMotor, 5.28887);
  
  wb_motor_set_velocity(RHipYawMotor, 5.28887);
  wb_motor_set_velocity(RHipPitchMotor, 5.28887);
  wb_motor_set_velocity(RKneePitchMotor, 5.28887);
  wb_motor_set_velocity(RAnklePitchMotor, 5.28887);
  wb_motor_set_velocity(RAnkleYawMotor, 5.28887);
  
  wb_motor_set_velocity(LHipYawMotor, 5.28887);
  wb_motor_set_velocity(LHipPitchMotor, 5.28887);
  wb_motor_set_velocity(LKneePitchMotor, 5.28887);
  wb_motor_set_velocity(LAnklePitchMotor, 5.28887);
  wb_motor_set_velocity(LAnkleYawMotor, 5.28887);
  
  wb_motor_set_velocity(NeckRollMotor, 5.28887);
}


// Set all motors to zero
static void default_position() {
  wb_motor_set_position(RShoulderPitchMotor, 0.0);
  wb_motor_set_position(RShoulderYawMotor, 0.0);
  wb_motor_set_position(RElbowYawMotor, 0.0);
  
  wb_motor_set_position(LShoulderPitchMotor, 0.0);
  wb_motor_set_position(LShoulderYawMotor, 0.0);
  wb_motor_set_position(LElbowYawMotor, 0.0);
  
  wb_motor_set_position(RHipYawMotor, 0.0);
  wb_motor_set_position(RHipPitchMotor, 0.0);
  wb_motor_set_position(RKneePitchMotor, 0.0);
  wb_motor_set_position(RAnklePitchMotor, 0.0);
  wb_motor_set_position(RAnkleYawMotor, 0.0);
  
  wb_motor_set_position(LHipYawMotor, 0.0);
  wb_motor_set_position(LHipPitchMotor, 0.0);
  wb_motor_set_position(LKneePitchMotor, 0.0);
  wb_motor_set_position(LAnklePitchMotor, 0.0);
  wb_motor_set_position(LAnkleYawMotor, 0.0);
  
  wb_motor_set_position(NeckRollMotor, 0.0);
}




int main(int argc, char **argv) {
  // Initialize everything for webots
  wb_robot_init();
  int time_step = wb_robot_get_basic_time_step();
  find_and_enable_devices();
  set_motor_velocities();
  
  // find_and_enalbe_camera();
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera,time_step);
  camera_width = wb_camera_get_width(camera);
  camera_height = wb_camera_get_height(camera);
  
  
  
  
  // Prepare output file
  FILE *motor_file = NULL;
  motor_file = fopen("../../data_output/motor_info.txt","w+");
  
  // Wait a second before starting
  wb_robot_step(1000);
  
  const unsigned char *image = wb_camera_get_image(camera);
  //image = wb_camera_get_image(camera);
  int r = 0;
  int g = 0;
  int b = 0;
 
  for (int x = camera_width/3.0; x < 2.0*camera_width/3.0; x++)
    for (int y = camera_height/4.0; y < 3*camera_height/5.0; y++) {
      r += wb_camera_image_get_red(image, camera_width, x, y);
      g += wb_camera_image_get_green(image, camera_width, x, y);
      b += wb_camera_image_get_blue(image, camera_width, x, y);
  }
  
  int color =0;
  if (r > 3.0*g && r > 3.0*b) {
    color = 1;
  } else if (b > 3.0*r && b > 3.0*g) {
    color = 2;
  } else if (g > 3.0*b && g > 3.0*r) {
    color = 3;
  }
  
  
  
  // Do the motion defined in a file
  bool endprint = 1;
  WbMotionRef YansheeMotion;
  
  
  if (color == 1) {
    YansheeMotion = wbu_motion_new("../../motions/WaveRight.motion");
  } else if (color == 2) {
    YansheeMotion = wbu_motion_new("../../motions/WaveBoth.motion");
  } else if (color == 3) {
    YansheeMotion = wbu_motion_new("../../motions/WaveLeft.motion");
  } else {
    YansheeMotion = wbu_motion_new("../../motions/FallDown.motion");
  }
  
  wbu_motion_play(YansheeMotion);
  
  while (wb_robot_step(TIME_STEP) != -1) {
  // wait for termination of motion, printing motor info during motion
    while (!wbu_motion_is_over(YansheeMotion)){  
         print_motor(motor_file);
         wb_robot_step(time_step);
     }
     if (endprint) {
         
         print_motor(motor_file);
         fclose(motor_file);
         endprint = 0;
         default_position();
         
     }
  };
 
  // Cleanup resources
  wb_robot_cleanup();
  wb_camera_disable(camera);
  return 0;
}
