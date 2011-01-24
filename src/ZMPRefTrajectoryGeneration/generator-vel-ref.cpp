/*
 * Copyright 2010,
 *
 * Andrei   Herdt
 * Olivier  Stasse
 *
 * JRL, CNRS/AIST
 *
 * This file is part of walkGenJrl.
 * walkGenJrl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * walkGenJrl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with walkGenJrl.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Research carried out within the scope of the
 *  Joint Japanese-French Robotics Laboratory (JRL)
 */
/*! This object constructs a QP as proposed by Herdt IROS 2010.
 */

#include "portability/gettimeofday.hh"

#include <ZMPRefTrajectoryGeneration/generator-vel-ref.hh>

using namespace std;
using namespace PatternGeneratorJRL;


GeneratorVelRef::GeneratorVelRef(SimplePluginManager *lSPM ) : MPCTrajectoryGeneration(lSPM)
{

  //Initialize the support state
  support_state_t CurrentSupport;
  CurrentSupport.Phase = 0;
  CurrentSupport.Foot = 1;
  CurrentSupport.TimeLimit = 1000000000;
  CurrentSupport.StepsLeft = 1;
  CurrentSupport.StateChanged = false;
  CurrentSupport.x = 0.0;
  CurrentSupport.y = 0.1;
  CurrentSupport.yaw = 0.0;
  CurrentSupport.StartTime = 0.0;
  Matrices_.SupportState(CurrentSupport);

}
	
		
GeneratorVelRef::~GeneratorVelRef()
{
  //TODO:
}

	
//void
//GeneratorVelRef::CallMethod(std::string &Method, std::istringstream &strm)
//{
//  //GeneratorVelRef::CallMethod(Method,strm);
//}

	
void 
GeneratorVelRef::setPonderation( double weight, int type)
{

  IntermedQPMat::objective_variant_t & Objective = Matrices_.Objective( type );
  Objective.weight = weight;

}


void
GeneratorVelRef::previewSupportStates( const SupportFSM * FSM, std::deque<support_state_t> & SupportStates_deq )
{

  // INITIALIZE QEUE OF SUPPORT STATES:
  // ----------------------------------
  const reference_t & RefVel = Matrices_.Reference();
  support_state_t & CurrentSupport = Matrices_.SupportState();
  FSM->setSupportState(m_CurrentTime, 0, CurrentSupport, RefVel);
  SupportStates_deq.push_back(CurrentSupport);


  // PREVIEW SUPPORT STATES:
  // -----------------------
  //initialize the previewed support state before previewing
  support_state_t PreviewedSupport = CurrentSupport;
  PreviewedSupport.StepNumber  = 0;

  for(int i=1;i<=m_N;i++)
    {
      FSM->setSupportState(m_CurrentTime, i, PreviewedSupport, RefVel);
      SupportStates_deq.push_back(PreviewedSupport);
    }

  generateSelectionMatrices(SupportStates_deq);

}


void
GeneratorVelRef::generateSelectionMatrices( const std::deque<support_state_t> & SupportStates_deq )
{

  IntermedQPMat::state_variant_t & State = Matrices_.State();
  const int & NbPrwSteps = SupportStates_deq.back().StepNumber;

  bool preserve = true;
  State.Vc.resize(m_N,!preserve);
  State.Vc.clear();
  State.V.resize(m_N,NbPrwSteps,!preserve);
  State.V.clear();
  State.VT.resize(NbPrwSteps,m_N,!preserve);
  State.VT.clear();
  State.Vc_f.resize(NbPrwSteps,!preserve);
  State.Vc_f.clear();
  State.V_f.resize(NbPrwSteps,NbPrwSteps,!preserve);
  State.V_f.clear();


  std::deque<support_state_t>::const_iterator SS_it;
  SS_it = SupportStates_deq.begin();//points at the cur. sup. st.

  SS_it++;
  for(int i=0;i<m_N;i++)
    {
      if(SS_it->StepNumber>0)
	{
	  State.V(i,SS_it->StepNumber-1) = State.VT(SS_it->StepNumber-1,i) = 1.0;
	  if(SS_it->StepNumber==1)
	    {
	      State.Vc_f(0) = 1.0;
	      State.V_f(0,0) = 1.0;
	    }
	  else if(SS_it->StepNumber>1)
	    {
	      State.V_f(SS_it->StepNumber-1,SS_it->StepNumber-2) = -1.0;
	      State.V_f(SS_it->StepNumber-1,SS_it->StepNumber-1) = 1.0;
	    }
	}
      else
	State.Vc(i) = 1.0;
      SS_it++;
    }

}


