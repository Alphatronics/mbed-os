/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "UBLOX_PPP_CellularNetwork.h"

using namespace mbed;

UBLOX_PPP_CellularNetwork::UBLOX_PPP_CellularNetwork(ATHandler &atHandler) : AT_CellularNetwork(atHandler)
{
}

UBLOX_PPP_CellularNetwork::~UBLOX_PPP_CellularNetwork()
{
}

bool UBLOX_PPP_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
    return requested_stack == IPV4_STACK ? true : false;
}

AT_CellularNetwork::RegistrationMode UBLOX_PPP_CellularNetwork::has_registration(RegistrationType reg_type)
{
    return (reg_type == C_REG || reg_type == C_GREG) ? RegistrationModeLAC : RegistrationModeDisable;
}

nsapi_error_t UBLOX_PPP_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opRat)
{
    _op_act = RAT_UNKNOWN;
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t UBLOX_PPP_CellularNetwork::do_user_authentication()
{
    // if user has defined user name and password we need to call CGAUTH before activating or modifying context
    if (_pwd && _uname) {
    	 _at.clear_error();
    	 //_at.cmd_start("AT+UAUTHREQ=?");
    	 //_at.cmd_stop();
        _at.cmd_start("AT+UAUTHREQ=");
        _at.write_int(1);
        _at.write_int(_authentication_type);
        _at.write_string(_uname);
        _at.write_string(_pwd);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return NSAPI_ERROR_OK;
}
