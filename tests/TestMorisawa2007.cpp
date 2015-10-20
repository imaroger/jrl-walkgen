/*
 * Copyright 2010,
 *
 * Olivier Stasse
 *
 * JRL, CNRS/AIST
 *
 * This file is part of jrl-walkgen.
 * jrl-walkgen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jrl-walkgen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with jrl-walkgen.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Research carried out within the scope of the
 *  Joint Japanese-French Robotics Laboratory (JRL)
 */
/* \file This file tests M. Morisawa's walking algorithm for
 * real-time CoM and ZMP trajectory generation
 */

#include "CommonTools.hh"
#include "TestObject.hh"
#include <metapod/models/hrp2_14/hrp2_14.hh>
#include <ZMPRefTrajectoryGeneration/DynamicFilter.hh>
#include <metapod/algos/rnea.hh>
#ifndef METAPOD_INCLUDES
#define METAPOD_INCLUDES
// metapod includes
#include <metapod/tools/print.hh>
#include <metapod/tools/initconf.hh>
#include <metapod/algos/rnea.hh>
#include <Eigen/StdVector>
#endif


#ifndef METAPOD_TYPEDEF2
#define METAPOD_TYPEDEF2
typedef double LocalFloatType2;
typedef metapod::Spatial::ForceTpl<LocalFloatType2> Force2;
typedef metapod::hrp2_14<LocalFloatType2> Robot_Model2;
typedef metapod::Nodes< Robot_Model2, Robot_Model2::BODY >::type Node2;
#endif

using namespace::PatternGeneratorJRL;
using namespace::PatternGeneratorJRL::TestSuite;
using namespace std;

enum Profiles_t {
  PROFIL_ANALYTICAL_ONLINE_WALKING,          // 1
  PROFIL_ANALYTICAL_SHORT_STRAIGHT_WALKING,  // 2
  PROFIL_ANALYTICAL_CLIMBING_STAIRS,         // 3
  PROFIL_ANALYTICAL_GOING_DOWN_STAIRS,       // 4
  PROFIL_ANALYTICAL_STEPPING_STONES,         // 5
  PROFIL_ANALYTICAL_WALKING_ON_BEAM,         // 6
  PROFIL_ANALYTICAL_GO_THROUGH_WALL          // 7
};

#define NBOFPREDEFONLINEFOOTSTEPS 11

double OnLineFootSteps[NBOFPREDEFONLINEFOOTSTEPS][4]={
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0},
 { 0.05, 0.0, 0.0, 0.0}
};

class TestMorisawa2007: public TestObject
{

private:
  bool m_TestChangeFoot;
  unsigned long int m_NbStepsModified;
  // New time between two steps.
  double m_deltatime;

  ComAndFootRealizationByGeometry * ComAndFootRealization_;
  /// Class that compute the dynamic and kinematic of the robot
  CjrlHumanoidDynamicRobot * cjrlHDR_ ;
  Robot_Model hrp2_14_ ;
  Robot_Model::confVector q_,dq_,ddq_;
  Force_HRP2_14 com_tensor_ ;

  SimplePluginManager * SPM ;

  int iteration ;
  vector<double> err_zmp_x ;
  vector<double> err_zmp_y ;


public:
  TestMorisawa2007(int argc, char*argv[], string &aString, int TestProfile):
      TestObject(argc, argv, aString)
  {
    m_TestProfile = TestProfile;
    m_TestChangeFoot = true;
    m_NbStepsModified = 0;
    m_deltatime = 0;

    SPM = 0 ;
    ComAndFootRealization_ = 0 ;
    iteration = 0 ;
    err_zmp_x.clear() ;
    err_zmp_y.clear() ;
  }

  ~TestMorisawa2007()
  {
    if ( ComAndFootRealization_ != 0 )
    {
      delete ComAndFootRealization_ ;
      ComAndFootRealization_ = 0 ;
    }
    if ( SPM != 0 )
    {
      delete SPM ;
      SPM = 0 ;
    }
  }

  bool doTest(ostream &os)
  {

    // Set time reference.
    m_clock.startingDate();

    /*! Open and reset appropriatly the debug files. */
    prepareDebugFiles();

    for (unsigned int lNbIt=0;lNbIt<m_OuterLoopNbItMax;lNbIt++)
      {
        os << "<===============================================================>"<<endl;
        os << "Iteration nb: " << lNbIt << endl;

        m_clock.startPlanning();

	/*! According to test profile initialize the current profile. */
	chooseTestProfile();

	m_clock.endPlanning();

	if (m_DebugHDR!=0)
	  {
	    m_DebugHDR->currentConfiguration(m_PreviousConfiguration);
	    m_DebugHDR->currentVelocity(m_PreviousVelocity);
	    m_DebugHDR->currentAcceleration(m_PreviousAcceleration);
	    m_DebugHDR->computeForwardKinematics();
	  }

	bool ok = true;
	while(ok)
	  {
	    m_clock.startOneIteration();

	    if (m_PGIInterface==0)
	      {
		ok = m_PGI->RunOneStepOfTheControlLoop(m_CurrentConfiguration,
						       m_CurrentVelocity,
						       m_CurrentAcceleration,
						       m_OneStep.ZMPTarget,
						       m_OneStep.finalCOMPosition,
						       m_OneStep.LeftFootPosition,
						       m_OneStep.RightFootPosition);
	      }
	    else if (m_PGIInterface==1)
	      {
		ok = m_PGI->RunOneStepOfTheControlLoop(m_CurrentConfiguration,
						       m_CurrentVelocity,
						       m_CurrentAcceleration,
						       m_OneStep.ZMPTarget);
	      }

	    m_OneStep.NbOfIt++;

	    m_clock.stopOneIteration();

	    m_PreviousConfiguration = m_CurrentConfiguration;
	    m_PreviousVelocity = m_CurrentVelocity;
	    m_PreviousAcceleration = m_CurrentAcceleration;

	    /*! Call the reimplemented method to generate events. */
	    if (ok)
	      {
		m_clock.startModification();
		generateEvent();
		m_clock.stopModification();

		m_clock.fillInStatistics();


		/*! Fill the debug files with appropriate information. */
		fillInDebugFiles();
	      }
	    else
	      {
		cerr << "Nothing to dump after " << m_OneStep.NbOfIt << endl;
	      }

	  }

	os << endl << "End of iteration " << lNbIt << endl;
	os << "<===============================================================>"<<endl;
      }

    ComputeAndDisplayZMPStatistic();
    string lProfileOutput= m_TestName;
    lProfileOutput +="TimeProfile.dat";
    m_clock.writeBuffer(lProfileOutput);
    m_clock.displayStatistics(os,m_OneStep);
    // Compare debugging files
    return compareDebugFiles();
  }