void 
GeneratorVelRef::computeGlobalReference(const deque<COMState> & TrunkStates_deq)
{

  reference_t & Ref = Matrices_.Reference();

  MAL_VECTOR_RESIZE(Ref.global.X,m_N);
  Ref.global.X.clear();
  MAL_VECTOR_RESIZE(Ref.global.Y,m_N);
  Ref.global.Y.clear();

  for( int i=0;i<m_N;i++)
    {
      Ref.global.X(i) = Ref.local.x*cos(TrunkStates_deq[(int)(i+1)*m_T_Prw/m_T_Ctr].yaw[0])-
	Ref.local.y*sin(TrunkStates_deq[(int)(i+1)*m_T_Prw/m_T_Ctr].yaw[0]);
      Ref.global.Y(i) = Ref.local.y*cos(TrunkStates_deq[(int)(i+1)*m_T_Prw/m_T_Ctr].yaw[0])+
	Ref.local.x*sin(TrunkStates_deq[(int)(i+1)*m_T_Prw/m_T_Ctr].yaw[0]);
    }

}


void 
GeneratorVelRef::initializeMatrices()
{

  IntermedQPMat::dynamics_t & Velocity = Matrices_.Dynamics( IntermedQPMat::VELOCITY );
  initializeMatrices( Velocity );
  IntermedQPMat::dynamics_t & COP = Matrices_.Dynamics( IntermedQPMat::COP );
  initializeMatrices( COP );
  IntermedQPMat::dynamics_t & Jerk = Matrices_.Dynamics( IntermedQPMat::JERK );
  initializeMatrices( Jerk );

  linear_inequality_t & IneqCoP = Matrices_.Inequalities( IntermedQPMat::INEQ_COP );
  initializeMatrices( IneqCoP );

}


void
GeneratorVelRef::initializeMatrices( linear_inequality_t & Inequalities)
{
  switch(Inequalities.type)
    {
    case IntermedQPMat::INEQ_COP:
      bool preserve = true;
      Inequalities.x.D.resize(4*m_N,m_N,!preserve);
      Inequalities.x.D.clear();
      Inequalities.y.D.resize(4*m_N,m_N,!preserve);
      Inequalities.y.D.clear();
      Inequalities.dc.resize(4*m_N,!preserve);
      Inequalities.dc.clear();
      break;
    }

}


void
GeneratorVelRef::initializeMatrices( IntermedQPMat::dynamics_t & Dynamics)
{

  bool preserve = true;
  Dynamics.U.resize(m_N,m_N,!preserve);
  Dynamics.U.clear();
  Dynamics.UT.resize(m_N,m_N,!preserve);
  Dynamics.UT.clear();
  Dynamics.S.resize(m_N,3,!preserve);
  Dynamics.S.clear();

  switch(Dynamics.type)
    {
    case IntermedQPMat::VELOCITY:
      for(int i=0;i<m_N;i++)
	{
	  Dynamics.S(i,0) = 0.0; Dynamics.S(i,1) = 1.0; Dynamics.S(i,2) = (i+1)*m_T_Prw;
	  for(int j=0;j<m_N;j++)
            if (j<=i)
              Dynamics.U(i,j) = Dynamics.UT(j,i) = (2*(i-j)+1)*m_T_Prw*m_T_Prw*0.5 ;
            else
	      Dynamics.U(i,j) = Dynamics.UT(j,i) = 0.0;
	}
      break;

    case IntermedQPMat::COP:
      for(int i=0;i<m_N;i++)
        {
          Dynamics.S(i,0) = 1.0; Dynamics.S(i,1) = (i+1)*m_T_Prw; Dynamics.S(i,2) = (i+1)*(i+1)*m_T_Prw*m_T_Prw*0.5-m_CoMHeight/9.81;
          for(int j=0;j<m_N;j++)
	    if (j<=i)
	      Dynamics.U(i,j) = Dynamics.UT(j,i) = (1 + 3*(i-j) + 3*(i-j)*(i-j)) * m_T_Prw*m_T_Prw*m_T_Prw/6.0 - m_T_Prw*m_CoMHeight/9.81;
	    else
	      Dynamics.U(i,j) = Dynamics.UT(j,i) = 0.0;
        }
      break;

    case IntermedQPMat::JERK:
      for(int i=0;i<m_N;i++)
        {
          Dynamics.S(i,0) = 0.0; Dynamics.S(i,1) = 0.0; Dynamics.S(i,2) = 0.0;
          for(int j=0;j<m_N;j++)
	    if (j==i)
	      Dynamics.U(i,j) = Dynamics.UT(j,i) = 1.0;
	    else
	      Dynamics.U(i,j) = Dynamics.UT(j,i) = 0.0;
        }
      break;

    }

}


