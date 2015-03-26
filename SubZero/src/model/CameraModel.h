/*
 * CameraModel.h
 *
 *  Created on: Jan 17, 2015
 *      Author: ahsueh1996
 */

#ifndef CAMERAMODEL_H_
#define CAMERAMODEL_H_

#include "Model.h"
#include "CameraState.h"

/**
 * This is a concrete child of the Model class. It implements functions
 * specific to communicating with the camera.
 */
class CameraModel: public Model {

private:
	CameraState* cameraState;
	CameraInterface* cameraInterface;

public:
	CameraModel(Observable* state, HwInterface* interface);
	/**
	 * Constructor
	 */
	virtual ~CameraModel();
	/**
	 * Deconstructor
	 */
};


#endif /* CAMERAMODEL_H_ */
