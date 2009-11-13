/*!\file ZMPDiscretization.h
  \brief This class generate all the values for the foot trajectories,
   and the desired ZMP based on a sequence of relative steps.
   If you want to change the reference trajectories, and the planning
   of the foot, thOn pageis is the object to modify.


   Copyright (c) 2005-2006, 
   @author Olivier Stasse,Ramzi Sellouati, Francois Keith
   
   JRL-Japan, CNRS/AIST

   All rights reserved.

  Please see License.txt for further information on license.         
*/

#ifndef _ZMP_DISCRETIZATION_H_
#define _ZMP_DISCRETIZATION_H_

#include <MatrixAbstractLayer/MatrixAbstractLayer.h>

/*! System includes */
#include <deque>
#include <string>

using namespace::std;

/*! Abstract robot dynamics includes */
#include <robotDynamics/jrlHumanoidDynamicRobot.h>

/*! Framework includes */
#include <Mathematics/PolynomeFoot.h>
#include <Mathematics/ConvexHull.h>
#include <PGTypes.h>
#include <PreviewControl/PreviewControl.h>
#include <ZMPRefTrajectoryGeneration/ZMPRefTrajectoryGeneration.h>
#include <FootTrajectoryGeneration/FootTrajectoryGenerationStandard.h>

namespace PatternGeneratorJRL
{

  /*! \brief This class is used to specify the current state of the 
    ZMP trajectory generator. 
   */
  class OnLineState
  {
    static const unsigned int IDLE_STATE = 0;
    static const unsigned int SINGLE_SUPPORT_PHASE= 1;
    static const unsigned int DOUBLE_SUPPORT_PHASE= 2;
    
  public:
    /*! \brief Default Constructor */
    OnLineState();
    /*! \brief Default destructor */
    ~OnLineState();

    /*! \brief Reading the state of the operator. */
    unsigned int operator()() const;

    /*! \brief Assigning a state to the object. */
    OnLineState & operator=(unsigned int NewState);

  private:
    unsigned int m_CurrentState;
  };

  /*! \brief Class to compute the trajectories of the ZMP reference trajectory 
    following Kajita's heuristic. 
    Basically during single support phase, the ZMP is at the center of
    the support foot. During double support phase, the ZMP is 
    on the line linking the two centers of each foot. 
  */
  class  ZMPDiscretization: public ZMPRefTrajectoryGeneration
    {
      public :
    
      /*!  Constructor */
      ZMPDiscretization(SimplePluginManager *lSPM,string DataFile="",CjrlHumanoidDynamicRobot *aHDR=0);
      
      /*!  Destructor */
      ~ZMPDiscretization();
      

      /** Generate ZMP discreatization from a vector of foot position.
	  ASSUME A COMPLETE MOTION FROM END TO START, and GENERATE EVERY VALUE.
	  
	  @param[out] ZMPPositions: Returns the ZMP reference values for the overall motion.
	  Those are absolute position in the world reference frame. The origin is the initial
	  position of the robot. The relative foot position specified are added.

	  @param[out] CoMPositions: Returns the COM reference values for the overall motion.
	  Those are absolute position in the world reference frame. The origin is the initial
	  position of the robot. The relative foot position specified are added.

	  @param[in] RelativeFootPositions: The set of 
	  relative steps to be performed by the robot.

	  @param[out] LeftFootAbsolutePositions: Returns the absolute position of the left foot.
	  According to the macro FULL_POLYNOME the trajectory will follow a third order
	  polynom or a fifth order. By experience it is wise to put a third order. 
	  A null acceleration might cause problem for the compensation of the Z-axis momentum.

	  @param[out] RightFootAbsolutePositions: Returns the absolute position of the right foot.
	  	  
	  @param[in] Xmax: The maximal distance of a hand along the X axis in the waist coordinates.

	  @param[in] lStartingCOMPosition: The initial position of the CoM.

	  @param[in] lStartingZMPPosition: The initial position of the ZMP.
	  
	  @param[in] InitLeftFootAbsolutePosition: The initial position of the left foot.
	  
	  @param[in] InitRightFootAbsolutePosition: The initial position of the right foot.

	   */
      void GetZMPDiscretization(deque<ZMPPosition> & ZMPPositions,
				deque<COMPosition> & CoMPositions,
				deque<RelativeFootPosition> &RelativeFootPositions,
				deque<FootAbsolutePosition> &LeftFootAbsolutePositions,
				deque<FootAbsolutePosition> &RightFootAbsolutePositions,
				double Xmax,
				COMPosition & lStartingCOMPosition,
				MAL_S3_VECTOR(,double) & lStartingZMPPosition,
				FootAbsolutePosition & InitLeftFootAbsolutePosition,
				FootAbsolutePosition & InitRightFootAbsolutePosition);

