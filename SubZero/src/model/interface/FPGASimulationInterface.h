/**
 * FPGASimulationInterface.h
 *
 * Created on: Jul 7, 2015
 * Author: ahsueh1996
 */

#ifndef FPGASIMULATIONINTERFACE_H
#define FPGASIMULATIONINTERFACE_H

#include <thread>
#include <iostream>
#include <string>
#include "HwInterface.h"
#include "../../util/data/Data.h"
#include "../../util/data/FPGAData.h"

enum Attributes {
    POWER,
    DEPTH,
    HEADING, // absolute, given by sonar
    YAW, // relative to the direction the sub is heading
    SPEED
};

/**
 * A concrete child of HwInterface that deals specifically with the FPGA.
 *
 * FPGASimulationInterface is responsible for:
 * 		1. Dealing with the data coming from FPGA: poll->decode
 * 		2. Managing its private buffer: store the new, delete the old
 * 		3. Dealing with the data going to the hardware: encode->send
 * 			- i.e. provide function that delivers commands and instructions
 * 			  to the FPGA (
 * 			  NOTE: only providing the functions to command not actually
 * 			  commanding. Submarine level thinking is done in the Controller.
 *
 * @version	0.0
 * @since	Jan 17 2015
 */

class FPGASimulationInterface : public HwInterface {

private:

    Logger* logger = new Logger("FPGASimulationInterface");

    /* ==========================================================================
     * 				INTERACTING WITH DATA COMING IN (FROM FPGA)
     * ==========================================================================
     * This interface class will be automatically polling and managing
     * the polling process privately within the interface at pollFrequency
     * using the functions below.
     */

    /**
     * Poll raw data from FPGA.
     */
    virtual void poll();

    /**
     * Decode the data.
     * @param	data	data to be decoded
     * @return	decoded data in a FPGAData format
     */
    virtual FPGAData* decode(std::string* data);

    void in();

public:

    /* ==========================================================================
     * 				INTERACTING WITH DATA GOING OUT (TO FPGA)
     * ==========================================================================
     * FPGASimulationInterface provides functions for the Controller to send data/commands/
     * messages to the FPGA.
     */

    /**
     * Give commands to FPGA.
     * @param	attr	attribute to set
     * @param	value	set attribute to this value
     */
    void set(Attributes attr, int value);

    /**
     * Send the data.
     * @param	data	data to be sent
     */
    virtual void send(std::string* data);

    /* ==========================================================================
     * 							CONSTRUCTOR AND DESTRUCTOR
     * ==========================================================================
     */

    /**
     * Constructor for FPGA Simulation Interface
     * @param	bufferSize	buffer size for the interface
     * @param	pollFrequency	frequency of polling (polls per second)
     */
    FPGASimulationInterface(int bufferSize, int pollFrequency);

    virtual void init();

    /**
     * Destructor
     */
    virtual ~FPGASimulationInterface();

};

#endif // FPGASIMULATIONINTERFACE_H