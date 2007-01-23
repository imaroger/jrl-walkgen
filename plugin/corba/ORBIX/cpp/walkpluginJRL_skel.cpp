// **********************************************************************
//
// Generated by the Orbix/E IDL-to-C++ Translator
//
// Copyright (c) 2003
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

// Version: 2.2.0

#include <OBE/CORBAServer.h>
#include <walkpluginJRL_skel.h>

#ifndef OBEC_INTEGER_VERSION
#   error No Orbix/E version defined! Is <OBE/CORBA.h> included?
#endif

#ifndef OBEC_NO_VERSION_CHECK
#   if (OBEC_INTEGER_VERSION != 2020000L)
#       error Orbix/E version mismatch!
#   endif
#endif

//
// IDL:walkpluginJRL/setTargetPos:1.0
//
void
POA_walkpluginJRL::_OB_op_setTargetPos(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setTargetPos(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setTargetPosNoWait:1.0
//
void
POA_walkpluginJRL::_OB_op_setTargetPosNoWait(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setTargetPosNoWait(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setArc:1.0
//
void
POA_walkpluginJRL::_OB_op_setArc(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setArc(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setArcNoWait:1.0
//
void
POA_walkpluginJRL::_OB_op_setArcNoWait(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setArcNoWait(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setRfootPos:1.0
//
void
POA_walkpluginJRL::_OB_op_setRfootPos(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setRfootPos(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setRfootPosNoWait:1.0
//
void
POA_walkpluginJRL::_OB_op_setRfootPosNoWait(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setRfootPosNoWait(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setLfootPos:1.0
//
void
POA_walkpluginJRL::_OB_op_setLfootPos(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setLfootPos(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setLfootPosNoWait:1.0
//
void
POA_walkpluginJRL::_OB_op_setLfootPosNoWait(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setLfootPosNoWait(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/getLegJointSpeed:1.0
//
void
POA_walkpluginJRL::_OB_op_getLegJointSpeed(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    ::dsequence_var _ob_a0;

    CORBA::Long _ob_r = getLegJointSpeed(_ob_a0.out());

    OBEC_OutputStreamImpl* _ob_out = OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    OBEC_OutputStreamImpl_write_long(_ob_out, _ob_r, _ob_cev);
    CORBA::ULong _ob_len0 = _ob_a0.in().length();
    OBEC_OutputStreamImpl_write_ulong(_ob_out, _ob_len0, _ob_cev);
    OBEC_OutputStreamImpl_write_double_array(_ob_out, _ob_a0.in().get_buffer(), _ob_len0, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/stopWalking:1.0
//
void
POA_walkpluginJRL::_OB_op_stopWalking(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    stopWalking();

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/waitArrival:1.0
//
void
POA_walkpluginJRL::_OB_op_waitArrival(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    waitArrival();

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/startStepping:1.0
//
void
POA_walkpluginJRL::_OB_op_startStepping(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    startStepping();

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/stopStepping:1.0
//
void
POA_walkpluginJRL::_OB_op_stopStepping(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    stopStepping();

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setWalkingVelocity:1.0
//
void
POA_walkpluginJRL::_OB_op_setWalkingVelocity(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a1 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    _ob_a2 = OBEC_InputStreamImpl_read_float(_ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setWalkingVelocity(_ob_a0, _ob_a1, _ob_a2);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/getWaistVelocity:1.0
//
void
POA_walkpluginJRL::_OB_op_getWaistVelocity(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Float _ob_a0;
    CORBA::Float _ob_a1;
    CORBA::Float _ob_a2;

    getWaistVelocity(_ob_a0, _ob_a1, _ob_a2);

    OBEC_OutputStreamImpl* _ob_out = OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    OBEC_OutputStreamImpl_write_float(_ob_out, _ob_a0, _ob_cev);
    OBEC_OutputStreamImpl_write_float(_ob_out, _ob_a1, _ob_cev);
    OBEC_OutputStreamImpl_write_float(_ob_out, _ob_a2, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/getWaistPositionAndOrientation:1.0
//
void
POA_walkpluginJRL::_OB_op_getWaistPositionAndOrientation(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    ::TransformQuaternion _ob_a0;
    CORBA::Float _ob_a1;

    getWaistPositionAndOrientation(_ob_a0, _ob_a1);

    OBEC_OutputStreamImpl* _ob_out = OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0._OB_marshal(_ob_out, _ob_cev);
    OBEC_OutputStreamImpl_write_float(_ob_out, _ob_a1, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/setWaistPositionAndOrientation:1.0
//
void
POA_walkpluginJRL::_OB_op_setWaistPositionAndOrientation(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    ::TransformQuaternion _ob_a0;
    OBEC_InputStreamImpl* _ob_in = OBEC_UpcallImpl_preUnmarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    ::TransformQuaternion::_OB_unmarshal(_ob_a0, _ob_in, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    setWaistPositionAndOrientation(_ob_a0);

    OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/getWaistAcceleration:1.0
//
void
POA_walkpluginJRL::_OB_op_getWaistAcceleration(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    ::TransformQuaternion _ob_a0;

    getWaistAcceleration(_ob_a0);

    OBEC_OutputStreamImpl* _ob_out = OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    _ob_a0._OB_marshal(_ob_out, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL/isWalking:1.0
//
void
POA_walkpluginJRL::_OB_op_isWalking(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    CORBA::Boolean _ob_r = isWalking();

    OBEC_OutputStreamImpl* _ob_out = OBEC_UpcallImpl_preMarshal(_ob_up, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;
    OBEC_OutputStreamImpl_write_boolean(_ob_out, _ob_r, _ob_cev);
    if (_ob_cev->_major != CORBA_NO_EXCEPTION)
        return;

    OBEC_UpcallImpl_postMarshal(_ob_up, _ob_cev);
}

//
// IDL:walkpluginJRL:1.0
//
CORBA::Boolean
POA_walkpluginJRL::_is_a(
    const char* type
) throw(CORBA::SystemException)
{
    const char** _ob_ids = ::walkpluginJRL::_OB_staticIds();
    for(CORBA::ULong _ob_i = 0 ; _ob_ids[_ob_i] != 0 ; _ob_i++)
        if(strcmp(type, _ob_ids[_ob_i]) == 0)
            return true;

    return false;
}

CORBA::RepositoryId
POA_walkpluginJRL::_primary_interface(
    const PortableServer::ObjectId&,
    PortableServer::POA_ptr
)
{
    return CORBA::string_dup(::walkpluginJRL::_OB_staticIds()[0]);
}

::walkpluginJRL_ptr
POA_walkpluginJRL::_this(
)
{
    CORBA::Object_var obj = _OB_createReference();
    ::walkpluginJRL_var result = ::walkpluginJRL::_narrow(obj);
    return result._retn();
}

void
POA_walkpluginJRL::_OB_dispatch(
    OBEC_UpcallImpl* _ob_up,
    CORBA_Environment* _ob_cev
)
{
    static const char* _ob_names[] =
    {
        "getLegJointSpeed",
        "getWaistAcceleration",
        "getWaistPositionAndOrientation",
        "getWaistVelocity",
        "isWalking",
        "sendMsg",
        "setArc",
        "setArcNoWait",
        "setLfootPos",
        "setLfootPosNoWait",
        "setRfootPos",
        "setRfootPosNoWait",
        "setTargetPos",
        "setTargetPosNoWait",
        "setWaistPositionAndOrientation",
        "setWalkingVelocity",
        "start",
        "startStepping",
        "stop",
        "stopStepping",
        "stopWalking",
        "waitArrival"
    };
    static const CORBA::ULong _ob_num_names = 22;

    const char* _ob_op = OBEC_UpcallImpl_operation(_ob_up, _ob_cev);
    switch(OBEC_BinarySearch(_ob_op, _ob_names, _ob_num_names))
    {
    case 0: // getLegJointSpeed
        _OB_op_getLegJointSpeed(_ob_up, _ob_cev);
        return;

    case 1: // getWaistAcceleration
        _OB_op_getWaistAcceleration(_ob_up, _ob_cev);
        return;

    case 2: // getWaistPositionAndOrientation
        _OB_op_getWaistPositionAndOrientation(_ob_up, _ob_cev);
        return;

    case 3: // getWaistVelocity
        _OB_op_getWaistVelocity(_ob_up, _ob_cev);
        return;

    case 4: // isWalking
        _OB_op_isWalking(_ob_up, _ob_cev);
        return;

    case 5: // sendMsg
        _OB_op_sendMsg(_ob_up, _ob_cev);
        return;

    case 6: // setArc
        _OB_op_setArc(_ob_up, _ob_cev);
        return;

    case 7: // setArcNoWait
        _OB_op_setArcNoWait(_ob_up, _ob_cev);
        return;

    case 8: // setLfootPos
        _OB_op_setLfootPos(_ob_up, _ob_cev);
        return;

    case 9: // setLfootPosNoWait
        _OB_op_setLfootPosNoWait(_ob_up, _ob_cev);
        return;

    case 10: // setRfootPos
        _OB_op_setRfootPos(_ob_up, _ob_cev);
        return;

    case 11: // setRfootPosNoWait
        _OB_op_setRfootPosNoWait(_ob_up, _ob_cev);
        return;

    case 12: // setTargetPos
        _OB_op_setTargetPos(_ob_up, _ob_cev);
        return;

    case 13: // setTargetPosNoWait
        _OB_op_setTargetPosNoWait(_ob_up, _ob_cev);
        return;

    case 14: // setWaistPositionAndOrientation
        _OB_op_setWaistPositionAndOrientation(_ob_up, _ob_cev);
        return;

    case 15: // setWalkingVelocity
        _OB_op_setWalkingVelocity(_ob_up, _ob_cev);
        return;

    case 16: // start
        _OB_op_start(_ob_up, _ob_cev);
        return;

    case 17: // startStepping
        _OB_op_startStepping(_ob_up, _ob_cev);
        return;

    case 18: // stop
        _OB_op_stop(_ob_up, _ob_cev);
        return;

    case 19: // stopStepping
        _OB_op_stopStepping(_ob_up, _ob_cev);
        return;

    case 20: // stopWalking
        _OB_op_stopWalking(_ob_up, _ob_cev);
        return;

    case 21: // waitArrival
        _OB_op_waitArrival(_ob_up, _ob_cev);
        return;
    }

    _OB_dispatchBase(_ob_up, _ob_cev);
}