//void
//GeneratorVelRef::addEqConstraint(std::deque<linear_inequality_ff_t> ConstraintsDeque,
//				 MAL_MATRIX (&DU, double), MAL_MATRIX (&DS, double))
//{
//  //TODO:
//}
	  

//void
//GeneratorVelRef::addIneqConstraint(std::deque<linear_inequality_ff_t> ConstraintsDeque,
//				   MAL_MATRIX (&DU, double), MAL_MATRIX (&DS, double))
//{
//  //TODO:
//}


void 
GeneratorVelRef::buildInequalitiesCoP(linear_inequality_t & Inequalities,
				      RelativeFeetInequalities * FCALS,
				      const std::deque< FootAbsolutePosition> & AbsoluteLeftFootPositions,
				      const std::deque<FootAbsolutePosition> & AbsoluteRightFootPositions,
				      const std::deque<support_state_t> & SupportStates_deq,
				      const std::deque<double> & PreviewedSupportAngles) const
{

  const support_state_t & CurrentSupport = SupportStates_deq.front();
  double CurrentSupportAngle;
  if( CurrentSupport.Foot==1 )
    CurrentSupportAngle = AbsoluteLeftFootPositions.back().theta*M_PI/180.0;
  else
    CurrentSupportAngle = AbsoluteRightFootPositions.back().theta*M_PI/180.0;
  convex_hull_t ZMPFeasibilityEdges;
  FCALS->setVertices( ZMPFeasibilityEdges,
		      CurrentSupportAngle,
		      CurrentSupport,
		      RelativeFeetInequalities::ZMP_CONSTRAINTS);

  //set constraints for the whole preview window
  double SupportAngle = CurrentSupportAngle;
  const int nEdges = 4;
  double D_x[nEdges] = {0.0, 0.0, 0.0, 0.0};
  double D_y[nEdges] = {0.0, 0.0, 0.0, 0.0};
  double dc[nEdges] = {0.0, 0.0, 0.0, 0.0};
  for( int i=1;i<=m_N;i++ )
    {
      const support_state_t & PrwSupport = SupportStates_deq[i];

      if( PrwSupport.StateChanged && PrwSupport.StepNumber>0 )
        SupportAngle = PreviewedSupportAngles[PrwSupport.StepNumber-1];

      if( PrwSupport.StateChanged )
        FCALS->setVertices( ZMPFeasibilityEdges,
			    SupportAngle,
			    PrwSupport,
			    RelativeFeetInequalities::ZMP_CONSTRAINTS);

      FCALS->computeLinearSystem( ZMPFeasibilityEdges, D_x, D_y, dc, PrwSupport );

      for(int j = 0;j < nEdges; j++)
        {
          Inequalities.x.D.push_back((i-1)*nEdges+j,i-1,D_x[j]);
          Inequalities.y.D.push_back((i-1)*nEdges+j,i-1,D_y[j]);
          Inequalities.dc((i-1)*nEdges+j) = dc[j];
        }

    }

}