  void ComputeStat(vector<double> vec,double &avg, double &max_abs)
  {
    double total = 0.0 ;
    avg = 0.0 ;
    max_abs = 0.0 ;
    for (unsigned int i = 0 ; i < vec.size() ; ++i)
    {
      double abs_value = sqrt(vec[i]*vec[i]) ;
      if( abs_value > max_abs)
        max_abs = abs_value ;

      total += abs_value ;
    }
    avg = total/vec.size() ;
    return ;
  }

  void ComputeAndDisplayZMPStatistic()
  {
    cout << "Statistic for Dzmp in x : " << endl ;
    double moy_delta_zmp_x = 0.0 ;
    double max_abs_err_x = 0.0 ;
    ComputeStat(err_zmp_x,moy_delta_zmp_x,max_abs_err_x);
    cout << "average : " << moy_delta_zmp_x << endl ;
    cout << "maxx error : " << max_abs_err_x << endl ;

    cout << "Statistic for Dzmp in y : " << endl ;
    double moy_delta_zmp_y = 0.0 ;
    double max_abs_err_y = 0.0 ;
    ComputeStat(err_zmp_y,moy_delta_zmp_y,max_abs_err_y);
    cout << "average : " << moy_delta_zmp_y << endl ;
    cout << "maxx error : " << max_abs_err_y << endl ;
    return ;
  }

  void init()
  {
    // Instanciate and initialize.
    string RobotFileName = m_VRMLPath + m_VRMLFileName;

    bool fileExist = false;
    {
      std::ifstream file (RobotFileName.c_str ());
      fileExist = !file.fail ();
    }
    if (!fileExist)
      throw std::string ("failed to open robot model");

    CreateAndInitializeHumanoidRobot(RobotFileName,
                                     m_SpecificitiesFileName,
                                     m_LinkJointRank,
                                     m_InitConfig,
                                     m_HDR, m_DebugHDR, m_PGI);

    // Specify the walking mode: here the default one.
    istringstream strm2(":walkmode 0");
    m_PGI->ParseCmd(strm2);

    MAL_VECTOR_RESIZE(m_CurrentConfiguration, m_HDR->numberDof());
    MAL_VECTOR_RESIZE(m_CurrentVelocity, m_HDR->numberDof());
    MAL_VECTOR_RESIZE(m_CurrentAcceleration, m_HDR->numberDof());

    MAL_VECTOR_RESIZE(m_PreviousConfiguration, m_HDR->numberDof());
    MAL_VECTOR_RESIZE(m_PreviousVelocity, m_HDR->numberDof());
    MAL_VECTOR_RESIZE(m_PreviousAcceleration, m_HDR->numberDof());

    for (unsigned int i = 18 ; i < 35 ; ++i)
    {
      m_CurrentConfiguration (i) = InitialPosition (i-6) ;
      m_PreviousConfiguration (i) = InitialPosition (i-6) ;
    }

    SPM = new SimplePluginManager();

    ComAndFootRealization_ = new ComAndFootRealizationByGeometry( (PatternGeneratorInterfacePrivate*) SPM );
    ComAndFootRealization_->setHumanoidDynamicRobot(m_HDR);
    ComAndFootRealization_->SetStepStackHandler(new StepStackHandler(SPM));
    ComAndFootRealization_->SetHeightOfTheCoM(0.814);
    ComAndFootRealization_->setSamplingPeriod(0.005);
    ComAndFootRealization_->Initialization();

    initIK();

    {
      istringstream strm2(":setfeetconstraint XY 0.09 0.04");
      m_PGI->ParseCmd(strm2);
    }

  }

protected:

  double filterprecision(double adb)
  {
    if (fabs(adb)<1e-7)
    return 0.0;

    double ladb2 = adb * 1e7;
    double lintadb2 = trunc(ladb2);
    return lintadb2/1e7;
  }

  void initIK()
  {
    MAL_VECTOR_DIM(BodyAngles,double,MAL_VECTOR_SIZE(InitialPosition));
    MAL_VECTOR_DIM(waist,double,6);
    for (int i = 0 ; i < 6 ; ++i )
    {
      waist(i) = 0;
    }
    for (unsigned int i = 0 ; i < (m_HDR->numberDof()-6) ; ++i )
    {
      BodyAngles(i) = InitialPosition(i);
    }
    MAL_S3_VECTOR(lStartingCOMState,double);

    lStartingCOMState(0) = m_OneStep.finalCOMPosition.x[0] ;
    lStartingCOMState(1) = m_OneStep.finalCOMPosition.y[0] ;
    lStartingCOMState(2) = m_OneStep.finalCOMPosition.z[0] ;
    ComAndFootRealization_->SetHeightOfTheCoM(0.814);
    ComAndFootRealization_->setSamplingPeriod(0.005);
    ComAndFootRealization_->Initialization();

    ComAndFootRealization_->InitializationCoM(BodyAngles,lStartingCOMState,
                                              waist,
                                              m_OneStep.LeftFootPosition, m_OneStep.RightFootPosition);
    ComAndFootRealization_->Initialization();

    for (int i = 0 ; i < 6 ; ++i )
    {
      m_CurrentConfiguration(i) = waist(i);
    }
    m_HDR->currentConfiguration(m_CurrentConfiguration);
  }

