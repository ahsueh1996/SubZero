/*
 * Controller.cpp
 *
 *  Created on: Mar 12, 2015
 *      Author: ed
 */

#include "Controller.h"
#include "ControllerThread.h"
#include "GUIView.h"
#include <iostream>

bool Controller::running = false;

Controller::Controller(){
    taskList = new QQueue <class Task* >;
}

Controller::Controller(std::vector<Model*> models, View* view){
    this->models = models;
    this->view = view;
    targetDepth = 250;
    targetYaw = 0;
    taskList = new QQueue <class Task* >;
}

void Controller::initialize(void) {
    logger->info("Initializing Controller");
    cT = new ControllerThread(taskList, &mutex);
    cT->moveToThread(&queueThread);
    connect(this, &Controller::beginCT, cT, &ControllerThread::executeTasks);
    connect(cT, &ControllerThread::resultReady, this, &Controller::finished);
    connect(&queueThread, &QThread::finished, cT, &QObject::deleteLater);
    queueThread.start();
    running = true;
    emit beginCT("Begin handling Commands");
}

    //Destructor to free pointers
Controller::~Controller(){
    if(queueThread.isRunning()){
        queueThread.quit();
        queueThread.wait();
    }
    while(!taskList->isEmpty()){
        Task *temp = taskList->dequeue();
        delete temp;
    }
    delete taskList;
}

bool Controller::isRunning() {
    return running;
}

void Controller::setStage(Stage *stage) {
    this->stage = stage;
}

void Controller::finished(const QString &s){
    std::cout << "Bye Bye Beautiful!!" << std::endl;
}

void Controller::switchToGUIView() {
    //TODO: Delete this somehow and create a new Controller for the GUI View
    stage->setViewContent(new GUIView(this));
    running = false;
}
static bool powerStatus = false;
void Controller::handlePowerButtonToggled() {

    logger->info("Adding Power Task to queue");
    powerStatus = !powerStatus;
    addTaskToQueue(TaskFactory::createPowerTask(models[FPGA]));
}

void Controller::handleMotorButtonClick() {
    logger->info("Adding Motor Task to queue");
    addTaskToQueue(TaskFactory::createMotorTask(models[FPGA]));
}

void Controller::handleMoveLeftButtonClick() {
    logger->info("Adding Move Left Task to queue");
    addTaskToQueue(TaskFactory::createTurnTask(models[FPGA], targetYaw, -15));
}

void Controller::handleMoveRightButtonClick() {
    logger->info("Adding Move Right Task to queue");
    addTaskToQueue(TaskFactory::createTurnTask(models[FPGA], targetYaw, 15));
}

void Controller::handleMoveForwardButtonClick() {
    logger->info("Adding Move Forward Task to queue");
    addTaskToQueue(TaskFactory::createSpeedTask(models[FPGA], 18*6));
}

void Controller::handleMoveBackwardButtonClick() {
    logger->info("Adding Move Backward Task to queue");
    addTaskToQueue(TaskFactory::createSpeedTask(models[FPGA], -18*6));
}

void Controller::handleStopButtonClick() {
    logger->info("Adding Stop Task to queue");
    addTaskToQueue(TaskFactory::createSpeedTask(models[FPGA], 0));
}

void Controller::handleSinkButtonClick() {
    logger->info("Adding Sink Task to queue");
    addTaskToQueue(TaskFactory::createDepthTask(models[FPGA], targetDepth, 50));
}

void Controller::handleRiseButtonClick() {
    logger->info("Adding Rise Task to queue");
    addTaskToQueue(TaskFactory::createDepthTask(models[FPGA], targetDepth, -50));
}

void Controller::handleGateTaskClick() {
    logger->info("Adding Gate Task to queue");
    addTaskToQueue(TaskFactory::createGateTask(models[FPGA], targetDepth, targetYaw));
}

void Controller::handlePathTaskClick() {
    logger->info("Adding Path Task to queue");
    addTaskToQueue(TaskFactory::createPathTask(models[FPGA], targetYaw));
}

void Controller::killAll() {
    logger->info("Exiting...");
    running = false;
}


void Controller::addTaskToQueue(Task *newTask)
{
    logger->trace("Requesting mutex lock to add task");
    mutex.lock();
    logger->trace("Lock request successful");
    taskList->enqueue(newTask);
    logger->trace("Enqueued new task");
    mutex.unlock();
    logger->trace("Unlocking mutex");

}