void
GeneratorVelRef::buildInequalitiesFeet(linear_inequality_t & Inequalities,
				       RelativeFeetInequalities * FCALS,
				       const std::deque< FootAbsolutePosition> & AbsoluteLeftFootPositions,
				       const std::deque<FootAbsolutePosition> & AbsoluteRightFootPositions,
				       const std::deque<support_state_t> & SupportStates_deq,
				       const std::deque<double> & PreviewedSupportAngles) const
{

  const support_state_t & CurrentSupport = SupportStates_deq.front();

  double CurrentSupportAngle;
  if( CurrentSupport.Foot==1 )
    CurrentSupportAngle = AbsoluteLeftFootPositions.back().theta*M_PI/180.0;
  else
    CurrentSupportAngle = AbsoluteRightFootPositions.back().theta*M_PI/180.0;

  convex_hull_t FootFeasibilityEdges;

  //set constraints for the whole preview window
  double SupportAngle = CurrentSupportAngle;

  // Arrays for the generated set of inequalities
  const int NbEdges = 5;
  double D_x[NbEdges] = {0.0, 0.0, 0.0, 0.0, 0.0};
  double D_y[NbEdges] = {0.0, 0.0, 0.0, 0.0, 0.0};
  double dc[NbEdges] = {0.0, 0.0, 0.0, 0.0, 0.0};

  int NbStepsPreviewed = SupportStates_deq.back().StepNumber;
  Inequalities.resize(NbEdges*NbStepsPreviewed,NbStepsPreviewed, false);

  int nb_step = 0;
  for( int i=1;i<=m_N;i++ )
    {

      const support_state_t & PrwSupport = SupportStates_deq[i];

      //foot positioning constraints
      if( PrwSupport.StateChanged && PrwSupport.StepNumber>0 && PrwSupport.Phase != 0)
	{

	  SupportAngle = PreviewedSupportAngles[PrwSupport.StepNumber-1];

	  if( PrwSupport.StepNumber == 1 )
	    SupportAngle = CurrentSupportAngle;
	  else
	    SupportAngle = PreviewedSupportAngles[PrwSupport.StepNumber-2];

	  FCALS->setVertices( FootFeasibilityEdges,
			      SupportAngle, PrwSupport,
			      RelativeFeetInequalities::FOOT_CONSTRAINTS);

	  FCALS->computeLinearSystem( FootFeasibilityEdges, D_x, D_y, dc, PrwSupport );

	  for(int j = 0;j < NbEdges; j++)
	    {
	      Inequalities.x.D.push_back((PrwSupport.StepNumber-1)*NbEdges+j, (PrwSupport.StepNumber-1), D_x[j]);
	      Inequalities.y.D.push_back((PrwSupport.StepNumber-1)*NbEdges+j, (PrwSupport.StepNumber-1), D_y[j]);
	      Inequalities.dc((PrwSupport.StepNumber-1)*NbEdges+j) = dc[j];
	    }

	  nb_step++;
	}
    }
}


void
GeneratorVelRef::buildConstraintsCoP(const linear_inequality_t & IneqCoP,
				     const IntermedQPMat::dynamics_t & CoP,
				     const IntermedQPMat::state_variant_t & State,
				     int NbStepsPreviewed, QPProblem & Pb)
{

  int NbInequalities = IneqCoP.dc.size();
  boost_ublas::matrix<double> MM(NbInequalities,m_N,false);

  // -D*U
  computeTerm(MM,-1.0,IneqCoP.x.D,CoP.U);
  Pb.addTerm(MM,QPProblem::MATRIX_DU,0,0);
  computeTerm(MM,-1.0,IneqCoP.y.D,CoP.U);
  Pb.addTerm(MM,QPProblem::MATRIX_DU,0,m_N);

  // +D*V
  computeTerm(MM,1.0,IneqCoP.x.D,State.V);
  Pb.addTerm(MM,QPProblem::MATRIX_DU,0,2*m_N);
  computeTerm(MM,1.0,IneqCoP.y.D,State.V);
  Pb.addTerm(MM,QPProblem::MATRIX_DU,0,2*m_N+NbStepsPreviewed);

  //constant part
  // +dc
  Pb.addTerm(IneqCoP.dc,QPProblem::VECTOR_DS,0);

  boost_ublas::vector<double> MV(NbInequalities,false);
  boost_ublas::matrix<double> MM2(NbInequalities,3,false);

  // -D*S_z*x
  computeTerm(MM2,1.0,IneqCoP.x.D,CoP.S);
  computeTerm(MV,-1.0,MM2,State.CoM.x);
  Pb.addTerm(MV,QPProblem::VECTOR_DS,0);
  computeTerm(MM2,1.0,IneqCoP.y.D,CoP.S);
  computeTerm(MV,-1.0,MM2,State.CoM.y);
  Pb.addTerm(MV,QPProblem::VECTOR_DS,0);

  // +D*Vc*FP
  computeTerm(MV, State.SupportState.x, IneqCoP.x.D, State.Vc);
  Pb.addTerm(MV,QPProblem::VECTOR_DS,0);
  computeTerm(MV, State.SupportState.y, IneqCoP.y.D, State.Vc);
  Pb.addTerm(MV,QPProblem::VECTOR_DS,0);

}


