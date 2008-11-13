#ifndef WALKPLUGINJRL_IDL_FILE
#define WALKPLUGINJRL_IDL_FILE

#include "${COMMON_IDL_OPENHRP}"

#if ${OPENHRP_VERSION_3}
/**
 * Position and Attitude
 * @param	px	x coordinates
 * @param	py	y coordinates
 * @param	pz	z coordinates
 * @param	qx	x axis
 * @param	qy	y axis
 * @param	qz	z axis
 * @param	qw	w axis
 */
struct TransformQuaternion
{
    float px;
    float py;
    float pz;
    float qx;
    float qy;
    float qz;
    float qw;
};

typedef sequence<double> dsequence;
#endif

interface walkpluginJRL : ${PLUGIN_NAMESPACE}Plugin
{
  /***********************************
   *
   *         Relative Motion
   *
   ***********************************/

  // Set the Target Position 
  // Function returns upon the robots arrival at destination. Must not
  // be used in the real-time part of a plugin
  void setTargetPos(in float x, in float y, in float th);

  // Set the Target Position without waiting for sequence
  // interpolation to finish. Takes time depending on distance so can
  // not be used in the real-time part of a plugin.
  void setTargetPosNoWait(in float x, in float y, in float th);

  // Set Arc
  // Function returns upon robots arrival at destination. Must not be
  // used in real-time part of plugin.
  void setArc(in float x, in float y, in float th);

  // Set Arc without waiting for sequence interpolation to
  // finish. Takes time depending on the distance so can not be used
  // in the real-time part of a plugin.
  void setArcNoWait(in float x, in float y, in float th);

  // Set the Right foot Position
  // Function returns upon robots arrival at destination. Must not be
  // used in real-time part of plugin.
  void setRfootPos(in float x, in float y, in float th);

  // Set the Right foot Position without waiting for sequence interpolation to
  // finish. Takes time depending on the distance so can not be used
  // in the real-time part of a plugin.
  void setRfootPosNoWait(in float x, in float y, in float th);

  // Set the Left foot Position
  // Function returns upon robots arrival at destination. Must not be
  // used in real-time part of plugin.
  void setLfootPos(in float x, in float y, in float th);

  // Set the Left foot Position without waiting for sequence interpolation to
  // finish. Takes time depending on the distance so can not be used
  // in the real-time part of a plugin.
  void setLfootPosNoWait(in float x, in float y, in float th);

  /************************************
   *
   *    Get low level information
   *
   ************************************/
  /// Get joint speed of the legs.
  long getLegJointSpeed(out ${DOUBLE_SEQUENCE} dq);

  /************************************
   *
   *        Walk Start/Stop
   *
   ************************************/
  // Stop Walking

  // Function returns when sequence interpolation has finished and
  // robot has halted. Must not be used in real-time part of plugin.
  void stopWalking();

  // Wait for robot to arrive at designated goal. Returns when robot
  // has come to a complete stop. Must not be used in real-time part
  // of plugin.
  void waitArrival();

  /************************************
   *
   *        Stepping
   *
   ************************************/
  // Start Stepping
  // Function returns immediately so can be used in real-time part of
  // plugin.
  void startStepping();

  // Stop Stepping
  // Function returns only after robot has come to a complete
  // stop. Function can not be used in real-time of plugin.
  void stopStepping();

  // Set parameter which make robot walk forwards. 
  // Function returns immediately so can be used in real-time part of
  // plugin.
  void setWalkingVelocity(in float dx, in float dy, in float dth);

  /**************************************
   *
   *         W a i s t
   *
   **************************************/
  // Get Waist Velocity 
  // Returns immediately so can be used in the real-time part of a
  // plugin
  void getWaistVelocity(out float vx, out float vy, out float omega);
  
  // Get Waist position and orientation.
  // Returns immediately so can be used in the real-time part of a 
  // plugin
  void getWaistPositionAndOrientation(out TransformQuaternion aTQ,
				      out float Orientation);



  // Set Waist position and orientation.
  // This function just reset the absolute reference.
  // Returns immediately so can be used in the real-time part of a 
  // plugin
  void setWaistPositionAndOrientation(in TransformQuaternion aTQ);

  
  // Get Waist acceleration
  // Returns immediately so can be used in the real-time part of a 
  // plugin
  void getWaistAcceleration(out TransformQuaternion aTQ);

  boolean isWalking();

  /*!******************************************************
   *
   * Interface specific to JRL Walking Pattern Generator
   * 
   *******************************************************/

  /*! \brief This method allows to send directly to the Pattern Generator control commands.
   */
  boolean ParseCmd(in string aCmd);

};

#endif /* WALKPLUGINJRL_IDL_FILE */