  void fillInDebugFiles()
  {
    TestObject::fillInDebugFiles();

    /// \brief calculate, from the CoM of computed by the preview control,
    ///    the corresponding articular position, velocity and acceleration
    /// ------------------------------------------------------------------
    MAL_VECTOR_DIM(aCOMState,double,6);
    MAL_VECTOR_DIM(aCOMSpeed,double,6);
    MAL_VECTOR_DIM(aCOMAcc,double,6);
    MAL_VECTOR_DIM(aLeftFootPosition,double,5);
    MAL_VECTOR_DIM(aRightFootPosition,double,5);

    aCOMState(0) = m_OneStep.finalCOMPosition.x[0];      aCOMSpeed(0) = m_OneStep.finalCOMPosition.x[1];      aCOMAcc(0) = m_OneStep.finalCOMPosition.x[2];
    aCOMState(1) = m_OneStep.finalCOMPosition.y[0];      aCOMSpeed(1) = m_OneStep.finalCOMPosition.y[1];      aCOMAcc(1) = m_OneStep.finalCOMPosition.y[2];
    aCOMState(2) = m_OneStep.finalCOMPosition.z[0];      aCOMSpeed(2) = m_OneStep.finalCOMPosition.z[1];      aCOMAcc(2) = m_OneStep.finalCOMPosition.z[2];
    aCOMState(3) = m_OneStep.finalCOMPosition.roll[0]  * 180/M_PI  ;  aCOMSpeed(3) = m_OneStep.finalCOMPosition.roll[1] /** * 180/M_PI  */ ;  aCOMAcc(3) = m_OneStep.finalCOMPosition.roll[2]/** * 180/M_PI  */ ;
    aCOMState(4) = m_OneStep.finalCOMPosition.pitch[0] * 180/M_PI  ;  aCOMSpeed(4) = m_OneStep.finalCOMPosition.pitch[1]/** * 180/M_PI  */ ;  aCOMAcc(4) = m_OneStep.finalCOMPosition.pitch[2]/** * 180/M_PI  */ ;
    aCOMState(5) = m_OneStep.finalCOMPosition.yaw[0] *180/M_PI;  aCOMSpeed(5) = m_OneStep.finalCOMPosition.yaw[1]/** * 180/M_PI  */ ; aCOMAcc(5) = m_OneStep.finalCOMPosition.yaw[2] /** * 180/M_PI  */;

    aLeftFootPosition(0) = m_OneStep.LeftFootPosition.x;      aRightFootPosition(0) = m_OneStep.RightFootPosition.x;
    aLeftFootPosition(1) = m_OneStep.LeftFootPosition.y;      aRightFootPosition(1) = m_OneStep.RightFootPosition.y;
    aLeftFootPosition(2) = m_OneStep.LeftFootPosition.z;      aRightFootPosition(2) = m_OneStep.RightFootPosition.z;
    aLeftFootPosition(3) = m_OneStep.LeftFootPosition.theta;  aRightFootPosition(3) = m_OneStep.RightFootPosition.theta;
    aLeftFootPosition(4) = m_OneStep.LeftFootPosition.omega;  aRightFootPosition(4) = m_OneStep.RightFootPosition.omega;
    ComAndFootRealization_->setSamplingPeriod(0.005);
    ComAndFootRealization_->ShiftFoot(false);
    ComAndFootRealization_->ComputePostureForGivenCoMAndFeetPosture(aCOMState, aCOMSpeed, aCOMAcc,
                                                                    aLeftFootPosition,
                                                                    aRightFootPosition,
                                                                    m_CurrentConfiguration,
                                                                    m_CurrentVelocity,
                                                                    m_CurrentAcceleration,
                                                                    20,
                                                                    1);

    m_CurrentConfiguration(28)= 0.174532925 ;     // RARM_JOINT6
    m_CurrentConfiguration(35)= 0.174532925 ;     // LARM_JOINT6

    // carry the weight in front of him
//    m_CurrentConfiguration(18)= 0.0 ;            // CHEST_JOINT0
//    m_CurrentConfiguration(19)= 0.015 ;            // CHEST_JOINT1
//    m_CurrentConfiguration(20)= 0.0 ;            // HEAD_JOINT0
//    m_CurrentConfiguration(21)= 0.0 ;            // HEAD_JOINT1
//    m_CurrentConfiguration(22)= -0.108210414 ;   // RARM_JOINT0
//    m_CurrentConfiguration(23)= 0.0383972435 ;    // RARM_JOINT1
//    m_CurrentConfiguration(24)= 0.474729557 ;     // RARM_JOINT2
//    m_CurrentConfiguration(25)= -1.41720735 ;    // RARM_JOINT3
//    m_CurrentConfiguration(26)= 1.45385927 ;     // RARM_JOINT4
//    m_CurrentConfiguration(27)= 0.509636142 ;     // RARM_JOINT5
//    m_CurrentConfiguration(28)= 0.174532925 ;     // RARM_JOINT6
//    m_CurrentConfiguration(29)= -0.108210414 ;    // LARM_JOINT0
//    m_CurrentConfiguration(30)= -0.129154365 ;    // LARM_JOINT1
//    m_CurrentConfiguration(31)= -0.333357887 ;    // LARM_JOINT2
//    m_CurrentConfiguration(32)= -1.41720735 ;     // LARM_JOINT3
//    m_CurrentConfiguration(33)= 1.45385927 ;      // LARM_JOINT4
//    m_CurrentConfiguration(34)= -0.193731547 ;    // LARM_JOINT5
//    m_CurrentConfiguration(35)= 0.174532925 ;     // LARM_JOINT6

//      // carry the weight over the head
//      m_CurrentConfiguration(18)= 0.0 ;            // CHEST_JOINT0
//      m_CurrentConfiguration(19)= 0.015 ;          // CHEST_JOINT1
//      m_CurrentConfiguration(20)= 0.0 ;            // HEAD_JOINT0
//      m_CurrentConfiguration(21)= 0.0 ;            // HEAD_JOINT1
//      m_CurrentConfiguration(22)= -1.4678219 ;     // RARM_JOINT0
//      m_CurrentConfiguration(23)= 0.0366519143 ;   // RARM_JOINT1
//      m_CurrentConfiguration(24)= 0.541052068 ;    // RARM_JOINT2
//      m_CurrentConfiguration(25)= -1.69296937 ;    // RARM_JOINT3
//      m_CurrentConfiguration(26)= 1.56556034 ;     // RARM_JOINT4
//      m_CurrentConfiguration(27)= 0.584685299 ;    // RARM_JOINT5
//      m_CurrentConfiguration(28)= 0.174532925 ;    // RARM_JOINT6
//      m_CurrentConfiguration(29)= -1.4678219 ;     // LARM_JOINT0
//      m_CurrentConfiguration(30)= -0.0366519143 ;  // LARM_JOINT1
//      m_CurrentConfiguration(31)= -0.541052068 ;   // LARM_JOINT2
//      m_CurrentConfiguration(32)= -1.69296937 ;    // LARM_JOINT3
//      m_CurrentConfiguration(33)= -1.56556034 ;     // LARM_JOINT4
//      m_CurrentConfiguration(34)= 0.584685299 ;    // LARM_JOINT5
//      m_CurrentConfiguration(35)= 0.174532925 ;    // LARM_JOINT6

//      // walk throug wall
//      m_CurrentConfiguration(18)= 0.0 ;            // CHEST_JOINT0
//      m_CurrentConfiguration(19)= -0.09 ;          // CHEST_JOINT1
//      m_CurrentConfiguration(20)= 0.0 ;            // HEAD_JOINT0
//      m_CurrentConfiguration(21)= -0.37 ;          // HEAD_JOINT1
//      m_CurrentConfiguration(22)= 0.0 ;            // RARM_JOINT0
//      m_CurrentConfiguration(23)= -M_PI/2 ;        // RARM_JOINT1
//      m_CurrentConfiguration(24)= 0.0 ;            // RARM_JOINT2
//      m_CurrentConfiguration(25)= 0.0 ;            // RARM_JOINT3
//      m_CurrentConfiguration(26)= 0.0 ;            // RARM_JOINT4
//      m_CurrentConfiguration(27)= 0.0 ;            // RARM_JOINT5
//      m_CurrentConfiguration(28)= 0.174532925 ;    // RARM_JOINT6
//      m_CurrentConfiguration(29)= 0.0 ;            // LARM_JOINT0
//      m_CurrentConfiguration(30)= M_PI/2 ;         // LARM_JOINT1
//      m_CurrentConfiguration(31)= 0.0 ;            // LARM_JOINT2
//      m_CurrentConfiguration(32)= 0.0 ;            // LARM_JOINT3
//      m_CurrentConfiguration(33)= 0.0 ;            // LARM_JOINT4
//      m_CurrentConfiguration(34)= 0.0 ;            // LARM_JOINT5
//      m_CurrentConfiguration(35)= 0.174532925 ;    // LARM_JOINT6

    // init config of the upper body
//    m_CurrentConfiguration(18)= m_HDR->currentConfiguration()(18) ; // CHEST_JOINT0
//    m_CurrentConfiguration(19)= m_HDR->currentConfiguration()(19) ; // CHEST_JOINT1
//    m_CurrentConfiguration(20)= m_HDR->currentConfiguration()(20) ; // HEAD_JOINT0
//    m_CurrentConfiguration(21)= m_HDR->currentConfiguration()(21) ; // HEAD_JOINT1
//    m_CurrentConfiguration(22)= m_HDR->currentConfiguration()(22) ; // RARM_JOINT0
//    m_CurrentConfiguration(23)= m_HDR->currentConfiguration()(23) ; // RARM_JOINT1
//    m_CurrentConfiguration(24)= m_HDR->currentConfiguration()(24) ; // RARM_JOINT2
//    m_CurrentConfiguration(25)= m_HDR->currentConfiguration()(25) ; // RARM_JOINT3
//    m_CurrentConfiguration(26)= m_HDR->currentConfiguration()(26) ; // RARM_JOINT4
//    m_CurrentConfiguration(27)= m_HDR->currentConfiguration()(27) ; // RARM_JOINT5
//    m_CurrentConfiguration(28)= m_HDR->currentConfiguration()(28) ; // RARM_JOINT6
//    m_CurrentConfiguration(29)= m_HDR->currentConfiguration()(29) ; // LARM_JOINT0
//    m_CurrentConfiguration(30)= m_HDR->currentConfiguration()(30) ; // LARM_JOINT1
//    m_CurrentConfiguration(31)= m_HDR->currentConfiguration()(31) ; // LARM_JOINT2
//    m_CurrentConfiguration(32)= m_HDR->currentConfiguration()(32) ; // LARM_JOINT3
//    m_CurrentConfiguration(33)= m_HDR->currentConfiguration()(33) ; // LARM_JOINT4
//    m_CurrentConfiguration(34)= m_HDR->currentConfiguration()(34) ; // LARM_JOINT5
//    m_CurrentConfiguration(35)= m_HDR->currentConfiguration()(35) ; // LARM_JOINT6


    // compute the 6D force applied at the CoM
    for(unsigned int i = 0 ; i < MAL_VECTOR_SIZE(m_CurrentConfiguration) ; ++i)
      {
        q_(i,0)   = m_CurrentConfiguration (i);
        dq_(i,0)  = m_CurrentVelocity      (i);
        ddq_(i,0) = m_CurrentAcceleration  (i);
      }
    metapod::rnea< Robot_Model, true >::run(hrp2_14_, q_, dq_, ddq_);
    vector<double> zmpmb = vector<double>(3,0.0);
    // extract the CoM momentum and forces
    RootNode & node_waist = boost::fusion::at_c< Robot_Model::BODY >(hrp2_14_.nodes);
    com_tensor_ = node_waist.body.iX0.applyInv(node_waist.joint.f);

    // compute the Multibody ZMP
    zmpmb[0] = - com_tensor_.n()[1] / com_tensor_.f()[2] ;
    zmpmb[1] =   com_tensor_.n()[0] / com_tensor_.f()[2] ;

    err_zmp_x.push_back(zmpmb[0]-m_OneStep.ZMPTarget(0)) ;
    err_zmp_y.push_back(zmpmb[1]-m_OneStep.ZMPTarget(1)) ;

    if (m_DebugFGPI)
      {
        ofstream aof;
        string aFileName;
        aFileName = m_TestName;
        aFileName += "TestFGPIFull.dat";
        if (m_OneStep.NbOfIt==1)
          {
            aof.open(aFileName.c_str(),ofstream::out);
          }
        aof.open(aFileName.c_str(),ofstream::app);
        aof.precision(8);
        aof.setf(ios::scientific, ios::floatfield);
        aof << filterprecision(m_OneStep.NbOfIt*0.005 ) << " "                            // 1
            << filterprecision(m_OneStep.finalCOMPosition.x[0] ) << " "                   // 2
            << filterprecision(m_OneStep.finalCOMPosition.y[0] ) << " "                   // 3
            << filterprecision(m_OneStep.finalCOMPosition.z[0] ) << " "                   // 4
            << filterprecision(m_OneStep.finalCOMPosition.yaw[0] ) << " "                 // 5
            << filterprecision(m_OneStep.finalCOMPosition.x[1] ) << " "                   // 6
            << filterprecision(m_OneStep.finalCOMPosition.y[1] ) << " "                   // 7
            << filterprecision(m_OneStep.finalCOMPosition.z[1] ) << " "                   // 8
            << filterprecision(m_OneStep.finalCOMPosition.yaw[1] ) << " "                 // 9
            << filterprecision(m_OneStep.finalCOMPosition.x[2] ) << " "                   // 10
            << filterprecision(m_OneStep.finalCOMPosition.y[2] ) << " "                   // 11
            << filterprecision(m_OneStep.finalCOMPosition.z[2] ) << " "                   // 12
            << filterprecision(m_OneStep.finalCOMPosition.yaw[2] ) << " "                 // 13
            << filterprecision(m_OneStep.ZMPTarget(0) ) << " "                            // 14
            << filterprecision(m_OneStep.ZMPTarget(1) ) << " "                            // 15
            << filterprecision(m_OneStep.ZMPTarget(2) ) << " "                            // 16
            << filterprecision(m_OneStep.LeftFootPosition.x  ) << " "                     // 17
            << filterprecision(m_OneStep.LeftFootPosition.y  ) << " "                     // 18
            << filterprecision(m_OneStep.LeftFootPosition.z  ) << " "                     // 19
            << filterprecision(m_OneStep.LeftFootPosition.dx  ) << " "                    // 20
            << filterprecision(m_OneStep.LeftFootPosition.dy  ) << " "                    // 21
            << filterprecision(m_OneStep.LeftFootPosition.dz  ) << " "                    // 22
            << filterprecision(m_OneStep.LeftFootPosition.ddx  ) << " "                   // 23
            << filterprecision(m_OneStep.LeftFootPosition.ddy  ) << " "                   // 24
            << filterprecision(m_OneStep.LeftFootPosition.ddz  ) << " "                   // 25
            << filterprecision(m_OneStep.LeftFootPosition.theta ) << " "                  // 26
            << filterprecision(m_OneStep.LeftFootPosition.dtheta ) << " "                 // 27
            << filterprecision(m_OneStep.LeftFootPosition.ddtheta ) << " "                // 28
            << filterprecision(m_OneStep.LeftFootPosition.omega  ) << " "                 // 29
            << filterprecision(m_OneStep.LeftFootPosition.omega2  ) << " "                // 30
            << filterprecision(m_OneStep.RightFootPosition.x ) << " "                     // 31
            << filterprecision(m_OneStep.RightFootPosition.y ) << " "                     // 32
            << filterprecision(m_OneStep.RightFootPosition.z ) << " "                     // 33
            << filterprecision(m_OneStep.RightFootPosition.dx ) << " "                    // 34
            << filterprecision(m_OneStep.RightFootPosition.dy ) << " "                    // 35
            << filterprecision(m_OneStep.RightFootPosition.dz ) << " "                    // 36
            << filterprecision(m_OneStep.RightFootPosition.ddx ) << " "                   // 37
            << filterprecision(m_OneStep.RightFootPosition.ddy ) << " "                   // 38
            << filterprecision(m_OneStep.RightFootPosition.ddz ) << " "                   // 39
            << filterprecision(m_OneStep.RightFootPosition.theta ) << " "                 // 40
            << filterprecision(m_OneStep.RightFootPosition.dtheta ) << " "                // 41
            << filterprecision(m_OneStep.RightFootPosition.ddtheta ) << " "               // 42
            << filterprecision(m_OneStep.RightFootPosition.omega  ) << " "                // 43
            << filterprecision(m_OneStep.RightFootPosition.omega2  ) << " "               // 44
            << filterprecision(zmpmb[0]) << " "                                           // 45
            << filterprecision(zmpmb[1]) << " "                                           // 46
            << filterprecision(zmpmb[2]) << " "                                          ;// 47
        for(unsigned int k = 0 ; k < m_CurrentConfiguration.size() ; k++){                // 48-53 -> 54-83
          aof << filterprecision( m_CurrentConfiguration(k) ) << " "  ;
        }
        aof << endl;
        aof.close();
    }




    /// \brief Create file .hip .pos .zmp
    /// ---------------------------------
    ofstream aof ;
    string root = "/opt/grx/HRP2LAAS/etc/mnaveau/" ;
    string aFileName = root + m_TestName + ".pos" ;
    if ( iteration == 0 )
    {
      aof.open(aFileName.c_str(),ofstream::out);
      aof.close();
    }
    aof.open(aFileName.c_str(),ofstream::app);
    aof.precision(8);
    aof.setf(ios::scientific, ios::floatfield);
    aof << filterprecision( iteration * 0.005 ) << " "  ; // 1
    for(unsigned int i = 6 ; i < m_CurrentConfiguration.size() ; i++){
      aof << filterprecision( m_CurrentConfiguration(i) ) << " "  ; // 2
    }
    for(unsigned int i = 0 ; i < 9 ; i++){
      aof << 0.0 << " "  ;
    }
    aof << 0.0  << endl ;
    aof.close();

    aFileName = root + m_TestName + ".hip" ;
    if ( iteration == 0 ){
      aof.open(aFileName.c_str(),ofstream::out);
      aof.close();
    }
    aof.open(aFileName.c_str(),ofstream::app);
    aof.precision(8);
    aof.setf(ios::scientific, ios::floatfield);
      aof << filterprecision( iteration * 0.005 ) << " "  ;                           // 1
      aof << filterprecision( m_OneStep.finalCOMPosition.roll[0]) << " "  ;  // 2
      aof << filterprecision( m_OneStep.finalCOMPosition.pitch[0]) << " "  ;// 3
      aof << filterprecision( m_OneStep.finalCOMPosition.yaw[0]) ;          // 4
      aof << endl ;
    aof.close();

    aFileName = root + m_TestName + ".waist" ;
    if ( iteration == 0 ){
      aof.open(aFileName.c_str(),ofstream::out);
      aof.close();
    }
    aof.open(aFileName.c_str(),ofstream::app);
    aof.precision(8);
    aof.setf(ios::scientific, ios::floatfield);
      aof << filterprecision( iteration * 0.005 ) << " "  ;                           // 1
      aof << filterprecision( m_OneStep.finalCOMPosition.roll[0]) << " "  ;  // 2
      aof << filterprecision( m_OneStep.finalCOMPosition.pitch[0]) << " "  ;// 3
      aof << filterprecision( m_OneStep.finalCOMPosition.yaw[0]) ;          // 4
      aof << endl ;
    aof.close();

    aFileName = root + m_TestName + ".zmp" ;
    if ( iteration == 0 ){
      aof.open(aFileName.c_str(),ofstream::out);
      aof.close();
    }
    FootAbsolutePosition aSupportState;
    if (m_OneStep.LeftFootPosition.stepType < 0 )
      aSupportState = m_OneStep.LeftFootPosition ;
    else
      aSupportState = m_OneStep.RightFootPosition ;

    aof.open(aFileName.c_str(),ofstream::app);
    aof.precision(8);
    aof.setf(ios::scientific, ios::floatfield);
      aof << filterprecision( iteration * 0.005 ) << " "  ;                                 // 1
      aof << filterprecision( m_OneStep.ZMPTarget(0) - m_CurrentConfiguration(0)) << " "  ; // 2
      aof << filterprecision( m_OneStep.ZMPTarget(1) - m_CurrentConfiguration(1) ) << " "  ;// 3
      aof << filterprecision( aSupportState.z  - m_CurrentConfiguration(2))  ;              // 4
      aof << endl ;
    aof.close();

    iteration++;
  }