void
GeneratorVelRef::buildConstraintsFeet(const linear_inequality_t & IneqFeet,
				      const IntermedQPMat::state_variant_t & State,
				      int NbStepsPreviewed, QPProblem & Pb)
{

  const int & NbConstraints = IneqFeet.dc.size();

  boost_ublas::matrix<double> MM(NbConstraints,NbStepsPreviewed,false);

  // -D*V_f
  computeTerm(MM,-1.0,IneqFeet.x.D,State.V_f);
  Pb.addTerm(MM,QPProblem::MATRIX_DU,4*m_N,2*m_N);
  computeTerm(MM,-1.0,IneqFeet.y.D,State.V_f);
  Pb.addTerm(MM,QPProblem::MATRIX_DU,4*m_N,2*m_N+NbStepsPreviewed);

  // +dc
  Pb.addTerm(IneqFeet.dc,QPProblem::VECTOR_DS,4*m_N);

  // +D*Vc_f*FP
  boost_ublas::vector<double> MV(NbConstraints*NbStepsPreviewed,false);
  computeTerm(MV, State.SupportState.x, IneqFeet.x.D, State.Vc_f);
  Pb.addTerm(MV,QPProblem::VECTOR_DS,4*m_N);
  computeTerm(MV, State.SupportState.y, IneqFeet.y.D, State.Vc_f);
  Pb.addTerm(MV,QPProblem::VECTOR_DS,4*m_N);

}


void
GeneratorVelRef::buildConstraints( QPProblem & Pb,
				  RelativeFeetInequalities * FCALS,
				  const std::deque< FootAbsolutePosition> & AbsoluteLeftFootPositions,
				  const std::deque<FootAbsolutePosition> & AbsoluteRightFootPositions,
				  const std::deque<support_state_t> & SupportStates_deq,
				  const std::deque<double> & PreviewedSupportAngles )
{

  //CoP constraints
  linear_inequality_t & IneqCoP = Matrices_.Inequalities(IntermedQPMat::INEQ_COP);
  buildInequalitiesCoP(IneqCoP, FCALS,
		       AbsoluteLeftFootPositions, AbsoluteRightFootPositions,
		       SupportStates_deq, PreviewedSupportAngles);

  const IntermedQPMat::dynamics_t & CoP = Matrices_.Dynamics(IntermedQPMat::COP);
  const IntermedQPMat::state_variant_t & State = Matrices_.State();
  int NbStepsPreviewed = SupportStates_deq.back().StepNumber;
  buildConstraintsCoP(IneqCoP, CoP, State, NbStepsPreviewed, Pb);

  //Feet constraints
  linear_inequality_t & IneqFeet = Matrices_.Inequalities(IntermedQPMat::INEQ_FEET);
  buildInequalitiesFeet(IneqFeet, FCALS,
			AbsoluteLeftFootPositions, AbsoluteRightFootPositions,
			SupportStates_deq, PreviewedSupportAngles);

  buildConstraintsFeet(IneqFeet, State, NbStepsPreviewed, Pb);

}


void 
GeneratorVelRef::buildInvariantPart(QPProblem & Pb)
{

  boost_ublas::matrix<double> weightMTM(m_N,m_N,false);

  //Constant terms in the Hessian
  // +a*U'*U
  const IntermedQPMat::objective_variant_t & Jerk = Matrices_.Objective(IntermedQPMat::JERK_MIN);
  computeTerm(weightMTM, Jerk.weight, Jerk.dyn->UT, Jerk.dyn->U);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 0, 0);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, m_N, m_N);

  // +a*U'*U
  const IntermedQPMat::objective_variant_t & InstVel = Matrices_.Objective(IntermedQPMat::INSTANT_VELOCITY);
  computeTerm(weightMTM, InstVel.weight, InstVel.dyn->UT, InstVel.dyn->U);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 0, 0);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, m_N, m_N);

  // +a*U'*U
  const IntermedQPMat::objective_variant_t & COPCent = Matrices_.Objective(IntermedQPMat::COP_CENTERING);
  computeTerm(weightMTM, COPCent.weight, COPCent.dyn->UT, COPCent.dyn->U);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 0, 0);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, m_N, m_N);

}


