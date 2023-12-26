/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
* @file lpa_hal.h
* @brief The lpa hal provides function call prototypes and structure definitions used for the  LPA hardware abstraction layer.
*
*/

#ifndef _LPA_HAL_H_
#define _LPA_HAL_H_

/**********************************************************************
                ENUMERATION DEFINITIONS
**********************************************************************/
/**
 * @brief Represents the preferred UICC (Universal Integrated Circuit Card) type.
 * This enumeration defines the possible value for preferred UICC.
 */
typedef enum _Cellular_PreferredUICC_t
{
    CELLULAR_UICC_USIM     = 0,  /**<Universal Integrated Circuit Card with Universal Subscriber Identity Module */
    CELLULAR_UICC_ESIM,          /**< Universal Integrated Circuit Card with embedded SIM*/
} CellularPreferredUICC_t;

/**
 * @brief Represents the status of the cellular device sim.
 *
 * This enumeration provides the values for different status of sim in the cellular devices.  
 */
typedef enum _CellularDeviceSimStatus_t
{
   SIM_PREPARATION_PENDING = 1,    /**< Represents the status as pending */
   SIM_PREPARATION_INPROGRESS,     /**< Represents the status as inprogress*/  
   SIM_PREPARATION_DOWNLOADED,     /**< Represents the status as downloaded*/
   SIM_PREPARATION_DONE,           /**< Represents the status as completed*/
} CellularDeviceSimStatus_t;

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
/**
* @defgroup lpa_hal LPA HAL
*
* @defgroup LPA_HAL_TYPES  LPA HAL Data Types
* @ingroup  lpa_hal
*
* @defgroup LPA_HAL_APIS   LPA HAL  APIs
* @ingroup  lpa_hal
*
**/

/**
* @addtogroup LPA_HAL_TYPES
* @{
*/

#define RETURN_ERROR        (-1)
#define RETURN_OK           (0)
#ifndef TRUE
#define TRUE                (1)
#endif
#ifndef FALSE
#define FALSE               (0)
#endif

/
/**
 * @brief Represents the esim profile object.
 *
 * Holds information related to the esim like the iccid, profile name and profile state.
 */
typedef  struct
{
    char iccid[32];          /**< @brief Represents the Integrated Circuit Card Identification. 
                                  Possible values include 98410800004860024951,98109909002143658739,
                                  98414102915071000054.*/
    int  profileState;       /**< @brief Represents the state of the profile.
                                  Possible values are 00 which represents disabled state and 01 which 
                                  represents enabled state. */

    char profileName[16];    /**< @brief Represents the profile name.
                                  Possible values are "Xfinity Mobile", "Comcast" and "CRTC".*/
} eSIMProfileStruct;

/**
* @}
*/

/**
* @addtogroup LPA_HAL_APIS
* @{
*/

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/


/*
 * TODO:
 *
 * 1. Extend the return codes by listing out the possible reasons of failure, to improve the interface in the future.
 *    This was reported during the review for header file migration to opensource github.
 *
 */


/**
* @brief - This callback sends to upper layer about the download progress.
*
* @param[in] progress - It is a character pointer which represents the download progress either have download completed progress or download in progress.
*                       \n The possible values are "download successful", "Activation Code decoded", "Initiate authentication done", "SM-DP+ address checking done", "Authenticate server done", "Authenticate client done", "PPR conditions check done", "Get Bound Profile Package done",
*                       \n "Send PIR notification to server done", "Clear PIR notification done", "Read pending notifications...", "<1> pending notification(s) detected", "Processing notification #1 <33>", "Notification <33> successfully sent", "Load Bound Profile Package done - Profile download successful".
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
typedef int (*cellular_sim_download_progress_callback)( char *progress );

/**
* @brief - Downloads the profile with activation code.
*
* @param[out] ActivationCodeStr - It is a 255 bytes character pointer that provides the activation code for the profile.
*                                 \n The possible values is "1$sm-v4-059-ppa-gtm.pr.go-esim.com$50949C4E5F62378896B6BE0EBEF554E6"
* @param[in] download_progress  - The variable is the function pointer to the call back function cellular_sim_download_progress_callback which receives information about the download progress.
*            progress - It is a character pointer which represents the download progress either have download completed progress or download in progress.
*                       \n The possible values are "download successful", "Activation Code decoded", "Initiate authentication done", "SM-DP+ address checking done", "Authenticate server done", "Authenticate client done", "PPR conditions check done", "Get Bound Profile Package done",
*                       \n "Send PIR notification to server done", "Clear PIR notification done", "Read pending notifications...", "<1> pending notification(s) detected", "Processing notification #1 <33>", "Notification <33> successfully sent", "Load Bound Profile Package done - Profile download successful".
*
* @return The status of the operation.
* @retval TRUE if modem device presents.
* @retval FALSE if modem device not presents.
*
*/
int cellular_esim_download_profile_with_activationcode(char* ActivationCodeStr, cellular_sim_download_progress_callback download_progress);

/**
* @brief - Downloads the profile from SMDS.
*
* @param[in] smds - It is a 128 bytes character pointer that represents the SMDS address.
*                   \n The possible value for smds is "oem-smds-json.demo.gemalto.com"
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_download_profile_from_smds(char* smds);

/**
* @brief - Downloads the profile from default SMDP.
*
* @param[in] smdp   - It is a 128 bytes character pointer that represents the SMDP address.
*                       \n The possible values for smdp is "deviceDefaultSMDPAddress=smdp-plus.test.gsma.com"
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_download_profile_from_defaultsmdp(char* smdp);

/**
* @brief - Gets the list of MNO profiles on eSIM.
*
* @param[out] profile_list - Pointer to eSIMProfileStruct structure that contains the list of MNO profiles.
*                   
* @param[out] nb_profiles - It is an integer pointer that provids the number of profiles.
*                           \n The maximum value is 2,147,483,647.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_get_profile_info(eSIMProfileStruct **profile_list, int *nb_profiles);

/**
* @brief - Enable MNO profile with matching ICCID.
*
* @param[in] iccid - It is a 10 bytes character pointer which represents the ICCID.
*                    \n The possible values for iccid are 98410800004860024951,98109909002143658739,98414102915071000054.
* @param[out] iccid_size - It is an integer which represents the size of the ICCID.
*                          \n The value is 10.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_enable_profile(char* iccid, int iccid_size);

/**
* @brief - Disable MNO profile with matching ICCID.
*
* @param[in] iccid -    It is a 10 bytes character pointer which represents the ICCID.
*                    \n The possible values for iccid are 98410800004860024951,98109909002143658739,
                        98414102915071000054.
* @param[out] iccid_size - It is an integer which represents the size of the ICCID.
*                          \n The value is 10.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_disable_profile(char* iccid, int iccid_size);

/**
* @brief - Delete MNO profile with matching ICCID.
*
* @param[in] iccid - It is a 10 bytes character pointer which represents the ICCID.
*                    \n The possible values for iccid are 98410800004860024951,98109909002143658739,
                        98414102915071000054.
* @param[out] iccid_size - It is an integer which represents the size of the ICCID.
*                          \n The value is 10.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_delete_profile(char* iccid, int iccid_size);

/**
* @brief - Initialize the LPA module that is to setup LPA SDK parameters.
* 
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_lpa_init(void);

/**
* @brief - Cleanup the LPA module.
* 
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_lpa_exit(void);

/**
* @brief - Get EID info.
* 
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_get_eid();

/**
* @brief - Get eUicc info.
* 
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_get_euicc();
#endif //_LPA_HAL_H_

/**
* @}
*/