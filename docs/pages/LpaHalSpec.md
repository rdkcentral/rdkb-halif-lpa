@mainpage

# LPA HAL Documentation


## Acronyms

- `HAL` \- Hardware Abstraction Layer
- `RDK-B` \- Reference Design Kit for Broadband Devices
- `OEM` \- Original Equipment Manufacture
- `LPA` \- Local Profile Assistants

## Description
The diagram below describes a high-level software architecture of the Lpa HAL module stack. 

![Lpa HAL Architecture Diag](images/Lpa_HAL_Architecture.png)

Lpa HAL is an abstraction layer, implemented to interact with vendor software's for getting the details of the profiles on the eSIM.

## Component Runtime Execution Requirements

### Initialization and Startup

There is no dependent API's is expected to be intialized for invoking the client module. But below API is used to setup the LPA SDK parameters
- `cellular_esim_lpa_init()`

3rd party vendors will implement appropriately to meet operational requirements. This interface is expected to block if the hardware is not ready.


## Threading Model

The interface is not thread safe.

Any module which is invoking the API should ensure calls are made in a thread safe manner.

Vendors can create internal threads/events to meet their operation requirements.  These should be responsible to synchronize between the calls, events and cleaned up on closure.

## Process Model

All API's are expected to be called from multiple process.

## Memory Model

The client is responsible to allocate and de-allocate memory for necessary API's as specified in API Documentation.
Different 3rd party vendors allowed to allocate memory for internal operational requirements. In this case 3rd party implementations should be responsible to de-allocate internally.

[TODO]: # (State a footprint requirement. Example: This should not exceed XXXX KB.)


## Power Management Requirements

The HAL is not involved in any of the power management operation.
Any power management state transitions MUST not affect the operation of the HAL.

## Asynchronous Notification Model

There are no asynchronous notifications.

## Blocking calls

The API's are expected to work synchronously and should complete within a time period commensurate with the complexity of the operation and in accordance with any relevant specification.
Any calls that can fail due to the lack of a response should have a timeout period in accordance with any API documentation.

[TODO]: # (As we state that they should complete within a time period, we need to state what that time target is, and pull it from the spec if required. Define the timeout requirement.)

## Internal Error Handling

All the Lpa HAL API's should return error synchronously as a return argument. HAL is responsible to handle system errors(e.g. out of memory) internally.

## Persistence Model

There is no requirement for HAL to persist any setting information. The caller is responsible to persist any settings related to their implementation.

## Nonfunctional requirements

Following non functional requirement should be supported by the component.

## Logging and debugging requirements

The component is should log all the error and critical informative messages which helps to debug/triage the issues and understand the functional flow of the system.

The logging should be consistence across all HAL components.

If the vendor is going to log then it has to be logged in `xxx_vendor_hal.log` file name which can be placed in `/rdklogs/logs/` directory.

Logging should be defined with log levels as per Linux standard logging.

## Memory and performance requirements

The component should not contributing more to memory and CPU utilization while performing normal operations and commensurate with the operation required.

## Quality Control

MTA HAL implementation should pass checks using third party tools like `Coverity`, `Black duck`, `Valgrind` without any issue to ensure quality.

There should not be any memory leaks/corruption introduced by HAL and underneath 3rd party software implementation.

## Licensing

Lpa HAL implementation is expected to released under the Apache License 2.0. 

## Build Requirements

The source code should be build under Linux Yocto environment and should be delivered as a shared library named as `libesim_lpa.la`
  
## Variability Management

Changes to the interface will be controlled by versioning, vendors will be expected to implement to a fixed version of the interface, and based on SLA agreements move to later versions as demand requires.

Each API interface will be versioned using [Semantic Versioning 2.0.0](https://semver.org/), the vendor code will comply with a specific version of the interface.

## Platform or Product Customization

None

## Interface API Documentation

All HAL function prototypes and datatype definitions are available in `lpa_hal.h` file.
    
1. Components/Process must `lpa_hal.h` to make use of Lpa hal capabilities.
2. Components/Process should add linker dependency for `libesim_lpa.la.`

## Theory of operation and key concepts

Covered as per "Description" sections in the API documentation.

### UML Diagrams

## Sequence Diagram

```mermaid
sequenceDiagram
participant Caller
participant Lpa HAL
participant Vendor
Caller->>Lpa HAL: Modem_XXX()
Lpa HAL->>Vendor: 
Vendor ->>Lpa HAL: 
Lpa HAL->>Caller: Modem_XXX() return
```