  void SpecializedRobotConstructor(   CjrlHumanoidDynamicRobot *& aHDR, CjrlHumanoidDynamicRobot *& aDebugHDR )
  {
    aHDR = NULL ;
    aDebugHDR = NULL ;

#ifdef WITH_HRP2DYNAMICS
    dynamicsJRLJapan::ObjectFactory aRobotDynamicsObjectConstructor;
    Chrp2OptHumanoidDynamicRobot *aHRP2HDR = new Chrp2OptHumanoidDynamicRobot( &aRobotDynamicsObjectConstructor );
    aHDR = aHRP2HDR;
    aDebugHDR = new Chrp2OptHumanoidDynamicRobot(&aRobotDynamicsObjectConstructor);
#endif
  }

  void StartAnalyticalOnLineWalking(PatternGeneratorInterface &aPGI)
  {
    CommonInitialization(aPGI);

    {
      istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":onlinechangestepframe relative");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":SetAutoFirstStep false");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":StartOnLineStepSequencing \
                          0.0 -0.105 0.0 0.0 \
                          0.2 0.19 0.0 0.0   \
                          0.2 -0.19 0.0 0.0  \
                          0.2 0.19 0.0 0.0   \
                          0.2 -0.19 0.0 0.0  \
                          0.2 0.19 0.0 0.0   \
                          0.0 -0.19 0.0 0.0  \
                          ");
      aPGI.ParseCmd(strm2);
    }
  }

  void StopOnLineWalking(PatternGeneratorInterface &aPGI)
  {
    istringstream strm2(":StopOnLineStepSequencing");
    aPGI.ParseCmd(strm2);
  }

  void AnalyticalShortStraightWalking(PatternGeneratorInterface &aPGI)
  {
    CommonInitialization(aPGI);
    {
      istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
      aPGI.ParseCmd(strm2);
    }
    {
      istringstream strm2(":stepstairseq \
                          0.0 -0.105 0.0 0.0 \
                          0.2 0.19 0.0 0.0 \
                          0.2 -0.19 0.0 0.0 \
                          0.2 0.19 0.0 0.0 \
                          0.2 -0.19 0.0 0.0 \
                          0.2 0.19 0.0 0.0 \
                          0.0 -0.19 0.0 0.0\
                          ");
      aPGI.ParseCmd(strm2);
    }
  }

  void AnalyticalClimbingStairs(PatternGeneratorInterface &aPGI)
  {
    CommonInitialization(aPGI);
    {
      istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":singlesupporttime 1.6");
      aPGI.ParseCmd(strm2);
    }

    {
      // warning  !! high speed double support !!
      istringstream strm2(":doublesupporttime 0.2");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":stepstairseq 0.0 -0.105 0.0 0.0\
                                         0.30 0.19 0.15 0.0\
                                         0.0 -0.19 0.0 0.0\
                                         0.30 0.19 0.15 0.0\
                                         0.0 -0.19 0.0 0.0\
                                         0.30 0.19 0.15 0.0\
                                         0.0 -0.19 0.0 0.0\
                                         0.30 0.19 0.15 0.0\
                                         0.0 -0.19 0.0 0.0\
                                         ");
      aPGI.ParseCmd(strm2);
    }

  }

  void AnalyticalGoingDownStairs(PatternGeneratorInterface &aPGI)
  {
    CommonInitialization(aPGI);
    {
      istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":singlesupporttime 1.6");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":doublesupporttime 0.2");
      aPGI.ParseCmd(strm2);
    }

    {
     istringstream strm2(":stepstairseq 0.0 -0.105 0.0 0.0\
                                        0.30 0.19 -0.15 0.0\
                                        0.0 -0.19 0.0 0.0\
                                        0.30 0.19 -0.15 0.0\
                                        0.0 -0.19 0.0 0.0\
                                        0.30 0.19 -0.15 0.0\
                                        0.0 -0.19 0.0 0.0\
                                        0.30 0.19 -0.15 0.0\
                                        0.0 -0.19 0.0 0.0\
                                        ");
      aPGI.ParseCmd(strm2);
    }

  }

 void AnalyticalSteppingStones(PatternGeneratorInterface &aPGI)
  {
    CommonInitialization(aPGI);
    {
      istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":singlesupporttime 1.4");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":doublesupporttime 0.2");
      aPGI.ParseCmd(strm2);
    }

    {
      istringstream strm2(":stepstairseq 0.0 -0.105 0.0 0.0\
                                        0.25 0.19 0.05 0.0\
                                        0.2 -0.19 0.05 0.0\
                                        0.25 0.19 0.05 0.0\
                                        0.2 -0.19 0.05 0.0\
                                        0.2 0.19 0.0 0.0\
                                        0.2 -0.19 -0.05 0.0\
                                        0.2 0.19 -0.05 0.0\
                                        0.2 -0.19 -0.05 0.0\
                                        0.2 0.19 0.0 0.0\
                                        0.0 -0.19 0.0 0.0");

      aPGI.ParseCmd(strm2);
    }

  }

 // Define here the function to Generate Walking on Beam motion
 void AnalyticalWalkingOnBeam(PatternGeneratorInterface &aPGI)
 {
   CommonInitialization(aPGI);
   {
     istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
     aPGI.ParseCmd(strm2);
   }

   {
     istringstream strm2(":singlesupporttime 2.1");
     aPGI.ParseCmd(strm2);
   }

   {
     istringstream strm2(":doublesupporttime 0.7");
     aPGI.ParseCmd(strm2);
   }

   {
       istringstream strm2(":stepstairseq\
                           0.0   0.105 0.0 0.0\
                           0.28 -0.145 0.0 0.0\
                           0.28  0.100 0.0 0.0\
                           0.28 -0.100 0.0 0.0\
                           0.28  0.100 0.0 0.0\
                           0.28 -0.100 0.0 0.0\
                           0.28  0.100 0.0 0.0\
                           0.28 -0.100 0.0 0.0\
                           0.28  0.100 0.0 0.0\
                           0.28 -0.100 0.0 0.0\
                           0.28  0.100 0.0 0.0\
                           0.28 -0.145 0.0 0.0\
                           0.2   0.190 0.0 0.0\
                           0.0  -0.190 0.0 0.0\
                           ");
     aPGI.ParseCmd(strm2);
   }
 }

 void AnalyticalGoThroughWall(PatternGeneratorInterface &aPGI)
 {
   CommonInitialization(aPGI);
   {
     istringstream strm2(":SetAlgoForZmpTrajectory Morisawa");
     aPGI.ParseCmd(strm2);
   }

   {
     istringstream strm2(":singlesupporttime 1.05");
     aPGI.ParseCmd(strm2);
   }

   {
     istringstream strm2(":doublesupporttime 0.15");
     aPGI.ParseCmd(strm2);
   }

   {
     istringstream strm2(":stepheight 0.02");
     aPGI.ParseCmd(strm2);
   }

   {
      // wait for andreas the step sequence
     istringstream strm2(":stepstairseq\
     0.06 2.0 0.0 0.0   \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
     0.0 -0.3 0.0 0.0   \
     0.0 0.2 0.0 0.0    \
    ");
      aPGI.ParseCmd(strm2);
    }
  }

 void chooseTestProfile()
 {

   switch(m_TestProfile)
   {
     case PROFIL_ANALYTICAL_SHORT_STRAIGHT_WALKING:
       AnalyticalShortStraightWalking(*m_PGI);
       break;

     case PROFIL_ANALYTICAL_CLIMBING_STAIRS:
       AnalyticalClimbingStairs(*m_PGI);
       break;

     case PROFIL_ANALYTICAL_GOING_DOWN_STAIRS:
       AnalyticalGoingDownStairs(*m_PGI);
       break;

     case PROFIL_ANALYTICAL_STEPPING_STONES:
       AnalyticalSteppingStones(*m_PGI);
       break;

     case PROFIL_ANALYTICAL_ONLINE_WALKING:
       StartAnalyticalOnLineWalking(*m_PGI);
       break;

     case PROFIL_ANALYTICAL_WALKING_ON_BEAM:
       AnalyticalWalkingOnBeam(*m_PGI);
       break;

     case PROFIL_ANALYTICAL_GO_THROUGH_WALL:
       AnalyticalGoThroughWall(*m_PGI);
       break;


     default:
       throw("No correct test profile");
       break;
   }
 }

  void generateEvent()
  {
    if (m_TestProfile==PROFIL_ANALYTICAL_SHORT_STRAIGHT_WALKING)
      return;
    if (m_TestProfile==PROFIL_ANALYTICAL_CLIMBING_STAIRS)
      return;
    if (m_TestProfile==PROFIL_ANALYTICAL_GOING_DOWN_STAIRS)
      return;
    if (m_TestProfile==PROFIL_ANALYTICAL_STEPPING_STONES)
      return;
    if (m_TestProfile==PROFIL_ANALYTICAL_WALKING_ON_BEAM)
      return;
    if (m_TestProfile==PROFIL_ANALYTICAL_GO_THROUGH_WALL)
      return;

    unsigned int StoppingTime = 20*200;


    double r = 100.0*(double)m_OneStep.NbOfIt/(double)StoppingTime;


    /* Stop after 30 seconds the on-line stepping */
    if (m_OneStep.NbOfIt>StoppingTime) 
      {
	StopOnLineWalking(*m_PGI);
      }
    else 
      {
	/* Stay on the spot during 5.0 s before stopping. */
	if (m_OneStep.NbOfIt<StoppingTime-200*5.0) 
	  {
	    if (m_OneStep.NbOfIt%200==0)
	      {
		cout << "\r" << "Progress " << (unsigned int)r << " " ;
		cout.flush();
	      }
	
	    double triggertime = 9.8*200 + m_deltatime*200;
	    if ((m_OneStep.NbOfIt>triggertime) &&
		m_TestChangeFoot)
	      {
		PatternGeneratorJRL::FootAbsolutePosition aFAP;
		if (m_NbStepsModified<NBOFPREDEFONLINEFOOTSTEPS)
		  {
		    aFAP.x = OnLineFootSteps[m_NbStepsModified][0];
		    aFAP.y = OnLineFootSteps[m_NbStepsModified][1];
		    aFAP.z = OnLineFootSteps[m_NbStepsModified][3];
		    aFAP.theta = OnLineFootSteps[m_NbStepsModified][2];
		  }
		else
		  {
		    aFAP.x=0.1;
		    aFAP.y=0.0;
		    aFAP.z=0.0;
		    aFAP.theta=0.0;
		  }
		double newtime;
		bool stepHandledCorrectly=true;
		try 
		  {
		    m_PGI->ChangeOnLineStep(0.805,aFAP,newtime);
		  }
		catch(...)
		  {
		    cerr << "Step not well handled." << endl;
		    stepHandledCorrectly=false;
		  }
		if (stepHandledCorrectly)
		  {
		    m_deltatime += newtime+0.025;
		    m_TestChangeFoot=true;
		    m_NbStepsModified++;
		    if (m_NbStepsModified==360)
		      m_TestChangeFoot=false;
		  }
		else 
		  {
		    m_deltatime += 0.005;
		  }
	      }
	  }
      }
  }
  
};

