#include <QtWidgets/QApplication>
#include "View.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	View w(NULL);
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(3, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	w.setFormat(format);
	w.LoadData("C:\\FOURDIX-diastolic.bin");
	QWidget window;

	QGroupBox grBox("Axis");
	QRadioButton radioXY("XY");
	QRadioButton radioYZ("YZ");
	QRadioButton radioXZ("XZ");
	radioXY.setChecked(true);
	QObject::connect(&radioXY, SIGNAL(toggled(bool)), &w, SLOT(axisSetXY(bool)));
	QObject::connect(&radioYZ, SIGNAL(toggled(bool)), &w, SLOT(axisSetYZ(bool)));
	QObject::connect(&radioXZ, SIGNAL(toggled(bool)), &w, SLOT(axisSetXZ(bool)));
	QVBoxLayout rbLay;
	rbLay.addWidget(&radioXY);
	rbLay.addWidget(&radioYZ);
	rbLay.addWidget(&radioXZ);
	grBox.setLayout(&rbLay);

	QGroupBox grBoxAlg("Visualization algorithm");
	QRadioButton radioQ("Quads");
	QRadioButton radioQS("Quadstips");
	QRadioButton radioTex("Texture");
	radioQ.setChecked(true);
	QObject::connect(&radioQ, SIGNAL(toggled(bool)), &w, SLOT(setQuads(bool)));
	QObject::connect(&radioQS, SIGNAL(toggled(bool)), &w, SLOT(setQuadstips(bool)));
	QObject::connect(&radioTex, SIGNAL(toggled(bool)), &w, SLOT(setTexture(bool)));
	QVBoxLayout rbAlgLay;
	rbAlgLay.addWidget(&radioQ);
	rbAlgLay.addWidget(&radioQS);
	rbAlgLay.addWidget(&radioTex);
	grBoxAlg.setLayout(&rbAlgLay);

	QPushButton btn("Load new data");
	QObject::connect(&btn, SIGNAL(clicked(bool)), &w, SLOT(loadNewData(bool)));
	
	QPushButton setMinMax("Set Min and Max");
	QObject::connect(&setMinMax, SIGNAL(clicked(bool)), &w, SLOT(setMM(bool)));
	QCheckBox toggleMM("Use user Min and Max");
	QObject::connect(&toggleMM, SIGNAL(stateChanged(int)), &w, SLOT(toggleUserMM(int)));
	QVBoxLayout MMLay;
	MMLay.addWidget(&toggleMM);
	MMLay.addWidget(&setMinMax);


	QHBoxLayout topLay;
	topLay.addWidget(&grBox);
	topLay.addWidget(&grBoxAlg);
	topLay.addLayout(&MMLay);
	topLay.addWidget(&btn);

	QVBoxLayout mainLay;
	mainLay.addLayout(&topLay, 1);
	mainLay.addWidget(&w, 3);
	window.setLayout(&mainLay);
	window.show();
	
	w.setFocusPolicy(Qt::StrongFocus);


	return a.exec();
}