      /*! Dump data files. */
      void DumpDataFiles(string ZMPFileName, string FootFileName,			       
			 deque<ZMPPosition> &ZMPPositions,
			 deque<FootAbsolutePosition> &FootAbsolutePositions);

      void DumpFootAbsolutePosition(string aFileName,
				    deque<FootAbsolutePosition> &aFootAbsolutePositions);

      /** Update the value of the foot configuration according to the 
	  current situation. */
      void UpdateFootPosition(deque<FootAbsolutePosition> &SupportFootAbsolutePositions,
			      deque<FootAbsolutePosition> &NoneSupportFootAbsolutePositions,
			      int index, int k, int indexinitial, double ModulationSupportTime,int StepType,
			      int LeftOrRight);

      /*! IIR filtering of ZMP Position X put in ZMP Position Y. */
      void FilterZMPRef(deque<ZMPPosition> &ZMPPositionsX,
			deque<ZMPPosition> &ZMPPositionsY);

      /*! ZMP shift parameters to shift ZMP position during Single support with respect to the normal ankle position */
      void SetZMPShift(std::vector<double> &ZMPShift);
	
      /*! Methods for on-line generation. (First version)
	The queues will be updated as follows:
	- The first values necessary to start walking will be inserted.
	- The initial positions of the feet will be taken into account
	according to InitLeftFootAbsolutePosition and InitRightFootAbsolutePosition.
	- The RelativeFootPositions stack will be taken into account,
	- The starting COM Position.
	Returns the number of steps which has been completely put inside 
	the queue of ZMP, and foot positions.
       */
      int InitOnLine(deque<ZMPPosition> & FinalZMPPositions,
		     deque<COMPosition> & CoMPositions,
                     deque<FootAbsolutePosition> & FinalLeftFootAbsolutePositions,
		     deque<FootAbsolutePosition> & FinalRightFootAbsolutePositions,
		     FootAbsolutePosition & InitLeftFootAbsolutePosition,
		     FootAbsolutePosition & InitRightFootAbsolutePosition,
		     deque<RelativeFootPosition> &RelativeFootPositions,
		     COMPosition & lStartingCOMPosition,
		     MAL_S3_VECTOR(,double) & lStartingZMPPosition );
      
      /*! \brief  Methods to update the stacks on-line. */
      void OnLine(double time,
		  deque<ZMPPosition> & FinalZMPPositions,					     
		  deque<COMPosition> & CoMPositions,
		  deque<FootAbsolutePosition> &FinalLeftFootAbsolutePositions,
		  deque<FootAbsolutePosition> &FinalRightFootAbsolutePositions);

      /*! \brief  Methods to update the stack on-line by inserting a new foot position. */
      void OnLineAddFoot(RelativeFootPosition & NewRelativeFootPosition,
			 deque<ZMPPosition> & FinalZMPPositions,					     
			 deque<COMPosition> & CoMPositions,
			 deque<FootAbsolutePosition> &FinalLeftFootAbsolutePositions,
			 deque<FootAbsolutePosition> &FinalRightFootAbsolutePositions,
			 bool EndSequence);

      /* ! \brief Method to change on line the landing position of a foot.
	 @return If the method failed it returns -1, 0 otherwise.
      */
      int OnLineFootChange(double time,
			   FootAbsolutePosition &aFootAbsolutePosition,
			   deque<ZMPPosition> & FinalZMPPositions,			     
			   deque<COMPosition> & CoMPositions,
			   deque<FootAbsolutePosition> &FinalLeftFootAbsolutePositions,
			   deque<FootAbsolutePosition> &FinalRightFootAbsolutePositions,
			   StepStackHandler * aStepStackHandler=0);

      /*! \brief Return the time at which it is optimal to regenerate a step in online mode. 
       */
      int ReturnOptimalTimeToRegenerateAStep();
      
      /// Update the current support foot posture using the relative support foot postion RFP.
      void UpdateCurrentSupportFootPosition(RelativeFootPosition aRFP);

