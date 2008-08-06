/*! \file FootTrajectoryGenerationMultiple.h
  \brief This object is in charge of maintaning the foot trajectory 
  generation for several intervals. 
  It relies on the FootTrajectoryGenerationStandard class.

  @ingroup foottrajectorygeneration

  Copyright (c) 2007,
  @author Olivier Stasse,
     
  JRL-Japan, CNRS/AIST

  All rights reserved.
   
  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:
   
  * Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  * Neither the name of the CNRS/AIST nor the names of its contributors 
  may be used to endorse or promote products derived from this software without specific prior written permission.
   
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS 
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER 
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FOOT_TRAJECTORY_GENERATION_MULTIPLE_H_

/* Walking pattern generation related inclusions */
#include <walkGenJrl/FootTrajectoryGeneration/FootTrajectoryGenerationStandard.h>

namespace PatternGeneratorJRL
{

  /*! @ingroup foottrajectorygeneration
      
      This class generates a trajectory for a complete leg relying on a
      set of description of the intervals.
      More precisely this object handles a set of FootTrajectoryGenerationStandard
      objects.
      Thus it acts as a container, and allow a coherent interface to a set
      of foot trajectory.

      Each interval is described by a time duration \f[ \Delta T_j \f],
      its nature which can be double support and in  single support 
      two subcategories exist: support foot and flying.
      
      
   */
  class FootTrajectoryGenerationMultiple : public SimplePlugin
  {
  public :

    /*! \name Constants to define the nature of the foot trajectory.
     @{
    */
    /*! Double support foot. */
    const static int DOUBLE_SUPPORT=0;

    /*! \name Single support foot subcategories
      @{ */
    /*! \brief The robot is in single support and the foot considered is the support foot */
    const static int SINGLE_SUPPORT_SUPPORT=1;
    /*! \brief The robot is in single support and the foot considered is flying. */
    const static int SINGLE_SUPPORT_FLYING=2;

    /*! @} */
    /*! @} */
    /*! Constructor: In order to compute some appropriate strategies,
      this class needs to extract specific details from the humanoid model. */
    FootTrajectoryGenerationMultiple(SimplePluginManager *lSPM,
				     dynamicsJRLJapan::HumanoidSpecificities *aHS);

    // Default destructor
    ~FootTrajectoryGenerationMultiple();
    
    /*! \brief Reimplementation of the call method for the plugin manager. 
      More explicitly this object will deal with the call which initialize
      the feet behaviors (\f$omega\f$, \f$ stepheight \f$) .
    */
    virtual void CallMethod(std::string &Method, std::istringstream &strm);

    
    /*! \name Methods related to the handling of the intervals.
      @{
     */
    /*! \brief Set number of intervals. */
    void SetNumberOfIntervals(int lNumberOfIntervals);

    /*! \brief Get number of intervals. */
    int GetNumberOfIntervals();

    /*! \brief Set the time for each interval. */
    void SetTimeIntervals(vector<double> & lDeltaTj);
    
    /*! \brief Get the time for each interval */
    void GetTimeIntervals(vector<double>  & lDeltaTj);
    
    /*! @} */
    
    /*! \brief Compute the value asked for according to :
      @param[in] axis: the axis along which the computation is done,
      @param[in] t: the time,
      @param[out] r: the result.
    */
    bool Compute(int axis, double t, double & r);
    
    /*! \brief Compute the value asked for according to :
      @param[in] t: the time,
      @param[out] aFootAbsolutePosition: a foot absolute position.
    */
    bool Compute(double t, FootAbsolutePosition & aFootAbsolutePosition);

    /*! \brief Compute the value asked for according to :
      @param[in] t: the time,
      @param[in] IndexInterval: Index of the interval to be used for the computation.
      @param[out] aFootAbsolutePosition: a foot absolute position.
    */
    bool Compute(double t, FootAbsolutePosition & aFootAbsolutePosition, unsigned int IndexInterval);
    
    /*! This method specifies the parameters for each of the polynome used by this
      object. In this case, as it is used for the 3rd order polynome. The polynome to
      which those parameters are set is specified with PolynomeIndex. 
      @param PolynomeIndex: Set to which axis the parameters will be applied. 
      @param AxisReference: Index to the axis to be used.
      @param TimeInterval: Set the time base of the polynome.
      @param FinalPosition: Set the final position of the polynome at TimeInterval.
      @param InitPosition: Initial position when computing the polynome at t= m_AbsoluteTimeReference.
      @param InitSpeed: Initial speed when computing the polynome at t=m_AbsoluteTimeReference.
    */
   int SetParametersWithInitPosInitSpeed(unsigned int PolynomeIndex,
					 int AxisReference,
					 double TimeInterval,
					 double FinalPosition,
					 double InitPosition,
					 double InitSpeed);

   /*! \name Methods related to the Absolute Time Reference. 
     This time specifies the beginning of the trajectory. 
     @{ */

   /*! Returns the time when the trajectory starts. */
   double GetAbsoluteTimeReference();

   /*! Set the time when the trajectory starts.  */
   void SetAbsoluteTimeReference(double lAbsoluteTimeReference);

   /*! @} */
   

  protected:

    /*! \brief Handle a set of object allowing the generation of the foot trajectory.*/
    vector<FootTrajectoryGenerationStandard *> m_SetOfFootTrajectoryGenerationObjects;

    /*! \brief Reference of humanoid specificities. */ 
    dynamicsJRLJapan::HumanoidSpecificities * m_HS;

    /*! \brief Set the absolute reference time for this set of intervals. */
    double m_AbsoluteTimeReference;

    /*! \brief Set of interval times. */
    vector<double> m_DeltaTj;

    /*! \brief Nature of the interval. */
    vector<int> m_NatureOfTheIntervals;

    /*! \brief Reference time for the polynomials. */
    vector<double> m_RefTime;
    
  };
};
#endif /* _FOOT_TRAJECTORY_GENERATION_MULTIPLE_H_ */