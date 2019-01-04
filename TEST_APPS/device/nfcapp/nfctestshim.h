/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _NFCTESTSHIM_H_INCLUDED
#define _NFCTESTSHIM_H_INCLUDED

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

#include "mbed_config.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/util.h"
#include "nfc/NFCDefinitions.h"


using mbed::nfc::nfc_rf_protocols_bitmask_t;

class NFCTestShim {
public:
    NFCTestShim(events::EventQueue &queue);

    static void cmd_get_last_nfc_error()
    {
        get_last_nfc_error();
    };
    static void cmd_set_last_nfc_error(int err)
    {
        set_last_nfc_error(err);
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    };
    static void cmd_get_conf_nfceeprom()
    {
        get_conf_nfceeprom();
    };
    static void get_last_nfc_error();
    static void set_last_nfc_error(int err);
    static void get_conf_nfceeprom();
    static void print_ndef_message(const mbed::Span<const uint8_t> &buffer,
                                   size_t length);

    void cmd_init();
    virtual nfc_err_t init() = 0;

    void cmd_set_smartposter(char *cmdUri);
    void cmd_erase();
    void cmd_write_long(char *data);
    void cmd_read_nfceeprom();
    void cmd_start_discovery(bool manual = false);
    void cmd_stop_discovery();
    void cmd_configure_rf_protocols(nfc_rf_protocols_bitmask_t protocols);
    void cmd_get_rf_protocols();

protected:
    // implement/declare EEPROM and Controller model underlying common BH and delegate specializations
    virtual nfc_err_t set_rf_protocols(nfc_rf_protocols_bitmask_t protocols)
    {
        return NFC_ERR_UNSUPPORTED ;
    };
    virtual nfc_rf_protocols_bitmask_t get_rf_protocols()
    {
        nfc_rf_protocols_bitmask_t none;
        return none;
    };
    virtual nfc_err_t start_discovery()
    {
        return NFC_ERR_UNSUPPORTED ;
    };
    virtual nfc_err_t stop_discovery()
    {
        return NFC_ERR_UNSUPPORTED ;
    };
    void set_discovery_restart_auto()
    {
        _discovery_restart = true;
    };
    void set_discovery_restart_manual()
    {
        _discovery_restart = false;
    };

protected:
    size_t _ndef_write_buffer_used;
    mbed::Span<uint8_t> ndef_poster_message; // message to build and send
    uint8_t _ndef_write_buffer[0x2000]; // if this buffer is smaller than the EEPROM, the driver may crash see IOTPAN-297
    uint8_t _ndef_buffer[0x2000];       // driver I/O buffer
    bool _discovery_restart;            // default true, restart discovery loop again on remote disconnect
    events::EventQueue &_queue;

};

// forward declare single instance
extern NFCTestShim *pNFC_Test_Shim;

#endif // _NFCTESTSHIM_H_INCLUDED