int PerformTests(int argc, char *argv[])
{
  std::string CompleteName = string(argv[0]);
  unsigned found = CompleteName.find_last_of("/\\");
  std::string TestName =  CompleteName.substr(found+1);
  int TestProfiles[7] = { PROFIL_ANALYTICAL_ONLINE_WALKING,
			  PROFIL_ANALYTICAL_SHORT_STRAIGHT_WALKING,
			  PROFIL_ANALYTICAL_CLIMBING_STAIRS,
			  PROFIL_ANALYTICAL_GOING_DOWN_STAIRS,
                          PROFIL_ANALYTICAL_STEPPING_STONES,
                          PROFIL_ANALYTICAL_WALKING_ON_BEAM,
                          PROFIL_ANALYTICAL_GO_THROUGH_WALL
              };
  int indexProfile=-1;

  if (TestName.compare(16,6,"OnLine")==0)
    indexProfile=0;
  if (TestName.compare(16,9,"ShortWalk")==0)
    indexProfile=1;
  if (TestName.compare(16,8,"Climbing")==0)
    indexProfile=2;
  if (TestName.compare(16,9,"GoingDown")==0)
    indexProfile=3;
  if (TestName.compare(16,14,"SteppingStones")==0)
    indexProfile=4;
  if (TestName.compare(16,13,"WalkingOnBeam")==0)
    indexProfile=5;
  if (TestName.compare(16,13,"GoThroughWall")==0)
    indexProfile=6;


  if (indexProfile==-1)
    {
      std::cerr << "CompleteName: " << CompleteName << std::endl;
      std::cerr<< " TestName: " << TestName <<std::endl;
      std::cerr<< "Failure to find the proper indexFile:" << TestName.substr(17,6) << endl;
      exit(-1);
    }
  TestMorisawa2007 aTM2007(argc,argv,
                           TestName,
                           TestProfiles[indexProfile]);
  aTM2007.init();
  try
    {
      if (!aTM2007.doTest(std::cout))
        {
          cout << "Failed test " << indexProfile << endl;
          return -1;
        }
      else
        cout << "Passed test " << indexProfile << endl;
        }
  catch (const char * astr)
    { cerr << "Failed on following error " << astr << std::endl;
      return -1; }
  return 0;
}

int main(int argc, char *argv[])
{
  try
    {
      return PerformTests(argc,argv);
    }
  catch (const std::string& msg)
    {
      std::cerr << msg << std::endl;
    }
  return 1;
}