void
GeneratorVelRef::updateProblem(QPProblem & Pb, const std::deque<support_state_t> & SupportStates_deq)
{

  Pb.clear(QPProblem::VECTOR_D);

  //Intermediate vector
  boost_ublas::vector<double> MV(m_N);
  MV.clear();

  // Final scaled products that are added to the QP
  MAL_MATRIX(weightMTM,double);
  MAL_VECTOR(weightMTV,double);

  const int NbStepsPreviewed = SupportStates_deq[m_N].StepNumber;

  const IntermedQPMat::state_variant_t & State = Matrices_.State();

  // Instant velocity terms
  const IntermedQPMat::objective_variant_t & InstVel = Matrices_.Objective(IntermedQPMat::INSTANT_VELOCITY);
  // Linear part
  // +a*U'*S*x
  computeTerm(weightMTV, InstVel.weight, InstVel.dyn->UT, MV, InstVel.dyn->S, State.CoM.x);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, 0);
  computeTerm(weightMTV, InstVel.weight, InstVel.dyn->UT, MV, InstVel.dyn->S, State.CoM.y);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, m_N);
  // +a*U'*ref
  computeTerm(weightMTV, -InstVel.weight, InstVel.dyn->UT, State.Ref.global.X);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, 0);
  computeTerm(weightMTV, -InstVel.weight, InstVel.dyn->UT, State.Ref.global.Y);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, m_N);

  // COP - centering terms
  const IntermedQPMat::objective_variant_t & COPCent = Matrices_.Objective(IntermedQPMat::COP_CENTERING);
  // Hessian
  // -a*U'*V
  computeTerm(weightMTM, -COPCent.weight, COPCent.dyn->UT, State.V);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 0, 2*m_N);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, m_N, 2*m_N+NbStepsPreviewed);
  // -a*V*U
  computeTerm(weightMTM, -COPCent.weight, State.VT, COPCent.dyn->U);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 2*m_N, 0);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 2*m_N+NbStepsPreviewed, m_N);
  //+a*V'*V
  computeTerm(weightMTM, COPCent.weight, State.VT, State.V);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 2*m_N, 2*m_N);
  Pb.addTerm(weightMTM, QPProblem::MATRIX_Q, 2*m_N+NbStepsPreviewed, 2*m_N+NbStepsPreviewed);

  //Linear part
  // -a*V'*S*x
  computeTerm(weightMTV, -COPCent.weight, State.VT, MV, COPCent.dyn->S, State.CoM.x);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, 2*m_N);
  computeTerm(weightMTV, -COPCent.weight, State.VT, MV, COPCent.dyn->S, State.CoM.y);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, 2*m_N+NbStepsPreviewed);
  // +a*V'*Vc*x
  computeTerm(weightMTV, COPCent.weight, State.VT, State.Vc, State.SupportState.x);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, 2*m_N);
  computeTerm(weightMTV, COPCent.weight, State.VT, State.Vc, State.SupportState.y);
  Pb.addTerm(weightMTV, QPProblem::VECTOR_D, 2*m_N+NbStepsPreviewed);

}


void
GeneratorVelRef::computeTerm(MAL_MATRIX (&weightMM, double), double weight,
			     const MAL_MATRIX (&M1, double), const MAL_MATRIX (&M2, double))
{
  weightMM = weight*MAL_RET_A_by_B(M1,M2);
}


void
GeneratorVelRef::computeTerm(MAL_MATRIX (&MM, double),
                             const MAL_MATRIX (&M1, double), const MAL_MATRIX (&M2, double))
{
  MM = MAL_RET_A_by_B(M1,M2);
}

void
GeneratorVelRef::computeTerm(MAL_VECTOR (&weightMV, double), double weight,
			     const MAL_MATRIX (&M, double), const MAL_VECTOR (&V, double))
{
  weightMV = weight*MAL_RET_A_by_B(M,V);
}


void
GeneratorVelRef::computeTerm(MAL_VECTOR (&weightMV, double),
			     double weight, const MAL_MATRIX (&M, double),
			     const MAL_VECTOR (&V, double), double scalar)
{
  weightMV = weight*scalar*MAL_RET_A_by_B(M,V);
}


void
GeneratorVelRef::computeTerm(MAL_VECTOR (&weightMV, double),
			     double weight, const MAL_MATRIX (&M1, double), MAL_VECTOR (&V1, double),
			     const MAL_MATRIX (&M2, double), const MAL_VECTOR (&V2, double))
{
  V1 = MAL_RET_A_by_B(M2,V2);
  weightMV = weight*MAL_RET_A_by_B(M1,V1);
}
