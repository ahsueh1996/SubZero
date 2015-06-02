/*
 * Main.cpp
 *
 *  Created on: Jan 7, 2015
 *      Author: mda
 */

#include "util/Logger.h"
#include "view/menu.h"
#include "../test/util/filter/FilterManagerTEST.h"
#include "../test/util/data/ImgDataTEST.h"
#include <string>
#include <QApplication>

using namespace std;

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	Timer* logTimer = new Timer();
	Logger::initialize(true, true, logTimer);
	Logger::trace("Logger initialized.");

    FilterManagerTEST::runUnits();
    ImgDataTEST::runUnits();

    Menu* newMenu = new Menu;
    newMenu->show();

    //Logger::close();

	return app.exec();
}

