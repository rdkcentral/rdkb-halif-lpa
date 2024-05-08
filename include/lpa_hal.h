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
/*
 * TODO: This module must re-structre it's naming convention for all types
 *       CellularPreferredUICC_t -> LPA_HAL_CellularPreferredUICC_t
 *       CELLULAR_UICC_USIM -> LPA_HAL_CELLULAR_UICC_USIM
 *       etc. everywhere.
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
                                  Example: 98410800004860024951,98109909002143658739,
                                  98414102915071000054.*/
    int  profileState;       /**< @brief Represents the state of the profile.
                                  Expected values are 00 which represents disabled state and 01 which 
                                  represents enabled state. */

    char profileName[16];    /**< @brief Represents the profile name.
                                  Expected values are "Xfinity Mobile", "Comcast" and "CRTC".*/
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
 * TODO: Extend the return codes by listing out the possible reasons of failure, to improve the interface in the future.
 *       This was reported during the review for header file migration to opensource github.
 *
 */


/**
* @brief - This callback communicates the progress of the eSIM profile download to the upper layers of the software.
*
* This function is a callback that reports various stages of the eSIM profile download process. It is called with status messages that indicate each step of the process, from initiating authentication to the final download of the profile package.
*
* @param[in] progress - Pointer to a character array containing messages that represent the current state or stage of the download process. The messages could indicate successful completion of a stage or ongoing progress. Examples include "download successful", "Activation Code decoded", "Initiate authentication done", etc.
*
* @return The status of the operation.
* @retval RETURN_OK if the current progress status is reported successfully to the upper layers.
* @retval RETURN_ERR if there is a failure in reporting the progress. This could be due to issues such as invalid progress message formats, communication failures between layers, or internal errors in handling the progress updates.
*/
typedef int (*cellular_sim_download_progress_callback)( char *progress );

/**
* @brief - Downloads the eSIM profile using a provided activation code.
*
* This function initiates the download and installation of an eSIM profile by using an activation code. It uses a callback to communicate download progress and status back to the caller.
*
* @param[in] ActivationCodeStr - Pointer to a character array containing the activation code needed to download the profile. The activation code is a string that uniquely identifies the profile to be downloaded.
*                                Example: "1$sm-v4-059-ppa-gtm.pr.go-esim.com$50949C4E5F62378896B6BE0EBEF554E6".
* @param[in] download_progress - Function pointer to the callback function cellular_sim_download_progress_callback, which receives and processes information about the download's progress. This callback may be triggered multiple times during the download process to provide updates such as "download successful", "Activation Code decoded", etc.
*
* @return The status of the download operation.
* @retval TRUE if the profile download is initiated successfully and the modem device is present.
* @retval FALSE if there is an error in initiating the download or if the modem device is not present.
* @note If the function returns FALSE, use lpaGetErrorCodeDescription(lpaGetErrorCode()) to obtain a more detailed error description.
*/
int cellular_esim_download_profile_with_activationcode(char* ActivationCodeStr, cellular_sim_download_progress_callback download_progress);

/**
* @brief - Downloads the eSIM profile from an SMDS (Subscription Manager Data Preparation Server) address.
*
* This function initiates the download of an eSIM profile by connecting to the specified SMDS address. The SMDS is responsible for the secure management and delivery of eSIM profiles.
*
* @param[in] smds - Pointer to a character array containing the SMDS address from which the profile will be downloaded. This should be a valid URL format.
*                   Example: "oem-smds-json.demo.gemalto.com".
*
* @return The status of the download operation.
* @retval RETURN_OK if the profile is successfully downloaded and ready to be installed.
* @retval RETURN_ERR if an error occurs during the download process. This could be due to an invalid SMDS address, connection issues, or problems accessing the SMDS server.
*/
int cellular_esim_download_profile_from_smds(char* smds);

/**
* @brief - Downloads an eSIM profile from a default SMDP (Subscription Manager Data Preparation) address.
*
* This function initiates the download of an eSIM profile by connecting to a predefined SMDP+ address specified in the function argument. The SMDP+ is used for secure downloading of eSIM profiles to devices.
*
* @param[in] smdp - Pointer to a character array containing the SMDP address from which the eSIM profile will be downloaded. This address must be a fully qualified URL.
*                    Example: "smdp-plus.test.gsma.com".
*
* @return The status of the download operation.
* @retval RETURN_OK if the profile is successfully downloaded and the device is ready for profile installation.
* @retval RETURN_ERR if an error occurs during the download process. Potential errors include incorrect or unreachable SMDP address, network connectivity issues, or configuration errors in the SMDP address.
*/
int cellular_esim_download_profile_from_defaultsmdp(char* smdp);

/**
* @brief - Retrieves a list of Mobile Network Operator (MNO) profiles stored on the eSIM.
*
* This function fetches information about all the MNO profiles available on the eSIM and provides a list along with the count of these profiles. The function allocates memory for storing the profile data, which the caller must manage and free appropriately.
*
* @param[out] profile_list - Pointer to an array of eSIMProfileStruct pointers. Each pointer in the array points to a structure containing the details of an individual MNO profile. 
*                            The memory for these structures is dynamically allocated by the function, and it is the responsibility of the caller to free this memory after use.
* @param[out] nb_profiles - Pointer to an integer that will be set by the function to indicate the number of MNO profiles found on the eSIM.
*
* @return The status of the operation.
* @retval RETURN_OK if the profiles are successfully retrieved.
* @retval RETURN_ERR if an error occurs during the retrieval process. This could be due to an inability to access the eSIM, a failure in memory allocation for the profile structures, or if no profiles are found on the eSIM.
*/
int cellular_esim_get_profile_info(eSIMProfileStruct **profile_list, int *nb_profiles);