      /// End phase of the walking.
      void EndPhaseOfTheWalking(  deque<ZMPPosition> &ZMPPositions,
				  deque<COMPosition> &FinalCOMPositions,
				  deque<FootAbsolutePosition> &LeftFootAbsolutePositions,
				  deque<FootAbsolutePosition> &RightFootAbsolutePositions);

      /*! Filter out the ZMP values and put them at the back FinalZMPPositions. */
      void FilterOutValues(deque<ZMPPosition> &ZMPPositions,
			   deque<ZMPPosition> &FinalZMPPositions,
			   bool InitPhase);
      
      /*! Set the ZMP neutral position in the global coordinates system */
      void setZMPNeutralPosition(const double aZMPNeutralPosition[2])
      { 
	m_ZMPNeutralPosition[0] = aZMPNeutralPosition[0];
	m_ZMPNeutralPosition[1] = aZMPNeutralPosition[1];
      }
      
    private:
      
      /*! \brief Register Methods for scripting. 
	This method register prevzmpinitprofil, zeroinitprofil,
	and previewcontroltime as accessible through scripting.
       */
      void RegisterMethodsForScripting();

      /*! \brief Initialize filter */
      void InitializeFilter();
      

      /*! \brief Reset a data file from its name. */
      void ResetADataFile(string &aDataFile);

      /*! \brief Dump references */
      void DumpReferences(deque<ZMPPosition> &FinalZMPPositions,
			  deque<ZMPPosition> &ZMPPositions);

      /* ! ModulationSupportCoefficient coeeficient to wait a little before foot is of the ground */
      double m_ModulationSupportCoefficient;

      /* ! Polynome to generate trajectories. */
      Polynome3 *m_PolynomeZMPTheta;
      
      /* ! ZMP shift parameters to shift ZMP position during 
	 Single support with respect to the normal ankle position */
      std::vector<double> m_ZMPShift;
      
      /*! Neutral ZMP position. */
      double m_ZMPNeutralPosition[2];
      
      /* ! Current absolute orientation of the Waist. */
      double m_CurrentAbsTheta;
      
      /* ! Current orientation of the support foot. */
      double m_AngleDiffToSupportFootTheta;

      /* ! Current angle difference from the ZMP to the support foot. */
      double m_AngleDiffFromZMPThetaToSupportFootTheta;
      
      /*! \name Handling the support foot position 
	@{
       */
      /* ! \brief Current absolute support position in 2D (but 
	 with homogeneous coordinates). */
      MAL_MATRIX(m_CurrentSupportFootPosition,double);

      /* ! \brief Previous position of the support foot */ 
      MAL_MATRIX(m_PrevCurrentSupportFootPosition,double);

      /*! @} */

      /* ! Window for the filtering of the ZMP positions.. */
      std::vector<double> m_ZMPFilterWindow;
      
      /* ! Keep a stack of two steps as a reference before sending them to the 
      external queues. */
      deque<RelativeFootPosition> m_RelativeFootPositions;
      
      /* ! Keep track of the time. */
      double m_CurrentTime;

      /* ! Keep track of the previous foot support position. */
      MAL_MATRIX(m_vdiffsupppre,double);
      
      /*!  Keep an object which relates the specificities
	with an abstract layer. */
      CjrlHumanoidDynamicRobot *m_HS;
      
      /* !  Matrices for the dynamical system. */
      MAL_MATRIX(m_A,double);
      MAL_MATRIX(m_B,double);
      MAL_MATRIX(m_C,double);
      
      /*! Object to handle foot trajectory generation */
      FootTrajectoryGenerationStandard * m_FootTrajectoryGenerationStandard;

      /*! Object to handle the preview control (mostly to get the CoM height). */
      PreviewControl *m_PC;

      /*! Initialization Profile */
      int m_InitializationProfile;

    public:

      const static int PREV_ZMP_INIT_PROFIL = 1;
      const static int ZERO_INIT_PROFIL = 2;
      /*! \brief Get the reference to the preview control object. */
      PreviewControl * GetPreviewControl()
      { return m_PC;}

      /*! \brief Set the reference to the preview control object. */
      int  SetPreviewControl(PreviewControl *aPC)
      { m_PC=aPC; return 0;}

      /*! \brief Provide the plugin functionnality. */
      void CallMethod(std::string &Method, std::istringstream &strm);


   };
};
#include <PreviewControl/PreviewControl.h>
#endif /* _FOOT_PRINT_H_*/