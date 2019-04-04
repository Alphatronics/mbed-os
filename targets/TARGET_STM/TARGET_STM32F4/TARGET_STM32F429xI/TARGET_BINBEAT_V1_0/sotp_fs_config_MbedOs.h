#ifndef PAL_HEADER_SOTP_FS
#define PAL_HEADER_SOTP_FS

/**
 * Note: NUCLEO_F429ZI and BINBEAT_V1_0 use "Software One Time Programming" (SOTP) 
 * as a software implementated "Root Of Trust" (RoT).
 * Below we find some define statements that configure the Cloud Client abstractions four our supported platforms.
 * 
 * As side info you may want to explore the file C:\work-mbed\atbinbeat\mbed-cloud-client\mbed-client-pal\Source\PAL-Impl\Services-API\pal_configuration.h
 * and documentation https://cloud.mbed.com/docs/current/porting/changing-a-customized-porting-layer.html
 * 
 */ 

/**
 * PAL_USE_HW_ROT (default value: 1): Defines whether PAL uses a hardware RoT:
 *
 * Set to 1 if the RoT is provided by the platform's hardware. In this case, you must use the function pal_plat_osGetRoTFromHW(uint8_t * key,size_t keyLenBytes) to fetch the RoT from the hardware.
 *
 * Set to 0 if the RoT is software-generated (either by the device's random generator or stored into the SOTP via the FCC). Please refer to the FCC documentation for more details.
 */
#define PAL_USE_HW_ROT 0

/**
 * The PAL_USE_HW_RTC flag defines whether PAL uses hardware RTC, where:
 *
 * 1 (default) is ON.
 * 0 is OFF.
 */
#define PAL_USE_HW_RTC 0

/**
 * PAL_USE_HW_TRNG (default value: 1): Defines whether PAL uses a hardware-based true random number generator (TRNG).
 */
#define PAL_USE_HW_TRNG 1

/**
 * PAL_USE_INTERNAL_FLASH* (default value: 0): Defines whether internal flash exists on the platform controller, where:
 *
 *   0 (default) means there is no internal flash on the platform controller.
 *   1 means there internal flash exists on the platform controller.
 *   Note: You must set the PAL_USE_INTERNAL_FLASH to 1 to use the following defines.
 */
#define PAL_USE_INTERNAL_FLASH 1

#define PAL_USE_SECURE_TIME 1

#define PAL_SIMULATOR_FLASH_OVER_FILE_SYSTEM 0



#if defined(TARGET_MODULE_BINBEAT_V1)
    /**
     * from "mbedOS_default.h"
     */ 
    #include "cmsis_os.h"

    #if (defined(MBED_DEBUG) && !defined(DEBUG))
        #define DEBUG
    #endif

    /*!
    * \brief This file is for more specific definitions (per board, if needed).
    *        if this file is defined it will be included from pal_configuration.h
    *        if not, the default file will be included - if needed
    */

    #ifndef PAL_RTOS_WAIT_FOREVER
        #define PAL_RTOS_WAIT_FOREVER osWaitForever
    #endif

    #ifndef PAL_NUMBER_OF_PARTITIONS
        #define PAL_NUMBER_OF_PARTITIONS 1
    #endif


    #ifndef PAL_FS_MOUNT_POINT_PRIMARY
        #if (MBED_VERSION >= 51000)  // 5.10 or above
            #define PAL_FS_MOUNT_POINT_PRIMARY    "/sd"           //!< User should change this for the his working folder
        #else
            #define PAL_FS_MOUNT_POINT_PRIMARY    "/sd"
        #endif
    #endif

    #ifndef PAL_FS_MOUNT_POINT_SECONDARY
        #if (PAL_NUMBER_OF_PARTITIONS == 2)
            #if (MBED_VERSION >= 51000)  // 5.10 or above
                #define PAL_FS_MOUNT_POINT_SECONDARY    "/sd2"
            #else
                #define PAL_FS_MOUNT_POINT_SECONDARY    "/sd2"
            #endif
        #else
            #define PAL_FS_MOUNT_POINT_SECONDARY PAL_FS_MOUNT_POINT_PRIMARY      //!< User should change this for the his working folder
        #endif
    #endif

    #ifndef PAL_NUM_OF_THREAD_INSTANCES
        #define PAL_NUM_OF_THREAD_INSTANCES 1
    #endif

    #ifndef PAL_MAX_SEMAPHORE_COUNT
        #define PAL_MAX_SEMAPHORE_COUNT 1024
    #endif

    #ifndef PAL_USE_INTERNAL_FLASH
        #define PAL_USE_INTERNAL_FLASH  1
    #endif

    #ifndef PAL_INT_FLASH_NUM_SECTIONS
        #define PAL_INT_FLASH_NUM_SECTIONS 2
    #endif


    /**
     * from "mbed-cloud-client\mbed-client-pal\Configs\pal_config\mbedOS\TARGET_NUCLEO_F429ZI\NUCLEO_default.h"
     */ 

    /*\brief  Starting Address for section 1 Minimum requirement size is 1KB and section must be consecutive sectors*/
    #ifndef PAL_INTERNAL_FLASH_SECTION_1_ADDRESS
    #define PAL_INTERNAL_FLASH_SECTION_1_ADDRESS    0x080C0000
    #endif

    /*\brief  Starting Address for section 2 Minimum requirement size is 1KB and section must be consecutive sectors*/
    #ifndef PAL_INTERNAL_FLASH_SECTION_2_ADDRESS
    #define PAL_INTERNAL_FLASH_SECTION_2_ADDRESS    0x080E0000
    #endif

    /*\brief  Size for section 1*/
    #ifndef PAL_INTERNAL_FLASH_SECTION_1_SIZE
    #define PAL_INTERNAL_FLASH_SECTION_1_SIZE       0x20000
    #endif

    /*\brief  Size for section 2*/
    #ifndef PAL_INTERNAL_FLASH_SECTION_2_SIZE
    #define PAL_INTERNAL_FLASH_SECTION_2_SIZE       0x20000
    #endif

    #ifndef PAL_USE_HW_ROT 
    #define PAL_USE_HW_ROT     0
    #endif

    #ifndef PAL_USE_HW_RTC
    #define PAL_USE_HW_RTC    0
    #endif
#else
    #include "mbedOS_default.h"
#endif


#endif //PAL_HEADER_SOTP_FS