/**
* @brief - Enables a specific Mobile Network Operator (MNO) profile on the device by using the provided ICCID to identify and match the profile.
*
* This function activates an MNO profile that corresponds to the ICCID given. It verifies the ICCID against available profiles on the eSIM and enables the matching one.
*
* @param[in] iccid - Pointer to a character array containing the ICCID of the profile to be enabled. The ICCID should be a string of 20 digits representing the unique identifier of the SIM profile.
* @param[in] iccid_size - Integer representing the number of characters in the ICCID string. This value should typically be 20 to match the standard ICCID length.
*
* @return The status of the operation.
* @retval RETURN_OK if the profile is successfully enabled.
* @retval RETURN_ERR if any error occurs during the enabling process. Errors may include an ICCID that does not match any profile, incorrect ICCID format or length, or internal errors in processing the enable request.
*/
int cellular_esim_enable_profile(char* iccid, int iccid_size);

/**
* @brief - Disables a Mobile Network Operator (MNO) profile that matches the specified ICCID.
*
* This function disables an eSIM profile using its unique ICCID. The ICCID must exactly match the identifier of the profile to be disabled, preventing its use until re-enabled.
*
* @param[in] iccid - Pointer to a character array containing the ICCID of the eSIM profile to be disabled. The ICCID should be a string of 20 digits representing the unique identifier of the SIM profile.
* @param[in] iccid_size - Integer representing the number of characters in the ICCID string, which should typically be 20 to match the standard length of an ICCID.
*
* @return The status of the operation.
* @retval RETURN_OK if the profile is successfully disabled.
* @retval RETURN_ERR if any error occurs during the disabling process. Possible errors include an ICCID that does not match any existing profile, incorrect ICCID format or length, or internal errors within the system during the profile disabling attempt.
*/
int cellular_esim_disable_profile(char* iccid, int iccid_size);

/**
* @brief - Deletes a Mobile Network Operator (MNO) profile that matches the specified ICCID.
*
* This function attempts to delete an eSIM profile using its unique ICCID. The ICCID must exactly match the identifier of the profile to be deleted.
*
* @param[in] iccid - Pointer to a character array containing the ICCID of the eSIM profile to be deleted. The ICCID should be a string of 20 digits representing the unique identifier of the SIM profile.
* @param[in] iccid_size - Integer representing the number of characters in the ICCID string, which should typically be 20 to match the standard length of an ICCID.
*
* @return The status of the operation.
* @retval RETURN_OK if the profile is successfully deleted.
* @retval RETURN_ERR if any error occurs during the deletion process. Possible errors include an ICCID that does not match any existing profile, incorrect ICCID format or length, or internal errors within the system during the profile deletion attempt.
*/
int cellular_esim_delete_profile(char* iccid, int iccid_size);

/**
* @brief - Initializes the Local Profile Assistant (LPA) module, setting up the necessary SDK parameters for eSIM management.
*
* This function configures the LPA module to handle eSIM operations. It sets various parameters, checks the modem's readiness, and ensures all necessary dependencies and settings are correctly established for managing eSIM profiles.
*
* @return The status of the operation.
* @retval RETURN_OK if the LPA module is successfully initialized and all configurations are correctly set.
* @retval RETURN_ERR if an error occurs during the initialization process. Possible errors include failure to detect or communicate with the modem, issues setting up required parameters (such as network settings or security certificates), or internal failures within the LPA SDK.
*/
int cellular_esim_lpa_init(void);

/**
* @brief - Cleans up and uninitializes the Local Profile Assistant (LPA) module used for managing eSIM profiles.
*
* This function deactivates and releases resources associated with the LPA module. It is typically called when the application or device is preparing to shut down or when the LPA functionality is no longer needed.
*
* @return The status of the operation.
* @retval RETURN_OK if the LPA module is successfully uninitialized and resources are released properly.
* @retval RETURN_ERR if an error occurs during the uninitialization process. Possible errors can include failure to release system resources, ongoing operations that prevent proper shutdown, or internal errors within the LPA module.
*/
int cellular_esim_lpa_exit(void);

/**
* @brief - Retrieves the EID (eUICC Identifier) of the embedded SIM (eSIM).
*
* This function fetches the unique identifier known as the EID from the device's eSIM. The EID is crucial for managing eSIM profiles and subscriptions.
*
* @return The status of the operation.
* @retval RETURN_OK if the EID is successfully retrieved.
* @retval RETURN_ERR if an error occurs during the retrieval process. Possible errors include failure to communicate with the eSIM, eSIM not being present or detected in the device, or issues with the eSIM's initialization or configuration.
*/
int cellular_esim_get_eid();

/**
* @brief - Retrieves information about the embedded Universal Integrated Circuit Card (eUICC).
*
* This function gathers details from the eUICC, commonly known as an embedded SIM or eSIM, which is integrated into the device. It provides access to the eUICC's information necessary for managing mobile subscriptions.
*
* @return The status of the operation.
* @retval RETURN_OK if the eUICC information is successfully retrieved.
* @retval RETURN_ERR if an error occurs during the information retrieval process. Possible errors can include failure to access the eUICC due to hardware issues, communication errors with the eUICC, or if the eUICC is not initialized or configured correctly.
*/
int cellular_esim_get_euicc();
#endif //_LPA_HAL_H_

/**
* @}
*/
