#include "Servo.h"
#include <cmath>

Servo::Servo(int dxl_id, dynamixel::PortHandler* portHandler) {
    m_dxl_id = dxl_id;
    //m_devicename = "/dev/ttyUSB0";
    m_portHandler = portHandler;
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
    Enable(true);
}

Servo::~Servo() {
    Enable(false); 
}

int Servo::Enable(bool enable) {
    int torque = enable ? 1 : 0;

    dxl_comm_result = packetHandler->write1ByteTxRx(m_portHandler, m_dxl_id, ADDR_MX_TORQUE_ENABLE, torque, &dxl_error); 
    SetTorque(TORQUE_SAFE); 
    if( CheckError() ) {
        return -1;
    }

    return 0;
}


void Servo::SetTorque(int torque) {

    dxl_comm_result = packetHandler->write2ByteTxRx(m_portHandler, m_dxl_id, ADDR_MX_GOAL_TORQUE, torque, &dxl_error);

}

int Servo::SetPositionSetpoint(uint16_t setpoint) {
    //Enable(true);
    dxl_comm_result = packetHandler->write2ByteTxRx(m_portHandler, m_dxl_id, ADDR_MX_GOAL_POSITION, setpoint, &dxl_error);

    if( CheckError() ) {
        return -1;
    }

    return 0;
}

bool Servo::IsMoving(int pos) {
    return (std::abs(GetPosition() - pos) > SETPOINT_THRESHOLD) ? true : false;
}

int Servo::GetPosition() {

    dxl_comm_result = packetHandler->read2ByteTxRx(m_portHandler, m_dxl_id, ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);

    if( CheckError() ) {
        return -1;
    }

    return dxl_present_position;
}

int Servo::SetVelocitySetpoint(uint16_t setpoint) {
    //Enable(true);
    dxl_comm_result = packetHandler->write2ByteTxRx(m_portHandler, m_dxl_id, ADDR_MX_GOAL_SPEED, setpoint, &dxl_error);

    if( CheckError() ) {
        return -1;
    }

    return 0;
}


bool Servo::CheckError() {

    if(dxl_comm_result != COMM_SUCCESS) {
        packetHandler->printTxRxResult(dxl_comm_result);
        return true;    
    } else if(dxl_error != 0) {
        packetHandler->printRxPacketError(dxl_error);
        return true;
    } else {
        return false;
    }

}
